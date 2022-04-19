#include "QuarryBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

QuarryBlock::QuarryBlock(CoordinateInBlocks blockPosition) {
	this->size = 2;
	this->depth = 5;
	this->blockPosition = blockPosition;
	this->currentMode = 1;
	this->updateDigBlock();
	this->currentDigBlock[2] = -1;
	this->showNormal(true);
}

QuarryBlock::QuarryBlock(int size, int depth, CoordinateInBlocks blockPosition, int currentMode, std::array<int, 3> currentDigBlock, std::array<int, 4> cornerBlockTypes) {
	this->size = size;
	this->depth = depth;
	this->blockPosition = blockPosition;
	this->currentMode = currentMode;
	this->currentDigBlock = currentDigBlock;
	this->cornerBlockTypes = cornerBlockTypes;
}


CoordinateInBlocks QuarryBlock::getBlockPosition() {
	return this->blockPosition;
}

void QuarryBlock::addCorners() {
	cornerBlockTypes[0] = (uint8_t)GetBlock(blockPosition + CoordinateInBlocks(size, -size, 0)).Type;
	cornerBlockTypes[1] = (uint8_t)GetBlock(blockPosition + CoordinateInBlocks(size, size, 0)).Type;
	cornerBlockTypes[2] = (uint8_t)GetBlock(blockPosition + CoordinateInBlocks(-size, -size, 0)).Type;
	cornerBlockTypes[3] = (uint8_t)GetBlock(blockPosition + CoordinateInBlocks(-size, size, 0)).Type;
	SetBlock(blockPosition + CoordinateInBlocks(size, -size, 0), mark1BlockID);
	SetBlock(blockPosition + CoordinateInBlocks(size, size, 0), mark2BlockID);
	SetBlock(blockPosition + CoordinateInBlocks(-size, -size, 0), mark3BlockID);
	SetBlock(blockPosition + CoordinateInBlocks(-size, size, 0), mark4BlockID);
}

void QuarryBlock::removeCorners() {
	SetBlock(blockPosition + CoordinateInBlocks(size, -size, 0), (EBlockType)cornerBlockTypes[0]);
	SetBlock(blockPosition + CoordinateInBlocks(size, size, 0), (EBlockType)cornerBlockTypes[1]);
	SetBlock(blockPosition + CoordinateInBlocks(-size, -size, 0), (EBlockType)cornerBlockTypes[2]);
	SetBlock(blockPosition + CoordinateInBlocks(-size, size, 0), (EBlockType)cornerBlockTypes[3]);
}

void QuarryBlock::incrementSize() {
	if (currentMode == 2) {
		removeCorners();
		size++;
		printSize();
		addCorners();
		updateDigBlock();
	}
}

void QuarryBlock::decrementSize() {
	if (currentMode == 2 && size > 2) {
		removeCorners();
		size--;
		printSize();
		addCorners();
		updateDigBlock();
	}
}

void QuarryBlock::incrementDepth() {
	if (currentMode == 2) {
		depth++;
		printDepth();
	}
}

void QuarryBlock::decrementDepth() {
	if (currentMode == 2 && depth > 3) {
		depth--;
		printDepth();
	}
}

void QuarryBlock::printDepth() {
	std::wstring message = L"Depth: ";
	message.append(std::to_wstring(depth));
	SpawnHintText(CoordinateInCentimeters(blockPosition) + CoordinateInCentimeters(0, 0, 125), message, 1, 1.2f);
}

void QuarryBlock::printSize() {
	std::wstring message = L"Size: ";
	int length = size * 2 + 1;
	message.append(std::to_wstring(length));
	message.append(L" x ");
	message.append(std::to_wstring(length));
	SpawnHintText(CoordinateInCentimeters(blockPosition) + CoordinateInCentimeters(0, 0, 125), message, 1, 1.2f);
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
	}
}

void QuarryBlock::toggleDigging() {
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

void QuarryBlock::toggleSettings() {
	if (currentMode == 1) {
		showNormal(false);
		showSettings(true);
		currentMode = 2;
	}
	else if (currentMode == 2) {
		showSettings(false);
		SpawnHintText(CoordinateInCentimeters(blockPosition) + CoordinateInCentimeters(0, 0, 125), L"Settings saved!", 2, 1.2f);
		showNormal(true);
		currentMode = 1;
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

void QuarryBlock::destroy() {
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

void QuarryBlock::updateDigBlock() {
	currentDigBlock[0] = size;
	currentDigBlock[1] = size;
	currentDigBlock[2] = -1;
}

void QuarryBlock::writeQuarryBlocks(std::ostream& file, const std::vector<QuarryBlock*>& quarryBlocks) {
	for (auto b : quarryBlocks) {
		file << b->size << ';' << b->depth << ';' << b->blockPosition.X << ';' << b->blockPosition.Y << ';' << b->blockPosition.Z << ';'
			 << b->currentMode << ';' << b->currentDigBlock[0] << ';' << b->currentDigBlock[1] << ';' << b->currentDigBlock[2] << ';'
			 << b->cornerBlockTypes[0] << ';' << b->cornerBlockTypes[1] << ';' << b->cornerBlockTypes[2] << ';' << b->cornerBlockTypes[3] << '\n';
	}
}

void QuarryBlock::writeQuarryBlocks(std::ostream&& file, const std::vector<QuarryBlock*>& quarryBlocks) {
	QuarryBlock::writeQuarryBlocks(file, quarryBlocks);
}

auto QuarryBlock::readQuarryBlocks(std::istream& file)->std::vector<QuarryBlock*> {
	std::vector<QuarryBlock*> quarryBlocks;
	auto line = std::string{};

	while (std::getline(file, line)) {
		size_t pos = line.find_first_of(';');
		int size = std::stoi(std::string{ line, 0, pos });

		int depth = std::stoi(std::string{ line, pos + 1 });

		CoordinateInBlocks blockPosition;
		pos = line.find_first_of(';', pos + 1);
		blockPosition.X = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		blockPosition.Y = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		blockPosition.Z = std::stoi(std::string{ line, pos + 1 });

		pos = line.find_first_of(';', pos + 1);
		int currentMode = std::stoi(std::string{ line, pos + 1 });

		std::array<int, 3> currentDigBlock{};
		pos = line.find_first_of(';', pos + 1);
		currentDigBlock[0] = std::stoi(std::string{line, pos + 1});
		pos = line.find_first_of(';', pos + 1);
		currentDigBlock[1] = std::stoi(std::string{line, pos + 1});
		pos = line.find_first_of(';', pos + 1);
		currentDigBlock[2] = std::stoi(std::string{line, pos + 1});

		std::array<int, 4> cornerBlockTypes{};
		pos = line.find_first_of(';', pos + 1);
		cornerBlockTypes[0] = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		cornerBlockTypes[1] = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		cornerBlockTypes[2] = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		cornerBlockTypes[3] = std::stoi(std::string{ line, pos + 1 });
		
		
		quarryBlocks.push_back(new QuarryBlock{ size, depth, blockPosition, currentMode, currentDigBlock, cornerBlockTypes });
	}

	return quarryBlocks;
}

auto QuarryBlock::readQuarryBlocks(std::istream&& file) -> std::vector<QuarryBlock*> {
	return readQuarryBlocks(file);
}