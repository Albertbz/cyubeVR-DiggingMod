#include "ExcavatorBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

ExcavatorBlock::ExcavatorBlock(CoordinateInBlocks blockPosition) {
	this->size = 2;
	this->depth = 5;
	this->blockPosition = blockPosition;
	this->currentMode = 1;
	this->updateDigBlock();
	this->currentDigBlock[2] = -1;
	this->showNormal(true);
}

ExcavatorBlock::ExcavatorBlock(int size, int depth, CoordinateInBlocks blockPosition, int currentMode, int currentDigBlockX, int currentDigBlockY, int currentDigBlockZ) {
	this->size = size;
	this->depth = depth;
	this->blockPosition = blockPosition;
	this->currentMode = currentMode;
	this->currentDigBlock[0] = currentDigBlockX;
	this->currentDigBlock[1] = currentDigBlockY;
	this->currentDigBlock[2] = currentDigBlockZ;
}


CoordinateInBlocks ExcavatorBlock::getBlockPosition() {
	return this->blockPosition;
}

void ExcavatorBlock::addCorners() {
	SetBlock(blockPosition + CoordinateInBlocks(size, size, 0), mark2BlockID);
	SetBlock(blockPosition + CoordinateInBlocks(size, -size, 0), mark1BlockID);
	SetBlock(blockPosition + CoordinateInBlocks(-size, size, 0), mark4BlockID);
	SetBlock(blockPosition + CoordinateInBlocks(-size, -size, 0), mark3BlockID);
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
		printDepth();
	}
}

void ExcavatorBlock::decrementDepth() {
	if (currentMode == 2 && depth > 3) {
		depth--;
		printDepth();
	}
}

void ExcavatorBlock::printDepth() {
	std::wstring message = L"Depth: ";
	message.append(std::to_wstring(depth));
	SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0,  110), message, 1, 1.2f);
}

void ExcavatorBlock::printSize() {
	std::wstring message = L"Size: ";
	int length = size * 2 + 1;
	message.append(std::to_wstring(length));
	message.append(L" x ");
	message.append(std::to_wstring(length));
	SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0, 120), message, 1, 1.2f);
}

bool ExcavatorBlock::dig() {
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
		currentMode = 2;
	}
	else if (currentMode == 2) {
		showSettings(false);
		SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0, 130), L"Settings saved!", 1, 1.2f);
		showNormal(true);
		currentMode = 1;
	}
}

void ExcavatorBlock::showSettings(bool show) {
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

void ExcavatorBlock::showNormal(bool show) {
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

void ExcavatorBlock::showFinished(bool show) {
	if (show) {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), exclBlockID); // Set Exclamation Mark block
	}
	else {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), EBlockType::Air);
	}
}

void ExcavatorBlock::showDigging(bool show) {
	if (show) {
		SetBlock(blockPosition + CoordinateInBlocks(0, 0, 6), crossBlockID); // Set Cross Mark block
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