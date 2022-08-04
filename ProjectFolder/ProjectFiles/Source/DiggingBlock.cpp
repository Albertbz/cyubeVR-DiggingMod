#include "DiggingBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

DiggingBlock::DiggingBlock(CoordinateInBlocks blockPosition) 
{
	this->length = 5;
	this->width = 5;
	this->depth = 5;
	this->blockPosition = blockPosition;
	this->currentMode = 1;
	this->currentDigBlock = CoordinateInBlocks(0, 0, 0);
	this->cornerBlocks = {};
	this->digOres = false;
	this->digDirection = 0;
	this->justReplaced = false;
	this->canClickLeft = true;
	this->canClickRight = true;
	this->currentHintTextHandle = nullptr;
}

DiggingBlock::DiggingBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, 
						   std::array<Block, 4> cornerBlocks, bool digOres, int digDirection) 
{
	this->length = length;
	this->width = width;
	this->depth = depth;
	this->blockPosition = blockPosition;
	this->currentMode = currentMode;
	this->currentDigBlock = currentDigBlock;
	this->cornerBlocks = cornerBlocks;
	this->digOres = digOres;
	this->digDirection = digDirection;
	
	this->justReplaced = false;
	this->canClickLeft = true;
	this->canClickRight = true;
	this->currentHintTextHandle = nullptr;
}

void DiggingBlock::setCorners() 
{
	cornerBlocks[0].infoPrev = GetAndSetBlock(blockPosition + cornerBlocks[0].position, cornerBlocks[0].infoBlock);
	cornerBlocks[1].infoPrev = GetAndSetBlock(blockPosition + cornerBlocks[1].position, cornerBlocks[1].infoBlock);
	cornerBlocks[2].infoPrev = GetAndSetBlock(blockPosition + cornerBlocks[2].position, cornerBlocks[2].infoBlock);
	cornerBlocks[3].infoPrev = GetAndSetBlock(blockPosition + cornerBlocks[3].position, cornerBlocks[3].infoBlock);
}

void DiggingBlock::removeCorners() 
{
	SetBlock(blockPosition + cornerBlocks[0].position, cornerBlocks[0].infoPrev);
	SetBlock(blockPosition + cornerBlocks[1].position, cornerBlocks[1].infoPrev);
	SetBlock(blockPosition + cornerBlocks[2].position, cornerBlocks[2].infoPrev);
	SetBlock(blockPosition + cornerBlocks[3].position, cornerBlocks[3].infoPrev);
}

void DiggingBlock::incrementSize() 
{
	if (currentMode == 2) {
		incrementLength('u');
		incrementLength('d');
		incrementWidth('l');
		incrementWidth('r');
	}
}

void DiggingBlock::decrementSize() 
{
	if (currentMode == 2) {
		decrementLength('u');
		decrementLength('d');
		decrementWidth('l');
		decrementWidth('r');
	}
}

void DiggingBlock::incrementDepth() 
{
	if (currentMode == 2) {
		if (digDirection != 6 || (digDirection == 6 && blockPosition.Z - depth > 1)) {
			depth++;
		}
	}
}

void DiggingBlock::decrementDepth() 
{
	if (currentMode == 2 && depth > 1) {
		depth--;
	}
}

void DiggingBlock::printDepth() 
{
	std::wstring message = L"Depth: ";
	message.append(std::to_wstring(depth));
	if (digDirection == 6 && blockPosition.Z - depth == 1) {
		message.append(L"\nMax depth reached.");
	}
	printHintText(getHintTextLocation(), message, 1);
}

void DiggingBlock::printSize() 
{
	std::wstring message = L"Size (LxWxD)\n---------------\n" + getSize();
	printHintText(getHintTextLocation(), message, 2);
}

std::wstring DiggingBlock::getSize() 
{
	return std::to_wstring(length) + L"x" + std::to_wstring(width) + L"x" + std::to_wstring(depth);
}

