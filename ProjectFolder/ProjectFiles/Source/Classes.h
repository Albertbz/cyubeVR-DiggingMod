#pragma once
#include "GameAPI.h"
class ExcavatorBlock {
public:
	int size;
	CoordinateInBlocks position;
	ExcavatorBlock(CoordinateInBlocks position) {
		this->position = position;
		this->size = 2;
		addCorners();
	}

	CoordinateInBlocks getPosition() {
		return this->position;
	}

	void addCorners() {
		SetBlock(position + CoordinateInBlocks(size, size, 0), EBlockType::Wallstone);
		SetBlock(position + CoordinateInBlocks(size, -size, 0), EBlockType::Wallstone);
		SetBlock(position + CoordinateInBlocks(-size, size, 0), EBlockType::Wallstone);
		SetBlock(position + CoordinateInBlocks(-size, -size, 0), EBlockType::Wallstone);
	}

	void removeCorners() {
		SetBlock(position + CoordinateInBlocks(size, size, 0), EBlockType::Air);
		SetBlock(position + CoordinateInBlocks(size, -size, 0), EBlockType::Air);
		SetBlock(position + CoordinateInBlocks(-size, size, 0), EBlockType::Air);
		SetBlock(position + CoordinateInBlocks(-size, -size, 0), EBlockType::Air);
	}

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