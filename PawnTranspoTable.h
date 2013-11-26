#ifndef PAWNTRANSPOTABLE_H_INCLUDED
#define PAWNTRANSPOTABLE_H_INCLUDED

class PawnTranspoTable
{
public:
	static PawnHashKey ZobristKeys[2][64];
	void RecordTableNode(PawnHashKey key, int score);
	static void InitializeHashKeys();
	PawnTableNode* ProbeTableNode();
	int GetNumberElements();
	PawnTranspoTable();

private:
	PawnTableNode nodeArray[PawnTableSize];
	inline int HashFunction(PawnHashKey key);
};

#endif