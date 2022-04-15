#include "ExcavatorBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

ExcavatorBlock::ExcavatorBlock(CoordinateInBlocks blockPosition) {
	this->size = 2;
	this->blockPosition = blockPosition;
	this->cornerBlockType = 1473066953;
	this->isDigging = false;
	this->updateDigBlock();
	this->currentDigBlock[2] = -1;
	this->addCorners();
}

ExcavatorBlock::ExcavatorBlock(int size, CoordinateInBlocks blockPosition, bool isDigging, int currentDigBlockX, int currentDigBlockY, int currentDigBlockZ) {
	this->size = size;
	this->blockPosition = blockPosition;
	this->cornerBlockType = 1473066953;
	this->isDigging = isDigging;
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
	if (!isDigging) {
		removeCorners();
		if (size >= 10) {
			size = 2;
		}
		else {
			size += 1;
		}
		addCorners();
		updateDigBlock();
	}
}

bool ExcavatorBlock::dig() {
	if (isDigging) {
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
				if (currentDigBlock[2] < -5) {
					currentDigBlock[2] = -1;
					isDigging = false;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}

void ExcavatorBlock::toggleDig() {
	if (!isDigging) {
		removeCorners();
		if (currentDigBlock[2] == -1) {
			removeFoliage();
		}
		isDigging = true;
	}
	else {
		addCorners();
		isDigging = false;
	}
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

void ExcavatorBlock::writeExcavatorBlocks(std::ostream& file, const std::vector<ExcavatorBlock*>& excavatorBlocks) {
	for (auto b : excavatorBlocks) {
		file << b->size << ';' << b->blockPosition.X << ';' << b->blockPosition.Y << ';' << b->blockPosition.Z << ';' 
			 << b->isDigging << ';' << b->currentDigBlock[0] << ';' << b->currentDigBlock[1] << ';' << b->currentDigBlock[2] << '\n';
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
		excavatorBlocks.push_back(new ExcavatorBlock{std::stoi(std::string{line, 0, pos1}), 
													 CoordinateInBlocks(std::stoi(std::string{line, pos1 + 1}), std::stoi(std::string{line, pos2 + 1}), std::stoi(std::string{line, pos3 + 1})),
													 stob(std::string{line, pos4 + 1}, false),
													 std::stoi(std::string{line, pos5 + 1}),
													 std::stoi(std::string{line, pos6 + 1}),
													 std::stoi(std::string{line, pos7 + 1})});
	}

	return excavatorBlocks;
}

auto ExcavatorBlock::readExcavatorBlocks(std::istream&& file) -> std::vector<ExcavatorBlock*> {
	return readExcavatorBlocks(file);
}

bool ExcavatorBlock::stob(std::string s, bool throw_on_error = true)
{
	auto result = false;    // failure to assert is false

	std::istringstream is(s);
	// first try simple integer conversion
	is >> result;
	return result;
}
