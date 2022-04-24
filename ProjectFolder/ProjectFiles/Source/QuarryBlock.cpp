#include "QuarryBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

QuarryBlock::QuarryBlock(CoordinateInBlocks blockPosition) 
	: DiggingBlock(blockPosition) {
	this->cornerBlocks[0].position = CoordinateInBlocks(2, -2, 0);
	this->cornerBlocks[1].position = CoordinateInBlocks(2, 2, 0);
	this->cornerBlocks[2].position = CoordinateInBlocks(-2, -2, 0);
	this->cornerBlocks[3].position = CoordinateInBlocks(-2, 2, 0);
	this->cornerBlocks[0].infoBlock = mark1BlockID;
	this->cornerBlocks[1].infoBlock = mark2BlockID;
	this->cornerBlocks[2].infoBlock = mark3BlockID;
	this->cornerBlocks[3].infoBlock = mark4BlockID;
	this->resetDigBlock();
	showNormal(true);
}

QuarryBlock::QuarryBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, std::array<Block, 4> cornerBlocks)
	: DiggingBlock(length, width, depth, blockPosition, currentMode, currentDigBlock, cornerBlocks) {
	
}

void QuarryBlock::dig() {
	if (currentMode == 3) {
		EBlockType currentBlockType = GetBlock(blockPosition + currentDigBlock).Type;
		if (currentBlockType == EBlockType::Stone || currentBlockType == EBlockType::Dirt || currentBlockType == EBlockType::Grass || currentBlockType == EBlockType::Sand) {
			SetBlock(blockPosition + currentDigBlock, EBlockType::Air);

			// If block was Grass, remove possible flower/foliage on top.
			if (currentBlockType == EBlockType::Grass) {
				EBlockType blockAboveType = GetBlock(blockPosition + currentDigBlock + CoordinateInBlocks(0, 0, 1)).Type;
				if (blockAboveType == EBlockType::Flower1 || blockAboveType == EBlockType::Flower2 || blockAboveType == EBlockType::Flower3 ||
					blockAboveType == EBlockType::Flower4 || blockAboveType == EBlockType::FlowerRainbow || blockAboveType == EBlockType::GrassFoliage) {
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
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 4), setBlockID); // Set Settings block
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 2), upBlockID); // Set Up block
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 1), downBlockID); // Set Down block
		SetBlock(blockPosition + CoordinateInBlocks(1, 0, 0), inBlockID); // Set In block
		SetBlock(blockPosition + CoordinateInBlocks(-1, 0, 0), outBlockID); // Set Out block
	}
	else {
		removeCorners();
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 4), EBlockType::Air);
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 2), EBlockType::Air);
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 1), EBlockType::Air); 
		SetBlock(blockPosition + CoordinateInBlocks(1, 0, 0), EBlockType::Air); 
		SetBlock(blockPosition + CoordinateInBlocks(-1, 0, 0), EBlockType::Air);
	}
}

void QuarryBlock::showNormal(bool show) {
	if (show) {
		addCorners();
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 4), setBlockID); // Set Settings block
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), checkBlockID); // Set Check Mark block
	}
	else {
		removeCorners();
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 4), EBlockType::Air);
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), EBlockType::Air);
	}
}

void QuarryBlock::showFinished(bool show) {
	if (show) {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), exclBlockID); // Set Exclamation Mark block
	}
	else {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), EBlockType::Air);
	}
}

void QuarryBlock::showDigging(bool show) {
	if (show) {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), crossBlockID); // Set Cross Mark block
	}
	else {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), EBlockType::Air);
	}
}

void QuarryBlock::resetDigBlock() {
	currentDigBlock.X = cornerBlocks[0].position.X;
	currentDigBlock.Y = cornerBlocks[1].position.Y;
	currentDigBlock.Z = -1;
}