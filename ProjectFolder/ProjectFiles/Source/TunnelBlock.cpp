#include "TunnelBlock.h"

TunnelBlock::TunnelBlock(CoordinateInBlocks blockPosition)
	: DiggingBlock(blockPosition) 
{

	CoordinateInCentimeters playerLocation = GetPlayerLocation();
	CoordinateInCentimeters blockPositionCm = CoordinateInCentimeters(blockPosition);
	int64_t xDifference = std::abs(playerLocation.X - blockPositionCm.X);
	int64_t yDifference = std::abs(playerLocation.Y - blockPositionCm.Y);
	
	if (playerLocation.X < blockPositionCm.X - 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		this->digDirection = 1;
	}
	else if (playerLocation.X > blockPositionCm.X + 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		this->digDirection = 2;
	}
	else if (playerLocation.Y < blockPositionCm.Y - 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		this->digDirection = 3;
	}
	else if (playerLocation.Y > blockPositionCm.Y + 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		this->digDirection = 4;
	}

	if (blockPosition.Z < 3 || blockPosition.Z > 797) {
		destroy();
		SpawnHintTextAdvanced(blockPosition + CoordinateInBlocks(0, 0, 1), L"Cannot place block here.", 3);
		return;
	}

	updateCornerBlocks();
	resetDigBlock();
	toggleSettings();
}

TunnelBlock::TunnelBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock,
	std::array<Block, 4> cornerBlocks, bool digOres, int digDirection)
	: DiggingBlock(length, width, depth, blockPosition, currentMode, currentDigBlock, cornerBlocks, digOres, digDirection) 
{
}

void TunnelBlock::updateCornerBlocks() 
{
	switch (digDirection) {
	case 1: 
		cornerBlocks[0] = { CoordinateInBlocks(0, -2, 2),  markerBlockID, BlockInfo() };
		cornerBlocks[1] = { CoordinateInBlocks(0, 2, 2),   markerBlockID, BlockInfo() };
		cornerBlocks[2] = { CoordinateInBlocks(0, -2, -2), markerBlockID, BlockInfo() };
		cornerBlocks[3] = { CoordinateInBlocks(0, 2, -2),  markerBlockID, BlockInfo() };
		break;
	case 2:
		cornerBlocks[0] = { CoordinateInBlocks(0, 2, 2),   markerBlockID, BlockInfo() };
		cornerBlocks[1] = { CoordinateInBlocks(0, -2, 2),  markerBlockID, BlockInfo() };
		cornerBlocks[2] = { CoordinateInBlocks(0, 2, -2),  markerBlockID, BlockInfo() };
		cornerBlocks[3] = { CoordinateInBlocks(0, -2, -2), markerBlockID, BlockInfo() };
		break;
	case 3:
		cornerBlocks[0] = { CoordinateInBlocks(2, 0, 2),   markerBlockID, BlockInfo() };
		cornerBlocks[1] = { CoordinateInBlocks(-2, 0, 2),  markerBlockID, BlockInfo() };
		cornerBlocks[2] = { CoordinateInBlocks(2, 0, -2),  markerBlockID, BlockInfo() };
		cornerBlocks[3] = { CoordinateInBlocks(-2, 0, -2), markerBlockID, BlockInfo() };
		break;
	case 4:
		cornerBlocks[0] = { CoordinateInBlocks(-2, 0, 2),  markerBlockID, BlockInfo() };
		cornerBlocks[1] = { CoordinateInBlocks(2, 0, 2),   markerBlockID, BlockInfo() };
		cornerBlocks[2] = { CoordinateInBlocks(-2, 0, -2), markerBlockID, BlockInfo() };
		cornerBlocks[3] = { CoordinateInBlocks(2, 0, -2),  markerBlockID, BlockInfo() };
		break;
	}
}

