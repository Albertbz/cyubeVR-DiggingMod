#pragma once
#include "GameAPI.h"
#include <array>

class DiggingBlock {
protected:
	int size;									// Used to define the size of the area (area is size*2+1 x size*2+1).
	int depth;									// How deep the hole will be.
	CoordinateInBlocks blockPosition;			// The position of the block.
	int currentMode;							// The current mode the Quarry block is in. 0 = destroyed, 1 = normal, 2 = settings, 3 = digging, 4 = finished digging.
	std::array<int, 3> currentDigBlock;			// The current block that is being dug out, relative to blockPosition.
	std::array<int, 4> cornerBlockTypes;		// The types of the four cornor blocks before they were changed.
	enum {
		mark1BlockID = 430459851,				// Enum with all of the blocks used for methods.
		mark2BlockID = 430459852,
		mark3BlockID = 430459853,
		mark4BlockID = 430459854,
		setBlockID = 1473066958,
		upBlockID = 1473066956,
		downBlockID = 1473066955,
		inBlockID = 1473066957,
		outBlockID = 1473066954,
		checkBlockID = 1473066960,
		exclBlockID = 1473066959,
		crossBlockID = 1473066961
	};
public:
	/**
	* Constructor for DiggingBlock for when a new block is
	* placed down.
	* 
	* @param blockPosition The position of the block.
	*/
	DiggingBlock(CoordinateInBlocks blockPosition);

	/**
	* Constructor for DiggingBlock for when loading in 
	* the file containing all currently placed DiggingBlocks.
	* 
	* @param size The size of the area to be dug out.
	* @param depth How deep the hole will be.
	* @param blockPosition The position of the DiggingBlock.
	* @param currentMode The current mode of the DiggingBlock.
	* @param currentDigBlock The coordinates for the block currently being dug out.
	* @param cornerBlockTypes The types of the corner blocks.
	*/
	DiggingBlock(int size, int depth, CoordinateInBlocks blockPosition, int currentMode, std::array<int, 3> currentDigBlock, std::array<int, 4> cornerBlockTypes);
	
	/**
	* Accessor method for the blockPosition field.
	* 
	* @return The position of the DiggingBlock.
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

	// Spawns a Text with the current depth.
	void printDepth();

	// Spawns a Text with the current size as the corresponding area.
	void printSize();

	// Digs/mines a single block if the DiggingBlock is digging at that moment.
	void dig();

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

	/**
	* Writes a vector/list of DiggingBlock instances to a file.
	*
	* @param file The file to write all of the instances to.
	* @param diggingBlocks The vector containing all of the
	*					   blocks currently in the world.
	*/
	static void writeBlocks(std::ostream& file, const std::vector<DiggingBlock*>& diggingBlocks);

	/**
	* Writes a vector/list of DiggingBlock instances to a file.
	*
	* @param file The file to write all of the instances to.
	* @param quarryBlocks The vector containing all of the
	*					  blocks currently in the world.
	*/
	static void writeBlocks(std::ostream&& file, const std::vector<DiggingBlock*>& diggingBlocks);


	/**
	* Reads a file of instances of DiggingBlock and returns it as a vector.
	*
	* @param file The file to read.
	*
	* @return A vector containing instances of all of the blocks currently
	*		  in the world.
	*/
	static auto readBlocks(std::istream& file)->std::vector<DiggingBlock*>;

	/**
	* Reads a file of instances of DiggingBlock and returns it as a vector.
	*
	* @param file The file to read.
	*
	* @return A vector containing instances of all of the blocks currently
	*		  in the world.
	*/
	static auto readBlocks(std::istream&& file)->std::vector<DiggingBlock*>;
};