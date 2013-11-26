#ifndef DEBUGGER_H_INCLUDED
#define DEBUGGER_H_INCLUDED

const string PerftTestFile = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\Perft.epd";
const string DrawTestFile = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\DrawTest.txt";
const string MateFinderTestFile = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\MateFinderTest.txt";
const string StaticExchangeTestFile = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\StaticExchange.txt";
const string MoveOrderingTestFile = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\MoveOrderingTest.txt";
const string GenerateMoveOrderingTestFile = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\GenerateMoveOrderingTest.txt";
const string DebugLogFile = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\DebugLog.txt";
const string DebugSearchTreeLogFile = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\DebugSearchTree.txt";
const string PawnStructureTestFile = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\PawnStructureTest.txt";

void TestMoveGeneration(const int& depth);
void TestDraw();
void TestMateFinder();
void TestStaticExchange();
void GenerateMoveOrderingTest(const int& depth);
void TestMoveOrdering();
void TestPawnStructure();
int TestSearchScore(const int& searchDepth);

class Debugger
{
public:
	static int score;
	static int searchDepth;
	static int numNodes[MaxSearchDepth];
	static int numTableHits;
	static int numAlphaCutoffs;
	static int numBetaCutoffs;
	static int nodesPerSecond;
	static int numTableFilled;
	static Move searchLine[MaxSearchDepth];
	static ChessTimer debugTimer;
	static stringstream searchTreeSs;

	static void WriteLog(int type);
	static void WriteEmptyLineLog();
	static void WriteSearchTreeLog();
	static void WriteSearchLog();
};

#endif