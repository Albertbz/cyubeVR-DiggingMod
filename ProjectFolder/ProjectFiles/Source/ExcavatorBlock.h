#pragma once
#include "GameAPI.h"
class ExcavatorBlock {
private:
	int size;								// Used to define the size of the area (area is size*2+1 x size*2+1).
	int depth;								// How deep the hole will be.
	CoordinateInBlocks blockPosition;		// The position of the block.
	UniqueID cornerBlockType;				// The type of block to be used as corners.
	int currentMode;						// The current mode the excavator block is in. 1 = normal, 2 = settings, 3 = digging, 4 = destroyed.
	int currentDigBlock[3];					// The current block that is being dug out, relative to blockPosition.
	
public:
	// Constructor for ExcavatorBlock.
	ExcavatorBlock(CoordinateInBlocks);
	ExcavatorBlock(int, int, CoordinateInBlocks, int, int, int, int);

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
	 * Decrement the size of the area that is to be dug out. 
	 */
	void decrementSize();

	/**
	 * Increments the depth of the hole to be dug out.
	 */
	void incrementDepth();

	/**
	 * Decrements the depth of the hole to be dug out.
	 */
	void decrementDepth();

	/**
	 * Digs/mines a single block if the block is digging at that moment.
	 */
	bool dig();

	/**
	 * Toggles digging mode.
	 */
	void toggleDig();

	/**
	 * Toggles settings mode. 
	 */
	void toggleSettings();

	/**
	 * Sets or removes all of the blocks needed for the settings mode.
  	 */
	void showSettings(bool);

	/**
	 * Sets or removes all of the blocks needed for the normal mode.
	 */
	void showNormal(bool);

	/**
	 * Removes and stops everything.
	 */
	void destroy();

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
};