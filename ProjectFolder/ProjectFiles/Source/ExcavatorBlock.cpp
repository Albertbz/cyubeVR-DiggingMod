#include "ExcavatorBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

ExcavatorBlock::ExcavatorBlock(CoordinateInBlocks blockPosition) {
	this->size = 2;
	this->depth = 5;
	this->blockPosition = blockPosition;
	this->cornerBlockType = 1473066953; // markBlockID
	this->currentMode = 1;
	this->updateDigBlock();
	this->currentDigBlock[2] = -1;
	this->showNormal(true);
}

ExcavatorBlock::ExcavatorBlock(int size, int depth, CoordinateInBlocks blockPosition, int currentMode, int currentDigBlockX, int currentDigBlockY, int currentDigBlockZ) {
	this->size = size;
	this->depth = depth;
	this->blockPosition = blockPosition;
	this->cornerBlockType = 1473066953; // markBlockID
	this->currentMode = currentMode;
	this->currentDigBlock[0] = currentDigBlockX;
	this->currentDigBlock[1] = currentDigBlockY;
	this->currentDigBlock[2] = currentDigBlockZ;
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
	if (currentMode == 2) {
		removeCorners();
		size++;
		addCorners();
		updateDigBlock();
	}
}

void ExcavatorBlock::decrementSize() {
	if (currentMode == 2 && size > 2) {
		removeCorners();
		size--;
		addCorners();
		updateDigBlock();
	}
}

void ExcavatorBlock::incrementDepth() {
	if (currentMode == 2) {
		depth++;
	}
}

void ExcavatorBlock::decrementDepth() {
	if (currentMode == 2 && depth > 3) {
		depth--;
	}
}

bool ExcavatorBlock::dig() {
	if (currentMode == 3) {
		EBlockType currentBlockType = GetBlock(blockPosition + CoordinateInBlocks(currentDigBlock[0], currentDigBlock[1], currentDigBlock[2])).Type;
		if (currentBlockType == EBlockType::Stone || currentBlockType == EBlockType::Dirt || currentBlockType == EBlockType::Grass) {
			SetBlock(blockPosition + CoordinateInBlocks(currentDigBlock[0], currentDigBlock[1], currentDigBlock[2]), EBlockType::Air);
		}
		currentDigBlock[0]--;
		if (currentDigBlock[0] < -size) {
			currentDigBlock[0] = size;
			currentDigBlock[1]--;
			if (currentDigBlock[1] < -size) {
				currentDigBlock[1] = size;
				currentDigBlock[2]--;
				if (currentDigBlock[2] < -depth) {
					currentDigBlock[2] = -1;
					showDigging(false);
					showFinished(true);
					currentMode = 4;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}

void ExcavatorBlock::toggleDigging() {
	if (currentMode == 1) {
		showNormal(false);
		if (currentDigBlock[2] == -1) {
			removeFoliage();
		}
		showDigging(true);
		currentMode = 3;
	}
	else if (currentMode == 3) {
		showDigging(false);
		showNormal(true);
		currentMode = 1;
	}
}

void ExcavatorBlock::toggleSettings() {
	if (currentMode == 1) {
		showNormal(false);
		showSettings(true);
		currentMode == 2;
	}
	else if (currentMode == 2) {
		showSettings(false);
		showNormal(true);
		currentMode == 1;
	}
}

void ExcavatorBlock::showSettings(bool show) {
	if (show) {
		addCorners();
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 4), 1473066958); // Set Settings block
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 2), 1473066956); // Set Up block
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 1), 1473066955); // Set Down block
		SetBlock(blockPosition + CoordinateInBlocks(1, 0, 0), 1473066957); // Set In block
		SetBlock(blockPosition + CoordinateInBlocks(-1, 0, 0), 1473066954); // Set Out block
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

void ExcavatorBlock::showNormal(bool show) {
	if (show) {
		addCorners();
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 4), 1473066958); // Set Settings block
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), 1473066960); // Set Check Mark block
	}
	else {
		removeCorners();
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 4), EBlockType::Air);
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), EBlockType::Air);
	}
}

