#include "TunnelBlock.h"

TunnelBlock::TunnelBlock(CoordinateInBlocks blockPosition)
	: DiggingBlock(blockPosition) {

	this->buttonBlocks[0] = { CoordinateInBlocks(0, 0, 1), setBlockID, BlockInfo() };

	this->buttonBlocks[5] = { CoordinateInBlocks(0, 0, 2), exclBlockID, BlockInfo() };
	this->buttonBlocks[6] = { CoordinateInBlocks(0, 0, 2), checkBlockID, BlockInfo() };
	this->buttonBlocks[7] = { CoordinateInBlocks(0, 0, 2), crossBlockID, BlockInfo() };

	CoordinateInCentimeters playerLocation = GetPlayerLocation();
	CoordinateInCentimeters blockPositionCm = CoordinateInCentimeters(blockPosition);
	int64_t xDifference = std::abs(playerLocation.X - blockPositionCm.X);
	int64_t yDifference = std::abs(playerLocation.Y - blockPositionCm.Y);
	
	if (playerLocation.X < blockPositionCm.X - 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		this->digDirection = 1;
		this->buttonBlocks[1] = { CoordinateInBlocks(0, -1, 0), upBlockID,   BlockInfo() };
		this->buttonBlocks[2] = { CoordinateInBlocks(0, 1, 0),  downBlockID, BlockInfo() };

		this->buttonBlocks[3] = { CoordinateInBlocks(0, -1, 0), inBlockID,  BlockInfo() };
		this->buttonBlocks[4] = { CoordinateInBlocks(0, 1, 0),  outBlockID, BlockInfo() };

		this->buttonBlocks[12] = { buttonBlocks[0].position + CoordinateInBlocks(0, 1, 0),  nextBlockID, BlockInfo() };
		this->buttonBlocks[13] = { buttonBlocks[0].position + CoordinateInBlocks(0, -1, 0), prevBlockID, BlockInfo() };

		this->buttonBlocks[18] = { CoordinateInBlocks(0, -1, 0), oresBlockID, BlockInfo() };

		this->buttonBlocks[8]  = { CoordinateInBlocks(0, -2, 1), tArrowLeftBlockID,  BlockInfo() };
		this->buttonBlocks[9]  = { CoordinateInBlocks(0, 2, 1),  tArrowRightBlockID, BlockInfo() };
		this->buttonBlocks[10] = { CoordinateInBlocks(0, 1, 2),  tArrowUpBlockID,    BlockInfo() };
		this->buttonBlocks[11] = { CoordinateInBlocks(0, 1, -2), tArrowDownBlockID,  BlockInfo() };

		this->buttonBlocks[14] = { CoordinateInBlocks(0, 2, -1),  tArrowLeftBlockID,  BlockInfo() };
		this->buttonBlocks[15] = { CoordinateInBlocks(0, -2, -1), tArrowRightBlockID, BlockInfo() };
		this->buttonBlocks[16] = { CoordinateInBlocks(0, -1, -2), tArrowUpBlockID,    BlockInfo() };
		this->buttonBlocks[17] = { CoordinateInBlocks(0, -1, 2),  tArrowDownBlockID,  BlockInfo() };
	}
	else if (playerLocation.X > blockPositionCm.X + 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		this->digDirection = 2;
		this->buttonBlocks[1] = { CoordinateInBlocks(0, 1, 0),  upBlockID,   BlockInfo() };
		this->buttonBlocks[2] = { CoordinateInBlocks(0, -1, 0), downBlockID, BlockInfo() };

		this->buttonBlocks[3] = { CoordinateInBlocks(0, 1, 0),  inBlockID,  BlockInfo() };
		this->buttonBlocks[4] = { CoordinateInBlocks(0, -1, 0), outBlockID, BlockInfo() };

		this->buttonBlocks[12] = { buttonBlocks[0].position + CoordinateInBlocks(0, -1, 0), nextBlockID, BlockInfo() };
		this->buttonBlocks[13] = { buttonBlocks[0].position + CoordinateInBlocks(0, 1, 0),  prevBlockID, BlockInfo() };

		this->buttonBlocks[18] = { CoordinateInBlocks(0, 1, 0), oresBlockID, BlockInfo() };

		this->buttonBlocks[8]  = { CoordinateInBlocks(0, 2, 1),   tArrowLeftBlockID,  BlockInfo() };
		this->buttonBlocks[9]  = { CoordinateInBlocks(0, -2, 1),  tArrowRightBlockID, BlockInfo() };
		this->buttonBlocks[10] = { CoordinateInBlocks(0, -1, 2),  tArrowUpBlockID,    BlockInfo() };
		this->buttonBlocks[11] = { CoordinateInBlocks(0, -1, -2), tArrowDownBlockID,  BlockInfo() };

		this->buttonBlocks[14] = { CoordinateInBlocks(0, -2, -1), tArrowLeftBlockID,  BlockInfo() };
		this->buttonBlocks[15] = { CoordinateInBlocks(0, 2, -1),  tArrowRightBlockID, BlockInfo() };
		this->buttonBlocks[16] = { CoordinateInBlocks(0, 1, -2),  tArrowUpBlockID,    BlockInfo() };
		this->buttonBlocks[17] = { CoordinateInBlocks(0, 1, 2),   tArrowDownBlockID,  BlockInfo() };
	}
	else if (playerLocation.Y < blockPositionCm.Y - 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		this->digDirection = 3;
		this->buttonBlocks[1] = { CoordinateInBlocks(1, 0, 0),  upBlockID,   BlockInfo() };
		this->buttonBlocks[2] = { CoordinateInBlocks(-1, 0, 0), downBlockID, BlockInfo() };

		this->buttonBlocks[3] = { CoordinateInBlocks(1, 0, 0),  inBlockID,  BlockInfo() };
		this->buttonBlocks[4] = { CoordinateInBlocks(-1, 0, 0), outBlockID, BlockInfo() };

		this->buttonBlocks[12] = { buttonBlocks[0].position + CoordinateInBlocks(-1, 0, 0), nextBlockID, BlockInfo() };
		this->buttonBlocks[13] = { buttonBlocks[0].position + CoordinateInBlocks(1, 0, 0),  prevBlockID, BlockInfo() };

		this->buttonBlocks[18] = { CoordinateInBlocks(1, 0, 0), oresBlockID, BlockInfo() };

		this->buttonBlocks[8]  = { CoordinateInBlocks(2, 0, 1),   tArrowLeftBlockID,  BlockInfo() };
		this->buttonBlocks[9]  = { CoordinateInBlocks(-2, 0, 1),  tArrowRightBlockID, BlockInfo() };
		this->buttonBlocks[10] = { CoordinateInBlocks(-1, 0, 2),  tArrowUpBlockID,    BlockInfo() };
		this->buttonBlocks[11] = { CoordinateInBlocks(-1, 0, -2), tArrowDownBlockID,  BlockInfo() };

		this->buttonBlocks[14] = { CoordinateInBlocks(-2, 0, -1), tArrowLeftBlockID,  BlockInfo() };
		this->buttonBlocks[15] = { CoordinateInBlocks(2, 0, -1),  tArrowRightBlockID, BlockInfo() };
		this->buttonBlocks[16] = { CoordinateInBlocks(1, 0, -2),  tArrowUpBlockID,    BlockInfo() };
		this->buttonBlocks[17] = { CoordinateInBlocks(1, 0, 2),   tArrowDownBlockID,  BlockInfo() };
	}
	else if (playerLocation.Y > blockPositionCm.Y + 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		this->digDirection = 4;
		this->buttonBlocks[1] = { CoordinateInBlocks(-1, 0, 0), upBlockID,   BlockInfo() };
		this->buttonBlocks[2] = { CoordinateInBlocks(1, 0, 0),  downBlockID, BlockInfo() };

		this->buttonBlocks[3] = { CoordinateInBlocks(-1, 0, 0), inBlockID,  BlockInfo() };
		this->buttonBlocks[4] = { CoordinateInBlocks(1, 0, 0),  outBlockID, BlockInfo() };

		this->buttonBlocks[12] = { buttonBlocks[0].position + CoordinateInBlocks(1, 0, 0),  nextBlockID, BlockInfo() };
		this->buttonBlocks[13] = { buttonBlocks[0].position + CoordinateInBlocks(-1, 0, 0), prevBlockID, BlockInfo() };

		this->buttonBlocks[18] = { CoordinateInBlocks(-1, 0, 0), oresBlockID, BlockInfo() };

		this->buttonBlocks[8]  = { CoordinateInBlocks(-2, 0, 1), tArrowLeftBlockID,  BlockInfo() };
		this->buttonBlocks[9]  = { CoordinateInBlocks(2, 0, 1),  tArrowRightBlockID, BlockInfo() };
		this->buttonBlocks[10] = { CoordinateInBlocks(1, 0, 2),  tArrowUpBlockID,    BlockInfo() };
		this->buttonBlocks[11] = { CoordinateInBlocks(1, 0, -2), tArrowDownBlockID,  BlockInfo() };

		this->buttonBlocks[14] = { CoordinateInBlocks(2, 0, -1),  tArrowLeftBlockID,  BlockInfo() };
		this->buttonBlocks[15] = { CoordinateInBlocks(-2, 0, -1), tArrowRightBlockID, BlockInfo() };
		this->buttonBlocks[16] = { CoordinateInBlocks(-1, 0, -2), tArrowUpBlockID,    BlockInfo() };
		this->buttonBlocks[17] = { CoordinateInBlocks(-1, 0, 2),  tArrowDownBlockID,  BlockInfo() };
	}

	updateCornerBlocks();
	resetDigBlock();
	showNormal(true);
}

