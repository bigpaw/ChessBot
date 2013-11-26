#ifndef MOVEGENERATOR_H_INCLUDED
#define MOVEGENERATOR_H_INCLUDED

extern const Bitboard AttackedByWhitePawnBitboard[64];
extern const Bitboard AttackedByBlackPawnBitboard[64];
extern const Bitboard KnightMoveBitboard[64];
extern const Bitboard KingMoveBitboard[64];
extern const int MagicRookShift[64];
extern const unsigned __int64 MagicRookHash[64];
extern const unsigned __int64 MagicRookMask[64];
extern const Bitboard* MagicRookIndex[64];
extern const int MagicBishopShift[64];
extern const unsigned __int64 MagicBishopHash[64];
extern const unsigned __int64 MagicBishopMask[64];
extern const Bitboard* MagicBishopIndex[64];
extern const int BitScanTable[64];

extern Bitboard MagicRookData[MagicRookDataLength];
extern Bitboard MagicBishopData[MagicBishopDataLength];

void InitializeMagicBitboards();
Bitboard InitializeRookMagic(const Square& square, const Bitboard& occupancy);
Bitboard InitializeBishopMagic(const Square& square, const Bitboard& occupancy);
Bitboard InitializeMagicOccupancy(const int* squares, const int& numSquares, const Bitboard& linocc);
bool WhiteAttack(const Square& square);
bool BlackAttack(const Square& square);
bool WhiteAttackNoKing(const Square& square);
bool BlackAttackNoKing(const Square& square);
void GenerateCaptures(int moveList[], int& moveIndex);
void GenerateNonCaptures(int moveList[], int& moveIndex);
void GenerateChecks(int moveList[], int& moveIndex);
void GenerateValidMoveList(int moveList[]);

inline unsigned __int64 LowestBit(const Bitboard& board)
{
	return board & -board;
}

inline int TrailingZeros(Bitboard board)
{
	board ^= (board - 1);
	unsigned int folded = (unsigned int)(board ^ (board >> 32));
	return BitScanTable[folded * 0x78291ACF >> 26];
}

inline void ResetLowestBit(Bitboard& board)
{
	board &= board - 1;
}

inline int ShortCastle(const int& castle)
{
	return castle & 0x1;
}

inline int LongCastle(const int& castle)
{
	return castle & 0x2;
}

inline Square Start(const Move& move)
{
	return move >> MoveBitShift::StartShift & 0x3F;
}

inline Square End(const Move& move)
{
	return move >> MoveBitShift::EndShift & 0x3F;
}

inline Piece Captured(const Move& move)
{
	return move >> MoveBitShift::CaptureShift & 0xF;
}

inline Piece Promoted(const Move& move)
{
	return move >> MoveBitShift::PromoteShift & 0xF;
}

inline Piece Moved(const Move& move)
{
	return move & 0xF;
}

inline int File(const int& square) {
	return square & 7;
}

inline int Rank(const int& square) {
	return square >> 3;
}

inline Bitboard RookMagicBitboardOccupancyMasked(const Square& square, const Bitboard& occupancy)
{
	return *(MagicRookIndex[square] + ((occupancy * MagicRookHash[square]) >> MagicRookShift[square]));
}

inline Bitboard BishopMagicBitboardOccupancyMasked(const Square& square, const Bitboard& occupancy)
{
	return *(MagicBishopIndex[square] + ((occupancy * MagicBishopHash[square]) >> MagicBishopShift[square]));
}

inline Bitboard RookMagicBitboard(const Square& square, const Bitboard& occupancy)
{
	return *(MagicRookIndex[square] + (((occupancy & MagicRookMask[square]) * MagicRookHash[square]) >> MagicRookShift[square]));
}

inline Bitboard BishopMagicBitboard(const Square& square, const Bitboard& occupancy)
{
	return *(MagicBishopIndex[square] + (((occupancy & MagicBishopMask[square]) * MagicBishopHash[square]) >> MagicBishopShift[square]));
}

inline bool WhiteInCheck()
{
	return BlackAttack(TrailingZeros(pieceBitboards[PieceIndex::WhiteKing]));
}

inline bool BlackInCheck()
{
	return WhiteAttack(TrailingZeros(pieceBitboards[PieceIndex::BlackKing]));
}

inline bool WhiteInCheckNoKing()
{
	return BlackAttackNoKing(TrailingZeros(pieceBitboards[PieceIndex::WhiteKing]));
}

inline bool BlackInCheckNoKing()
{
	return WhiteAttackNoKing(TrailingZeros(pieceBitboards[PieceIndex::BlackKing]));
}

inline bool IsXrayPiece(const Piece& piece)
{
	return piece > 7 || piece < 4;
}

inline bool IsPawn(const Piece& piece)
{
	return piece < 4;
}

inline int PopulationCountFew(Bitboard board)
{
	int count = 0;
	while (board != 0)
	{
		count++;
		board &= board - 1; // reset LS1B
	}
	return count;
}

inline int PopulationCountMore(Bitboard board) //TODO: Change for more efficient?
{
	int count = 0;
	while (board != 0)
	{
		count++;
		board &= board - 1; // reset LS1B
	}
	return count;
}



#endif