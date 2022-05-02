#pragma once
#include "GameAPI.h"
#include "Misc.h"
#include <array>

class DiggingBlock {
public:
	// The length of the hole.
	int length;
	// The width of the hole.
	int width;
	// The depth of the hole.
	int depth;
	// The position of the block.
	CoordinateInBlocks blockPosition;
	// The current mode the Quarry block is in. 
	// 0 = destroyed, 1 = normal, 2 = settings, 3 = digging, 4 = finished digging.
	int currentMode;
	// The current block that is being dug out, relative to blockPosition.
	CoordinateInBlocks currentDigBlock;
	// An array of all of the Corner blocks.
	std::array<Block, 4> cornerBlocks;
	// An array of all of the Button blocks. 
	// 0=set, 1=-depth, 2=+depth, 3=-size, 4=+size, 5=excl, 6=check, 
	// 7=cross, 8=+W1, 9=+W2, 10=+L1, 11=+L2, 12=next, 13=prev, 
	// 14=-W1, 15=-W2, 16=-L1, 17=-L2, 18=ores.
	std::array<Block, buttonBlocksAmount> buttonBlocks;
	// The current page of the settings. 
	// 1=depth, 2=simple size, 3=advanced size, 4=dig ores.
	int settingsPage;
	// Keeps track of whether to dig ores as well.
	bool digOres;
	// Keeps track of what direction to dig in. 
	// 1=+x, 2=-x, 3=+y, 4=-y, 5=+z, 6=-z.
	int digDirection;
	// Enum with all of the blocks used for methods.
	enum {
		mark1BlockID = 430459851,
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
		crossBlockID = 1473066961,
		leftBlockID = 577305854,
		rightBlockID = 577305855,
		backBlockID = 577305856,
		frontBlockID = 577305857,
		nextBlockID = 527579106,
		prevBlockID = 527579107,
		oresBlockID = 527579108,
		tMark11BlockID = 633982711,
		tMark12BlockID = 633982712,
		tMark13BlockID = 633982713,
		tMark14BlockID = 633982714,
		tMark21BlockID = 633982721,
		tMark22BlockID = 633982722,
		tMark23BlockID = 633982723,
		tMark24BlockID = 633982724,
		tMark31BlockID = 633982731,
		tMark32BlockID = 633982732,
		tMark33BlockID = 633982733,
		tMark34BlockID = 633982734,
		tMark41BlockID = 633982741,
		tMark42BlockID = 633982742,
		tMark43BlockID = 633982743,
		tMark44BlockID = 633982744,
		tArrowUpBlockID = 1205720236,
		tArrowUpBlockID = 1205720237
	};
public:
	/**
	* Constructor for when a new block is
	* placed down.
	* 
	* @param blockPosition The position of the block.
	*/
	DiggingBlock(CoordinateInBlocks blockPosition);

	/**
	* Constructor for when loading in the file containing 
	* all currently placed blocks.
	* 
	* @param size The size of the area to be dug out.
	* @param depth How deep the hole will be.
	* @param blockPosition The position of the block.
	* @param currentMode The current mode of the block.
	* @param currentDigBlock The coordinates for the block currently being dug out.
	* @param cornerBlocks The corner blocks.
	* @param buttonBlocks The button blocks.
	* @param settingsPage The current settings page.
	* @param digOres Whether to dig ores.
	*/
	DiggingBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, 
			     std::array<Block, 4> cornerBlocks, std::array<Block, buttonBlocksAmount> buttonBlocks, int settingsPage, bool digOres,
				 int digDirection);

	// Adds the corners of the area that is to be dug out.
	void setCorners();

	// Removes the corners of the area that is to be dug out.
	void removeCorners();

	/**
	* Increments the length of the area that is to be dug out.
	* 
	* @param block The block that is hit - b/f.
	*/
	virtual void incrementLength(char block) = 0;

	/**
	* Decrements the length of the area that is to be dug out.
	*
	* @param block The block that is hit - b/f.
	*/
	virtual void decrementLength(char block) = 0;

	/**
	* Increments the width of the area that is to be dug out.
	*
	* @param block The block that is hit - l/r.
	*/
	virtual void incrementWidth(char block) = 0;

	/**
	* Decrements the width of the area that is to be dug out.
	*
	* @param block The block that is hit - l/r.
	*/
	virtual void decrementWidth(char block) = 0;

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

	/**
	* Gets the size of the area (lxwxd) as a wString.
	* 
	* @return The size of the area (lxwxd) as a wString.
	*/
	wString getSize();

	// Digs/mines a single block if the DiggingBlock is digging at that moment.
	virtual void dig() = 0;

	// Toggles digging mode.
	void toggleDigging();

	// Toggles settings mode.
	void toggleSettings();

	// Goes to the finished mode.
	void finishedDigging();

	/**
	* Sets or removes all of the blocks needed for the settings mode.
	*
	* @param show Whether to show the blocks.
	*/
	virtual void showSettings(bool show) = 0;

	/**
	* Sets or removes all of the blocks needed for the normal mode.
	*
	* @param show Whether to show the blocks.
	*/
	virtual void showNormal(bool show) = 0;

	/**
	* Sets or removes all of the blocks needed for the finished mode.
	*
	* @param show Whether to show the blocks.
	*/
	virtual void showFinished(bool show) = 0;

	/**
	* Sets or removes all of the blocks needed for the digging mode.
	*
	* @param show Whether to show the blocks.
	*/
	virtual void showDigging(bool show) = 0;

	// Removes and stops everything.
	void destroy();

	// Resets the currentDigBlock to fit with length & width.
	virtual void resetDigBlock() = 0;

	/**
	* Computes the time it will take to dig the current hole and
	* gives it as a wstring.
	* 
	* @return A wstring containing the time it will take to
	* dig out the current hole.
	*/
	std::wstring timeToDig();

	/**
	* Computes whether the given block is a diggable block and
	* can therefore be removed by a Digging block.
	* 
	* @param block The block to test.
	* 
	* @return A boolean telling whether the block is diggable.
	*/
	bool diggableBlock(BlockInfo block);

	// Updates the locations of the Button blocks in the settings mode 
	// to correspond to player's location.
	//virtual void updateSettingsBlockLocations() = 0;

	// Toggles whether to dig ores.
	void toggleDigOres();

	// Prints what digOres is set to.
	void printDigOres();

	// Turns to the next page in the settings mode.
	void nextSettingsPage();

	// Turns to the previous page in the settings mode.
	void prevSettingsPage();

	// Sets the blocks corresponding to the current settings page.
	virtual void setSettingsBlocks() = 0;

	// Removes the blocks corresponding to the current settings page.
	virtual void removeSettingsBlocks() = 0;

	/**
	* Updates infoPrev and sets the button block specified.
	* 
	* @param i The index of the button block to place.
	*/
	void setButtonBlock(int i);

	/**
	* Removes the button block specified.
	*
	* @param i The index of the button block to place.
	*/
	void removeButtonBlock(int i);

	// Prints a message saying what settings you are changing.
	void printSettingsPage();
};