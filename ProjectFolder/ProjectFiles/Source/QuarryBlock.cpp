#include "QuarryBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

QuarryBlock::QuarryBlock(CoordinateInBlocks blockPosition) 
	: DiggingBlock(blockPosition) {
	this->digDirection = 6;

	this->cornerBlocks[0] = { CoordinateInBlocks(2, -2, 0), markerBlockID, BlockInfo() };
	this->cornerBlocks[1] = { CoordinateInBlocks(2, 2, 0), markerBlockID, BlockInfo() };
	this->cornerBlocks[2] = { CoordinateInBlocks(-2, -2, 0), markerBlockID, BlockInfo() };
	this->cornerBlocks[3] = { CoordinateInBlocks(-2, 2, 0), markerBlockID, BlockInfo() };

	this->textPlacement = CoordinateInCentimeters(0, 0, 40);

	resetDigBlock();
	toggleSettings();
}

QuarryBlock::QuarryBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, 
						 std::array<Block, 4> cornerBlocks, bool digOres, int digDirection)
	: DiggingBlock(length, width, depth, blockPosition, currentMode, currentDigBlock, cornerBlocks, digOres, digDirection) {
	this->textPlacement = CoordinateInCentimeters(0, 0, 40);
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

void QuarryBlock::incrementLength(char block) {
	if (currentMode == 2) {
		removeCorners();
		length++;
		if (block == 'u') {
			cornerBlocks[0].position.X++;
			cornerBlocks[1].position.X++;
		}
		else if (block == 'd') {
			cornerBlocks[2].position.X--;
			cornerBlocks[3].position.X--;
		}
		else {
			length--;
		}
		setCorners();
		resetDigBlock();
	}
}

void QuarryBlock::decrementLength(char block) {
	if (currentMode == 2 && length > 3) {
		removeCorners();
		length--;
		if (block == 'u' && cornerBlocks[0].position.X > 1) {
			cornerBlocks[0].position.X--;
			cornerBlocks[1].position.X--;
		}
		else if (block == 'd' && cornerBlocks[2].position.X < -1) {
			cornerBlocks[2].position.X++;
			cornerBlocks[3].position.X++;
		}
		else {
			length++;
		}
		setCorners();
		resetDigBlock();
	}
}

void QuarryBlock::incrementWidth(char block) {
	if (currentMode == 2) {
		removeCorners();
		width++;
		if (block == 'r') {
			cornerBlocks[1].position.Y++;
			cornerBlocks[3].position.Y++;
		}
		else if (block == 'l') {
			cornerBlocks[0].position.Y--;
			cornerBlocks[2].position.Y--;
		}
		else {
			width--;
		}
		setCorners();
		resetDigBlock();
	}
}

void QuarryBlock::decrementWidth(char block) {
	if (currentMode == 2 && width > 3) {
		removeCorners();
		width--;
		if (block == 'r' && cornerBlocks[1].position.Y > 1) {
			cornerBlocks[1].position.Y--;
			cornerBlocks[3].position.Y--;
		}
		else if (block == 'l' && cornerBlocks[0].position.Y < -1) {
			cornerBlocks[0].position.Y++;
			cornerBlocks[2].position.Y++;
		}
		else {
			width++;
		}
		setCorners();
		resetDigBlock();
	}
}

void QuarryBlock::resetDigBlock() {
	currentDigBlock.X = cornerBlocks[0].position.X;
	currentDigBlock.Y = cornerBlocks[1].position.Y;
	currentDigBlock.Z = -1;
}

void QuarryBlock::clickCheck(CoordinateInCentimeters fingerLocation, bool& canClick, CoordinateInCentimeters positionCm, bool leftHand)
{
	if (canClick) {
		if (fingerLocation.Z < positionCm.Z + 25) {
			clickRegister(fingerLocation, leftHand);
			canClick = false;
		}
	}
	else {
		if (fingerLocation.Z >= positionCm.Z + 25) {
			canClick = true;
		}
	}
}

CoordinateInCentimeters QuarryBlock::getCorner()
{
	return blockPosition + CoordinateInCentimeters(-25, -25, 25);
}

bool QuarryBlock::isBetween(std::pair<int, int> bottomLeft, std::pair<int, int> topRight, CoordinateInCentimeters fingerPos)
{
	CoordinateInCentimeters corner = getCorner();

	return fingerPos.X >= corner.X + bottomLeft.first - 1 && fingerPos.X <= corner.X + topRight.first - 1 && fingerPos.Y >= corner.Y + bottomLeft.second && fingerPos.Y <= corner.Y + topRight.second;
}

void QuarryBlock::setOffBlock()
{
	SetBlock(blockPosition, quarryOffBlockID);
}

void QuarryBlock::setOnBlock()
{
	SetBlock(blockPosition, quarryOnBlockID);
}

void QuarryBlock::setSettingsBlock()
{
	SetBlock(blockPosition, quarrySetBlockID);
}
