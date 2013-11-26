#include "Include.h"

#define RandomKey(gen) (HashKey)gen() << 32 ^ gen()

HashKey TranspoTable::ZobristKeys[12][64] = {0};
HashKey TranspoTable::EnPassantKeys[64] = {0};
HashKey TranspoTable::ChangeSideKey = 0;
HashKey TranspoTable::WhiteShortCastleKey = 0;
HashKey TranspoTable::WhiteLongCastleKey = 0;
HashKey TranspoTable::BlackShortCastleKey = 0;
HashKey TranspoTable::BlackLongCastleKey = 0;

TranspoTable::TranspoTable()
{
}

void TranspoTable::InitializeHashKeys()
{
	boost::mt19937 generator;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			ZobristKeys[i][j] = RandomKey(generator);
		}
	}

	for (int i = 0; i < 64; i++)
	{
		EnPassantKeys[i] = RandomKey(generator);
	}

	ChangeSideKey = RandomKey(generator);
	WhiteShortCastleKey = RandomKey(generator);
	WhiteLongCastleKey = RandomKey(generator);
	BlackShortCastleKey =RandomKey(generator);
	BlackLongCastleKey = RandomKey(generator);
}

int TranspoTable::HashFunction(HashKey key)
{
	return key % TableSize;
}

void TranspoTable::RecordTableNode(HashKey key, Move move, int score, int flag, int depth)
{
	int index = HashFunction(key);
	TableNode* node = &nodeArray[index];

	//if (score == -1693) {
	//	cout <<CoordinateMoveNotation(move) << " " << flag << "huh?" << endl;}

	//if (abs(score) >= Outcome::Checkmate - MaxSearchDepth)
	//{
	//	if (flag == LowerBound) 
	//	{
	//		if (score > 0) score += currentDepth;
	//		else score -= currentDepth;
	//	}
	//	else if (flag == Exact)
	//	{
	//		if (score > 0) 
	//		{
	//			score += currentDepth;
	//			flag = LowerBound;
	//		}
	//		else 
	//		{
	//			score -= currentDepth;
	//			flag = UpperBound;
	//		}
	//	}
	//}

	node->hashKey = key;
	node->move = move;
	node->data = flag << HashBitShift::FlagShift | depth << HashBitShift::DepthShift;
	node->score = score;
	// TODO Add Sequence
}

TableNode* TranspoTable::ProbeTableNode()
{
	return &nodeArray[HashFunction(currentPositionKey)];
}

int TranspoTable::GetNumberElements()
{
	int num = 0;
	for (int i = 0; i < TableSize; i++)
	{
		if (nodeArray[i].hashKey != 0)
		{
			num++;
		}
	}
	return num;
}