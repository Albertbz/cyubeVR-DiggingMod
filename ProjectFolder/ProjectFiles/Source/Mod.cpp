#include "GameAPI.h"
#include "QuarryBlock.h"
#include "TunnelBlock.h"
#include <fstream>
#include <iostream>


/************************************************************
	Custom Variables for the mod
*************************************************************/

void location() {}

// Stores all Quarry blocks currently in the world
std::vector<QuarryBlock> quarryBlocks;
// Stores all Tunnel blocks currently in the world
std::vector<TunnelBlock> tunnelBlocks;

// The name of the world
std::wstring worldName;
// The path of the DLL file.
std::wstring path = getModulePath(location);

// All blocks with possible interactions
const int quarryBlockID = 1473066952;
const int mark1BlockID = 430459851;
const int mark2BlockID = 430459852;
const int mark3BlockID = 430459853;
const int mark4BlockID = 430459854;
const int outBlockID = 1473066954;
const int downBlockID = 1473066955;
const int upBlockID = 1473066956;
const int inBlockID = 1473066957;
const int setBlockID = 1473066958;
const int exclBlockID = 1473066959;
const int checkBlockID = 1473066960;
const int crossBlockID = 1473066961;
const int leftBlockID = 577305854;
const int rightBlockID = 577305855;
const int backBlockID = 577305856;
const int frontBlockID = 577305857;
const int nextBlockID = 527579106;
const int prevBlockID = 527579107;
const int oresBlockID = 527579108;
const int tunBlockID = 894654498;
const int tMark11BlockID = 633982711;
const int tMark12BlockID = 633982712;
const int tMark13BlockID = 633982713;
const int tMark14BlockID = 633982714;
const int tMark21BlockID = 633982721;
const int tMark22BlockID = 633982722;
const int tMark23BlockID = 633982723;
const int tMark24BlockID = 633982724;
const int tMark31BlockID = 633982731;
const int tMark32BlockID = 633982732;
const int tMark33BlockID = 633982733;
const int tMark34BlockID = 633982734;
const int tMark41BlockID = 633982741;
const int tMark42BlockID = 633982742;
const int tMark43BlockID = 633982743;
const int tMark44BlockID = 633982744;
const int tArrowUpBlockID = 1205720236;
const int tArrowDownBlockID = 1205720237;
const int tArrowLeftBlockID = 1205720238;
const int tArrowRightBlockID = 1205720239;
const int tDepthInBlockID = 867089596;
const int tDepthOutBlockID = 867089597;

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { quarryBlockID, outBlockID, downBlockID, upBlockID, inBlockID, setBlockID, exclBlockID, checkBlockID, 
								crossBlockID, leftBlockID, rightBlockID, backBlockID, frontBlockID, nextBlockID, prevBlockID,
								mark1BlockID, mark2BlockID, mark3BlockID, mark4BlockID, oresBlockID, tunBlockID, tMark11BlockID,
								tMark12BlockID, tMark13BlockID, tMark14BlockID, tMark21BlockID, tMark22BlockID, tMark23BlockID,
								tMark24BlockID, tMark31BlockID, tMark32BlockID, tMark33BlockID, tMark34BlockID, tMark41BlockID,
								tMark42BlockID, tMark43BlockID, tMark44BlockID, tArrowUpBlockID, tArrowDownBlockID, 
								tArrowLeftBlockID, tArrowRightBlockID, tDepthInBlockID, tDepthOutBlockID }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 5;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

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
		
		// Adds the newly placed Tunnel block to the list of Quarry blocks (if it doesn't already exist).
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
	if (CustomBlockID == quarryBlockID) {

		// Goes through all Quarry blocks, deletes the one that was destroyed.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == At) {
				it->destroy();
				quarryBlocks.erase(it);
				break;
			}
		}
	}
	else if (CustomBlockID == tunBlockID) {

		// Goes through all Tunnel blocks, deletes the one that was destroyed.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == At) {
				it->destroy();
				tunnelBlocks.erase(it);
				break;
			}
		}
	}
}


