#pragma once
#include "GameAPI.h"

const int buttonBlocksAmount = 19;

struct Block {
	CoordinateInBlocks position = CoordinateInBlocks(0, 0, 0);	// The position of the block (possibly in relation to another block).
	BlockInfo infoBlock = BlockInfo();							// The BlockInfo for the block.
	BlockInfo infoPrev = BlockInfo();							// The BlockInfo for the previous block, a.k.a., the original one.
};

template <typename T>
void writeBlocks(std::ostream& file, const std::vector<T>& blocks) {
	for (auto b : blocks) {
		file << b.length << ';' << b.width << ';' << b.depth << ';' << b.blockPosition.X << ';' << b.blockPosition.Y << ';' << b.blockPosition.Z << ';'
			<< b.currentMode << ';' << b.currentDigBlock.X << ';' << b.currentDigBlock.Y << ';' << b.currentDigBlock.Z << ';';
		for (const auto& i : b.cornerBlocks) {
			file << i.position.X << ';' << i.position.Y << ';' << i.position.Z << ';' << i.infoBlock.CustomBlockID << ';';;
			if (i.infoPrev.Type == EBlockType::ModBlock) {
				file << 'C' << ';' << (int)i.infoPrev.CustomBlockID << ';';
			}
			else {
				file << 'N' << ';' << (int)i.infoPrev.Type << ';';
			}
		}
		for (const auto& i : b.buttonBlocks) {
			file << i.position.X << ';' << i.position.Y << ';' << i.position.Z << ';' << i.infoBlock.CustomBlockID << ';';
			if (i.infoPrev.Type == EBlockType::ModBlock) {
				file << 'C' << ';' << (int)i.infoPrev.CustomBlockID << ';';
			}
			else {
				file << 'N' << ';' << (int)i.infoPrev.Type << ';';
			}
		}
		file << b.settingsPage << ';' << b.digOres << ';' << b.digDirection << ';';
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

		CoordinateInBlocks currentDigBlock;
		pos = line.find_first_of(';', pos + 1);
		currentDigBlock.X = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		currentDigBlock.Y = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		currentDigBlock.Z = std::stoi(std::string{ line, pos + 1 });

		std::array<Block, 4> cornerBlocks;
		for (auto &i : cornerBlocks) {
			pos = line.find_first_of(';', pos + 1);
			i.position.X = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.position.Y = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.position.Z = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.infoBlock = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			if (std::string{ line, pos + 1 } == "C") {
				pos = line.find_first_of(';', pos + 1);
				i.infoPrev = std::stoi(std::string{ line, pos + 1 });
			}
			else {
				pos = line.find_first_of(';', pos + 1);
				i.infoPrev = (EBlockType)std::stoi(std::string{ line, pos + 1 });
			}
		}

		std::array<Block, buttonBlocksAmount> buttonBlocks;
		for (auto &i : buttonBlocks) {
			pos = line.find_first_of(';', pos + 1);
			i.position.X = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.position.Y = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.position.Z = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.infoBlock = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			if (std::string{ line, pos + 1 } == "C") {
				pos = line.find_first_of(';', pos + 1);
				i.infoPrev = std::stoi(std::string{ line, pos + 1 });
			}
			else {
				pos = line.find_first_of(';', pos + 1);
				i.infoPrev = (EBlockType)std::stoi(std::string{ line, pos + 1 });
			}
		}

		pos = line.find_first_of(';', pos + 1);
		int settingsPage = std::stoi(std::string{ line, pos + 1 });

		pos = line.find_first_of(';', pos + 1);
		bool digOres = std::stoi(std::string{ line, pos + 1 });

		pos = line.find_first_of(';', pos + 1);
		int digDirection = std::stoi(std::string{ line, pos + 1 });

		blocks.push_back(T(length, width, depth, blockPosition, currentMode, currentDigBlock, cornerBlocks, buttonBlocks, settingsPage, digOres, digDirection));
	}

	return blocks;
}

template <typename T>
auto readBlocks(std::istream&& file) -> std::vector<T> {
	return readBlocks<T>(file);
}
