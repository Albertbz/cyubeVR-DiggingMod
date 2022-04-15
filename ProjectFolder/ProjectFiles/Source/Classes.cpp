#include "Classes.h"

ExcavatorBlock::ExcavatorBlock(CoordinateInBlocks blockPosition) {
	this->blockPosition = blockPosition;
	this->size = 2;
	this->cornerBlockType = EBlockType::Wallstone;
	this->isDigging = false;
	this->currentDigLayer = -1;
	this->updateDigBlock();
	this->addCorners();
}


CoordinateInBlocks ExcavatorBlock::getBlockPosition() {
	return this->blockPosition;
}

void ExcavatorBlock::addCorners() {
	SetBlock(blockPosition + CoordinateInBlocks(size, size, 0), cornerBlockType);
	SetBlock(blockPosition + CoordinateInBlocks(size, -size, 0), cornerBlockType);
	SetBlock(blockPosition + CoordinateInBlocks(-size, size, 0), cornerBlockType);
	SetBlock(blockPosition + CoordinateInBlocks(-size, -size, 0), cornerBlockType);
}

void ExcavatorBlock::removeCorners() {
	SetBlock(blockPosition + CoordinateInBlocks(size, size, 0), EBlockType::Air);
	SetBlock(blockPosition + CoordinateInBlocks(size, -size, 0), EBlockType::Air);
	SetBlock(blockPosition + CoordinateInBlocks(-size, size, 0), EBlockType::Air);
	SetBlock(blockPosition + CoordinateInBlocks(-size, -size, 0), EBlockType::Air);
}

void ExcavatorBlock::incrementSize() {
	if (!isDigging) {
		removeCorners();
		if (size >= 10) {
			size = 2;
		}
		else {
			size += 1;
		}
		addCorners();
	}
}

void ExcavatorBlock::dig() {
	if (isDigging) {
		EBlockType currentBlockType = GetBlock(blockPosition + CoordinateInBlocks(currentDigBlock[0], currentDigBlock[1], currentDigLayer)).Type;
		if (currentBlockType == EBlockType::Stone || currentBlockType == EBlockType::Dirt || currentBlockType == EBlockType::Grass) {
			SetBlock(blockPosition + CoordinateInBlocks(currentDigBlock[0], currentDigBlock[1], currentDigLayer), EBlockType::Air);
		}
		currentDigBlock[0]--;
		if (currentDigBlock[0] < -size) {
			currentDigBlock[0] = size;
			currentDigBlock[1]--;
			if (currentDigBlock[1] < -size) {
				currentDigBlock[1] = size;
				currentDigLayer--;
				if (currentDigLayer < -5) {
					isDigging = false;
				}
			}
		}
	}
}

void ExcavatorBlock::startDig() {
	removeCorners();
	removeFoliage();
	updateDigBlock();
	isDigging = true;
}

void ExcavatorBlock::updateDigBlock() {
	currentDigBlock[0] = size;
	currentDigBlock[1] = size;
}

void ExcavatorBlock::removeFoliage() {
	for (int i = size; i >= -size; i--) { 
		for (int j = size; j >= -size; j--) {
			EBlockType currentBlockType = GetBlock(blockPosition + CoordinateInBlocks(i, j, 0)).Type;
			if (currentBlockType == EBlockType::Flower1 || currentBlockType == EBlockType::Flower2 || currentBlockType == EBlockType::Flower3 ||
				currentBlockType == EBlockType::Flower4 || currentBlockType == EBlockType::FlowerRainbow || currentBlockType == EBlockType::GrassFoliage) {
				SetBlock(blockPosition + CoordinateInBlocks(i, j, 0), EBlockType::Air);
			}
		}
	}
}