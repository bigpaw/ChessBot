#ifndef TRANSPOTABLE_H_INCLUDED
#define TRANSPOTABLE_H_INCLUDED

//inline int Score (const TableNode* node)
//{
//	return node->data >> HashBitShift::ScoreShift & 0xFFF;
//}

inline int Flag (const TableNode* node)
{
	return node->data >> HashBitShift::FlagShift & 0x7;
}

inline int Depth (const TableNode* node)
{
	return node->data >> HashBitShift::DepthShift & 0xFF;
}

inline int Sequence (const TableNode* node)
{
	return node->data & 0x7;
}

class TranspoTable
{
public:
	static HashKey ZobristKeys[12][64];
	static HashKey EnPassantKeys[64];
	static HashKey ChangeSideKey;
	static HashKey WhiteShortCastleKey;
	static HashKey WhiteLongCastleKey;
	static HashKey BlackShortCastleKey;
	static HashKey BlackLongCastleKey;

	TranspoTable();
	static void InitializeHashKeys();
	void RecordTableNode(HashKey key, Move move, int score, int flag, int depth);
	TableNode* ProbeTableNode();
	int GetNumberElements();

private:
	TableNode nodeArray[TableSize];
	inline int HashFunction(HashKey key);

};

#endif