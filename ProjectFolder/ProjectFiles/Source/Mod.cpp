#include "GameAPI.h"
#include "ExcavatorBlock.h"
#include <fstream>
#include <iostream>

/************************************************************
	Custom Variables for the mod
*************************************************************/

// Stores all excavator blocks currently in the world
std::vector<ExcavatorBlock*> excavatorBlocks; 

// All blocks with possible interactions
const int excBlockID = 1473066952;
//const int markBlockID = 1473066953; // Purely for decoration
const int outBlockID = 1473066954;
const int downBlockID = 1473066955;
const int upBlockID = 1473066956;
const int inBlockID = 1473066957;
const int setBlockID = 1473066958;
//const int exclBlockID = 1473066959; // Purely for decoration
const int checkBlockID = 1473066960;
const int crossBlockID = 1473066961;


/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { excBlockID, outBlockID, downBlockID, upBlockID, inBlockID, setBlockID, checkBlockID, crossBlockID }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 5;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/************************************************************* 
	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/


// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	if (CustomBlockID == excBlockID) { 

		// Adds the newly placed Excavator block to the list of excavator blocks (if it doesn't already exist).
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
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	if (CustomBlockID == excBlockID) {

		// Goes through all Excavator blocks, deletes the one that was destroyed.
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
	if (CustomBlockID == checkBlockID || CustomBlockID == crossBlockID) {
		if (ToolName == L"T_Arrow") {

			// Goes through all Excavator blocks, toggles the digging of the one the Check/Cross Mark block belongs to.
			for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {		
				if ((*it)->getBlockPosition() == (At - CoordinateInBlocks(0, 0, 6))) {
					(*it)->toggleDigging();
					ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
					break;
				}
			}
		}
	}
	else if (CustomBlockID == setBlockID) {
		if (ToolName == L"T_Stick") {
			SpawnHintText(At - CoordinateInBlocks(0, 1, 0), L"Message", 5);
			// Goes through all Excavator blocks, toggles the settings of the one the Settings block belongs to.
			for (auto it = excavatorBlocks.begin(); it != excavatorBlocks.end(); it++) {
				if ((*it)->getBlockPosition() == (At - CoordinateInBlocks(0, 0, 4))) {
					(*it)->toggleSettings();
					ExcavatorBlock::writeExcavatorBlocks(std::ofstream{ "ExcavatorBlocks.txt" }, excavatorBlocks);
					break;
				}
			}
		}
	}
	else if (CustomBlockID == outBlockID) {
		if (ToolName == L"T_Stick") {

			// Goes through all Excavator blocks, increments the size of the one the Outwards block belongs to.
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

			// Goes through all Excavator blocks, decrements the size of the one the Inwards block belongs to.
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

			// Goes through all Excavator blocks, decrements the depth of the one the Up block belongs to.
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

			// Goes through all Excavator blocks, increments the depth of the one the Down block belongs to.
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

	// Goes through all Excavator blocks and runs the dig method.
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

	// Loads all Excavator blocks previously placed in the world into the excavatorBlocks vector.
	excavatorBlocks = ExcavatorBlock::readExcavatorBlocks(std::ifstream{ "ExcavatorBlocks.txt" });
}

// Run once when the world is exited
void Event_OnExit()
{

}

/*******************************************************

	For all the available game functions you can call, look at the GameAPI.h file

*******************************************************/
