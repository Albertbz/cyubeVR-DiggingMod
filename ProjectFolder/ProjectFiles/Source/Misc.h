#pragma once
#include "GameAPI.h"

struct Block {
	CoordinateInBlocks position = CoordinateInBlocks(0, 0, 0);	// The position of the block (possibly in relation to another block).
	BlockInfo infoBlock = BlockInfo();							// The BlockInfo for the block.
	BlockInfo infoPrev = BlockInfo();							// The BlockInfo for the previous block, a.k.a., the original one.
};

template <typename T>
void writeBlocks(std::ostream& file, const std::vector<T>& blocks) {
	// Go through all blocks
	for (auto b : blocks) {
		// Save length, width, depth, block position, current mode, current dig block position
		file << b.length << ';' << b.width << ';' << b.depth << ';' << b.blockPosition.X << ';' << b.blockPosition.Y << ';' << b.blockPosition.Z << ';'
			<< b.currentMode << ';' << b.currentDigBlock.X << ';' << b.currentDigBlock.Y << ';' << b.currentDigBlock.Z << ';';

		// Save the corner positions
		for (const auto& i : b.corners) {
			file << i.X << ';' << i.Y << ';' << i.Z << ';';
		}

		// Save whether to dig ores, dig direction
		file << b.digOres << ';' << b.digDirection;
		file << '\n';
	}
}

template <typename T>
void writeBlocks(std::ostream&& file, const std::vector<T>& blocks) {
	writeBlocks<T>(file, blocks);
}

template <typename T> 
auto readBlocks(std::istream& file)->std::vector<T> {
	std::vector<T> blocks;
	auto line = std::string{};

	while (std::getline(file, line)) {
		// Get the length
		size_t pos = line.find_first_of(';');
		if (pos == std::string::npos) goto cancel;
		int length = std::stoi(std::string{ line, 0, pos });

		// Get the width
		int width = std::stoi(std::string{ line, pos + 1 });

		// Get the depth
		pos = line.find_first_of(';', pos + 1);
		if (pos == std::string::npos) goto cancel;
		int depth = std::stoi(std::string{ line, pos + 1 });

		// Get the block position
		CoordinateInBlocks blockPosition;
		pos = line.find_first_of(';', pos + 1);
		if (pos == std::string::npos) goto cancel;
		blockPosition.X = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		if (pos == std::string::npos) goto cancel;
		blockPosition.Y = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		if (pos == std::string::npos) goto cancel;
		blockPosition.Z = std::stoi(std::string{ line, pos + 1 });

		// Get the current mode
		pos = line.find_first_of(';', pos + 1);
		if (pos == std::string::npos) goto cancel;
		int currentMode = std::stoi(std::string{ line, pos + 1 });

		// Get the current dig block
		CoordinateInBlocks currentDigBlock;
		pos = line.find_first_of(';', pos + 1);
		if (pos == std::string::npos) goto cancel;
		currentDigBlock.X = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		if (pos == std::string::npos) goto cancel;
		currentDigBlock.Y = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		if (pos == std::string::npos) goto cancel;
		currentDigBlock.Z = std::stoi(std::string{ line, pos + 1 });

		// Get the corners
		std::array<CoordinateInBlocks, 4> corners;
		for (auto &i : corners) {
			pos = line.find_first_of(';', pos + 1);
			if (pos == std::string::npos) goto cancel;
			i.X = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			if (pos == std::string::npos) goto cancel;
			i.Y = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			if (pos == std::string::npos) goto cancel;
			i.Z = std::stoi(std::string{ line, pos + 1 });
		}

		// Get whether to dig ores
		pos = line.find_first_of(';', pos + 1);
		if (pos == std::string::npos) goto cancel;
		bool digOres = std::stoi(std::string{ line, pos + 1 });

		// Get dig direction
		pos = line.find_first_of(';', pos + 1);
		if (pos == std::string::npos) goto cancel;
		int digDirection = std::stoi(std::string{ line, pos + 1 });

		// Add the digging block to the blocks
		pos = line.find_first_of(';', pos + 1);
		if (pos != std::string::npos) goto cancel;
		blocks.push_back(T(length, width, depth, blockPosition, currentMode, currentDigBlock, corners, digOres, digDirection));
	}
	cancel:

	return blocks;
}

template <typename T>
auto readBlocks(std::istream&& file) -> std::vector<T> {
	return readBlocks<T>(file);
}