TunnelBlock::TunnelBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock,
	std::array<Block, 4> cornerBlocks, std::array<Block, buttonBlocksAmount> buttonBlocks, int settingsPage, bool digOres,
	int digDirection)
	: DiggingBlock(length, width, depth, blockPosition, currentMode, currentDigBlock, cornerBlocks, buttonBlocks, settingsPage, digOres, digDirection) {

}

void TunnelBlock::updateCornerBlocks() {
	switch (digDirection) {
	case 1: 
		cornerBlocks[0] = { CoordinateInBlocks(0, -2, 2),  tMark11BlockID, BlockInfo() };
		cornerBlocks[1] = { CoordinateInBlocks(0, 2, 2),   tMark12BlockID, BlockInfo() };
		cornerBlocks[2] = { CoordinateInBlocks(0, -2, -2), tMark13BlockID, BlockInfo() };
		cornerBlocks[3] = { CoordinateInBlocks(0, 2, -2),  tMark14BlockID, BlockInfo() };
		break;
	case 2:
		cornerBlocks[0] = { CoordinateInBlocks(0, 2, 2),   tMark21BlockID, BlockInfo() };
		cornerBlocks[1] = { CoordinateInBlocks(0, -2, 2),  tMark22BlockID, BlockInfo() };
		cornerBlocks[2] = { CoordinateInBlocks(0, 2, -2),  tMark23BlockID, BlockInfo() };
		cornerBlocks[3] = { CoordinateInBlocks(0, -2, -2), tMark24BlockID, BlockInfo() };
		break;
	case 3:
		cornerBlocks[0] = { CoordinateInBlocks(2, 0, 2),   tMark31BlockID, BlockInfo() };
		cornerBlocks[1] = { CoordinateInBlocks(-2, 0, 2),  tMark32BlockID, BlockInfo() };
		cornerBlocks[2] = { CoordinateInBlocks(2, 0, -2),  tMark33BlockID, BlockInfo() };
		cornerBlocks[3] = { CoordinateInBlocks(-2, 0, -2), tMark34BlockID, BlockInfo() };
		break;
	case 4:
		cornerBlocks[0] = { CoordinateInBlocks(-2, 0, 2),  tMark41BlockID, BlockInfo() };
		cornerBlocks[1] = { CoordinateInBlocks(2, 0, 2),   tMark42BlockID, BlockInfo() };
		cornerBlocks[2] = { CoordinateInBlocks(-2, 0, -2), tMark43BlockID, BlockInfo() };
		cornerBlocks[3] = { CoordinateInBlocks(2, 0, -2),  tMark44BlockID, BlockInfo() };
		break;
	}
}

