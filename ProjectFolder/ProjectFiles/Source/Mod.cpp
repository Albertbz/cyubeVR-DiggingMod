#include "GameAPI.h"
#include "QuarryBlock.h"
#include "TunnelBlock.h"
#include <fstream>
#include <iostream>


/************************************************************
	Custom Variables for the mod
*************************************************************/

// Tick tracker to take care of saving blocks
int tickNum = 0;
// Stores all Quarry blocks currently in the world
std::vector<QuarryBlock> quarryBlocks;
// Stores all Tunnel blocks currently in the world
std::vector<TunnelBlock> tunnelBlocks;

// The path of the save folder.
std::wstring path;

// All blocks with possible interactions
const int quarryBlockID = 1473066952;
const int quarryOffBlockID = 380980661;
const int quarryOnBlockID = 380980662;
const int quarrySetBlockID = 380980663;

const int tunBlockID = 894654498;
const int tunOffBlockID = 1244240001;
const int tunOnBlockID = 1244240002;
const int tunSetBlockID = 1244240003;

const int markerBlockID = 430459851;

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { quarryBlockID, quarryOffBlockID, quarryOnBlockID, quarrySetBlockID,
								tunBlockID, tunOffBlockID, tunOnBlockID, tunSetBlockID, markerBlockID }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 10;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/************************************************************* 
	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/


// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	if (CustomBlockID == quarryBlockID) {

		// Adds the newly placed Quarry block to the list of Quarry blocks (if it doesn't already exist).
		bool newBlock = true;
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == At) {
				newBlock = false;
				break;
			}
		}
		if (newBlock) {
			quarryBlocks.push_back(QuarryBlock(At));
		}
	}
	else if (CustomBlockID == tunBlockID) {
		
		// Adds the newly placed Tunnel block to the list of Tunnel blocks (if it doesn't already exist).
		bool newBlock = true;
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == At) {
				newBlock = false;
				break;
			}
		}
		if (newBlock) {
			tunnelBlocks.push_back(TunnelBlock(At));
		}
	}
}


// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	switch (CustomBlockID) {
	case quarryBlockID:
	case quarryOffBlockID:
	case quarryOnBlockID:
	case quarrySetBlockID:
		// Goes through all Quarry blocks, deletes the one that was destroyed.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == At) {
				if (it->justReplaced) {
					it->justReplaced = false;
				}
				else {
					it->destroy();
					quarryBlocks.erase(it);
				}
				break;
			}
		}
		break;
	case tunBlockID:
	case tunOffBlockID:
	case tunOnBlockID:
	case tunSetBlockID:
		// Goes through all Tunnel blocks, deletes the one that was destroyed.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == At) {
				if (it->justReplaced) {
					it->justReplaced = false;
				}
				else {
					it->destroy();
					tunnelBlocks.erase(it);
				}
				break;
			}
		}
		break;
	}
	
}


// Run every time a block is hit by a tool
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{
	bool isLeftover = true; // Keeps track of whether the block is leftover (from bug or something else), i.e., it doesn't belong to a Digging Block.

	switch (CustomBlockID) {
	case quarryBlockID:
	case quarryOffBlockID:
	case quarryOnBlockID:
	case quarrySetBlockID:
	case tunBlockID:
	case tunOffBlockID:
	case tunOnBlockID:
	case tunSetBlockID:
		isLeftover = false;
		break;
	case markerBlockID:
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->isCornerBlock(At)) {
				isLeftover = false;
				break;
			}
		}

		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->isCornerBlock(At)) {
				isLeftover = false;
				break;
			}
		}
		break;
	}


	if (isLeftover) {
		SetBlock(At, EBlockType::Air);
	}
}


// Run X times per second, as specified in the TickRate variable at the top
void Event_Tick()
{

	// Goes through all Quarry blocks and runs needed methods.
	for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
		it->runCheck();
		if (tickNum % 2 == 0) {
			it->dig();
		}
	}

	// Goes through all Quarry blocks and runs the dig method.
	for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
		it->runCheck();
		if (tickNum % 2 == 0) {
			it->dig();
		}
	}

	switch (tickNum) {
	case 100: 
		// Saves all Quarry blocks to a file for later loading.
		writeBlocks<QuarryBlock>(std::ofstream{ path + L"QuarryBlocks.txt" }, quarryBlocks);

		// Saves all Tunnel blocks to a file for later loading.
		writeBlocks<TunnelBlock>(std::ofstream{ path + L"TunnelBlocks.txt" }, tunnelBlocks);
		tickNum = 0;
		break;
	default:
		tickNum++;
		break;
	}
}


// Run once when the world is loaded
void Event_OnLoad(bool CreatedNewWorld)
{
	path = GetThisModSaveFolderPath(L"DiggingMod");

	// Loads all Quarry blocks previously placed in the world into the quarryBlocks vector.
	quarryBlocks = readBlocks<QuarryBlock>(std::ifstream{ path + L"QuarryBlocks.txt" });

	auto itQ = quarryBlocks.begin();
	while (itQ != quarryBlocks.end()) {
		UniqueID blockID = GetBlock(itQ->blockPosition).CustomBlockID;
		if (blockID != quarryOffBlockID && 
			blockID != quarryOnBlockID &&
			blockID != quarrySetBlockID) {
			itQ->destroy();
			itQ = quarryBlocks.erase(itQ);
		}
		else {
			itQ++;
		}
	}

	// Loads all Tunnel blocks previously placed in the world into the quarryBlocks vector.
	tunnelBlocks = readBlocks<TunnelBlock>(std::ifstream{ path + L"TunnelBlocks.txt" });

	auto itT = tunnelBlocks.begin();
	while (itT != tunnelBlocks.end()) {
		UniqueID blockID = GetBlock(itT->blockPosition).CustomBlockID;
		if (blockID != tunOffBlockID &&
			blockID != tunOnBlockID &&
			blockID != tunSetBlockID) {
			itT->destroy();
			itT = tunnelBlocks.erase(itT);
		}
		else {
			itT++;
		}
	}
}

// Run once when the world is exited
void Event_OnExit()
{
	// Saves all Quarry blocks to a file for later loading.
	writeBlocks<QuarryBlock>(std::ofstream{ path + L"QuarryBlocks.txt"}, quarryBlocks);

	// Saves all Tunnel blocks to a file for later loading.
	writeBlocks<TunnelBlock>(std::ofstream{ path + L"TunnelBlocks.txt" }, tunnelBlocks);
}



/*******************************************************

	Advanced functions

*******************************************************/

// Run every time any block is placed by the player
void Event_AnyBlockPlaced(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{

}

// Run every time any block is destroyed by the player
void Event_AnyBlockDestroyed(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{

}

// Run every time any block is hit by a tool
void Event_AnyBlockHitByTool(CoordinateInBlocks At, BlockInfo Type, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{

}

/*******************************************************

	For all the available game functions you can call, look at the GameAPI.h file

*******************************************************/
