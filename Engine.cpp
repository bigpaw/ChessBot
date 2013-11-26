#include "Include.h"

Piece pieceTypes[64] = {0};
Bitboard pieceBitboards[19] = {0};
int storeWhiteCastle[MaxSearchDepth] = {0};
int storeBlackCastle[MaxSearchDepth] = {0};
HashKey storeHashKeys[MaxSearchDepth] = {0};
int storeEnPassant[MaxSearchDepth] = {0};
int storePieceSquareScore[MaxSearchDepth] = {0};
int repetitionFifty[MaxSearchDepth + 1] = {0};
Move principalVariation[MaxSearchDepth] = {0};
Move killerMoves[2][MaxSearchDepth] = {0};
int historyHeuristicBoard[64][64] = {0};

int enPassant = 0;
int whiteCastle = 0;
int blackCastle = 0;
bool whiteToMove = 0;
int currentDepth = 0;
HashKey currentPositionKey = 0;
PawnHashKey pawnStructureKey = 0;
int pieceSquareScore = 0;
int whitePieceScore = 0;
int blackPieceScore = 0;
int whitePawnScore = 0;
int blackPawnScore = 0;
int numNodes = 0;
int allotedTime = 0;
int engineTimeLeft = 0;
int opponentTimeLeft = 0;
TimerType timerType = Conventional;
int incrementTime = 0;
int timePerMove = 0;
Move validMoveList[MaxNumberMoves] = {0};

int movesSinceIrreversible = 0;
HashKey repetitionKeyList[100 + MaxSearchDepth] = {0};

Move bestRootMove = NoMove;
MoveOrderingData moveDataArray[MaxSearchDepth] = {};
ChessTimer chessTimer = ChessTimer();
TranspoTable* transpoTable ;
PawnTranspoTable* pawnTranspoTable;

void InitializeEngine(string fenString)
{
	TranspoTable::InitializeHashKeys();
	PawnTranspoTable::InitializeHashKeys();
	InitializeMagicBitboards();
	MoveOrderingData::InitializeOccupancyBitboards();
	InitializeEvaluation();

	if (bookVariablesDeleted) //For Perft: comment out
	{
		LoadOpeningBookReference();
	}
	//openingBook = new OpeningBook(fenString);
	//searchTreeInfo = new SearchTreeInfo(fenString);

	InitializePosition(fenString);
	InitializeBookKey(fenString);

	transpoTable = new TranspoTable();
	pawnTranspoTable = new PawnTranspoTable();
	//killerMoves = new int[2][chess.constants.General.MaxSearchDepth];
	//validMoveList = new int[chess.constants.General.MaxNumberMoves];
	//principalVariation = new int[chess.constants.General.MaxSearchDepth];
	//stopWatch = new StopWatch();

	winboardSearchDepthLimit = MaxSearchDepth;
	winboardPlayingWhite = false;

	GenerateValidMoveList(validMoveList);
}