void TunnelBlock::dig() {
	if (currentMode == 3) {
		BlockInfo currentBlock = GetBlock(blockPosition + currentDigBlock);
		if (diggableBlock(currentBlock)) {
			SetBlock(blockPosition + currentDigBlock, EBlockType::Air);

			// If block was Grass, remove possible flower/foliage on top.
			if (currentBlock.Type == EBlockType::Grass) {
				BlockInfo blockAbove = GetBlock(blockPosition + currentDigBlock + CoordinateInBlocks(0, 0, 1));
				if (blockAbove.Type != EBlockType::ModBlock && (blockAbove.Type == EBlockType::Flower1 || blockAbove.Type == EBlockType::Flower2 ||
					blockAbove.Type == EBlockType::Flower3 || blockAbove.Type == EBlockType::Flower4 || blockAbove.Type == EBlockType::FlowerRainbow ||
					blockAbove.Type == EBlockType::GrassFoliage)) {
					SetBlock(blockPosition + currentDigBlock + CoordinateInBlocks(0, 0, 1), EBlockType::Air);
				}
			}
		}

		switch (digDirection) {
		case 1:
			currentDigBlock.Y++;
			if (currentDigBlock.Y > cornerBlocks[1].position.Y) {
				currentDigBlock.Y = cornerBlocks[0].position.Y;
				currentDigBlock.Z--;
				if (currentDigBlock.Z < cornerBlocks[2].position.Z) {
					currentDigBlock.Z = cornerBlocks[1].position.Z;
					currentDigBlock.X++;
					if (currentDigBlock.X > depth) {
						currentDigBlock.X = 1;
						finishedDigging();
					}
				}
			}
			break;
		case 2:
			currentDigBlock.Y--;
			if (currentDigBlock.Y < cornerBlocks[1].position.Y) {
				currentDigBlock.Y = cornerBlocks[0].position.Y;
				currentDigBlock.Z--;
				if (currentDigBlock.Z < cornerBlocks[2].position.Z) {
					currentDigBlock.Z = cornerBlocks[1].position.Z;
					currentDigBlock.X--;
					if (currentDigBlock.X < -depth) {
						currentDigBlock.X = -1;
						finishedDigging();
					}
				}
			}
			break;
		case 3:
			currentDigBlock.X--;
			if (currentDigBlock.X < cornerBlocks[1].position.X) {
				currentDigBlock.X = cornerBlocks[0].position.X;
				currentDigBlock.Z--;
				if (currentDigBlock.Z < cornerBlocks[2].position.Z) {
					currentDigBlock.Z = cornerBlocks[1].position.Z;
					currentDigBlock.Y++;
					if (currentDigBlock.Y > depth) {
						currentDigBlock.Y = 1;
						finishedDigging();
					}
				}
			}
			break;
		case 4:
			currentDigBlock.X++;
			if (currentDigBlock.X > cornerBlocks[1].position.X) {
				currentDigBlock.X = cornerBlocks[0].position.X;
				currentDigBlock.Z--;
				if (currentDigBlock.Z < cornerBlocks[2].position.Z) {
					currentDigBlock.Z = cornerBlocks[1].position.Z;
					currentDigBlock.Y--;
					if (currentDigBlock.Y < -depth) {
						currentDigBlock.Y = -1;
						finishedDigging();
					}
				}
			}
			break;
		}
	}
}

