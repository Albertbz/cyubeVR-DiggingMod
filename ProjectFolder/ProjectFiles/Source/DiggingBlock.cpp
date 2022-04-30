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
	this->currentDigBlock = CoordinateInBlocks(0, 0, 0);
	this->cornerBlocks = {};
	this->buttonBlocks = {};
	this->settingsPage = 1;
	this->digOres = false;
	this->digDirection = 1;
}

DiggingBlock::DiggingBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, 
						   std::array<Block, 4> cornerBlocks, std::array<Block, buttonBlocksAmount> buttonBlocks, int settingsPage, bool digOres,
						   int digDirection) {
	this->length = length;
	this->width = width;
	this->depth = depth;
	this->blockPosition = blockPosition;
	this->currentMode = currentMode;
	this->currentDigBlock = currentDigBlock;
	this->cornerBlocks = cornerBlocks;
	this->buttonBlocks = buttonBlocks;
	this->settingsPage = settingsPage;
	this->digOres = digOres;
	this->digDirection = digDirection;
}

void DiggingBlock::setCorners() {
	cornerBlocks[0].infoPrev = GetBlock(blockPosition + cornerBlocks[0].position);
	cornerBlocks[1].infoPrev = GetBlock(blockPosition + cornerBlocks[1].position);
	cornerBlocks[2].infoPrev = GetBlock(blockPosition + cornerBlocks[2].position);
	cornerBlocks[3].infoPrev = GetBlock(blockPosition + cornerBlocks[3].position);
	SetBlock(blockPosition + cornerBlocks[0].position, cornerBlocks[0].infoBlock);
	SetBlock(blockPosition + cornerBlocks[1].position, cornerBlocks[1].infoBlock);
	SetBlock(blockPosition + cornerBlocks[2].position, cornerBlocks[2].infoBlock);
	SetBlock(blockPosition + cornerBlocks[3].position, cornerBlocks[3].infoBlock);
}

void DiggingBlock::removeCorners() {
	SetBlock(blockPosition + cornerBlocks[0].position, cornerBlocks[0].infoPrev);
	SetBlock(blockPosition + cornerBlocks[1].position, cornerBlocks[1].infoPrev);
	SetBlock(blockPosition + cornerBlocks[2].position, cornerBlocks[2].infoPrev);
	SetBlock(blockPosition + cornerBlocks[3].position, cornerBlocks[3].infoPrev);
}

void DiggingBlock::incrementSize() {
	if (currentMode == 2) {
		incrementLength('b');
		incrementLength('f');
		incrementWidth('l');
		incrementWidth('r');
	}
}

void DiggingBlock::decrementSize() {
	if (currentMode == 2) {
		decrementLength('b');
		decrementLength('f');
		decrementWidth('l');
		decrementWidth('r');
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
	SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0, 75), message, 0.5f, 1.5f);
}

void DiggingBlock::printSize() {
	std::wstring message = L"Size (LxWxD)\n---------------\n" + getSize();
	SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0, 75), message, 1, 1.5f);
}

wString DiggingBlock::getSize() {
	return std::to_wstring(length) + L"x" + std::to_wstring(width) + L"x" + std::to_wstring(depth);
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
		printSettingsPage();
	}
	else if (currentMode == 2) {
		showSettings(false);
		SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0, 75), L"Settings saved!\nIt will take\n" + timeToDig() + L"\nto dig out the " + getSize() + L" hole.", 5, 2.0f);
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

std::wstring DiggingBlock::timeToDig() {
	int totalSeconds = length * width * depth / 5;
	int seconds = totalSeconds % 60;
	int totalMinutes = totalSeconds / 60;
	int minutes = totalMinutes % 60;
	int hours = totalMinutes / 60;

	std::wstring message;
	if (hours > 0) {
		message.append(std::to_wstring(hours));
		message.append(L" hour");
		if (hours > 1) {
			message.append(L"s");
		}
		if (minutes > 0 && seconds > 0) {
			message.append(L", ");
		}
		else if ((minutes == 0 && seconds > 0) || (minutes > 0 && seconds == 0)) {
			message.append(L" and ");
		}
	}
	if (minutes > 0) {
		message.append(std::to_wstring(minutes));
		message.append(L" minute");
		if (minutes > 1) {
			message.append(L"s");
		}
		if (hours > 0 && seconds > 0) {
			message.append(L", and ");
		}
		else if (seconds > 0 && hours == 0) {
			message.append(L" and ");
		}
	}
	if (seconds > 0) {
		message.append(std::to_wstring(seconds));
		message.append(L" second");
		if (seconds > 1) {
			message.append(L"s");
		}
	}
	return message;
}

bool DiggingBlock::diggableBlock(BlockInfo block) {
	bool diggable = false;
	if (block.Type != EBlockType::ModBlock) {
		diggable = block.Type == EBlockType::Stone || block.Type == EBlockType::Dirt || block.Type == EBlockType::Grass || block.Type == EBlockType::Sand;
		if (!diggable && digOres) {
			diggable = block.Type == EBlockType::Ore_Coal || block.Type == EBlockType::Ore_Copper || block.Type == EBlockType::Ore_Gold ||
				block.Type == EBlockType::Ore_Iron || block.Type == EBlockType::CrystalBlock;
		}
	}
	return diggable;
}

void DiggingBlock::nextSettingsPage() {
	if (settingsPage < 4) {
		removeSettingsBlocks();
		settingsPage++;
		printSettingsPage();
		setSettingsBlocks();
	}
}

void DiggingBlock::prevSettingsPage() {
	if (settingsPage > 1) {
		removeSettingsBlocks();
		settingsPage--;
		printSettingsPage();
		setSettingsBlocks();
	}
}

void DiggingBlock::setButtonBlock(int i) {
	buttonBlocks[i].infoPrev = GetBlock(blockPosition + buttonBlocks[i].position);
	SetBlock(blockPosition + buttonBlocks[i].position, buttonBlocks[i].infoBlock);
}

void DiggingBlock::removeButtonBlock(int i) {
	SetBlock(blockPosition + buttonBlocks[i].position, buttonBlocks[i].infoPrev);
}

void DiggingBlock::toggleDigOres() {
	if (digOres) {
		digOres = false;
	}
	else {
		digOres = true;
	}
}

void DiggingBlock::printDigOres() {
	wString message = L"The Quarry will now\n";
	if (digOres) {
		message.append(L"dig ores!");
	}
	else {
		message.append(L"not dig ores!");
	}
	SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0, 75), message, 1, 1.5f);
}

void DiggingBlock::printSettingsPage() {
	if (settingsPage == 1) {
		SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0, 125), L"Change depth", 2);
	}
	else if (settingsPage == 2) {
		SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0, 125), L"Change size (simple)", 2);
	}
	else if (settingsPage == 3) {
		SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0, 125), L"Change size (advanced)", 2);
	}
	else if (settingsPage == 4) {
		SpawnHintText(blockPosition + CoordinateInCentimeters(0, 0, 125), L"Toggle digging of\nores", 2);
	}
}