// Run every time a block is hit by a tool
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, wString ToolName)
{
	bool isLeftover = true; // Keeps track of whether the block is leftover (from bug or something else), i.e., it doesn't belong to a Digging Block.

	switch (CustomBlockID) {
	case quarryBlockID:
	case tunBlockID:
		isLeftover = false;
		break;
	case checkBlockID:
	case crossBlockID:
		// Goes through all Quarry blocks, toggles the digging of the one the Check/Cross Mark block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[6].position)) { // Specifically checks the Check Mark block (should always be the same as Cross Mark).
				it->toggleDigging();
				isLeftover = false;
				break;
			}
		}

		// Goes through all Tunnel blocks, toggles the digging of the one the Check/Cross Mark block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[6].position)) { // Specifically checks the Check Mark block (should always be the same as Cross Mark).
				it->toggleDigging();
				isLeftover = false;
				break;
			}
		}
		break;
	case setBlockID:
		// Goes through all Quarry blocks, toggles the settings of the one the Settings block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[0].position)) {
				it->toggleSettings();
				isLeftover = false;
				break;
			}
		}

		// Goes through all Tunnel blocks, toggles the settings of the one the Settings block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[0].position)) {
				it->toggleSettings();
				isLeftover = false;
				break;
			}
		}
		break;
	case outBlockID:
		// Goes through all Quarry blocks, increments the size of the one the Outwards block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[4].position)) {
				it->incrementSize();
				it->printSize();
				isLeftover = false;
				break;
			}
		}

		// Goes through all Tunnel blocks, increments the size of the one the Outwards block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[4].position)) {
				it->incrementSize();
				it->printSize();
				isLeftover = false;
				break;
			}
		}
		break;
	case inBlockID:
		// Goes through all Quarry blocks, decrements the size of the one the Inwards block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[3].position)) {
				it->decrementSize();
				it->printSize();
				isLeftover = false;
				break;
			}
		}

		// Goes through all Tunnel blocks, decrements the size of the one the Inwards block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[3].position)) {
				it->decrementSize();
				it->printSize();
				isLeftover = false;
				break;
			}
		}
		break;
	case upBlockID:
		// Goes through all Quarry blocks, decrements the depth of the one the Up block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[1].position)) {
				it->decrementDepth();
				it->printDepth();
				isLeftover = false;
				break;
			}
		}
		break;
	case downBlockID:
		// Goes through all Quarry blocks, increments the depth of the one the Down block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[2].position)) {
				it->incrementDepth();
				it->printDepth();
				isLeftover = false;
				break;
			}
		}
		break;
	case tDepthInBlockID:
		// Goes through all Tunnel blocks, decrements the depth of the one the Up block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[1].position)) {
				it->decrementDepth();
				it->printDepth();
				isLeftover = false;
				break;
			}
		}
		break;
	case tDepthOutBlockID:
		// Goes through all Tunnel blocks, increments the depth of the one the Down block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[2].position)) {
				it->incrementDepth();
				it->printDepth();
				isLeftover = false;
				break;
			}
		}
		break;
	case mark1BlockID:
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->cornerBlocks[0].position)) {
				isLeftover = false;
				break;
			}
		}
		break;
	case mark2BlockID:
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->cornerBlocks[1].position)) {
				isLeftover = false;
				break;
			}
		}
		break;
	case mark3BlockID:
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->cornerBlocks[2].position)) {
				isLeftover = false;
				break;
			}
		}
		break;
	case mark4BlockID:
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->cornerBlocks[3].position)) {
				isLeftover = false;
				break;
			}
		}
		break;
	case tMark11BlockID:
	case tMark21BlockID:
	case tMark31BlockID:
	case tMark41BlockID:
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->cornerBlocks[0].position)) {
				isLeftover = false;
				break;
			}
		}
		break;
	case tMark12BlockID:
	case tMark22BlockID:
	case tMark32BlockID:
	case tMark42BlockID:
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->cornerBlocks[1].position)) {
				isLeftover = false;
				break;
			}
		}
		break;
	case tMark13BlockID:
	case tMark23BlockID:
	case tMark33BlockID:
	case tMark43BlockID:
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->cornerBlocks[2].position)) {
				isLeftover = false;
				break;
			}
		}
		break;
	case tMark14BlockID:
	case tMark24BlockID:
	case tMark34BlockID:
	case tMark44BlockID:
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->cornerBlocks[3].position)) {
				isLeftover = false;
				break;
			}
		}
		break;
	case nextBlockID:
		// Goes through all Quarry blocks, goes to the next page of the settings of the one the Next block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[12].position)) {
				it->nextSettingsPage();
				isLeftover = false;
				break;
			}
		}

		// Goes through all Tunnel blocks, goes to the next page of the settings of the one the Next block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[12].position)) {
				it->nextSettingsPage();
				isLeftover = false;
				break;
			}
		}
		break;
	case prevBlockID:
		// Goes through all Quarry blocks, goes to the previous page of the settings of the one the Previous block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[13].position)) {
				it->prevSettingsPage();
				isLeftover = false;
				break;
			}
		}

		// Goes through all Tunnel blocks, goes to the previous page of the settings of the one the Previous block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[13].position)) {
				it->prevSettingsPage();
				isLeftover = false;
				break;
			}
		}
		break;
	case leftBlockID:
		// Goes through all Quarry blocks, increments or decrements width of the one the Left block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[8].position)) {
				it->incrementWidth('l');
				isLeftover = false;
				break;
			}
			else if (it->blockPosition == (At - it->buttonBlocks[14].position)) {
				it->decrementWidth('l');
				isLeftover = false;
				break;
			}
		}
		break;
	case rightBlockID:
		// Goes through all Quarry blocks, increments or decrements width of the one the Right block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[9].position)) {
				it->incrementWidth('r');
				isLeftover = false;
				break;
			}
			else if (it->blockPosition == (At - it->buttonBlocks[15].position)) {
				it->decrementWidth('r');
				isLeftover = false;
				break;
			}
		}
		break;
	case backBlockID:
		// Goes through all Quarry blocks, increments back or decrements front width of the one the Back block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[10].position)) {
				it->incrementLength('b');
				isLeftover = false;
				break;
			}
			else if (it->blockPosition == (At - it->buttonBlocks[16].position)) {
				it->decrementLength('b');
				isLeftover = false;
				break;
			}
		}
		break;
	case frontBlockID:
		// Goes through all Quarry blocks, increments front or decrements back width of the one the Front block belongs to.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[11].position)) {
				it->incrementLength('f');
				isLeftover = false;
				break;
			}
			else if (it->blockPosition == (At - it->buttonBlocks[17].position)) {
				it->decrementLength('f');
				isLeftover = false;
				break;
			}
		}
		break;
	case tArrowLeftBlockID:
		// Goes through all Tunnel blocks, increments or decrements width of the one the Left block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[8].position)) {
				it->incrementWidth('l');
				isLeftover = false;
				break;
			}
			else if (it->blockPosition == (At - it->buttonBlocks[14].position)) {
				it->decrementWidth('l');
				isLeftover = false;
				break;
			}
		}
		break;
	case tArrowRightBlockID:
		// Goes through all Tunnel blocks, increments or decrements width of the one the Right block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[9].position)) {
				it->incrementWidth('r');
				isLeftover = false;
				break;
			}
			else if (it->blockPosition == (At - it->buttonBlocks[15].position)) {
				it->decrementWidth('r');
				isLeftover = false;
				break;
			}
		}
		break;
	case tArrowUpBlockID:
		// Goes through all Tunnel blocks, increments back or decrements front width of the one the Back block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[10].position)) {
				it->incrementLength('b');
				isLeftover = false;
				break;
			}
			else if (it->blockPosition == (At - it->buttonBlocks[16].position)) {
				it->decrementLength('b');
				isLeftover = false;
				break;
			}
		}
		break;
	case tArrowDownBlockID:
		// Goes through all Tunnel blocks, increments front or decrements back width of the one the Front block belongs to.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[11].position)) {
				it->incrementLength('f');
				isLeftover = false;
				break;
			}
			else if (it->blockPosition == (At - it->buttonBlocks[17].position)) {
				it->decrementLength('f');
				isLeftover = false;
				break;
			}
		}
		break;
	case oresBlockID:
		// Goes through all Quarry blocks, toggles whether to dig ores for the corresponding Quarry block.
		for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[18].position)) {
				it->toggleDigOres();
				it->printDigOres();
				isLeftover = false;
				break;
			}
		}

		// Goes through all Tunnel blocks, toggles whether to dig ores for the corresponding Quarry block.
		for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
			if (it->blockPosition == (At - it->buttonBlocks[18].position)) {
				it->toggleDigOres();
				it->printDigOres();
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

	// Goes through all Quarry blocks and runs the dig method.
	for (auto it = quarryBlocks.begin(); it != quarryBlocks.end(); it++) {
		it->dig();
	}

	// Goes through all Quarry blocks and runs the dig method.
	for (auto it = tunnelBlocks.begin(); it != tunnelBlocks.end(); it++) {
		it->dig();
	}
}


