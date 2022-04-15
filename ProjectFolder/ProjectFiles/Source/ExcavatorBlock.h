#pragma once
#include "GameAPI.h"
class ExcavatorBlock {
private:
	int size;								// Used to define the size of the area (area is size*2+1 x size*2+1).
	CoordinateInBlocks blockPosition;		// The position of the block.
	UniqueID cornerBlockType;				// The type of block to be used as corners.
	bool isDigging;							// Boolean to tell whether digging is in progress.
	int currentDigBlock[3];					// The current block that is being dug out, relative to blockPosition.
	
public:
	// Constructor for ExcavatorBlock.
	ExcavatorBlock(CoordinateInBlocks);
	ExcavatorBlock(int, CoordinateInBlocks, bool, int, int, int);

	/**
	 * Accessor method for the blockPosition field.
	 * 
	 * @return the position of the excavator block.
	*/
	CoordinateInBlocks getBlockPosition();

	/**
	 * Adds the corners of the area that is to be dug out.
	 */
	void addCorners();

	/**
	 * Removes the corners of the area that is to be dug out.
	 */
	void removeCorners();

	/**
	 * Increments the size of the area that is to be dug out.
	 */
	void incrementSize();

	/**
	 * Digs/mines a single block if the block is digging at that moment.
	 */
	bool dig();

	/**
	 * Toggles digging.
	 */
	void toggleDig();

	/**
	 * Updates the currentDigBlock to fit with size.
	 */
	void updateDigBlock();

	/**
	 * Removes all foliage that is on the top layer of Grass blocks.
	 */
	void removeFoliage();

	/**
	 * Writes a vector/list of ExcavatorBlock instances to a file.
	 */
	static void writeExcavatorBlocks(std::ostream&, const std::vector<ExcavatorBlock*>&);
	static void writeExcavatorBlocks(std::ostream&&, const std::vector<ExcavatorBlock*>&);

	static auto readExcavatorBlocks(std::istream& i)->std::vector<ExcavatorBlock*>;
	static auto readExcavatorBlocks(std::istream&& i)->std::vector<ExcavatorBlock*>;
private:
	static bool stob(std::string, bool);
};