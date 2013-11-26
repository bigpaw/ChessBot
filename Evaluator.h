#ifndef EVALUATOR_H_INCLUDED
#define EVALUATOR_H_INCLUDED

extern const int PieceSquareTable[14][64];
extern const int BishopMobility[15];
extern const int RookMobility[15];
extern const int KnightMobility[9];
extern const int KnightMobilityAdjustmentByPawn[9];
extern const int RookMobilityAdjustmentByPawn[9];

extern Bitboard WhiteFrontSpanBitboard[64];
extern Bitboard BlackFrontSpanBitboard[64];
extern Bitboard SideSpanBitboard[64];
extern Bitboard FileSpanBitboard[64];

int MobilityScore();
int PawnStructureScore();
int PawnStructureScoreDebug();
bool IsProtectedWhitePawn(Square square);
bool IsProtectedBlackPawn(Square square);
bool IsIsolatedWhitePawn(Square square);
bool IsIsolatedBlackPawn(Square square);
int Evaluate();
void InitializeEvaluation();

#endif