void TunnelBlock::incrementLength(char block) {
	if (currentMode == 2) {
		removeCorners();
		if (block == 'b') {
			cornerBlocks[0].position.Z++;
			cornerBlocks[1].position.Z++;
			length++;
		}
		else if (block == 'f') {
			cornerBlocks[2].position.Z--;
			cornerBlocks[3].position.Z--;
			length++;
		}
		setCorners();
		resetDigBlock();
	}
}

void TunnelBlock::decrementLength(char block) {
	if (currentMode == 2 && length > 5) {
		removeCorners();
		if (block == 'f' && cornerBlocks[0].position.Z > 2) {
			cornerBlocks[0].position.Z--;
			cornerBlocks[1].position.Z--;
			length--;
		}
		else if (block == 'b' && cornerBlocks[2].position.Z < -2) {
			cornerBlocks[2].position.Z++;
			cornerBlocks[3].position.Z++;
			length--;
		}
		setCorners();
		resetDigBlock();
	}
}

void TunnelBlock::incrementWidth(char block) {
	if (currentMode == 2) {
		removeCorners();
		if (block == 'r') {
			switch (digDirection) {
			case 1:
				cornerBlocks[1].position.Y++;
				cornerBlocks[3].position.Y++;
				width++;
				break;
			case 2:
				cornerBlocks[1].position.Y--;
				cornerBlocks[3].position.Y--;
				width++;
				break;
			case 3:
				cornerBlocks[1].position.X--;
				cornerBlocks[3].position.X--;
				width++;
				break;
			case 4:
				cornerBlocks[1].position.X++;
				cornerBlocks[3].position.X++;
				width++;
				break;
			}
		}
		else if (block == 'l') {
			switch (digDirection) {
			case 1:
				cornerBlocks[0].position.Y--;
				cornerBlocks[2].position.Y--;
				width++;
				break;
			case 2:
				cornerBlocks[0].position.Y++;
				cornerBlocks[2].position.Y++;
				width++;
				break;
			case 3:
				cornerBlocks[0].position.X++;
				cornerBlocks[2].position.X++;
				width++;
				break;
			case 4:
				cornerBlocks[0].position.X--;
				cornerBlocks[2].position.X--;
				width++;
				break;
			}
		}
		setCorners();
		resetDigBlock();
	}
}