void InitializePosition(string fenString)
{
	using namespace boost;

	char_separator<char> fieldSep(" ");
	char_separator<char> rankSep("/");
	tokenizer<char_separator<char> > fieldToken(fenString, fieldSep);
	tokenizer<char_separator<char> >::iterator fieldIter = fieldToken.begin();

	tokenizer<char_separator<char> > rankToken(*fieldIter, rankSep);
	tokenizer<char_separator<char> >::iterator rankIter = rankToken.begin();

	currentPositionKey = 0;
	pieceSquareScore = 0;
	whitePieceScore = 0;
	blackPieceScore = 0;
	whitePawnScore = 0;
	blackPawnScore = 0;
	currentDepth = 0;
	whiteCastle = 0;
	blackCastle = 0;

	//{ Piece Positions

	int currentSquare = 56;
	for (; rankIter != rankToken.end(); rankIter++, currentSquare -= 16)
	{
		for (int i = 0; i < rankIter->size(); i++)
		{
			switch ((*rankIter)[i])
			{
			case 'P':
				pieceTypes[currentSquare] = PieceIndex::WhitePawn;
				pieceBitboards[PieceIndex::WhitePawn] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhitePawn - 2][currentSquare];
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][currentSquare];
				whitePawnScore += PieceScore::PawnScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::WhitePawn - 2][currentSquare];
				currentSquare++;
				break;
			case 'N':
				pieceTypes[currentSquare] = PieceIndex::WhiteKnight;
				pieceBitboards[PieceIndex::WhiteKnight] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteKnight - 2][currentSquare];
				whitePieceScore += PieceScore::KnightScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::WhiteKnight - 2][currentSquare];
				currentSquare++;
				break;
			case 'K':
				pieceTypes[currentSquare] = PieceIndex::WhiteKing;
				pieceBitboards[PieceIndex::WhiteKing] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteKing - 2][currentSquare];
				pieceSquareScore += PieceSquareTable[PieceIndex::WhiteKing - 2][currentSquare];
				currentSquare++;
				break;
			case 'B':
				pieceTypes[currentSquare] = PieceIndex::WhiteBishop;
				pieceBitboards[PieceIndex::WhiteBishop] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteBishop - 2][currentSquare];
				whitePieceScore += PieceScore::BishopScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::WhiteBishop - 2][currentSquare];
				currentSquare++;
				break;
			case 'R':
				pieceTypes[currentSquare] = PieceIndex::WhiteRook;
				pieceBitboards[PieceIndex::WhiteRook] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteRook - 2][currentSquare];
				whitePieceScore += PieceScore::RookScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::WhiteRook - 2][currentSquare];
				currentSquare++;
				break;
			case 'Q':
				pieceTypes[currentSquare] = PieceIndex::WhiteQueen;
				pieceBitboards[PieceIndex::WhiteQueen] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteQueen - 2][currentSquare];
				whitePieceScore += PieceScore::QueenScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::WhiteQueen - 2][currentSquare];
				currentSquare++;
				break;
			case 'p':
				pieceTypes[currentSquare] = PieceIndex::BlackPawn;
				pieceBitboards[PieceIndex::BlackPawn] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackPawn - 2][currentSquare];
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][currentSquare];
				blackPawnScore += PieceScore::PawnScore;
				pieceSquareScore -= PieceSquareTable[PieceIndex::BlackPawn - 2][currentSquare];
				currentSquare++;
				break;
			case 'n':
				pieceTypes[currentSquare] = PieceIndex::BlackKnight;
				pieceBitboards[PieceIndex::BlackKnight] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackKnight - 2][currentSquare];
				blackPieceScore += PieceScore::KnightScore;
				pieceSquareScore -= PieceSquareTable[PieceIndex::BlackKnight - 2][currentSquare];
				currentSquare++;
				break;
			case 'k':
				pieceTypes[currentSquare] = PieceIndex::BlackKing;
				pieceBitboards[PieceIndex::BlackKing] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackKing - 2][currentSquare];
				pieceSquareScore -= PieceSquareTable[PieceIndex::BlackKing - 2][currentSquare];
				currentSquare++;
				break;
			case 'b':
				pieceTypes[currentSquare] = PieceIndex::BlackBishop;
				pieceBitboards[PieceIndex::BlackBishop] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackBishop - 2][currentSquare];
				blackPieceScore += PieceScore::BishopScore;
				pieceSquareScore -= PieceSquareTable[PieceIndex::BlackBishop - 2][currentSquare];
				currentSquare++;
				break;
			case 'r':
				pieceTypes[currentSquare] = PieceIndex::BlackRook;
				pieceBitboards[PieceIndex::BlackRook] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackRook - 2][currentSquare];
				blackPieceScore += PieceScore::RookScore;
				pieceSquareScore -= PieceSquareTable[PieceIndex::BlackRook - 2][currentSquare];
				currentSquare++;
				break;
			case 'q':
				pieceTypes[currentSquare] = PieceIndex::BlackQueen;
				pieceBitboards[PieceIndex::BlackQueen] ^= 1ull << currentSquare;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackQueen - 2][currentSquare];
				blackPieceScore += PieceScore::QueenScore;
				pieceSquareScore -= PieceSquareTable[PieceIndex::BlackQueen - 2][currentSquare];
				currentSquare++;
				break;
			default:
				try
				{
					int numEmptySquares = lexical_cast<int>((*rankIter)[i]);
					for (int j = 0; j < numEmptySquares; j++)
					{
						pieceTypes[currentSquare] = PieceIndex::NoPiece;
						currentSquare++;
					}
				}
				catch (...)
				{
					throw "Invalid FEN Notation (Piece position).";
				}
				break;
			}

		}
	}

	pieceBitboards[PieceIndex::WhiteBishopQueen] = pieceBitboards[PieceIndex::WhiteBishop] | pieceBitboards[PieceIndex::WhiteQueen];
	pieceBitboards[PieceIndex::WhiteRookQueen] = pieceBitboards[PieceIndex::WhiteRook] | pieceBitboards[PieceIndex::WhiteQueen];
	pieceBitboards[PieceIndex::WhitePieces] = pieceBitboards[PieceIndex::WhitePawn] | pieceBitboards[PieceIndex::WhiteKnight] | pieceBitboards[PieceIndex::WhiteKing] | pieceBitboards[PieceIndex::WhiteBishop] | pieceBitboards[PieceIndex::WhiteRook] | pieceBitboards[PieceIndex::WhiteQueen];
	pieceBitboards[PieceIndex::BlackBishopQueen] = pieceBitboards[PieceIndex::BlackBishop] | pieceBitboards[PieceIndex::BlackQueen];
	pieceBitboards[PieceIndex::BlackRookQueen] = pieceBitboards[PieceIndex::BlackRook] | pieceBitboards[PieceIndex::BlackQueen];
	pieceBitboards[PieceIndex::BlackPieces] = pieceBitboards[PieceIndex::BlackPawn] | pieceBitboards[PieceIndex::BlackKnight] | pieceBitboards[PieceIndex::BlackKing] | pieceBitboards[PieceIndex::BlackBishop] | pieceBitboards[PieceIndex::BlackRook] | pieceBitboards[PieceIndex::BlackQueen];
	pieceBitboards[PieceIndex::AllPieces] = pieceBitboards[PieceIndex::WhitePieces] | pieceBitboards[PieceIndex::BlackPieces];
	fieldIter++;

	//{ Side to Play

	if (*fieldIter == "w")
		whiteToMove = true;
	else if (*fieldIter == "b")
	{
		whiteToMove = false;
		currentPositionKey ^= TranspoTable::ChangeSideKey;
	}
	else
		throw "Invalid FEN Notation (Side to move).";
	fieldIter++;
	//{ Castling Rights

	for (int i = 0; i < fieldIter->size(); i++)
	{
		switch ((*fieldIter)[i])
		{
		case 'K':
			whiteCastle |= 0x1;
			currentPositionKey ^= TranspoTable::WhiteShortCastleKey;
			break;
		case 'Q':
			whiteCastle |= 0x2;
			currentPositionKey ^= TranspoTable::WhiteLongCastleKey;
			break;
		case 'k':
			blackCastle |= 0x1;
			currentPositionKey ^= TranspoTable::BlackShortCastleKey;
			break;
		case 'q':
			blackCastle |= 0x2;
			currentPositionKey ^= TranspoTable::BlackLongCastleKey;
			break;
		case '-':
			break;
		default:
			throw "Invalid FEN Notation (Castling).";
		}
	}
	fieldIter++;

	//}

	//{ En Passant Rights

	if (*fieldIter != "-")
	{
		try
		{
			int square = CoordinateToSquare(*fieldIter);
			enPassant = square;
			currentPositionKey ^= TranspoTable::EnPassantKeys[square];
		} catch (...)
		{
			throw "Invalid FEN Notation (En passant).";
		}
	} else
	{
		enPassant = NoEnPassant;
	}
	fieldIter++;

	//}

	repetitionFifty[0] = lexical_cast<int>(*fieldIter);

}

