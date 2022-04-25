#include "QuarryBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

QuarryBlock::QuarryBlock(CoordinateInBlocks blockPosition) 
	: DiggingBlock(blockPosition) {
	this->cornerBlocks[0] = { CoordinateInBlocks(2, -2, 0), mark1BlockID, BlockInfo() };
	this->cornerBlocks[1] = { CoordinateInBlocks(2, 2, 0), mark2BlockID, BlockInfo() };
	this->cornerBlocks[2] = { CoordinateInBlocks(-2, -2, 0), mark3BlockID, BlockInfo() };
	this->cornerBlocks[3] = { CoordinateInBlocks(-2, 2, 0), mark4BlockID, BlockInfo() };

	this->buttonBlocks[0] = { CoordinateInBlocks(0, 0, 4), setBlockID, BlockInfo() };
	this->buttonBlocks[1] = { CoordinateInBlocks(0, 0, 2), upBlockID ,BlockInfo() };
	this->buttonBlocks[2] = { CoordinateInBlocks(0, 0, 1), downBlockID, BlockInfo() };
	this->buttonBlocks[3] = { CoordinateInBlocks(0, 0, 0), inBlockID, BlockInfo() };
	this->buttonBlocks[4] = { CoordinateInBlocks(0, 0, 0), outBlockID, BlockInfo() };
	this->buttonBlocks[5] = { CoordinateInBlocks(0, 0, 6), exclBlockID, BlockInfo() };
	this->buttonBlocks[6] = { CoordinateInBlocks(0, 0, 6), checkBlockID, BlockInfo() };
	this->buttonBlocks[7] = { CoordinateInBlocks(0, 0, 6), crossBlockID, BlockInfo() };
	this->buttonBlocks[8] = { CoordinateInBlocks(0, 0, 0), leftBlockID, BlockInfo() };
	this->buttonBlocks[9] = { CoordinateInBlocks(0, 0, 0), rightBlockID, BlockInfo() };
	this->buttonBlocks[10] = { CoordinateInBlocks(0, 0, 0), backBlockID, BlockInfo() };
	this->buttonBlocks[11] = { CoordinateInBlocks(0, 0, 0), frontBlockID, BlockInfo() };
	this->buttonBlocks[12] = { CoordinateInBlocks(0, 0, 0), nextBlockID, BlockInfo() };
	this->buttonBlocks[13] = { CoordinateInBlocks(0, 0, 0), prevBlockID, BlockInfo() };

	this->resetDigBlock();
	showNormal(true);
}

QuarryBlock::QuarryBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, 
						 std::array<Block, 4> cornerBlocks, std::array<Block, 14> buttonBlocks, int settingsPage)
	: DiggingBlock(length, width, depth, blockPosition, currentMode, currentDigBlock, cornerBlocks, buttonBlocks, settingsPage) {
	
}

void QuarryBlock::dig() {
	if (currentMode == 3) {
		BlockInfo currentBlock = GetBlock(blockPosition + currentDigBlock);
		if (diggableBlock(currentBlock)) {
			SetBlock(blockPosition + currentDigBlock, EBlockType::Air);

			// If block was Grass, remove possible flower/foliage on top.
			if (currentBlock.Type == EBlockType::Grass) {
				BlockInfo blockAbove = GetBlock(blockPosition + currentDigBlock + CoordinateInBlocks(0, 0, 1));
				if (blockAbove.Type != EBlockType::ModBlock && (blockAbove.Type == EBlockType::Flower1 || blockAbove.Type == EBlockType::Flower2 || 
					blockAbove.Type == EBlockType::Flower3 || blockAbove.Type == EBlockType::Flower4 || blockAbove.Type == EBlockType::FlowerRainbow || 
					blockAbove.Type == EBlockType::GrassFoliage)) {
					SetBlock(blockPosition + currentDigBlock + CoordinateInBlocks(0, 0, 1), EBlockType::Air);
				}
			}
		}

		currentDigBlock.X--;
		if (currentDigBlock.X < cornerBlocks[2].position.X) {
			currentDigBlock.X = cornerBlocks[0].position.X;
			currentDigBlock.Y--;
			if (currentDigBlock.Y < cornerBlocks[0].position.Y) {
				currentDigBlock.Y = cornerBlocks[1].position.Y;
				currentDigBlock.Z--;
				if (currentDigBlock.Z < -depth) {
					currentDigBlock.Z = -1;
					finishedDigging();
				}
			}
		}
	}
}

