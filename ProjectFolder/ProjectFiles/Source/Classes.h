#pragma once
#include "GameAPI.h"
class ExcavatorBlock {
public:
	int size;
	CoordinateInBlocks position;
	ExcavatorBlock(CoordinateInBlocks position) {
		this->position = position;
		this->size = 2;
	}

	CoordinateInBlocks getPosition() {
		return this->position;
	}

	void incrementSize() {
		SetBlock(position + CoordinateInBlocks(size, size, 0), EBlockType::Air);
		SetBlock(position + CoordinateInBlocks(size, -size, 0), EBlockType::Air);
		SetBlock(position + CoordinateInBlocks(-size, size, 0), EBlockType::Air);
		SetBlock(position + CoordinateInBlocks(-size, -size, 0), EBlockType::Air);
		if (size >= 10) {
			size = 2;
		}
		else {
			size += 1;
		}
		SetBlock(position + CoordinateInBlocks(size, size, 0), EBlockType::Wallstone);
		SetBlock(position + CoordinateInBlocks(size, -size, 0), EBlockType::Wallstone);
		SetBlock(position + CoordinateInBlocks(-size, size, 0), EBlockType::Wallstone);
		SetBlock(position + CoordinateInBlocks(-size, -size, 0), EBlockType::Wallstone);
	}
};