#include "Include.h"

int Debugger::score = 0;
int Debugger::searchDepth = 0;
int Debugger::numNodes[MaxSearchDepth]={0};
int Debugger::numTableHits = 0;
int Debugger::numAlphaCutoffs = 0;
int Debugger::numBetaCutoffs = 0;
int Debugger::nodesPerSecond = 0;
int Debugger::numTableFilled = 0;
Move Debugger::searchLine[MaxSearchDepth] = {0};
ChessTimer Debugger::debugTimer = ChessTimer();
stringstream Debugger::searchTreeSs;

int TestSearchScore(const int& searchDepth)
{
#ifdef DEBUG_SEARCHTREE
	Debugger::searchTreeSs.clear();
#endif
	allotedTime = INT32_MAX;
	int nodes = SearchRoot(searchDepth, -Infinity, Infinity);
	FillPrincipleVariation(searchDepth);
	cout << GetThinkingOutput(searchDepth, nodes) << endl;
#ifdef DEBUG_SEARCHTREE
	Debugger::WriteLog(DebugTypeSearchTree);
#endif
	return nodes;
}

void TestMoveGeneration(const int& depth)
{
	using namespace boost;
	using namespace std;

	string line;
	ifstream file(PerftTestFile.c_str());

	int totalErrors = 0;

	while (getline(file, line))
	{
		char_separator<char> sep(";");
		tokenizer<char_separator<char> > tokens(line, sep);
		tokenizer<char_separator<char> > ::iterator iter = tokens.begin();

		ResetEngine();
		InitializeEngine(*iter++);
		cout << "Testing: " << line << endl;
		int fileDepth = 1;

		for (; iter != tokens.end(); iter++)
		{
			if (fileDepth > depth) break;
			string inString = *iter;
			size_t spaceIndex = inString.find(" ");
			string correct = inString.substr(spaceIndex + 1, inString.length() - spaceIndex - 1);
			unsigned __int64 testScore = lexical_cast<unsigned __int64 > (correct);
			unsigned __int64 perftScore = Perft(fileDepth);
			if (testScore == perftScore)
			{
				cout << "Depth " + lexical_cast<string > (fileDepth) + ": Correct" << endl;
			}
			else
			{
				totalErrors++;
				cout << "Depth " + lexical_cast<string > (fileDepth) + ": INCORRECT (Score " + lexical_cast<string > (perftScore) +
					", Correct score " + lexical_cast<string> (testScore) + ")" << endl;
			}
			fileDepth++;
		}

		cout << endl;
	}

	file.close();
	cout << "Total errors: " << lexical_cast<string > (totalErrors) << endl;
}

void TestDraw()
{
	using namespace boost;
	using namespace std;

	string line;
	ifstream file(DrawTestFile.c_str());

	int totalErrors = 0;

	while (getline(file, line))
	{
		char_separator<char> sep(";");
		tokenizer<char_separator<char> > tokens(line, sep);
		tokenizer<char_separator<char> > ::iterator iter = tokens.begin();

		ResetEngine();
		InitializeEngine(*iter);
		cout << "Testing: " << (*iter++) << endl;
		int searchScore = TestSearchScore(lexical_cast<int>(*iter++));

		if (searchScore == Outcome::Draw)
		{
			cout << "Correct" << endl;
		} else
		{
			totalErrors++;
			cout << "INCORRECT: " << searchScore << endl;
		}
	}

	file.close();
	cout << "Total errors: " << lexical_cast<string > (totalErrors) << endl;
}

void TestMateFinder()
{
	using namespace boost;
	using namespace std;

	string line;
	ifstream file(MateFinderTestFile.c_str());

	int totalErrors = 0;

	while (getline(file, line))
	{
		char_separator<char> sep(";");
		tokenizer<char_separator<char> > tokens(line, sep);
		tokenizer<char_separator<char> > ::iterator iter = tokens.begin();

		ResetEngine();
		InitializeEngine(*iter);
		cout << "\r\nTesting: " << (*iter++) << endl;
		int dtm = lexical_cast<int>(*iter++);		
		int searchScore = abs(TestSearchScore(dtm + 1));

		if (dtm == Outcome::Checkmate - searchScore)
		{
			cout << "Correct: " << searchScore << endl;
		} else
		{
			totalErrors++;
			cout << "INCORRECT: " << searchScore << endl;
		}
	}

	cout << "\r\nTotal errors: " << totalErrors << endl;
}

void TestPawnStructure()
{
	using namespace boost;
	using namespace std;

	string line;
	ifstream file(PawnStructureTestFile.c_str());

	int totalErrors = 0;

	while (getline(file, line))
	{
		char_separator<char> sep(";");
		tokenizer<char_separator<char> > tokens(line, sep);
		tokenizer<char_separator<char> > ::iterator iter = tokens.begin();
		ResetEngine();
		InitializeEngine(*iter);
		cout << "\r\nTesting: " << (*iter++) << endl;
		cout << "Score: " << PawnStructureScoreDebug();
	}

	cout << "\r\nTotal errors: " << totalErrors << endl;
}

