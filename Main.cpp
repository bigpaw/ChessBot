#include "Include.h"

int main()
{
	//TestStaticExchange();
	//TestMoveGeneration(6);
	//InitializeEngine("rnbqkbnr/ppp1pppp/8/3p4/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1");
	//InitializeEngine(NewGameFenString);

	//cout << Perft(6) << endl;
	//Divide(3);
	//GenerateMoveOrderingTest(7);
	//TestMoveOrdering();

	//TestMateFinder();
	//TestDraw();
	//TestPawnStructure();

	//ResetEngine();
	//InitializeEngine("8/5k2/8/8/7K/8/6p1/8 b - - 0 71 ");
	//TestSearchScore(2);
	//TestSearchScore(3);
	//TestSearchScore(4);
	//TestSearchScore(5);
	//TestSearchScore(6);
	//TestSearchScore(7);
	//TestSearchScore(8);
	//TestSearchScore(9);

	//InitializeEngine("6k1/1p2rpp1/pR1K4/6Pp/1PP1Pp1P/1P3P2/8/8 b - - 9 37 ");
	//TestSearchScore(17);

	bool quit = false;
	while (!quit)
	{
		string inputString;
		getline(std::cin, inputString);
		if (inputString != "")
		{
			quit = WinboardProcessInput(inputString);
		}
	}

	//string s;
	//getline(cin, s);

	return 0;
}