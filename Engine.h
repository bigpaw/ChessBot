#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

extern int enPassant;
extern int whiteCastle;
extern int blackCastle;
extern bool whiteToMove;
extern Piece pieceTypes[64];
extern Bitboard pieceBitboards[19];
extern int currentDepth;
extern HashKey currentPositionKey;
extern PawnHashKey pawnStructureKey;
extern int pieceSquareScore;
extern int storeWhiteCastle[MaxSearchDepth];
extern int storeBlackCastle[MaxSearchDepth];
extern HashKey storeHashKeys[MaxSearchDepth];
extern int storeEnPassant[MaxSearchDepth];
extern int storePieceSquareScore[MaxSearchDepth];
extern int repetitionFifty[MaxSearchDepth + 1];
extern HashKey repetitionKeyList[100 + MaxSearchDepth];
extern int whitePieceScore;
extern int blackPieceScore;
extern int whitePawnScore;
extern int blackPawnScore;

extern int movesSinceIrreversible;

extern TranspoTable* transpoTable;
extern PawnTranspoTable* pawnTranspoTable;
extern Move principalVariation[MaxSearchDepth];
extern Move bestRootMove;
extern ChessTimer chessTimer;
extern int numNodes;
extern int allotedTime;
extern MoveOrderingData moveDataArray[MaxSearchDepth];
extern Move killerMoves[2][MaxSearchDepth];
extern int historyHeuristicBoard[64][64];
extern int engineTimeLeft;
extern int opponentTimeLeft;
extern TimerType timerType;
extern int incrementTime;
extern int timePerMove;
extern Move validMoveList[MaxNumberMoves];

void ResetEngine();
void InitializeEngine(string fenString);
void InitializePosition(string fenString);
__int64 Perft(int depth);
void Divide(int depth);
void FillPrincipleVariation(const int& depth);
Move GetNextPrincipleVariationMove();
string GetThinkingOutput(const int& ply, const int& score);
void StartSearch();
void PlayEngineMove(const Move& move);
int CalculateAllotedTime();

#endif