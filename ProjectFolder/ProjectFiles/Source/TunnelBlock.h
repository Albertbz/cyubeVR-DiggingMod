#pragma once
#include "DiggingBlock.h"

class TunnelBlock : public DiggingBlock {
public:
	/**
	* Constructor for TunnelBlock used when a new Tunnel
	* block is placed down.
	*
	* @param blockPosition The position of the Tunnel block.
	*/
	TunnelBlock(CoordinateInBlocks blockPosition);

	/**
	* Constructor for TunnelBlock used when loading in
	* the file containing all currently placed Tunnel
	* blocks.
	*
	* @param length The length of the area to be dug out.
	* @param width The width of the area to be dug out.
	* @param depth How deep the hole will be.
	* @param blockPosition The position of the Tunnel block.
	* @param currentMode The current mode of the Tunnel block.
	* @param currentDigBlock The coordinates for the block currently being dug out.
	* @param corners The corner positions.
	* @param digOres Whether to dig ores.
	* @param digDirection The direction the block is digging.
	*/
	TunnelBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, std::array<CoordinateInBlocks,4> corners, bool digOres, int digDirection);

	// Updates the corner blocks to match with digDirection.
	void updateCornerBlocks();
	
	// Digs/mines a single block if the Tunnel block is digging at that moment.
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
	* Gets the location that hint texts are to be spawned at.
	*
	* @return The location that hint texts are to be spawned at.
	*/
	CoordinateInCentimeters getHintTextLocation();

	void setAreaSelection();
};