// Run once when the world is loaded
void Event_OnLoad()
{
	worldName = GetWorldName();
	
	// Loads all Quarry blocks previously placed in the world into the quarryBlocks vector.
	quarryBlocks = readBlocks<QuarryBlock>(std::ifstream{ path + L"\\BlockInstances\\" + worldName + L"-QuarryBlocks.txt"});

	auto itQ = quarryBlocks.begin();
	while (itQ != quarryBlocks.end()) {
		if (GetBlock(itQ->blockPosition).CustomBlockID != quarryBlockID) {
			itQ->destroy();
			itQ = quarryBlocks.erase(itQ);
		}
		else {
			itQ++;
		}
	}

	// Loads all Tunnel blocks previously placed in the world into the quarryBlocks vector.
	tunnelBlocks = readBlocks<TunnelBlock>(std::ifstream{ path + L"\\BlockInstances\\" + worldName + L"-TunnelBlocks.txt" });

	auto itT = tunnelBlocks.begin();
	while (itT != tunnelBlocks.end()) {
		if (GetBlock(itT->blockPosition).CustomBlockID != tunBlockID) {
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
	writeBlocks<QuarryBlock>(std::ofstream{ path + L"\\BlockInstances\\" + worldName + L"-QuarryBlocks.txt"}, quarryBlocks);

	// Saves all Tunnel blocks to a file for later loading.
	writeBlocks<TunnelBlock>(std::ofstream{ path + L"\\BlockInstances\\" + worldName + L"-TunnelBlocks.txt" }, tunnelBlocks);
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
void Event_AnyBlockHitByTool(CoordinateInBlocks At, BlockInfo Type, wString ToolName)
{

}

/*******************************************************

	For all the available game functions you can call, look at the GameAPI.h file

*******************************************************/
