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
}

DiggingBlock::DiggingBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, std::array<int, 3> currentDigBlock, std::array<Block, 4> cornerBlocks) {
	this->length = length;
	this->width = width;
	this->depth = depth;
	this->blockPosition = blockPosition;
	this->currentMode = currentMode;
	this->currentDigBlock = currentDigBlock;
	this->cornerBlocks = cornerBlocks;
}

void DiggingBlock::addCorners() {
	cornerBlocks[0].infoPrev = GetBlock(blockPosition + cornerBlocks[0].position);
	cornerBlocks[1].infoPrev = GetBlock(blockPosition + cornerBlocks[1].position);
	cornerBlocks[2].infoPrev = GetBlock(blockPosition + cornerBlocks[2].position);
	cornerBlocks[3].infoPrev = GetBlock(blockPosition + cornerBlocks[3].position);
	SetBlock(blockPosition + cornerBlocks[0].position, cornerBlocks[0].infoPrev);
	SetBlock(blockPosition + cornerBlocks[1].position, cornerBlocks[1].infoPrev);
	SetBlock(blockPosition + cornerBlocks[2].position, cornerBlocks[2].infoPrev);
	SetBlock(blockPosition + cornerBlocks[3].position, cornerBlocks[3].infoPrev);
}

void DiggingBlock::removeCorners() {
	SetBlock(blockPosition + cornerBlocks[0].position, cornerBlocks[0].infoPrev);
	SetBlock(blockPosition + cornerBlocks[1].position, cornerBlocks[1].infoPrev);
	SetBlock(blockPosition + cornerBlocks[2].position, cornerBlocks[2].infoPrev);
	SetBlock(blockPosition + cornerBlocks[3].position, cornerBlocks[3].infoPrev);
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
	SpawnHintText(CoordinateInCentimeters(blockPosition) + CoordinateInCentimeters(0, 0, 125), message, 0.5f, 1.2f);
}

void DiggingBlock::printSize() {
	std::wstring message = L"Size: ";
	message.append(std::to_wstring(length));
	message.append(L" x ");
	message.append(std::to_wstring(width));
	SpawnHintText(CoordinateInCentimeters(blockPosition) + CoordinateInCentimeters(0, 0, 125), message, 0.5f, 1.2f);
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

		// calculate time to dig
		int totalSeconds = length * width * depth / 5;
		int seconds = totalSeconds % 60;
		int totalMinutes = totalSeconds / 60;
		int minutes = totalMinutes % 60;
		int hours = totalMinutes / 60;
		std::wstring message = L"Settings saved! It will take ";
		message.append(std::to_wstring(hours));
		message.append(L" hours, ");
		message.append(std::to_wstring(minutes));
		message.append(L" minutes, and ");
		message.append(std::to_wstring(seconds));
		message.append(L"seconds to be dug out.");

		SpawnHintText(CoordinateInCentimeters(blockPosition) + CoordinateInCentimeters(0, 0, 125), message, 5, 1.5f);
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