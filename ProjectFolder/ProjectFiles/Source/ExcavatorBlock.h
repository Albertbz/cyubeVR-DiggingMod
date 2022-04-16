#pragma once
#include "GameAPI.h"
class ExcavatorBlock {
private:
	int size;								// Used to define the size of the area (area is size*2+1 x size*2+1).
	int depth;								// How deep the hole will be.
	CoordinateInBlocks blockPosition;		// The position of the block.
	UniqueID cornerBlockType;				// The type of block to be used as corners.
	int currentMode;						// The current mode the excavator block is in. 0 = destroyed, 1 = normal, 2 = settings, 3 = digging, 4 = finished digging.
	int currentDigBlock[3];					// The current block that is being dug out, relative to blockPosition.
	
public:
	// Constructor for ExcavatorBlock.
	/**
	 * Constructor for ExcavatorBlock used when a new excavator
	 * block is placed down.
	 * 
	 * @param blockPosition The position of the excavator block.
	 */
	ExcavatorBlock(CoordinateInBlocks blockPosition);

	/**
	 * Constructor for ExcavatorBlock used when loading in 
	 * the file containing all currently placed excavator
	 * blocks.
	 * 
	 * @param size The size of the area to be dug out.
	 * @param depth How deep the hole will be.
	 * @param blockPosition The position of the excavator block.
	 * @param currentMode The current mode of the excavator block.
	 * @param currentDigBlockX The X coordinate for the block currently
	 *						   being dug out.
	 * @param currentDigBlockY The Y coordinate for the block currently
	 *						   being dug out.
	 * @param currentDigBlockZ The Z coordinate for the block currently
	 *						   being dug out.
	 */
	ExcavatorBlock(int size, int depth, CoordinateInBlocks blockPosition, int currentMode, int currentDigBlockX, int currentDigBlockY, int currentDigBlockZ);

	/**
	 * Accessor method for the blockPosition field.
	 * 
	 * @return The position of the excavator block.
	*/
	CoordinateInBlocks getBlockPosition();

	// Adds the corners of the area that is to be dug out.
	void addCorners();

	// Removes the corners of the area that is to be dug out.
	void removeCorners();

	// Increments the size of the area that is to be dug out.
	void incrementSize();

	// Decrement the size of the area that is to be dug out.
	void decrementSize();

	//Increments the depth of the hole to be dug out.
	void incrementDepth();

	// Decrements the depth of the hole to be dug out.
	void decrementDepth();

	// Digs/mines a single block if the excavator block is digging at that moment.
	bool dig();

	// Toggles digging mode.
	void toggleDigging();

	// Toggles settings mode.
	void toggleSettings();

	/**
	 * Sets or removes all of the blocks needed for the settings mode.
	 * 
	 * @param show Whether to show the blocks.
  	 */
	void showSettings(bool show);

	/**
	 * Sets or removes all of the blocks needed for the normal mode.
	 * 
	 * @param show Whether to show the blocks.
	 */
	void showNormal(bool show);

	/**
	 * Sets or removes all of the blocks needed for the finished mode.
	 *
	 * @param show Whether to show the blocks.
	 */
	void showFinished(bool show);

	/**
	 * Sets or removes all of the blocks needed for the digging mode.
	 *
	 * @param show Whether to show the blocks.
	 */
	void showDigging(bool show);

	// Removes and stops everything.
	void destroy();

	// Updates the currentDigBlock to fit with size.
	void updateDigBlock();

	// Removes all foliage that is on the top layer of Grass blocks.
	void removeFoliage();

	/**
	 * Writes a vector/list of ExcavatorBlock instances to a file.
	 * 
	 * @param file The file to write all of the excavator blocks to.
	 * @param excavatorBlocks The vector containing all of the excavator
	 *						  blocks currently in the world.
	 */
	static void writeExcavatorBlocks(std::ostream& file, const std::vector<ExcavatorBlock*>& excavatorBlocks);

	/**
	 * Writes a vector/list of ExcavatorBlock instances to a file.
	 *
	 * @param file The file to write all of the excavator blocks to.
	 * @param excavatorBlocks The vector containing all of the excavator
	 *						  blocks currently in the world.
	 */
	static void writeExcavatorBlocks(std::ostream&& file, const std::vector<ExcavatorBlock*>& excavatorBlocks);


	/**
	 * Reads a file of instances of ExcavatorBlock and returns it as a vector.
	 * 
	 * @param file The file to read.
	 * 
	 * @return A vector containing all of the excavator blocks currently
	 *		   in the world.
	 */
	static auto readExcavatorBlocks(std::istream& file)->std::vector<ExcavatorBlock*>;

	/**
	 * Reads a file of instances of ExcavatorBlock and returns it as a vector.
	 *
	 * @param file The file to read.
	 *
	 * @return A vector containing all of the excavator blocks currently
	 *		   in the world.
	 */
	static auto readExcavatorBlocks(std::istream&& file)->std::vector<ExcavatorBlock*>;
};