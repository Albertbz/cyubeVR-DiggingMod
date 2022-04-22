#include "DiggingBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

DiggingBlock::DiggingBlock(CoordinateInBlocks blockPosition) {
	this->length = 5;
	this->width = 5;
	this->depth = 5;
	this->blockPosition = blockPosition;
	this->currentMode = 1;
	this->resetDigBlock();
	this->showNormal(true);
}

DiggingBlock::DiggingBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, std::array<int, 3> currentDigBlock, std::array<std::pair<CoordinateInBlocks, EBlockType>, 4> cornerBlocks) {
	this->length = length;
	this->width = width;
	this->depth = depth;
	this->blockPosition = blockPosition;
	this->currentMode = currentMode;
	this->currentDigBlock = currentDigBlock;
	this->cornerBlocks = cornerBlocks;
}


CoordinateInBlocks DiggingBlock::getBlockPosition() {
	return this->blockPosition;
}

void DiggingBlock::addCorners() {
	cornerBlocks[0].second = GetBlock(blockPosition + cornerBlocks[0].first).Type;
	cornerBlocks[1].second = GetBlock(blockPosition + cornerBlocks[1].first).Type;
	cornerBlocks[2].second = GetBlock(blockPosition + cornerBlocks[2].first).Type;
	cornerBlocks[3].second = GetBlock(blockPosition + cornerBlocks[3].first).Type;
	SetBlock(blockPosition + cornerBlocks[0].first, mark1BlockID);
	SetBlock(blockPosition + cornerBlocks[1].first, mark2BlockID);
	SetBlock(blockPosition + cornerBlocks[2].first, mark3BlockID);
	SetBlock(blockPosition + cornerBlocks[3].first, mark4BlockID);
}

void DiggingBlock::removeCorners() {
	SetBlock(blockPosition + cornerBlocks[0].first, cornerBlocks[0].second);
	SetBlock(blockPosition + cornerBlocks[1].first, cornerBlocks[1].second);
	SetBlock(blockPosition + cornerBlocks[2].first, cornerBlocks[2].second);
	SetBlock(blockPosition + cornerBlocks[3].first, cornerBlocks[3].second);
}

void DiggingBlock::incrementLength(char side) {
	if (currentMode == 2) {
		removeCorners();
		length++;
		if (side == 'l') {
			cornerBlocks[0].first.X++;
			cornerBlocks[1].first.X++;
		}
		else if (side == 'r') {
			cornerBlocks[2].first.X--;
			cornerBlocks[3].first.X--;
		}
		addCorners();
		resetDigBlock();
	}
}

void DiggingBlock::decrementLength(char side) {
	if (currentMode == 2 && length > 5) {
		removeCorners();
		length--;
		if (side == 'l' && cornerBlocks[0].first.X > 2) {
			cornerBlocks[0].first.X--;
			cornerBlocks[1].first.X--;
		}
		else if (side == 'r' && cornerBlocks[2].first.X < -2) {
			cornerBlocks[2].first.X++;
			cornerBlocks[3].first.X++;
		}
		else {
			length++;
		}
		addCorners();
		resetDigBlock();
	}
}

void DiggingBlock::incrementWidth(char side) {
	if (currentMode == 2) {
		removeCorners();
		width++;
		if (side == 'f') {
			cornerBlocks[1].first.Y++;
			cornerBlocks[3].first.Y++;
		}
		else if (side == 'b') {
			cornerBlocks[0].first.Y--;
			cornerBlocks[2].first.Y--;
		}
		addCorners();
		resetDigBlock();
	}
}

void DiggingBlock::decrementWidth(char side) {
	if (currentMode == 2 && width > 5) {
		removeCorners();
		width--;
		if (side == 'f' && cornerBlocks[1].first.Y > 2) {
			cornerBlocks[1].first.X--;
			cornerBlocks[3].first.X--;
		}
		else if (side == 'b' && cornerBlocks[0].first.Y < -2) {
			cornerBlocks[0].first.X++;
			cornerBlocks[2].first.X++;
		}
		else {
			width++;
		}
		addCorners();
		resetDigBlock();
	}
}

void DiggingBlock::incrementSize() {
	if (currentMode == 2) {
		incrementLength('l');
		incrementLength('r');
		incrementWidth('f');
		incrementWidth('b');
	}
}