bool DiggingBlock::digSuccess()
{
	BlockInfo currentBlock = GetBlock(blockPosition + currentDigBlock);
	if (diggableBlock(currentBlock)) {
		bool setCorrect = SetBlock(blockPosition + currentDigBlock, EBlockType::Air);

		// If out of loaded chunk and the block therefore is not set (SetBlock returns false), do not continue digging.
		if (!setCorrect) {
			return false;
		}

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
	else if (!currentBlock.IsValid()) {
		return false;
	}
	return true;
}

void DiggingBlock::toggleDigging() 
{
	if (currentMode == 1) {
		justReplaced = true;
		setOnBlock();
		currentMode = 3;
	}
	else if (currentMode == 3) {
		justReplaced = true;
		setOffBlock();
		currentMode = 1;
	}
}

void DiggingBlock::toggleSettings() 
{
	if (currentMode == 1) {
		justReplaced = true;
		setSettingsBlock();
		setCorners();
		currentMode = 2;
	}
	else if (currentMode == 2) {
		removeCorners();
		justReplaced = true;
		setOffBlock();
		printHintText(getHintTextLocation(), L"Settings saved!\nIt will take\n" + timeToDig() + L"\nto dig out the " + getSize() + L" hole.", 5, 1, 2);
		currentMode = 1;
	}
}

void DiggingBlock::finishedDigging() 
{
	if (currentMode == 3) {
		justReplaced = true;
		setNormalBlock();
		SpawnHintTextAdvanced(GetPlayerLocationHead() + GetPlayerViewDirection() * 50, L"A Digging Block at the location\nX=" + std::to_wstring(blockPosition.X / 2) + L", Y=" + std::to_wstring(blockPosition.Y / 2) + L", Z=" + std::to_wstring(blockPosition.Z / 2) + L"\nhas finished digging.", 10);
		currentMode = 4;
	}
}

void DiggingBlock::destroy() 
{
	if (currentMode == 2) {
		removeCorners();
	}
	currentMode = 0;
}

std::wstring DiggingBlock::timeToDig() 
{
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

bool DiggingBlock::diggableBlock(BlockInfo block) 
{
	bool diggable = false;
	if (block.Type != EBlockType::ModBlock) {
		diggable = block.Type == EBlockType::Stone || block.Type == EBlockType::Dirt || block.Type == EBlockType::Grass || 
				   block.Type == EBlockType::Sand || block.Type == EBlockType::StoneMined;
		if (!diggable && digOres) {
			diggable = block.Type == EBlockType::Ore_Coal || block.Type == EBlockType::Ore_Copper || block.Type == EBlockType::Ore_Gold ||
					   block.Type == EBlockType::Ore_Iron || block.Type == EBlockType::CrystalBlock;
		}
	}
	return diggable;
}

void DiggingBlock::toggleDigOres() 
{
	if (digOres) {
		digOres = false;
	}
	else {
		digOres = true;
	}
}

void DiggingBlock::printDigOres() 
{
	wString message = L"The block will now\n";
	if (digOres) {
		message.append(L"dig ores!");
	}
	else {
		message.append(L"not dig ores!");
	}
	printHintText(getHintTextLocation(), message, 2);
}

CoordinateInBlocks DiggingBlock::getPlayerDirection()
{
	CoordinateInCentimeters playerLocation = GetPlayerLocation();
	CoordinateInCentimeters blockPositionCm = CoordinateInCentimeters(blockPosition);
	int64_t xDifference = std::abs(playerLocation.X - blockPositionCm.X);
	int64_t yDifference = std::abs(playerLocation.Y - blockPositionCm.Y);
	if (playerLocation.X > blockPositionCm.X + 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		return CoordinateInBlocks(1, 0, 0);
	}
	else if (playerLocation.X < blockPositionCm.X - 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		return CoordinateInBlocks(-1, 0, 0);
	}
	else if (playerLocation.Y > blockPositionCm.Y + 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		return CoordinateInBlocks(0, 1, 0);
	}
	else if (playerLocation.Y < blockPositionCm.Y - 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		return CoordinateInBlocks(0, -1, 0);
	}
}

bool DiggingBlock::isCornerBlock(CoordinateInBlocks blockPos)
{
	return blockPosition == blockPos - cornerBlocks[0].position || 
		   blockPosition == blockPos - cornerBlocks[1].position ||
		   blockPosition == blockPos - cornerBlocks[2].position ||
		   blockPosition == blockPos - cornerBlocks[3].position;
}

void DiggingBlock::runCheck()
{
	CoordinateInCentimeters fingerTipLocationLeft = GetIndexFingerTipLocation(true);
	CoordinateInCentimeters fingerTipLocationRight = GetIndexFingerTipLocation(false);
	CoordinateInCentimeters positionCm = CoordinateInCentimeters(blockPosition);

	clickCheck(fingerTipLocationLeft, canClickLeft, positionCm, true);
	clickCheck(fingerTipLocationRight, canClickRight, positionCm, false);
}

void DiggingBlock::clickRegister(CoordinateInCentimeters fingerLocation, bool leftHand)
{
	switch (currentMode) {
	case 1: // Normal/Off
		if (isBetween(std::pair(20, 20), std::pair(31, 31), fingerLocation)) {
			toggleDigging();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(8, 21), std::pair(17, 30), fingerLocation) || 
				 isBetween(std::pair(21, 8), std::pair(30, 17), fingerLocation) ||
				 isBetween(std::pair(21, 34), std::pair(30, 43), fingerLocation) ||
				 isBetween(std::pair(34, 21), std::pair(43, 30), fingerLocation)) {
			
			toggleSettings();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		break;
	case 2: // Settings
		if (isBetween(std::pair(20, 20), std::pair(31, 31), fingerLocation)) {
			toggleSettings();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(35, 7), std::pair(42, 14), fingerLocation)) {
			incrementSize();
			printSize();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(35, 16), std::pair(42, 23), fingerLocation)) {
			decrementSize();
			printSize();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(39, 28), std::pair(42, 35), fingerLocation)) {
			incrementDepth();
			printDepth();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(35, 28), std::pair(38, 31), fingerLocation)) {
			for (int i = 0; i < 5; i++) {
				incrementDepth();
			}
			printDepth();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(35, 32), std::pair(38, 35), fingerLocation)) {
			for (int i = 0; i < 10; i++) {
				incrementDepth();
			}
			printDepth();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(39, 37), std::pair(42, 44), fingerLocation)) {
			decrementDepth();
			printDepth();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(35, 37), std::pair(38, 40), fingerLocation)) {
			for (int i = 0; i < 5; i++) {
				decrementDepth();
			}
			printDepth();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(35, 41), std::pair(38, 44), fingerLocation)) {
			for (int i = 0; i < 10; i++) {
				decrementDepth();
			}
			printDepth();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(9, 37), std::pair(16, 44), fingerLocation)) {
			toggleDigOres();
			printDigOres();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(13, 7), std::pair(15, 12), fingerLocation)) {
			incrementWidth('l');
			printSize();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(16, 7), std::pair(18, 12), fingerLocation)) {
			decrementWidth('l');
			printSize();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(16, 19), std::pair(18, 24), fingerLocation)) {
			incrementWidth('r');
			printSize();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(13, 19), std::pair(15, 24), fingerLocation)) {
			decrementWidth('r');
			printSize();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(19, 13), std::pair(24, 15), fingerLocation)) {
			incrementLength('u');
			printSize();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(19, 16), std::pair(24, 18), fingerLocation)) {
			decrementLength('u');
			printSize();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(7, 16), std::pair(12, 18), fingerLocation)) {
			incrementLength('d');
			printSize();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(7, 13), std::pair(12, 15), fingerLocation)) {
			decrementLength('d');
			printSize();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		break;
	case 3: // Digging/On
		if (isBetween(std::pair(20, 20), std::pair(31, 31), fingerLocation)) {
			toggleDigging();
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		break;
	default:
		break;
	}
}

void DiggingBlock::printHintText(CoordinateInCentimeters location, std::wstring text, float duration, float sizeMul, float sizeMulVer, float fontMul)
{
	// Remove the old hint text with the saved handle, and spawn the
	// new one as well as save its handle.

	DestroyHintText(currentHintTextHandle);
	currentHintTextHandle = SpawnHintTextAdvanced(location, text, duration, sizeMul, sizeMulVer, fontMul);
}