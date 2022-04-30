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
	* @param buttonBlocks The button blocks.
	* @param settingsPage The current settings page.
	* @param digOres Whether to dig ores.
	*/
	QuarryBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, 
				std::array<Block, 4> cornerBlocks, std::array<Block, buttonBlocksAmount> buttonBlocks, int settingsPage, bool digOres,
				int digDirection);

	// Digs/mines a single block if the Quarry block is digging at that moment.
	void dig();

	/**
	* Increments the length of the area that is to be dug out.
	*
	* @param block The block that is hit - b/f.
	*/
	void incrementLength(char block);

	/**
	* Decrements the length of the area that is to be dug out.
	*
	* @param block The block that is hit - b/f.
	*/
	void decrementLength(char block);

	/**
	* Increments the width of the area that is to be dug out.
	*
	* @param block The block that is hit - l/r.
	*/
	void incrementWidth(char block);

	/**
	* Decrements the width of the area that is to be dug out.
	*
	* @param block The block that is hit - l/r.
	*/
	void decrementWidth(char block);

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

	// Updates the locations of the Button blocks in the settings mode 
	// to correspond to player's location.
	void updateSettingsBlockLocations();

	// Sets the settings blocks to match the current settings page.
	void setSettingsBlocks();

	// Removes the settings blocks matching the current settings page.
	void removeSettingsBlocks();
};