void DiggingBlock::decrementSize() {
	if (currentMode == 2) {
		decrementLength('l');
		decrementLength('r');
		decrementWidth('f');
		decrementWidth('b');
	}
}

void DiggingBlock::incrementDepth() {
	if (currentMode == 2) {
		depth++;
	}
}

void DiggingBlock::decrementDepth() {
	if (currentMode == 2 && depth > 1) {
		depth--;
	}
}

void DiggingBlock::printDepth() {
	std::wstring message = L"Depth: ";
	message.append(std::to_wstring(depth));
	SpawnHintText(CoordinateInCentimeters(blockPosition) + CoordinateInCentimeters(0, 0, 125), message, 1, 1.2f);
}

void DiggingBlock::printSize() {
	std::wstring message = L"Size: ";
	message.append(std::to_wstring(length));
	message.append(L" x ");
	message.append(std::to_wstring(width));
	SpawnHintText(CoordinateInCentimeters(blockPosition) + CoordinateInCentimeters(0, 0, 125), message, 1, 1.2f);
}

void DiggingBlock::dig() {
	
}

void DiggingBlock::toggleDigging() {
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

void DiggingBlock::toggleSettings() {
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

void DiggingBlock::finishedDigging() {
	if (currentMode == 3) {
		showDigging(false);
		showFinished(true);
		currentMode = 4;
	}
}

void DiggingBlock::showSettings(bool show) {
	
}

void DiggingBlock::showNormal(bool show) {
	
}

void DiggingBlock::showFinished(bool show) {
	
}

void DiggingBlock::showDigging(bool show) {
	
}

void DiggingBlock::destroy() {
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

void DiggingBlock::resetDigBlock() {
	currentDigBlock[0] = cornerBlocks[0].first.X;
	currentDigBlock[1] = cornerBlocks[1].first.X;
	currentDigBlock[2] = -1;
}

void DiggingBlock::writeBlocks(std::ostream& file, const std::vector<DiggingBlock*>& diggingBlocks) {
	for (auto b : diggingBlocks) {
		file << b->length << ';' << b->width << ';' << b->depth << ';' << b->blockPosition.X << ';' << b->blockPosition.Y << ';' << b->blockPosition.Z << ';'
			<< b->currentMode << ';' << b->currentDigBlock[0] << ';' << b->currentDigBlock[1] << ';' << b->currentDigBlock[2] << ';'
			<< (int)b->cornerBlocks[0].second << ';' << (int)b->cornerBlocks[1].second << ';' << (int)b->cornerBlocks[2].second << ';' << (int)b->cornerBlocks[3].second << '\n';
	}
}

void DiggingBlock::writeBlocks(std::ostream&& file, const std::vector<DiggingBlock*>& diggingBlocks) {
	DiggingBlock::writeBlocks(file, diggingBlocks);
}

auto DiggingBlock::readBlocks(std::istream& file)->std::vector<DiggingBlock*> {
	std::vector<DiggingBlock*> diggingBlocks;
	auto line = std::string{};

	while (std::getline(file, line)) {
		size_t pos = line.find_first_of(';');
		int length = std::stoi(std::string{ line, 0, pos });

		int width = std::stoi(std::string{ line, pos + 1 });

		pos = line.find_first_of(';', pos + 1);
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
		currentDigBlock[0] = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		currentDigBlock[1] = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		currentDigBlock[2] = std::stoi(std::string{ line, pos + 1 });

		std::array<std::pair<CoordinateInBlocks, EBlockType>, 4> cornerBlocks;
		pos = line.find_first_of(';', pos + 1);
		cornerBlocks[0].second = (EBlockType)std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		cornerBlocks[1].second = (EBlockType)std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		cornerBlocks[2].second = (EBlockType)std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		cornerBlocks[3].second = (EBlockType)std::stoi(std::string{ line, pos + 1 });


		diggingBlocks.push_back(new DiggingBlock{ length, width, depth, blockPosition, currentMode, currentDigBlock, cornerBlocks });
	}

	return diggingBlocks;
}

auto DiggingBlock::readBlocks(std::istream&& file) -> std::vector<DiggingBlock*> {
	return readBlocks(file);
}