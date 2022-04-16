#include "GameAPI.h"
#include "ExcavatorBlock.h"
#include <fstream>
#include <iostream>

// Custom variables
std::vector<ExcavatorBlock*> excavatorBlocks;
const int excBlockID = 1473066952;
const int markBlockID = 1473066953;
const int outBlockID = 1473066954;
const int downBlockID = 1473066955;
const int upBlockID = 1473066956;
const int inBlockID = 1473066957;
const int setBlockID = 1473066958;

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { excBlockID, markBlockID, outBlockID, downBlockID, upBlockID, inBlockID, setBlockID }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 0.2;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/************************************************************* 
//	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/


// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID)
{
	if (CustomBlockID == excBlockID) {
		// Adds the newly placed excavator block to the list of excavator blocks
		bool newBlock = true;
		for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
			if ((*it)->getBlockPosition() == At) {
				newBlock = false;
				break;
			}
		}
		if (newBlock) {
			excavatorBlocks.push_back(new ExcavatorBlock(At));
			ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
		}
	}
}


// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID)
{
	if (CustomBlockID == excBlockID) {
		// Goes through all excavator blocks, deletes the one that was destroyed
		for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
			if ((*it)->getBlockPosition() == At) {
				(*it)->destroy();
				excavatorBlocks.erase(it);
				ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
				break;
			}
		}
	}
}


// Run every time a block is hit by a tool
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, std::wstring ToolName)
{
	if (CustomBlockID == excBlockID) {
		if (ToolName == L"T_Stick") {
			for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
				if ((*it)->getBlockPosition() == At) {
					(*it)->toggleDig();
					ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
					break;
				}
			}
		}
	}
	else if (CustomBlockID == setBlockID) {
		if (ToolName == L"T_Stick") {
			for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
				if ((*it)->getBlockPosition() == (At + CoordinateInBlocks(0, 0, -4))) {
					(*it)->toggleSettings();
					ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
					break;
				}
			}
		}
	}
	else if (CustomBlockID == outBlockID) {
		if (ToolName == L"T_Stick") {
			for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
				if ((*it)->getBlockPosition() == (At - CoordinateInBlocks(-1, 0, 0))) {
					(*it)->incrementSize();
					ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
					break;
				}
			}
		}
	}
	else if (CustomBlockID == inBlockID) {
		if (ToolName == L"T_Stick") {
			for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
				if ((*it)->getBlockPosition() == (At - CoordinateInBlocks(1, 0, 0))) {
					(*it)->decrementSize();
					ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
					break;
				}
			}
		}
	}
	else if (CustomBlockID == upBlockID) {
		if (ToolName == L"T_Stick") {
			for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
				if ((*it)->getBlockPosition() == (At - CoordinateInBlocks(0, 0, 2))) {
					(*it)->decrementDepth();
					ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
					break;
				}
			}
		}
	}
	else if (CustomBlockID == downBlockID) {
		if (ToolName == L"T_Stick") {
			for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
				if ((*it)->getBlockPosition() == (At - CoordinateInBlocks(0, 0, 1))) {
					(*it)->incrementDepth();
					ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
					break;
				}
			}
		}
	}
}


// Run X times per second, as specified in the TickRate variable at the top
void Event_Tick()
{
	for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
		bool hasDug = (*it)->dig();
		if (hasDug) {
			ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
		}
	}
}


// Run once when the world is loaded
void Event_OnLoad()
{
	excavatorBlocks = ExcavatorBlock::readExcavatorBlocks(std::ifstream{ "ExcavatorBlocks.txt" });
}


/*******************************************************

	For all the available game functions you can call, look at the GameAPI.h file

*******************************************************/
