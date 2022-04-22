#include "QuarryBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

QuarryBlock::QuarryBlock(CoordinateInBlocks blockPosition) 
	: DiggingBlock(blockPosition) {

}

QuarryBlock::QuarryBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, std::array<int, 3> currentDigBlock, std::array<std::pair<CoordinateInBlocks, EBlockType>, 4> cornerBlocks)
	: DiggingBlock(length, width, depth, blockPosition, currentMode, currentDigBlock, cornerBlocks) {
	
}

void QuarryBlock::dig() {
	if (currentMode == 3) {
		EBlockType currentBlockType = GetBlock(blockPosition + CoordinateInBlocks(currentDigBlock[0], currentDigBlock[1], currentDigBlock[2])).Type;
		if (currentBlockType == EBlockType::Stone || currentBlockType == EBlockType::Dirt || currentBlockType == EBlockType::Grass) {
			SetBlock(blockPosition + CoordinateInBlocks(currentDigBlock[0], currentDigBlock[1], currentDigBlock[2]), EBlockType::Air);

			// If block was Grass, remove possible flower/foliage on top.
			if (currentBlockType == EBlockType::Grass) {
				EBlockType blockAboveType = GetBlock(blockPosition + CoordinateInBlocks(currentDigBlock[0], currentDigBlock[1], currentDigBlock[2] + 1)).Type;
				if (blockAboveType == EBlockType::Flower1 || blockAboveType == EBlockType::Flower2 || blockAboveType == EBlockType::Flower3 ||
					blockAboveType == EBlockType::Flower4 || blockAboveType == EBlockType::FlowerRainbow || blockAboveType == EBlockType::GrassFoliage) {
					SetBlock(blockPosition + CoordinateInBlocks(currentDigBlock[0], currentDigBlock[1], currentDigBlock[2] + 1), EBlockType::Air);
				}
			}
		}
		currentDigBlock[0]--;
		if (currentDigBlock[0] < cornerBlocks[2].first.X) {
			currentDigBlock[0] = cornerBlocks[0].first.X;
			currentDigBlock[1]--;
			if (currentDigBlock[1] < cornerBlocks[0].first.Y) {
				currentDigBlock[1] = cornerBlocks[1].first.Y;
				currentDigBlock[2]--;
				if (currentDigBlock[2] < -depth) {
					currentDigBlock[2] = -1;
					finishedDigging();
				}
			}
		}
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
