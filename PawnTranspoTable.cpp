#include "Include.h"

PawnHashKey PawnTranspoTable::ZobristKeys[2][64] = {0};

PawnTranspoTable::PawnTranspoTable()
{
}

void PawnTranspoTable::InitializeHashKeys()
{
	std::mt19937 generator;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			ZobristKeys[i][j] = generator();
		}
	}
}

int PawnTranspoTable::HashFunction(PawnHashKey key)
{
	return key % PawnTableSize;
}

void PawnTranspoTable::RecordTableNode(PawnHashKey key, int score)
{
	int index = HashFunction(key);
	PawnTableNode* node = &nodeArray[index];
	node->hashKey = key;
	node->score = score;
}

PawnTableNode* PawnTranspoTable::ProbeTableNode()
{
	return &nodeArray[HashFunction(pawnStructureKey)];
}

int PawnTranspoTable::GetNumberElements()
{
	int num = 0;
	for (int i = 0; i < PawnTableSize; i++)
	{
		if (nodeArray[i].hashKey != 0)
		{
			num++;
		}
	}
	return num;
}