void TunnelBlock::dig() 
{
	if (currentMode == 3) {
		if (digSuccess()) {
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
}

void TunnelBlock::incrementLength(char direction) 
{
	if (currentMode == 2) {
		removeCorners();
		if (direction == 'u' && blockPosition.Z + cornerBlocks[0].position.Z < 798) {
			cornerBlocks[0].position.Z++;
			cornerBlocks[1].position.Z++;
			length++;
		}
		else if (direction == 'd' && blockPosition.Z + cornerBlocks[2].position.Z > 1) {
			cornerBlocks[2].position.Z--;
			cornerBlocks[3].position.Z--;
			length++;
		}
		setCorners();
		resetDigBlock();
	}
}

void TunnelBlock::decrementLength(char direction) 
{
	if (currentMode == 2 && length > 3) {
		removeCorners();
		if (direction == 'u' && cornerBlocks[0].position.Z > 1) {
			cornerBlocks[0].position.Z--;
			cornerBlocks[1].position.Z--;
			length--;
		}
		else if (direction == 'd' && cornerBlocks[2].position.Z < -1) {
			cornerBlocks[2].position.Z++;
			cornerBlocks[3].position.Z++;
			length--;
		}
		setCorners();
		resetDigBlock();
	}
}

void TunnelBlock::incrementWidth(char direction) 
{
	if (currentMode == 2) {
		removeCorners();
		if (direction == 'r') {
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
		else if (direction == 'l') {
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

void TunnelBlock::decrementWidth(char direction) 
{
	if (currentMode == 2 && width > 3) {
		removeCorners();
		if (direction == 'r') {
			switch (digDirection) {
			case 1:
				if (cornerBlocks[1].position.Y > 1) {
					cornerBlocks[1].position.Y--;
					cornerBlocks[3].position.Y--;
					width--;
				}
				break;
			case 2:
				if (cornerBlocks[1].position.Y < -1) {
					cornerBlocks[1].position.Y++;
					cornerBlocks[3].position.Y++;
					width--;
				}
				break;
			case 3:
				if (cornerBlocks[1].position.X < -1) {
					cornerBlocks[1].position.X++;
					cornerBlocks[3].position.X++;
					width--;
				}
				break;
			case 4:
				if (cornerBlocks[1].position.X > 1) {
					cornerBlocks[1].position.X--;
					cornerBlocks[3].position.X--;
					width--;
				}
				break;
			}
		}
		else if (direction == 'l') {
			switch (digDirection) {
			case 1:
				if (cornerBlocks[0].position.Y < -1) {
					cornerBlocks[0].position.Y++;
					cornerBlocks[2].position.Y++;
					width--;
				}
				break;
			case 2:
				if (cornerBlocks[0].position.Y > 1) {
					cornerBlocks[0].position.Y--;
					cornerBlocks[2].position.Y--;
					width--;
				}
				break;
			case 3:
				if (cornerBlocks[0].position.X > 1) {
					cornerBlocks[0].position.X--;
					cornerBlocks[2].position.X--;
					width--;
				}
				break;
			case 4:
				if (cornerBlocks[0].position.X < -1) {
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

void TunnelBlock::resetDigBlock() 
{
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

void TunnelBlock::clickCheck(CoordinateInCentimeters fingerLocation, bool& canClick, CoordinateInCentimeters positionCm, bool leftHand)
{
	if (canClick) {
		switch (digDirection) {
		case 1:
			if (fingerLocation.X >= positionCm.X - 25) {
				clickRegister(fingerLocation, leftHand);
				canClick = false;
			}
			break;
		case 2:
			if (fingerLocation.X < positionCm.X + 25) {
				clickRegister(fingerLocation, leftHand);
				canClick = false;
			}
			break;
		case 3:
			if (fingerLocation.Y > positionCm.Y - 25) {
				clickRegister(fingerLocation, leftHand);
				canClick = false;
			}
			break;
		case 4:
			if (fingerLocation.Y <= positionCm.Y + 25) {
				clickRegister(fingerLocation, leftHand);
				canClick = false;
			}
			break;
		}
	}
	else {
		switch (digDirection) {
		case 1:
			if (fingerLocation.X < positionCm.X - 25) {
				canClick = true;
			}
			break;
		case 2:
			if (fingerLocation.X >= positionCm.X + 25) {
				canClick = true;
			}
			break;
		case 3:
			if (fingerLocation.Y <= positionCm.Y - 25) {
				canClick = true;
			}
			break;
		case 4:
			if (fingerLocation.Y > positionCm.Y + 25) {
				canClick = true;
			}
			break;
		}
	}
}

CoordinateInCentimeters TunnelBlock::getCorner()
{
	CoordinateInCentimeters corner{};
	switch (digDirection) {
	case 1:
		corner = blockPosition + CoordinateInCentimeters(0, -25, -25);
		break;
	case 2:
		corner = blockPosition + CoordinateInCentimeters(0, 25, -25);
		break;
	case 3:
		corner = blockPosition + CoordinateInCentimeters(25, 0, -25);
		break;
	case 4:
		corner = blockPosition + CoordinateInCentimeters(-25, 0, -25);
		break;
	}
	return corner;
}

bool TunnelBlock::isBetween(std::pair<int, int> bottomLeft, std::pair<int, int> topRight, CoordinateInCentimeters fingerPos)
{
	CoordinateInCentimeters corner = getCorner();
	bool result = false;

	switch (digDirection) {
	case 1:
		result = fingerPos.Z >= corner.Z + bottomLeft.first - 1 && fingerPos.Z <= corner.Z + topRight.first - 1 && fingerPos.Y >= corner.Y + bottomLeft.second && fingerPos.Y <= corner.Y + topRight.second;
		break;
	case 2:
		result = fingerPos.Z >= corner.Z + bottomLeft.first - 1 && fingerPos.Z <= corner.Z + topRight.first - 1 && fingerPos.Y <= corner.Y - bottomLeft.second + 1 && fingerPos.Y >= corner.Y - topRight.second + 1;
		break;
	case 3:
		result = fingerPos.Z >= corner.Z + bottomLeft.first - 1 && fingerPos.Z <= corner.Z + topRight.first - 1 && fingerPos.X <= corner.X - bottomLeft.second && fingerPos.X >= corner.X - topRight.second;
		break;
	case 4:
		result = fingerPos.Z >= corner.Z + bottomLeft.first - 1 && fingerPos.Z <= corner.Z + topRight.first - 1 && fingerPos.X >= corner.X + bottomLeft.second - 1 && fingerPos.X <= corner.X + topRight.second - 1;
		break;
	}

	return result;
}

void TunnelBlock::setOffBlock()
{
	SetBlock(blockPosition, tunOffBlockID);
}

void TunnelBlock::setOnBlock()
{
	SetBlock(blockPosition, tunOnBlockID);
}

void TunnelBlock::setSettingsBlock()
{
	SetBlock(blockPosition, tunSetBlockID);
}

void TunnelBlock::setNormalBlock()
{
	SetBlock(blockPosition, tunBlockID);
}

CoordinateInCentimeters TunnelBlock::getHintTextLocation()
{
	return blockPosition + CoordinateInCentimeters(0, 0, 50);
}
