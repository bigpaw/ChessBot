#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

struct TableNode 
{
public:
	TableNode() 
	{
	}

	TableNode(HashKey _hashKey, int _move, int _data, int _score)
	{
		hashKey = _hashKey;
		move = _move;
		data = _data;
		score = _score;
	}

	HashKey hashKey;
	Move  move;
	int data;
	int score;
};

struct PawnTableNode 
{
public:
	PawnTableNode()
	{
	}

	PawnTableNode(PawnHashKey _hashKey, int _score)
	{
		hashKey = _hashKey;
		score = _score;
	}

	PawnHashKey hashKey;
	int score;
};

struct StaticExchangeData
{
public:
	int piece;
	int value;
	int square;

	StaticExchangeData()
	{
	}

	StaticExchangeData(int _piece, int _value, int _square)
	{
		piece = _piece;
		value = _value;
		square = _square;
	}

};

//struct RepetitionData
//{
//public:
//	Move move;
//	HashKey key;
//
//	RepetitionData()
//	{
//	}
//
//	RepetitionData(Move _move, HashKey _key)
//	{
//		move = _move;
//		key = _key;
//	}
//};

struct BookReference
{
public:
    HashKey key;
    int offset;

    BookReference(const HashKey& _key = 0, const int& _offset = 0)
    {
        key = _key;
        offset = _offset;
    }
};

struct OpeningMoveData
{
public:
    Move move;
    int resultScore;
    int timesPlayed;

    OpeningMoveData(const int& _move, const int& _resultScore, const int& _timesPlayed)
    {
        move = _move;
        resultScore = _resultScore;
        timesPlayed = _timesPlayed;
    }
};

#endif