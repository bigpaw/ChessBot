#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#define DEBUG_MODE
//#define DEBUG_SEARCHTREE

const int TableSize = 128 * 1024 * 1024 / 16 + 17; //128 * 1024 * 1024 / 16 + 9;
const int PawnTableSize = 4 * 1024 * 1024 / 64 + 3;
const int PawnTableWhiteIndex = 0;
const int PawnTableBlackIndex = 1;
const int MaxNumberMoves = 128;
const int MaxSearchDepth = 50;
const int MagicBishopDataLength = 5248;
const int MagicRookDataLength = 102400;
const unsigned __int64 MagicNumber = 0x07EDD5E59A4E28C2;
const int NoEnPassant = -1;
const int NoCastling = 0;
const int NoMove = 0;
const int Infinity = 1000000;
const int InterruptionCheckInterval = 100000;
const int ThreadInterruptedValue = 2147483647;
const int StopSearchTimeUpValue = 2147483646; //Keep StopSearchTimeUp < ThreadInterruptedValue (and any other search interruption values)
const int NumberOpeningBookTrials = 5;
const string NewGameFenString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

enum PieceIndex
{
	NoPiece = 0,
	EnPassant = 15,
	WhitePieces = 0,
	BlackPieces = 1,
	WhitePawn = 2,
	BlackPawn = 3,
	WhiteKnight = 4,
	BlackKnight = 5,
	WhiteKing = 6,
	BlackKing = 7,
	WhiteQueen = 8,
	BlackQueen = 9,
	WhiteRook = 10,
	BlackRook = 11,
	WhiteBishop = 12,
	BlackBishop = 13,
	AllPieces = 14,
	WhiteBishopQueen = 15,
	BlackBishopQueen = 16,
	WhiteRookQueen = 17,
	BlackRookQueen = 18
};

enum FileBitboard : Bitboard
{
	FileOne = 0xFEFEFEFEFEFEFEFE,
	FileTwo = 0xFDFDFDFDFDFDFDFD,
	FileThree = 0xFBFBFBFBFBFBFBFB,
	FileFour = 0xF7F7F7F7F7F7F7F7,
	FileFive = 0xEFEFEFEFEFEFEFEF,
	FileSix = 0xDFDFDFDFDFDFDFDF,
	FileSeven = 0xBFBFBFBFBFBFBFBF,
	FileEight = 0x7F7F7F7F7F7F7F7F
};

const Bitboard FileBitboardByIndex[9] = 
{
	0xFEFEFEFEFEFEFEFE, 0xFDFDFDFDFDFDFDFD, 0xFBFBFBFBFBFBFBFB, 0xF7F7F7F7F7F7F7F7, 0xEFEFEFEFEFEFEFEF, 0xDFDFDFDFDFDFDFDF, 0xBFBFBFBFBFBFBFBF, 0x7F7F7F7F7F7F7F7F, 0xFFFFFFFFFFFFFFFF
};

enum RankBitboard : Bitboard
{
	RankOne = 0xFF,
	RankTwo = 0XFF00,
	RankThree = 0xFF0000,
	RankFour = 0xFF000000,
	RankFive = 0xFF00000000,
	RankSix = 0xFF0000000000,
	RankSeven = 0xFF000000000000,
	RankEight = 0xFF00000000000000
};

enum Squares 
{
	A1 = 0,
	B1 = 1,
	C1 = 2,
	D1 = 3,
	E1 = 4,
	F1 = 5,
	G1 = 6,
	H1 = 7,
	A2 = 8,
	B2 = 9,
	C2 = 10,
	D2 = 11,
	E2 = 12,
	F2 = 13,
	G2 = 14,
	H2 = 15,
	A3 = 16,
	B3 = 17,
	C3 = 18,
	D3 = 19,
	E3 = 20,
	F3 = 21,
	G3 = 22,
	H3 = 23,
	A4 = 24,
	B4 = 25,
	C4 = 26,
	D4 = 27,
	E4 = 28,
	F4 = 29,
	G4 = 30,
	H4 = 31,
	A5 = 32,
	B5 = 33,
	C5 = 34,
	D5 = 35,
	E5 = 36,
	F5 = 37,
	G5 = 38,
	H5 = 39,
	A6 = 40,
	B6 = 41,
	C6 = 42,
	D6 = 43,
	E6 = 44,
	F6 = 45,
	G6 = 46,
	H6 = 47,
	A7 = 48,
	B7 = 49,
	C7 = 50,
	D7 = 51,
	E7 = 52,
	F7 = 53,
	G7 = 54,
	H7 = 55,
	A8 = 56,
	B8 = 57,
	C8 = 58,
	D8 = 59,
	E8 = 60,
	F8 = 61,
	G8 = 62,
	H8 = 63
};

enum MoveBitShift 
{
	StartShift = 18,
	EndShift = 12,
	CaptureShift = 8,
	PromoteShift = 4
};

enum HashType
{
	UpperBound = 0x1,
	LowerBound = 0x2,
	Exact = 0x3,
	NoFlag = 0x4
};

enum HashBitShift 
{
	ScoreShift = 14,
	FlagShift = 11,
	DepthShift = 3
};

enum Evaluation
{
	IsolatedPawnAdjustment = -10,
	IsolatedDoublePawnAdjustment = -60,
	BackwardPawnAdjustment = -10,
	DoublePawnAdjustment = -20,
	TriplePawnAdjustment = -100,
	IsolatedTriplePawnAdjustment = -60,
	EndgameMaterialScoreLimit = 1300
};

enum Outcome 
{
	Checkmate = 100000,
	Stalemate = 0,
	Draw = 0
};

enum PieceScore 
{
	PawnScore = 100,
	KnightScore = 300,
	BishopScore = 325,
	RookScore = 500,
	QueenScore = 875,
	KingScore = 20000
};

const int PieceScoreByIndex[14] = 
{
	0, 0,
	PawnScore, PawnScore,
	KnightScore, KnightScore,
	KingScore, KingScore,
	QueenScore, QueenScore,
	RookScore, RookScore,
	BishopScore, BishopScore
};

enum Searching
{
	FutilityPruningDepthLimit = 2,
	NullMoveShallowPruning = 2,
	NullMoveDeepPruning = 3,
	NullMoveShallowLimit = 2,
	NullMoveDeeptLimit = 6,
	ZeroWindowSearchDepthLimit = 2
};

const int FutilityPruningMarginByDepth[4] = 
{
	0, 200, 500, 800
};

enum DebuggerLogType 
{
	DebugTypeSearchTree = 2,
	DebugTypeSearchLine = 1,
	DebugTypeEmptyLine = 0
};


enum TimerType
{
	Conventional,
	Incremental,
	SecondsPerMove
};


#endif