void QuarryBlock::incrementLength(char side) {
	if (currentMode == 2) {
		removeCorners();
		length++;
		if (side == 'l') {
			cornerBlocks[0].position.X++;
			cornerBlocks[1].position.X++;
		}
		else if (side == 'r') {
			cornerBlocks[2].position.X--;
			cornerBlocks[3].position.X--;
		}
		addCorners();
		resetDigBlock();
	}
}

void QuarryBlock::decrementLength(char side) {
	if (currentMode == 2 && length > 5) {
		removeCorners();
		length--;
		if (side == 'l' && cornerBlocks[0].position.X > 2) {
			cornerBlocks[0].position.X--;
			cornerBlocks[1].position.X--;
		}
		else if (side == 'r' && cornerBlocks[2].position.X < -2) {
			cornerBlocks[2].position.X++;
			cornerBlocks[3].position.X++;
		}
		else {
			length++;
		}
		addCorners();
		resetDigBlock();
	}
}

void QuarryBlock::incrementWidth(char side) {
	if (currentMode == 2) {
		removeCorners();
		width++;
		if (side == 'f') {
			cornerBlocks[1].position.Y++;
			cornerBlocks[3].position.Y++;
		}
		else if (side == 'b') {
			cornerBlocks[0].position.Y--;
			cornerBlocks[2].position.Y--;
		}
		addCorners();
		resetDigBlock();
	}
}

void QuarryBlock::decrementWidth(char side) {
	if (currentMode == 2 && width > 5) {
		removeCorners();
		width--;
		if (side == 'f' && cornerBlocks[1].position.Y > 2) {
			cornerBlocks[1].position.Y--;
			cornerBlocks[3].position.Y--;
		}
		else if (side == 'b' && cornerBlocks[0].position.Y < -2) {
			cornerBlocks[0].position.Y++;
			cornerBlocks[2].position.Y++;
		}
		else {
			width++;
		}
		addCorners();
		resetDigBlock();
	}
}

void QuarryBlock::showSettings(bool show) {
	if (show) {
		addCorners();
		updateSettingsBlockLocations();
		buttonBlocks[0].infoPrev = GetBlock(blockPosition + buttonBlocks[0].position);
		SetBlock(blockPosition + buttonBlocks[0].position, buttonBlocks[0].infoBlock); // Set Settings block

		buttonBlocks[1].infoPrev = GetBlock(blockPosition + buttonBlocks[1].position);
		SetBlock(blockPosition + buttonBlocks[1].position, buttonBlocks[1].infoBlock); // Set Up block

		buttonBlocks[2].infoPrev = GetBlock(blockPosition + buttonBlocks[2].position);
		SetBlock(blockPosition + buttonBlocks[2].position, buttonBlocks[2].infoBlock); // Set Down block

		buttonBlocks[3].infoPrev = GetBlock(blockPosition + buttonBlocks[3].position);
		SetBlock(blockPosition + buttonBlocks[3].position, buttonBlocks[3].infoBlock); // Set In block

		buttonBlocks[4].infoPrev = GetBlock(blockPosition + buttonBlocks[4].position);
		SetBlock(blockPosition + buttonBlocks[4].position, buttonBlocks[4].infoBlock); // Set Out block

		buttonBlocks[12].infoPrev = GetBlock(blockPosition + buttonBlocks[12].position);
		SetBlock(blockPosition + buttonBlocks[12].position, buttonBlocks[12].infoBlock); // Set Next block

		buttonBlocks[13].infoPrev = GetBlock(blockPosition + buttonBlocks[13].position);
		SetBlock(blockPosition + buttonBlocks[13].position, buttonBlocks[13].infoBlock); // Set Previous block

	}
	else {
		removeCorners();
		SetBlock(blockPosition + buttonBlocks[0].position, buttonBlocks[0].infoPrev);
		SetBlock(blockPosition + buttonBlocks[1].position, buttonBlocks[1].infoPrev);
		SetBlock(blockPosition + buttonBlocks[2].position, buttonBlocks[2].infoPrev);
		SetBlock(blockPosition + buttonBlocks[3].position, buttonBlocks[3].infoPrev);
		SetBlock(blockPosition + buttonBlocks[4].position, buttonBlocks[4].infoPrev);
		SetBlock(blockPosition + buttonBlocks[12].position, buttonBlocks[12].infoPrev);
		SetBlock(blockPosition + buttonBlocks[13].position, buttonBlocks[13].infoPrev);
	}
}

