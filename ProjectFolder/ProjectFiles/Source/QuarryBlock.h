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
	* @param length The length of the area to be dug out.
	* @param width The width of the area to be dug out.
	* @param depth How deep the hole will be.
	* @param blockPosition The position of the Quarry block.
	* @param currentMode The current mode of the Quarry block.
	* @param currentDigBlock The coordinates for the block currently being dug out.
	* @param corners The corner positions.
	* @param digOres Whether to dig ores.
	* @param digDirection The direction the block is digging.
	* 
	*/
	QuarryBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, std::array<CoordinateInBlocks,4> corners, bool digOres, int digDirection);

	// Digs/mines a single block if the Quarry block is digging at that moment.
	void dig();

	/**
	* Increments the length of the area that is to be dug out.
	*
	* @param direction The direction that the button is - u/d.
	*/
	void incrementLength(char direction);

	/**
	* Decrements the length of the area that is to be dug out.
	*
	* @param direction The direction that the button is - u/d.
	*/
	void decrementLength(char direction);

	/**
	* Increments the width of the area that is to be dug out.
	*
	* @param direction The direction that the button is - l/r.
	*/
	void incrementWidth(char direction);

	/**
	* Decrements the width of the area that is to be dug out.
	*
	* @param direction The direction that the button is - l/r.
	*/
	void decrementWidth(char direction);
	
	// Resets the currentDigBlock to fit with length & width.
	void resetDigBlock();

	/**
	* Checks if anything on the interface is being clicked and acts on it
	* if need be.
	*
	* @param fingerLocation The location of the given finger.
	* @param canClick The canClick field that corresponds to given finger.
	* @param positionCm The position of the main block in centimeters.
	*/
	void clickCheck(CoordinateInCentimeters fingerLocation, bool& canClick, CoordinateInCentimeters positionCm, bool leftHand);

	/**
	* Gets the corner of the interface to be used for isBetween.
	*
	* @return The corner of the interface.
	*/
	CoordinateInCentimeters getCorner();

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
	bool isBetween(std::pair<int, int> bottomLeft, std::pair<int, int> topRight, CoordinateInCentimeters fingerPos);

	// Sets the specific Off block.
	void setOffBlock();

	// Sets the specific On block.
	void setOnBlock();

	// Sets the specific Settings block.
	void setSettingsBlock();

	// Sets the specific normal block.
	void setNormalBlock();

	/*
	* Calculates where on a circle of the given radius from the given location
	* that a hint text should be spawned.
	*
	* @param location The location of the block (center of it).
	* @param height The height offset for the location.
	* @param radius	The radius of the circle.
	*
	* @return The location that a hint text should be spawned at.
	*/
	CoordinateInCentimeters getHintTextLocationHelper(CoordinateInCentimeters location, int height, int radius);

	/*
	* Gets the location that hint texts are to be spawned at.
	*
	* @return The location that hint texts are to be spawned at.
	*/
	CoordinateInCentimeters getHintTextLocation();

	
	void setAreaSelection();
}; 