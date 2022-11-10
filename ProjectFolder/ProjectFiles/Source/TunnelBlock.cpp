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

TunnelBlock::TunnelBlock(int length, int width, int depth, CoordinateInBlocks blockPosition, int currentMode, CoordinateInBlocks currentDigBlock, std::array<CoordinateInBlocks,4> corners, bool digOres, int digDirection)
	: DiggingBlock(length, width, depth, blockPosition, currentMode, currentDigBlock, corners, digOres, digDirection) 
{
}

void TunnelBlock::updateCornerBlocks() 
{
	switch (digDirection) {
	case 1: 
		corners[0] = CoordinateInBlocks(0, -2, 2);
		corners[1] = CoordinateInBlocks(0, 2, 2);
		corners[2] = CoordinateInBlocks(0, -2, -2);
		corners[3] = CoordinateInBlocks(0, 2, -2);
		break;
	case 2:
		corners[0] = CoordinateInBlocks(0, 2, 2);
		corners[1] = CoordinateInBlocks(0, -2, 2);
		corners[2] = CoordinateInBlocks(0, 2, -2);
		corners[3] = CoordinateInBlocks(0, -2, -2);
		break;
	case 3:
		corners[0] = CoordinateInBlocks(2, 0, 2);
		corners[1] = CoordinateInBlocks(-2, 0, 2);
		corners[2] = CoordinateInBlocks(2, 0, -2);
		corners[3] = CoordinateInBlocks(-2, 0, -2);
		break;
	case 4:
		corners[0] = CoordinateInBlocks(-2, 0, 2);
		corners[1] = CoordinateInBlocks(2, 0, 2);
		corners[2] = CoordinateInBlocks(-2, 0, -2);
		corners[3] = CoordinateInBlocks(2, 0, -2);
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
				if (currentDigBlock.Y > corners[1].Y) {
					currentDigBlock.Y = corners[0].Y;
					currentDigBlock.Z--;
					if (currentDigBlock.Z < corners[2].Z) {
						currentDigBlock.Z = corners[1].Z;
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
				if (currentDigBlock.Y < corners[1].Y) {
					currentDigBlock.Y = corners[0].Y;
					currentDigBlock.Z--;
					if (currentDigBlock.Z < corners[2].Z) {
						currentDigBlock.Z = corners[1].Z;
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
				if (currentDigBlock.X < corners[1].X) {
					currentDigBlock.X = corners[0].X;
					currentDigBlock.Z--;
					if (currentDigBlock.Z < corners[2].Z) {
						currentDigBlock.Z = corners[1].Z;
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
				if (currentDigBlock.X > corners[1].X) {
					currentDigBlock.X = corners[0].X;
					currentDigBlock.Z--;
					if (currentDigBlock.Z < corners[2].Z) {
						currentDigBlock.Z = corners[1].Z;
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
		removeAreaSelection();
		if (direction == 'u' && blockPosition.Z + corners[0].Z < 798) {
			corners[0].Z++;
			corners[1].Z++;
			length++;
		}
		else if (direction == 'd' && blockPosition.Z + corners[2].Z > 1) {
			corners[2].Z--;
			corners[3].Z--;
			length++;
		}
		setAreaSelection();
		resetDigBlock();
	}
}

void TunnelBlock::decrementLength(char direction) 
{
	if (currentMode == 2 && length > 3) {
		removeAreaSelection();
		if (direction == 'u' && corners[0].Z > 1) {
			corners[0].Z--;
			corners[1].Z--;
			length--;
		}
		else if (direction == 'd' && corners[2].Z < -1) {
			corners[2].Z++;
			corners[3].Z++;
			length--;
		}
		setAreaSelection();
		resetDigBlock();
	}
}

void TunnelBlock::incrementWidth(char direction) 
{
	if (currentMode == 2) {
		removeAreaSelection();
		if (direction == 'r') {
			switch (digDirection) {
			case 1:
				corners[1].Y++;
				corners[3].Y++;
				width++;
				break;
			case 2:
				corners[1].Y--;
				corners[3].Y--;
				width++;
				break;
			case 3:
				corners[1].X--;
				corners[3].X--;
				width++;
				break;
			case 4:
				corners[1].X++;
				corners[3].X++;
				width++;
				break;
			}
		}
		else if (direction == 'l') {
			switch (digDirection) {
			case 1:
				corners[0].Y--;
				corners[2].Y--;
				width++;
				break;
			case 2:
				corners[0].Y++;
				corners[2].Y++;
				width++;
				break;
			case 3:
				corners[0].X++;
				corners[2].X++;
				width++;
				break;
			case 4:
				corners[0].X--;
				corners[2].X--;
				width++;
				break;
			}
		}
		setAreaSelection();
		resetDigBlock();
	}
}

void TunnelBlock::decrementWidth(char direction) 
{
	if (currentMode == 2 && width > 3) {
		removeAreaSelection();
		if (direction == 'r') {
			switch (digDirection) {
			case 1:
				if (corners[1].Y > 1) {
					corners[1].Y--;
					corners[3].Y--;
					width--;
				}
				break;
			case 2:
				if (corners[1].Y < -1) {
					corners[1].Y++;
					corners[3].Y++;
					width--;
				}
				break;
			case 3:
				if (corners[1].X < -1) {
					corners[1].X++;
					corners[3].X++;
					width--;
				}
				break;
			case 4:
				if (corners[1].X > 1) {
					corners[1].X--;
					corners[3].X--;
					width--;
				}
				break;
			}
		}
		else if (direction == 'l') {
			switch (digDirection) {
			case 1:
				if (corners[0].Y < -1) {
					corners[0].Y++;
					corners[2].Y++;
					width--;
				}
				break;
			case 2:
				if (corners[0].Y > 1) {
					corners[0].Y--;
					corners[2].Y--;
					width--;
				}
				break;
			case 3:
				if (corners[0].X > 1) {
					corners[0].X--;
					corners[2].X--;
					width--;
				}
				break;
			case 4:
				if (corners[0].X < -1) {
					corners[0].X++;
					corners[2].X++;
					width--;
				}
				break;
			}
		}
		setAreaSelection();
		resetDigBlock();
	}
}

void TunnelBlock::resetDigBlock() 
{
	switch (digDirection) {
	case 1:
		currentDigBlock.X = 1;
		currentDigBlock.Y = corners[0].Y;
		currentDigBlock.Z = corners[0].Z;
		break;
	case 2:
		currentDigBlock.X = -1;
		currentDigBlock.Y = corners[0].Y;
		currentDigBlock.Z = corners[0].Z;
		break;
	case 3:
		currentDigBlock.X = corners[0].X;
		currentDigBlock.Y = 1;
		currentDigBlock.Z = corners[0].Z;
		break;
	case 4:
		currentDigBlock.X = corners[0].X;
		currentDigBlock.Y = -1;
		currentDigBlock.Z = corners[0].Z;
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

void TunnelBlock::setAreaSelection()
{
	switch (digDirection) {
	case 1:
		for (int i = blockPosition.Y + corners[2].Y; i <= blockPosition.Y + corners[1].Y; i++) {
			for (int j = blockPosition.Z + corners[2].Z; j <= blockPosition.Z + corners[1].Z; j++) {
				SpawnBPModActor(CoordinateInBlocks(blockPosition.X, i, j), L"ParticleActors", L"AreaSelectionParticle+X");
			}
		}
		break;
	case 2:
		for (int i = blockPosition.Y + corners[3].Y; i <= blockPosition.Y + corners[0].Y; i++) {
			for (int j = blockPosition.Z + corners[3].Z; j <= blockPosition.Z + corners[0].Z; j++) {
				SpawnBPModActor(CoordinateInBlocks(blockPosition.X, i, j), L"ParticleActors", L"AreaSelectionParticle-X");
			}
		}
		break;
	case 3:
		for (int i = blockPosition.X + corners[3].X; i <= blockPosition.X + corners[0].X; i++) {
			for (int j = blockPosition.Z + corners[3].Z; j <= blockPosition.Z + corners[0].Z; j++) {
				SpawnBPModActor(CoordinateInBlocks(i, blockPosition.Y, j), L"ParticleActors", L"AreaSelectionParticle+Y");
			}
		}
		break;
	case 4:
		for (int i = blockPosition.X + corners[2].X; i <= blockPosition.X + corners[1].X; i++) {
			for (int j = blockPosition.Z + corners[2].Z; j <= blockPosition.Z + corners[1].Z; j++) {
				SpawnBPModActor(CoordinateInBlocks(i, blockPosition.Y, j), L"ParticleActors", L"AreaSelectionParticle-Y");
			}
		}
		break;
	}
}