void ResetEngine() 
{
	memset(pieceTypes, 0, sizeof(pieceTypes));
	memset(pieceBitboards, 0, sizeof(pieceBitboards));
	memset(storeWhiteCastle, 0, sizeof(storeWhiteCastle));
	memset(storeBlackCastle, 0, sizeof(storeBlackCastle));
	memset(storeHashKeys, 0, sizeof(storeHashKeys));
	memset(storeEnPassant, 0, sizeof(storeEnPassant));
	memset(storePieceSquareScore, 0, sizeof(storePieceSquareScore));
	memset(repetitionFifty, 0, sizeof(repetitionFifty));
	memset(principalVariation, 0, sizeof(principalVariation));
	memset(killerMoves, 0, sizeof(killerMoves));
	memset(historyHeuristicBoard, 0, sizeof(historyHeuristicBoard));

	enPassant = 0;
	whiteCastle = 0;
	blackCastle = 0;
	whiteToMove = 0;
	currentDepth = 0;
	currentPositionKey = 0;
	pawnStructureKey = 0;
	pieceSquareScore = 0;
	whitePieceScore = 0;
	blackPieceScore = 0;
	whitePawnScore = 0;
	blackPawnScore = 0;

	numNodes = 0;
	allotedTime = 0;
	engineTimeLeft = 0;
	opponentTimeLeft = 0;
	timerType = Conventional;
	incrementTime = 0;
	timePerMove = 0;
	memset(validMoveList, 0, sizeof(validMoveList));

	movesSinceIrreversible = 0;
	memset(repetitionKeyList, 0, sizeof(repetitionKeyList));

	openingBookKey = 0;
	numOutOfOpeningBook = 0;

	delete pawnTranspoTable;
	delete transpoTable;
}

__int64 Perft(int depth)
{
	__int64 totalNodes = 0;
	if (depth == 0)
	{
		return 1;
	}

	int moveList[MaxNumberMoves] = {0};
	GenerateValidMoveList(moveList);
	for (int i = 0; i < MaxNumberMoves && moveList[i] != NoMove; i++)
	{
		MakeMove(moveList[i]);
		totalNodes += Perft(depth - 1);
		UndoMove(moveList[i]);
	}

	return totalNodes;
}

void Divide(int depth)
{
	__int64 totalNodes = 0;

	int moveList[MaxNumberMoves] = {0};
	GenerateValidMoveList(moveList);
	for (int i = 0; i < MaxNumberMoves && moveList[i] != NoMove; i++)
	{
		MakeMove(moveList[i]);
		__int64 nodes = Perft(depth - 1);
		totalNodes += nodes;
		cout << CoordinateMoveNotation(moveList[i]);
		cout << ":\t" << nodes << endl;
		UndoMove(moveList[i]);
	}

	cout << "Total Nodes: " << totalNodes << endl;
}