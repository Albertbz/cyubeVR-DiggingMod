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
	// The current mode the Digging block is in. 
	// 0 = destroyed, 1 = normal, 2 = settings, 3 = digging, 4 = finished digging.
	int currentMode;
	// The current block that is being dug out, relative to blockPosition.
	CoordinateInBlocks currentDigBlock;
	// An array of all of the Corner blocks.
	std::array<Block, 4> cornerBlocks;
	// Keeps track of whether to dig ores as well.
	bool digOres;
	// Keeps track of what direction to dig in. 
	// 0=N/A, 1=+x, 2=-x, 3=+y, 4=-y, 5=+z, 6=-z.
	int digDirection;
	// A boolean to keep track of whether the digging block was just replaced.
	bool justReplaced;
	// A bool to keep track of whether the player can click the interface with the left hand.
	bool canClickLeft;
	// A bool to keep track of whether the player can click the interface with the right hand.
	bool canClickRight;
	// The placement of the text that appears - relative to blockPosition.
	CoordinateInCentimeters textPlacement;

	// Enum with all of the blocks used for methods.
	enum {
		quarryBlockID = 1473066952,
		quarryOffBlockID = 380980661,
		quarryOnBlockID = 380980662,
		quarrySetBlockID = 380980663,
		tunBlockID = 894654498,
		tunOffBlockID = 1244240001,
		tunOnBlockID = 1244240002,
		tunSetBlockID = 1244240003,
		markerBlockID = 430459851
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
	* @param length The length of the area to be dug out.
	* @param width The width of the area to be dug out.
	* @param depth How deep the hole will be.
	* @param blockPosition The position of the block.
	* @param currentMode The current mode of the block.
	* @param currentDigBlock The coordinates for the block currently being dug out.
	* @param cornerBlocks The corner blocks.
	* @param digOres Whether to dig ores.
	* @param digDirection The direction the block is digging.
	*/
	DiggingBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, 
			     std::array<Block, 4> cornerBlocks, bool digOres, int digDirection);

	// Adds the corners of the area that is to be dug out.
	void setCorners();

	// Removes the corners of the area that is to be dug out.
	void removeCorners();

	/**
	* Increments the length of the area that is to be dug out.
	* 
	* @param direction The direction that the button is - u/d.
	*/
	virtual void incrementLength(char direction) = 0;

	/**
	* Decrements the length of the area that is to be dug out.
	*
	* @param direction The direction that the button is - u/d.
	*/
	virtual void decrementLength(char direction) = 0;

	/**
	* Increments the width of the area that is to be dug out.
	*
	* @param direction The direction that the button is - l/r.
	*/
	virtual void incrementWidth(char direction) = 0;

	/**
	* Decrements the width of the area that is to be dug out.
	*
	* @param direction The direction that the button is - l/r.
	*/
	virtual void decrementWidth(char direction) = 0;

	// Increments the size of the area that is to be dug out.
	void incrementSize();

	// Decrement the size of the area that is to be dug out.
	void decrementSize();

	// Increments the depth of the hole to be dug out.
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

	/**
	* Tries to dig and returns whether the digging was a success. If any error
	* is encountered (such as trying to dig in unloaded chunks), will return
	* false.
	* 
	* @return Whether the digging was a success.
	*/
	bool digSuccess();

	// Toggles digging mode.
	void toggleDigging();

	// Toggles settings mode.
	void toggleSettings();

	// Goes to the finished mode.
	void finishedDigging();

	// Sets the specific Off block.
	virtual void setOffBlock() = 0;

	// Sets the specific On block.
	virtual void setOnBlock() = 0;

	// Sets the specific Settings block.
	virtual void setSettingsBlock() = 0;

	// Sets the specific normal block.
	virtual void setNormalBlock() = 0;

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

	/**
	* Computes what direction the player is compared to the block.
	* 
	* @return The direction the player is compared to the block (so if 
	*		  +x, then will give CoordinateInBlocks(1, 0, 0)).
	*/
	CoordinateInBlocks getPlayerDirection();

	/**
	* Computes whether the given block is a corner block of this Digging Block.
	* 
	* @param blockPos The position of the possible corner block.
	* 
	* @return Whether the given block is a corner block of this Digging Block.
	*/
	bool isCornerBlock(CoordinateInBlocks blockPos);

	// Checks for any action by the player.
	void runCheck();

	/**
	* Checks if anything on the interface is being clicked and acts on it
	* if need be.
	*
	* @param fingerLocation The location of the given finger.
	* @param canClick The canClick field that corresponds to given finger.
	* @param positionCm The position of the main block in centimeters.
	*/
	virtual void clickCheck(CoordinateInCentimeters fingerLocation, bool& canClick, CoordinateInCentimeters positionCm, bool leftHand) = 0;

	/**
	* Acts on a click, i.e., checks what is being clicked and does what
	* is supposed to happen in accordance with chosen functionality.
	*
	* @param fingerLocation The location of the given finger.
	*/
	void clickRegister(CoordinateInCentimeters fingerLocation, bool leftHand);

	/**
	* Gets the corner of the interface to be used for isBetween.
	* 
	* @return The corner of the interface.
	*/
	virtual CoordinateInCentimeters getCorner() = 0;

	/**
	* Computes whether the finger is between the two points given.
	* 
	* @param bottomLeft The coordinates for the top left corner in centimeters.
	* @param topRight The coordinates for the bottom right corner in centimeters.
	* @param fingerPos The position of the finger.
	* 
	* @return Whether the given finger is between the two given corners.
	* 
	*/
	virtual bool isBetween(std::pair<int, int> bottomLeft, std::pair<int, int> topRight, CoordinateInCentimeters fingerPos) = 0;
};