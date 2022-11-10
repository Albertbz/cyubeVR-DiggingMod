#include "QuarryBlock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

QuarryBlock::QuarryBlock(CoordinateInBlocks blockPosition) 
	: DiggingBlock(blockPosition) 
{
	this->digDirection = 6;

	this->corners[0] = CoordinateInBlocks(2, -2, 0);
	this->corners[1] = CoordinateInBlocks(2, 2, 0);
	this->corners[2] = CoordinateInBlocks(-2, -2, 0);
	this->corners[3] = CoordinateInBlocks(-2, 2, 0);

	if (blockPosition.Z < 6) {
		if (blockPosition.Z == 1) {
			destroy();
			SpawnHintTextAdvanced(blockPosition + CoordinateInBlocks(0, 0, 1), L"Cannot place block here.", 3);
			return;
		}
		this->depth = blockPosition.Z - 1;
	}

	resetDigBlock();
	toggleSettings();
}

QuarryBlock::QuarryBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, std::array<CoordinateInBlocks,4> corners, bool digOres, int digDirection)
	: DiggingBlock(length, width, depth, blockPosition, currentMode, currentDigBlock, corners, digOres, digDirection) 
{
	if (this->blockPosition.Z - this->depth < 1) {
		this->depth = this->blockPosition.Z - 1;
		if (this->currentDigBlock.Z < -this->depth) {
			this->currentDigBlock.Z = -this->depth;
		}
	}
}

void QuarryBlock::dig() 
{
	if (currentMode == 3) {
		if (digSuccess()) {
			currentDigBlock.X--;
			if (currentDigBlock.X < corners[2].X) {
				currentDigBlock.X = corners[0].X;
				currentDigBlock.Y--;
				if (currentDigBlock.Y < corners[0].Y) {
					currentDigBlock.Y = corners[1].Y;
					currentDigBlock.Z--;
					if (currentDigBlock.Z < -depth) {
						currentDigBlock.Z = -1;
						finishedDigging();
					}
				}
			}
		}
	}
}

void QuarryBlock::incrementLength(char block) 
{
	if (currentMode == 2) {
		removeAreaSelection();
		length++;
		if (block == 'u') {
			corners[0].X++;
			corners[1].X++;
		}
		else if (block == 'd') {
			corners[2].X--;
			corners[3].X--;
		}
		else {
			length--;
		}
		setAreaSelection();
		resetDigBlock();
	}
}

void QuarryBlock::decrementLength(char block) 
{
	if (currentMode == 2 && length > 3) {
		removeAreaSelection();
		length--;
		if (block == 'u' && corners[0].X > 1) {
			corners[0].X--;
			corners[1].X--;
		}
		else if (block == 'd' && corners[2].X < -1) {
			corners[2].X++;
			corners[3].X++;
		}
		else {
			length++;
		}
		setAreaSelection();
		resetDigBlock();
	}
}

void QuarryBlock::incrementWidth(char block) 
{
	if (currentMode == 2) {
		removeAreaSelection();
		width++;
		if (block == 'r') {
			corners[1].Y++;
			corners[3].Y++;
		}
		else if (block == 'l') {
			corners[0].Y--;
			corners[2].Y--;
		}
		else {
			width--;
		}
		setAreaSelection();
		resetDigBlock();
	}
}

void QuarryBlock::decrementWidth(char block) 
{
	if (currentMode == 2 && width > 3) {
		removeAreaSelection();
		width--;
		if (block == 'r' && corners[1].Y > 1) {
			corners[1].Y--;
			corners[3].Y--;
		}
		else if (block == 'l' && corners[0].Y < -1) {
			corners[0].Y++;
			corners[2].Y++;
		}
		else {
			width++;
		}
		setAreaSelection();
		resetDigBlock();
	}
}

void QuarryBlock::resetDigBlock() 
{
	currentDigBlock.X = corners[0].X;
	currentDigBlock.Y = corners[1].Y;
	currentDigBlock.Z = -1;
}

void QuarryBlock::clickCheck(CoordinateInCentimeters fingerLocation, bool& canClick, CoordinateInCentimeters positionCm, bool leftHand)
{
	if (canClick) {
		if (fingerLocation.Z < positionCm.Z + 25) {
			clickRegister(fingerLocation, leftHand);
			canClick = false;
		}
	}
	else {
		if (fingerLocation.Z >= positionCm.Z + 25) {
			canClick = true;
		}
	}
}

