#pragma once
#include "GameAPI.h"
class ExcavatorBlock {
public:
	int size; // The size of the area (area is size*2+1 x size*2+1)
	CoordinateInBlocks position; // The position of the block
	EBlockType cornerBlockType; // The type of block to be used as corners
	ExcavatorBlock(CoordinateInBlocks position) {
		this->position = position;
		this->size = 2;
		this->cornerBlockType = EBlockType::Wallstone;
		addCorners();
	}

	/*
	Accessor method for the position field.
	*/
	CoordinateInBlocks getPosition() {
		return this->position;
	}

	/*
	Adds the corners of the area that is to be dug out.
	*/
	void addCorners() {
		SetBlock(position + CoordinateInBlocks(size, size, 0), cornerBlockType);
		SetBlock(position + CoordinateInBlocks(size, -size, 0), cornerBlockType);
		SetBlock(position + CoordinateInBlocks(-size, size, 0), cornerBlockType);
		SetBlock(position + CoordinateInBlocks(-size, -size, 0), cornerBlockType);
	}

	/*
	Removes the corners of the area that is to be dug out.
	*/
	void removeCorners() {
		SetBlock(position + CoordinateInBlocks(size, size, 0), EBlockType::Air);
		SetBlock(position + CoordinateInBlocks(size, -size, 0), EBlockType::Air);
		SetBlock(position + CoordinateInBlocks(-size, size, 0), EBlockType::Air);
		SetBlock(position + CoordinateInBlocks(-size, -size, 0), EBlockType::Air);
	}

	/*
	Increments the size of the area that is to be dug out.
	*/
	void incrementSize() {
		removeCorners();
		if (size >= 10) {
			size = 2;
		}
		else {
			size += 1;
		}
		addCorners();
	}
};