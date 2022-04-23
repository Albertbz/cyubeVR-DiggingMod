#pragma once
#include "GameAPI.h"
#include "DiggingBlock.h"
#include <array>
class QuarryBlock : public DiggingBlock {
public:
	/**
	* Constructor for QuarryBlock used when a new Quarry
	* block is placed down.
	*
	* @param blockPosition The position of the Quarry block.
	*/
	QuarryBlock(CoordinateInBlocks blockPosition);

	/**
	* Constructor for QuarryBlock used when loading in
	* the file containing all currently placed Quarry
	* blocks.
	*
	* @param size The size of the area to be dug out.
	* @param depth How deep the hole will be.
	* @param blockPosition The position of the Quarry block.
	* @param currentMode The current mode of the Quarry block.
	* @param currentDigBlock The coordinates for the block currently being dug out.
	* @param cornerBlocks The corner blocks.
	*/
	QuarryBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, std::array<int, 3> currentDigBlock, std::array<Block, 4> cornerBlocks);

	// Digs/mines a single block if the Quarry block is digging at that moment.
	void dig();

	/**
	* Increments the length of the area that is to be dug out.
	*
	* @param side The side (l/r) that is to be incremented.
	*/
	void incrementLength(char side);

	/**
	* Decrements the length of the area that is to be dug out.
	*
	* @param side The side (l/r) that is to be decremented.
	*/
	void decrementLength(char side);

	/**
	* Increments the width of the area that is to be dug out.
	*
	* @param side The side (b/f) that is to be incremented.
	*/
	void incrementWidth(char side);

	/**
	* Decrements the width of the area that is to be dug out.
	*
	* @param side The side (b/f) that is to be decremented.
	*/
	void decrementWidth(char side);

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
	
	// Resets the currentDigBlock to fit with length & width.
	void resetDigBlock();
};