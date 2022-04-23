#pragma once
#include "GameAPI.h"
struct Block {
	CoordinateInBlocks position;
	BlockInfo infoPrev;
	BlockInfo infoBlock;
};

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

		std::array<int, 3> currentDigBlock{};
		for (auto &i : currentDigBlock) {
			pos = line.find_first_of(';', pos + 1);
			i = std::stoi(std::string{ line, pos + 1 });
		}

		std::array<Block, 4> cornerBlocks;
		for (auto &i : cornerBlocks) {
			pos = line.find_first_of(';', pos + 1);
			i.position.X = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.position.Y = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.position.Z = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			if (std::string{ line, pos + 1 } == "C") {
				pos = line.find_first_of(';', pos + 1);
				i.info = std::stoi(std::string{ line, pos + 1 });
			}
			else {
				pos = line.find_first_of(';', pos + 1);
				i.info = (EBlockType)std::stoi(std::string{ line, pos + 1 });
			}
		}

		blocks.push_back(T(length, width, depth, blockPosition, currentMode, currentDigBlock, cornerBlocks));
	}

	return blocks;
}

template <typename T>
auto readBlocks(std::istream&& file) -> std::vector<T> {
	return readBlocks<T>(file);
}

template <typename T>
void writeBlocks(std::ostream& file, const std::vector<T>& blocks) {
	for (auto b : blocks) {
		file << b.length << ';' << b.width << ';' << b.depth << ';' << b.blockPosition.X << ';' << b.blockPosition.Y << ';' << b.blockPosition.Z << ';'
			<< b.currentMode << ';';
		for (auto i : b.currentDigBlock) {
			file << i << ';';
		}
		for (auto i : b.cornerBlocks) {
			file << i.position.X << ';' << i.position.Y << ';' << i.position.Z << ';';
			if (i.info.Type == EBlockType::ModBlock) {
				file << 'C' << ';' << (int)i.info.CustomBlockID << '; ';
			}
			else {
				file << "N" << ';' << (int)i.info.Type << ';';
			}
		}
		file << '\n';
	}
}

template <typename T>
void writeBlocks(std::ostream&& file, const std::vector<T>& blocks) {
	writeBlocks<T>(file, blocks);
}
