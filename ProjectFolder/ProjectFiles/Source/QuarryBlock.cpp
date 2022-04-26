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
	this->buttonBlocks[1] = { CoordinateInBlocks(0, 0, 0), upBlockID ,BlockInfo() };
	this->buttonBlocks[2] = { CoordinateInBlocks(0, 0, 0), downBlockID, BlockInfo() };
	this->buttonBlocks[3] = { CoordinateInBlocks(0, 0, 0), inBlockID, BlockInfo() };
	this->buttonBlocks[4] = { CoordinateInBlocks(0, 0, 0), outBlockID, BlockInfo() };
	this->buttonBlocks[5] = { CoordinateInBlocks(0, 0, 6), exclBlockID, BlockInfo() };
	this->buttonBlocks[6] = { CoordinateInBlocks(0, 0, 6), checkBlockID, BlockInfo() };
	this->buttonBlocks[7] = { CoordinateInBlocks(0, 0, 6), crossBlockID, BlockInfo() };
	this->buttonBlocks[8] = { CoordinateInBlocks(0, -5, 0), leftBlockID, BlockInfo() };
	this->buttonBlocks[9] = { CoordinateInBlocks(0, 5, 0), rightBlockID, BlockInfo() };
	this->buttonBlocks[10] = { CoordinateInBlocks(5, 0, 0), backBlockID, BlockInfo() };
	this->buttonBlocks[11] = { CoordinateInBlocks(-5, 0, 0), frontBlockID, BlockInfo() };
	this->buttonBlocks[12] = { CoordinateInBlocks(0, 0, 0), nextBlockID, BlockInfo() };
	this->buttonBlocks[13] = { CoordinateInBlocks(0, 0, 0), prevBlockID, BlockInfo() };
	this->buttonBlocks[14] = { CoordinateInBlocks(0, 4, 0), leftBlockID, BlockInfo() };
	this->buttonBlocks[15] = { CoordinateInBlocks(0, -4, 0), rightBlockID, BlockInfo() };
	this->buttonBlocks[16] = { CoordinateInBlocks(-4, 0, 0), backBlockID, BlockInfo() };
	this->buttonBlocks[17] = { CoordinateInBlocks(4, 0, 0), frontBlockID, BlockInfo() };

	this->resetDigBlock();
	showNormal(true);
}

QuarryBlock::QuarryBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, 
						 std::array<Block, 4> cornerBlocks, std::array<Block, 18> buttonBlocks, int settingsPage, bool digOres)
	: DiggingBlock(length, width, depth, blockPosition, currentMode, currentDigBlock, cornerBlocks, buttonBlocks, settingsPage, digOres) {
	
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
		setSettingsBlocks();
	}
	else {
		removeCorners();
		removeSettingsBlocks();
	}
}

void QuarryBlock::showNormal(bool show) {
	if (show) {
		addCorners();
		setButtonBlock(0); // Set Settings block
		setButtonBlock(6); // Set Check Mark block
	}
	else {
		removeCorners();
		removeButtonBlock(0);
		removeButtonBlock(6);
	}
}

void QuarryBlock::showFinished(bool show) {
	if (show) {
		setButtonBlock(5); // Set Exclamation Mark block
	}
	else {
		removeButtonBlock(5);
	}
}

void QuarryBlock::showDigging(bool show) {
	if (show) {
		setButtonBlock(7); // Set Cross Mark block
	}
	else {
		removeButtonBlock(7);
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
		buttonBlocks[1].position = CoordinateInBlocks(0, 1, 0);
		buttonBlocks[2].position = CoordinateInBlocks(0, -1, 0);
		buttonBlocks[3].position = CoordinateInBlocks(0, 1, 0);
		buttonBlocks[4].position = CoordinateInBlocks(0, -1, 0);
		buttonBlocks[12].position = buttonBlocks[0].position + CoordinateInBlocks(0, -1, 0);
		buttonBlocks[13].position = buttonBlocks[0].position + CoordinateInBlocks(0, 1, 0);
	}
	else if (playerLocation.X < blockPositionCm.X - 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		buttonBlocks[1].position = CoordinateInBlocks(0, -1, 0);
		buttonBlocks[2].position = CoordinateInBlocks(0, 1, 0);
		buttonBlocks[3].position = CoordinateInBlocks(0, -1, 0);
		buttonBlocks[4].position = CoordinateInBlocks(0, 1, 0);
		buttonBlocks[12].position = buttonBlocks[0].position + CoordinateInBlocks(0, 1, 0);
		buttonBlocks[13].position = buttonBlocks[0].position + CoordinateInBlocks(0, -1, 0);
	}
	else if (playerLocation.Y > blockPositionCm.Y + 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		buttonBlocks[1].position = CoordinateInBlocks(-1, 0, 0);
		buttonBlocks[2].position = CoordinateInBlocks(1, 0, 0);
		buttonBlocks[3].position = CoordinateInBlocks(-1, 0, 0);
		buttonBlocks[4].position = CoordinateInBlocks(1, 0, 0);
		buttonBlocks[12].position = buttonBlocks[0].position + CoordinateInBlocks(1, 0, 0);
		buttonBlocks[13].position = buttonBlocks[0].position + CoordinateInBlocks(-1, 0, 0);
	}
	else if (playerLocation.Y < blockPositionCm.Y - 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		buttonBlocks[1].position = CoordinateInBlocks(1, 0, 0);
		buttonBlocks[2].position = CoordinateInBlocks(-1, 0, 0);
		buttonBlocks[3].position = CoordinateInBlocks(1, 0, 0);
		buttonBlocks[4].position = CoordinateInBlocks(-1, 0, 0);
		buttonBlocks[12].position = buttonBlocks[0].position + CoordinateInBlocks(-1, 0, 0);
		buttonBlocks[13].position = buttonBlocks[0].position + CoordinateInBlocks(1, 0, 0);
	}
}

void QuarryBlock::setSettingsBlocks() {
	setButtonBlock(0); // Set Settings block

	if (settingsPage != 1) {
		setButtonBlock(13); // Set Previous block
	}
	if (settingsPage != 4) {
		setButtonBlock(12); // Set Next block
	}


	if (settingsPage == 1) {
		setButtonBlock(1); // Set Up block
		setButtonBlock(2); // Set Down block
	}
	else if (settingsPage == 2) {
		setButtonBlock(3); // Set In block
		setButtonBlock(4); // Set Out block
	}
	else if (settingsPage == 3) {
		setButtonBlock(8); // Set Left block (increment)
		setButtonBlock(9); // Set Right block (increment)
		setButtonBlock(10); // Set Back block (increment)
		setButtonBlock(11); // Set Front block (increment)
		setButtonBlock(14); // Set Left block (decrement)
		setButtonBlock(15); // Set Right block (decrement)
		setButtonBlock(16); // Set Back block (decrement)
		setButtonBlock(17); // Set Front block (decrement)
	}
}

void QuarryBlock::removeSettingsBlocks() {
	removeButtonBlock(0);

	if (settingsPage != 1) {
		removeButtonBlock(13);
	}
	if (settingsPage != 4) {
		removeButtonBlock(12);
	}


	if (settingsPage == 1) {
		removeButtonBlock(1);
		removeButtonBlock(2);
	}
	else if (settingsPage == 2) {
		removeButtonBlock(3);
		removeButtonBlock(4);
	}
	else if (settingsPage == 3) {
		removeButtonBlock(8);
		removeButtonBlock(9);
		removeButtonBlock(10);
		removeButtonBlock(11);
		removeButtonBlock(14);
		removeButtonBlock(15);
		removeButtonBlock(16);
		removeButtonBlock(17);
	}
}