void TestStaticExchange()
{
	using namespace boost;
	using namespace std;

	string line;
	ifstream file(StaticExchangeTestFile.c_str());

	int totalErrors = 0;

	while (getline(file, line))
	{
		char_separator<char> sep(";");
		tokenizer<char_separator<char> > tokens(line, sep);
		tokenizer<char_separator<char> > ::iterator iter = tokens.begin();

		ResetEngine();
		InitializeEngine(*iter);	
		cout << "\r\nTesting: " << (*iter++) << "; Move: ";
		string moveString = *iter;
		cout << (*iter++) << endl;
		int testScore = lexical_cast<int>(*iter++);		
		int staticExchangeScore = StaticExchangeScore(MoveNotation(moveString));

		if (testScore == staticExchangeScore)
		{
			cout << "Correct" << endl;
		} else
		{
			totalErrors++;
			cout << "INCORRECT: "<< " Score obtained " << staticExchangeScore << " vs " << testScore << endl;
		}
	}

	cout << "\r\nTotal errors: " << totalErrors << endl;

}

void GenerateMoveOrderingTest(const int& depth) 
{
	using namespace boost;
	using namespace std;

	string line;
	remove(MoveOrderingTestFile.c_str());
	ofstream outFile(MoveOrderingTestFile.c_str());
	ifstream inFile(GenerateMoveOrderingTestFile.c_str());

	while (getline(inFile, line))
	{
		char_separator<char> sep(";");
		tokenizer<char_separator<char> > tokens(line, sep);
		tokenizer<char_separator<char> > ::iterator iter = tokens.begin();

		ResetEngine();
		InitializeEngine(*iter);	
		outFile << *iter << ";";
		cout << "\r\nGenerating for: " << (*iter) << endl;
		int searchScore = TestSearchScore(depth);

		outFile << depth << ";" << searchScore << endl;
	}

	outFile.close();
}

void TestMoveOrdering()
{	
	using namespace boost;
	using namespace std;

	string line;
	ifstream file(MoveOrderingTestFile.c_str());

	int totalErrors = 0;

	while (getline(file, line))
	{
		char_separator<char> sep(";");
		tokenizer<char_separator<char> > tokens(line, sep);
		tokenizer<char_separator<char> > ::iterator iter = tokens.begin();

		ResetEngine();
		InitializeEngine(*iter);
		cout << "Testing: " << (*iter++) << endl;
		ShowBoard();

		int searchScore = TestSearchScore(lexical_cast<int>(*iter++));
		int scoreWoMoveOrdering = lexical_cast<int>(*iter);

		if (searchScore ==scoreWoMoveOrdering)
		{
			cout << "Correct" << endl;
		} else
		{
			totalErrors++;
			cout << "INCORRECT: Score obtained: " << searchScore << ", Correct score: " << scoreWoMoveOrdering << endl;
		}
	}

	file.close();
	cout << "Total errors: " << lexical_cast<string > (totalErrors) << endl;
}

void Debugger::WriteLog(int type)
{
	switch (type)
	{
	case DebugTypeSearchLine:
		WriteSearchLog();
		break;
	case DebugTypeEmptyLine:
		WriteEmptyLineLog();
		break;
	case DebugTypeSearchTree:
		WriteSearchTreeLog();
		break;
	}
}

void Debugger::WriteEmptyLineLog()
{
	ofstream outFile(DebugLogFile.c_str(), ofstream::out | ofstream::app);
	outFile << endl;
	outFile.close();       
}

void Debugger::WriteSearchTreeLog()
{
	ofstream outFile(DebugSearchTreeLogFile.c_str(), ofstream::out | ofstream::app);

	outFile << searchTreeSs.str();
	outFile.close();
}

void Debugger::WriteSearchLog()
{
	ofstream outFile(DebugLogFile.c_str(), ofstream::out | ofstream::app);

	stringstream ss;

	ss << boost::format("Depth: %3i") % searchDepth;
	ss << boost::format(",  Score: %7.2f") % (score / 100.0);
	ss << boost::format(",  Nodes: %9i") % numNodes[searchDepth];


	int milliseconds = debugTimer.GetElapsedTime();
	ss << boost::format(",  Time: %6i") % milliseconds;
	if (milliseconds == 0)
	{
		ss << "ms,  NPS: N/A";
	} else
	{
		ss << boost::format("ms,  NPS: %8i") % (numNodes[searchDepth] * 1000 / milliseconds);
	}
	ss << boost::format(",  Table hits: %7i") % numTableHits;
	ss << boost::format(",  %% Hits: %5.2f") % ((double) numTableHits * 100 / numNodes[searchDepth]);
	ss << boost::format(",  Table filled: %5.2f%%") % ((double) (numTableFilled) * 100 / TableSize);
	if (searchDepth >= 3)
	{
		ss << boost::format(",  EBF: %5.2f") % ((float) numNodes[searchDepth] / numNodes[searchDepth - 1]);
	}
	ss << endl;

	outFile << ss.str();
	outFile.close();
}