CoordinateInCentimeters QuarryBlock::getCorner()
{
	return blockPosition + CoordinateInCentimeters(-25, -25, 25);
}

bool QuarryBlock::isBetween(std::pair<int, int> bottomLeft, std::pair<int, int> topRight, CoordinateInCentimeters fingerPos)
{
	CoordinateInCentimeters corner = getCorner();

	return fingerPos.X >= corner.X + bottomLeft.first - 1 && fingerPos.X <= corner.X + topRight.first - 1 && fingerPos.Y >= corner.Y + bottomLeft.second && fingerPos.Y <= corner.Y + topRight.second;
}

void QuarryBlock::setOffBlock()
{
	SetBlock(blockPosition, quarryOffBlockID);
}

void QuarryBlock::setOnBlock()
{
	SetBlock(blockPosition, quarryOnBlockID);
}

void QuarryBlock::setSettingsBlock()
{
	SetBlock(blockPosition, quarrySetBlockID);
}

void QuarryBlock::setNormalBlock()
{
	SetBlock(blockPosition, quarryBlockID);
}

CoordinateInCentimeters QuarryBlock::getHintTextLocationHelper(CoordinateInCentimeters location, int height, int radius)
{
	CoordinateInCentimeters loc = location + CoordinateInCentimeters(0, 0, height); // The location to use for the hint text that is spawned - this being the default.

	// Calculate where on a circle with the formula x^2+y^2=radius^2 to spawn the hint text,
	// with the default vector being the location of the player relative to the center - 
	// but if on top of the location block, use the direction the player is looking instead
	// because it can look pretty funky otherwise.
	CoordinateInCentimeters playerLoc = GetPlayerLocationHead();
	CoordinateInCentimeters directionVector = location - playerLoc; // Default vector to use for calculation.

	if (playerLoc.X >= location.X - 25 && playerLoc.X <= location.X + 25 && playerLoc.Y >= location.Y - 25 && playerLoc.Y <= location.Y + 25) {
		directionVector = CoordinateInCentimeters(GetPlayerViewDirection() * 100); // Vector to use if on top of location block.
	}

	// Variables for the quadratic equation.
	int64_t a = directionVector.X * directionVector.X + directionVector.Y * directionVector.Y;
	int64_t b = 2 * (playerLoc.X * directionVector.X + playerLoc.Y * directionVector.Y - location.X * directionVector.X - location.Y * directionVector.Y);
	int64_t c = location.X * location.X + location.Y * location.Y + playerLoc.X * playerLoc.X + playerLoc.Y * playerLoc.Y - 2 * location.X * playerLoc.X - 2 * location.Y * playerLoc.Y - (radius * radius);

	int64_t d = b * b - 4 * a * c; // The discriminant.

	if (d > 0) { // Two intersections.
		double t1 = (-b + sqrt(d)) / (2 * a);
		double t2 = (-b - sqrt(d)) / (2 * a);
		double t = std::max(t1, t2);
		double x = playerLoc.X + t * directionVector.X;
		double y = playerLoc.Y + t * directionVector.Y;
		loc.X = (int)round(x);
		loc.Y = (int)round(y);
	}
	else if (d == 0) { // One intersection - just use the point found. Should technically never happen.
		double t = (-b) / (2.0 * a);
		double x = playerLoc.X + t * directionVector.X;
		double y = playerLoc.Y + t * directionVector.Y;
		loc.X = (int)round(x);
		loc.Y = (int)round(y);
	}
	// No intersections can also technically never happen because either the player is outside the block,
	// where the center is then used to calculate the vector to use (meaning there will always be two intersections),
	// or the player is inside the block, where the direction the player is looking is used as the vector,
	// meaning there will always be an intersection because the player is always looking out at the circle.

	return loc;
}

CoordinateInCentimeters QuarryBlock::getHintTextLocation()
{
	return getHintTextLocationHelper(CoordinateInCentimeters(blockPosition), 50, 40);
}


void QuarryBlock::setAreaSelection()
{
	for (int i = blockPosition.X + corners[2].X; i <= blockPosition.X + corners[1].X; i++) {
		for (int j = blockPosition.Y + corners[2].Y; j <= blockPosition.Y + corners[1].Y; j++) {
			SpawnBPModActor(CoordinateInBlocks(i, j, blockPosition.Z), L"ParticleActors", L"AreaSelectionParticle-Z");
		}
	}
}