void TunnelBlock::decrementWidth(char block) {
	if (currentMode == 2 && width > 5) {
		removeCorners();
		if (block == 'l') {
			switch (digDirection) {
			case 1:
				if (cornerBlocks[1].position.Y > 2) {
					cornerBlocks[1].position.Y--;
					cornerBlocks[3].position.Y--;
					width--;
				}
				break;
			case 2:
				if (cornerBlocks[1].position.Y < -2) {
					cornerBlocks[1].position.Y++;
					cornerBlocks[3].position.Y++;
					width--;
				}
				break;
			case 3:
				if (cornerBlocks[1].position.X < -2) {
					cornerBlocks[1].position.X++;
					cornerBlocks[3].position.X++;
					width--;
				}
				break;
			case 4:
				if (cornerBlocks[1].position.X > 2) {
					cornerBlocks[1].position.X--;
					cornerBlocks[3].position.X--;
					width--;
				}
				break;
			}
		}
		else if (block == 'r') {
			switch (digDirection) {
			case 1:
				if (cornerBlocks[0].position.Y < -2) {
					cornerBlocks[0].position.Y++;
					cornerBlocks[2].position.Y++;
					width--;
				}
				break;
			case 2:
				if (cornerBlocks[0].position.Y > 2) {
					cornerBlocks[0].position.Y--;
					cornerBlocks[2].position.Y--;
					width--;
				}
				break;
			case 3:
				if (cornerBlocks[0].position.X > 2) {
					cornerBlocks[0].position.X--;
					cornerBlocks[2].position.X--;
					width--;
				}
				break;
			case 4:
				if (cornerBlocks[0].position.X < -2) {
					cornerBlocks[0].position.X++;
					cornerBlocks[2].position.X++;
					width--;
				}
				break;
			}
		}
		setCorners();
		resetDigBlock();
	}
}

