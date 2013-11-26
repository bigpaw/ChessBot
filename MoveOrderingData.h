#ifndef MOVEORDERINGDATA_H_INCLUDED
#define MOVEORDERINGDATA_H_INCLUDED

enum NextMoveType
{
	HashMoveType,
	GenerateCapturesType,
	GoodCapturesType,
	KillerOneType,
	KillerTwoType,
	GenerateNonCapturesType,
	NonCapturesType,
	BadCapturesType
};

class MoveOrderingData
{
public:
	NextMoveType moveType;
	Move hashMove;

	MoveOrderingData();
	Move GetNextMove();
	static void OrderMoves(Move moves[], int scores[], int numMoves);
	static void InitializeOccupancyBitboards();
	static bool ValidateNonCapture(Move move);
	static bool ValidateMove(Move move);

private:
	int captureIndex;
	int nonCaptureIndex;
	Move nonCaptureList[MaxNumberMoves];
	Move captureList[MaxNumberMoves];
	int moveScore[MaxNumberMoves];
	
	static Bitboard occupancyBitboards[64][64];

};

#endif