void QuarryBlock::showNormal(bool show) {
	if (show) {
		addCorners();
		buttonBlocks[0].infoPrev = GetBlock(blockPosition + buttonBlocks[0].position);
		SetBlock(blockPosition + buttonBlocks[0].position, buttonBlocks[0].infoBlock); // Set Settings block

		buttonBlocks[6].infoPrev = GetBlock(blockPosition + buttonBlocks[6].position);
		SetBlock(blockPosition + buttonBlocks[6].position, buttonBlocks[6].infoBlock); // Set Check Mark block
	}
	else {
		removeCorners();
		SetBlock(blockPosition + buttonBlocks[0].position, buttonBlocks[0].infoPrev);
		SetBlock(blockPosition + buttonBlocks[6].position, buttonBlocks[6].infoPrev);
	}
}

void QuarryBlock::showFinished(bool show) {
	if (show) {
		buttonBlocks[5].infoPrev = GetBlock(blockPosition + buttonBlocks[5].position);
		SetBlock(blockPosition + buttonBlocks[5].position, buttonBlocks[5].infoBlock); // Set Exclamation Mark block
	}
	else {
		SetBlock(blockPosition + buttonBlocks[5].position, buttonBlocks[5].infoPrev);
	}
}

void QuarryBlock::showDigging(bool show) {
	if (show) {
		buttonBlocks[7].infoPrev = GetBlock(blockPosition + buttonBlocks[7].position);
		SetBlock(blockPosition + buttonBlocks[7].position, buttonBlocks[7].infoBlock); // Set Cross Mark block
	}
	else {
		SetBlock(blockPosition + buttonBlocks[7].position, buttonBlocks[7].infoPrev);
	}
}

void QuarryBlock::resetDigBlock() {
	currentDigBlock.X = cornerBlocks[0].position.X;
	currentDigBlock.Y = cornerBlocks[1].position.Y;
	currentDigBlock.Z = -1;
}

void QuarryBlock::updateSettingsBlockLocations() {
	CoordinateInCentimeters playerLocation = GetPlayerLocation();
	CoordinateInCentimeters blockPositionCm = CoordinateInCentimeters(blockPosition);
	int xDifference = std::abs(playerLocation.X - blockPositionCm.X); 
	int yDifference = std::abs(playerLocation.Y - blockPositionCm.Y);

	if (playerLocation.X > blockPositionCm.X + 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		buttonBlocks[3].position = CoordinateInBlocks(0, 1, 0);
		buttonBlocks[4].position = CoordinateInBlocks(0, -1, 0);
		buttonBlocks[12].position = buttonBlocks[0].position + CoordinateInBlocks(0, -1, 0);
		buttonBlocks[13].position = buttonBlocks[0].position + CoordinateInBlocks(0, 1, 0);
	}
	else if (playerLocation.X < blockPositionCm.X - 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		buttonBlocks[3].position = CoordinateInBlocks(0, -1, 0);
		buttonBlocks[4].position = CoordinateInBlocks(0, 1, 0);
		buttonBlocks[12].position = buttonBlocks[0].position + CoordinateInBlocks(0, 1, 0);
		buttonBlocks[13].position = buttonBlocks[0].position + CoordinateInBlocks(0, -1, 0);
	}
	else if (playerLocation.Y > blockPositionCm.Y + 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		buttonBlocks[3].position = CoordinateInBlocks(-1, 0, 0);
		buttonBlocks[4].position = CoordinateInBlocks(1, 0, 0);
		buttonBlocks[12].position = buttonBlocks[0].position + CoordinateInBlocks(1, 0, 0);
		buttonBlocks[13].position = buttonBlocks[0].position + CoordinateInBlocks(-1, 0, 0);
	}
	else if (playerLocation.Y < blockPositionCm.Y - 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		buttonBlocks[3].position = CoordinateInBlocks(1, 0, 0);
		buttonBlocks[4].position = CoordinateInBlocks(-1, 0, 0);
		buttonBlocks[12].position = buttonBlocks[0].position + CoordinateInBlocks(-1, 0, 0);
		buttonBlocks[13].position = buttonBlocks[0].position + CoordinateInBlocks(1, 0, 0);
	}
}