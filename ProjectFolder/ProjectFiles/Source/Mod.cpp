#include "GameAPI.h"
#include "ExcavatorBlock.h"
#include <fstream>
#include <iostream>

// Custom variables
std::vector<ExcavatorBlock*> excavatorBlocks;

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { 1473066952, 1473066953 }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 0.2;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/************************************************************* 
//	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/


// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID)
{
	if (CustomBlockID == 1473066952) {
		// Adds the newly placed excavator block to the list of excavator blocks
		excavatorBlocks.push_back(new ExcavatorBlock(At)); 
		ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
	}
}


// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID)
{
	if (CustomBlockID == 1473066952) {
		// Goes through all excavator blocks, deletes the one that was destroyed
		for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
			if ((*it)->getBlockPosition() == At) {
				(*it)->removeCorners();
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

	if (ToolName == L"T_Stick") {
		if (CustomBlockID == 1473066952) {
			// Increments the size of the area to be dug out (when hitting excavator block with a stick)
			for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
				if ((*it)->getBlockPosition() == At) {
					(*it)->incrementSize();
					ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
					break;
				}
			}
		}
	}
	else if (ToolName == L"T_Pickaxe_Stone") {

	}
	else if (ToolName == L"T_Arrow") {
		if (CustomBlockID == 1473066952) {
			for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
				if ((*it)->getBlockPosition() == At) {
					(*it)->toggleDig();
					ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
					break;
				}
			}
		}
	}
	else {

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