void TunnelBlock::showSettings(bool show) {
	if (show) {
		setCorners();
		setSettingsBlocks();
	}
	else {
		removeCorners();
		removeSettingsBlocks();
	}
}

void TunnelBlock::showNormal(bool show) {
	if (show) {
		setCorners();
		setButtonBlock(0); // Set Settings block
		setButtonBlock(6); // Set Check Mark block
	}
	else {
		removeCorners();
		removeButtonBlock(0);
		removeButtonBlock(6);
	}
}

void TunnelBlock::showFinished(bool show) {
	if (show) {
		setButtonBlock(5); // Set Exclamation Mark block
	}
	else {
		removeButtonBlock(5);
	}
}

void TunnelBlock::showDigging(bool show) {
	if (show) {
		setButtonBlock(7); // Set Cross Mark block
	}
	else {
		removeButtonBlock(7);
	}
}

void TunnelBlock::resetDigBlock() {
	switch (digDirection) {
	case 1:
		currentDigBlock.X = 1;
		currentDigBlock.Y = cornerBlocks[0].position.Y;
		currentDigBlock.Z = cornerBlocks[0].position.Z;
		break;
	case 2:
		currentDigBlock.X = -1;
		currentDigBlock.Y = cornerBlocks[0].position.Y;
		currentDigBlock.Z = cornerBlocks[0].position.Z;
		break;
	case 3:
		currentDigBlock.X = cornerBlocks[0].position.X;
		currentDigBlock.Y = 1;
		currentDigBlock.Z = cornerBlocks[0].position.Z;
		break;
	case 4:
		currentDigBlock.X = cornerBlocks[0].position.X;
		currentDigBlock.Y = -1;
		currentDigBlock.Z = cornerBlocks[0].position.Z;
		break;
	}
}

void TunnelBlock::setSettingsBlocks() {
	setButtonBlock(0); // Set Settings block

	if (settingsPage != 1) {
		setButtonBlock(13); // Set Previous block
	}
	if (settingsPage != 4) {
		setButtonBlock(12); // Set Next block
	}


	if (settingsPage == 1) {
		setButtonBlock(1); // Set Up block
		setButtonBlock(2); // Set Down block
	}
	else if (settingsPage == 2) {
		setButtonBlock(3); // Set In block
		setButtonBlock(4); // Set Out block
	}
	else if (settingsPage == 3) {
		setButtonBlock(8); // Set Left block (increment)
		setButtonBlock(9); // Set Right block (increment)
		setButtonBlock(10); // Set Back block (increment)
		setButtonBlock(11); // Set Front block (increment)
		setButtonBlock(14); // Set Left block (decrement)
		setButtonBlock(15); // Set Right block (decrement)
		setButtonBlock(16); // Set Back block (decrement)
		setButtonBlock(17); // Set Front block (decrement)
	}
	else if (settingsPage == 4) {
		setButtonBlock(18); // Set Ores block
	}
}

void TunnelBlock::removeSettingsBlocks() {
	removeButtonBlock(0);

	if (settingsPage != 1) {
		removeButtonBlock(13);
	}
	if (settingsPage != 4) {
		removeButtonBlock(12);
	}


	if (settingsPage == 1) {
		removeButtonBlock(1);
		removeButtonBlock(2);
	}
	else if (settingsPage == 2) {
		removeButtonBlock(3);
		removeButtonBlock(4);
	}
	else if (settingsPage == 3) {
		removeButtonBlock(8);
		removeButtonBlock(9);
		removeButtonBlock(10);
		removeButtonBlock(11);
		removeButtonBlock(14);
		removeButtonBlock(15);
		removeButtonBlock(16);
		removeButtonBlock(17);
	}
	else if (settingsPage == 4) {
		removeButtonBlock(18);
	}
}