void ExcavatorBlock::showFinished(bool show) {
	if (show) {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), 1473066959); // Set Exclamation Mark block
	}
	else {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), EBlockType::Air);
	}
}

void ExcavatorBlock::showDigging(bool show) {
	if (show) {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), 1473066961); // Set Cross Mark block
	}
	else {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), EBlockType::Air);
	}
}

void ExcavatorBlock::destroy() {
	switch (currentMode) {
	case 1:
		showNormal(false);
		break;
	case 2:
		showSettings(false);
		break;
	case 3:
		showDigging(false);
		break;
	case 4:
		showFinished(false);
		break;
	};
	currentMode = 0;
}

void ExcavatorBlock::updateDigBlock() {
	currentDigBlock[0] = size;
	currentDigBlock[1] = size;
	currentDigBlock[2] = -1;
}

void ExcavatorBlock::removeFoliage() {
	for (int i = size; i >= -size; i--) { 
		for (int j = size; j >= -size; j--) {
			// Possibly add for height as well
			EBlockType currentBlockType = GetBlock(blockPosition + CoordinateInBlocks(i, j, 0)).Type;
			if (currentBlockType == EBlockType::Flower1 || currentBlockType == EBlockType::Flower2 || currentBlockType == EBlockType::Flower3 ||
				currentBlockType == EBlockType::Flower4 || currentBlockType == EBlockType::FlowerRainbow || currentBlockType == EBlockType::GrassFoliage) {
				SetBlock(blockPosition + CoordinateInBlocks(i, j, 0), EBlockType::Air);
			}
		}
	}
}

void ExcavatorBlock::writeExcavatorBlocks(std::ostream& file, const std::vector<ExcavatorBlock*>& excavatorBlocks) {
	for (auto b : excavatorBlocks) {
		file << b->size << ';' << b->depth << ';' << b->blockPosition.X << ';' << b->blockPosition.Y << ';' << b->blockPosition.Z << ';'
			 << b->currentMode << ';' << b->currentDigBlock[0] << ';' << b->currentDigBlock[1] << ';' << b->currentDigBlock[2] << '\n';
	}
}

void ExcavatorBlock::writeExcavatorBlocks(std::ostream&& file, const std::vector<ExcavatorBlock*>& excavatorBlocks) {
	ExcavatorBlock::writeExcavatorBlocks(file, excavatorBlocks);
}

auto ExcavatorBlock::readExcavatorBlocks(std::istream& file)->std::vector<ExcavatorBlock*> {
	std::vector<ExcavatorBlock*> excavatorBlocks;
	auto line = std::string{};

	while (std::getline(file, line)) {
		auto pos1 = line.find_first_of(';');
		auto pos2 = line.find_first_of(';', pos1 + 1);
		auto pos3 = line.find_first_of(';', pos2 + 1);
		auto pos4 = line.find_first_of(';', pos3 + 1);
		auto pos5 = line.find_first_of(';', pos4 + 1);
		auto pos6 = line.find_first_of(';', pos5 + 1);
		auto pos7 = line.find_first_of(';', pos6 + 1);
		auto pos8 = line.find_first_of(';', pos7 + 1);
		excavatorBlocks.push_back(new ExcavatorBlock{std::stoi(std::string{line, 0, pos1}),
													 std::stoi(std::string{line, pos1 + 1}),
													 CoordinateInBlocks(std::stoi(std::string{line, pos2 + 1}), std::stoi(std::string{line, pos3 + 1}), std::stoi(std::string{line, pos4 + 1})),
													 std::stoi(std::string{line, pos5 + 1}),
													 std::stoi(std::string{line, pos6 + 1}),
													 std::stoi(std::string{line, pos7 + 1}),
													 std::stoi(std::string{line, pos8 + 1})});
	}

	return excavatorBlocks;
}

auto ExcavatorBlock::readExcavatorBlocks(std::istream&& file) -> std::vector<ExcavatorBlock*> {
	return readExcavatorBlocks(file);
}