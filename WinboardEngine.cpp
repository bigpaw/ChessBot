#include "Include.h"

void WinboardSendFeature(const int& version)
{
	std::string featureString("feature");

	featureString.append(" ping=1");
	featureString.append(" setboard=1");
	featureString.append(" playother=1");
	featureString.append(" san=0");
	featureString.append(" usermove=1");
	featureString.append(" time=1");
	featureString.append(" draw=0");
	featureString.append(" sigint=0");
	featureString.append(" sigterm=0");
	featureString.append(" reuse=1");
	featureString.append(" analyze=0");
	featureString.append(" myname=\"Siwi\"");
	featureString.append(" variants=\"normal\"");
	featureString.append(" colors=0");
	featureString.append(" ics=0");
	featureString.append(" name=0");
	featureString.append(" pause=0");
	featureString.append(" done=1");

	std::cout << featureString << std::endl;
}

void WinboardSendMove(const std::string& moveString)
{
	std::cout << "move " + moveString << std::endl;
}

void WinboardSendIllegalMove(const std::string& moveString)
{
	std::cout << "Illegal Move: " + moveString << std::endl;
}

void WinboardSendPong(const int& ping)
{
	while (winboardIsEngineSearching)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(50));
	}
	std::cout << "pong " + boost::lexical_cast<std::string>(ping) << std::endl;
}

void WinboardSendThinkingOutput(const std::string& outputString)
{
	std::cout << outputString << std::endl;
}
//}

void WinboardUserMove(const std::string& moveString)
{
	Move userMove;
	if ((userMove = WinboardTranslateUserMove(moveString)) != NoMove)
	{
		if (numOutOfOpeningBook < NumberOpeningBookTrials)
			UpdateOpeningBookKey(userMove); //This order must be maintained
		MakeMove(userMove);

		if (!whiteToMove) //White just moved
		{
			if (Moved(userMove) == PieceIndex::WhitePawn || Captured(userMove) != PieceIndex::NoPiece || (Moved(userMove) == PieceIndex::WhiteKing && (abs(Start(userMove) - End(userMove)) == 2)))
				movesSinceIrreversible = 0;
			else	
				movesSinceIrreversible++;
			repetitionKeyList[movesSinceIrreversible] = currentPositionKey;
		}
		else
		{
			if (Moved(userMove) == PieceIndex::BlackPawn || Captured(userMove) != PieceIndex::NoPiece || (Moved(userMove) == PieceIndex::BlackKing && (abs(Start(userMove) - End(userMove)) == 2)))
				movesSinceIrreversible = 0;
			else	
				movesSinceIrreversible++;
			repetitionKeyList[movesSinceIrreversible] = currentPositionKey;
		}
		repetitionFifty[0] = movesSinceIrreversible;

		if (!winboardIsForcedMode && !winboardGameEnded)
			winboardSearchThread = boost::thread(StartSearch);
	}
	else
	{
		WinboardSendIllegalMove(moveString);
	}
}

void WinboardForce()
{
	if (winboardIsEngineSearching)
	{
		winboardSearchThread.interrupt();
		winboardSearchThread.join();
	}
}

void WinboardPlayNowFx()
{
	if (winboardIsEngineSearching)
	{
		winboardSearchThread.interrupt();
		winboardSearchThread.join();
	}
}

void WinboardGo()
{
	winboardPlayingWhite = whiteToMove;
	if (!winboardGameEnded)
		winboardSearchThread = boost::thread(StartSearch);
}

void WinboardPlayOther()
{
	winboardPlayingWhite = !whiteToMove;
	//    if (PonderAllowed && !GameEnded)
	//        Ponder();
}

void WinboardSetBoard(const std::string& fenString)
{
	InitializePosition(fenString);
}

void WinboardSetDepth(const int& depth)
{
	winboardSearchDepthLimit = depth;
}

void WinboardQuit()
{
	if (winboardIsEngineSearching)
	{
		winboardSearchThread.interrupt();
		winboardSearchThread.join();
	}
}

void WinboardEngineTime(const int& centiSeconds)
{
	engineTimeLeft = centiSeconds * 10;
}

void WinboardUserTime(const int& centiSeconds)
{
	opponentTimeLeft = centiSeconds * 10;
}

void WinboardTimeLevel(const std::string& timeString)
{
	int base, increment(0), numMoves;
	TimerType type;
	WinboardParseTimeString(timeString, type, numMoves, base, increment);

	timerType = type;
	incrementTime = increment;
}

void WinboardTimeSet(const int& seconds)
{
	timerType = TimerType::SecondsPerMove;
	timePerMove = seconds * 1000;
}

void WinboardParseTimeString(const std::string& timeString, TimerType& type, int& numMoves, int& base, int& increment)
{
	using namespace boost;

	char_separator<char> fieldSep(" ");
	tokenizer<char_separator<char> > fieldToken(timeString, fieldSep);
	tokenizer<char_separator<char> >::iterator fieldIter = fieldToken.begin();

	numMoves = lexical_cast<int>(*fieldIter++);
	if (numMoves == 0) type = TimerType::Incremental;
	else type = TimerType::Conventional;

	std::string baseString = *fieldIter++;
	size_t colonIndex = baseString.find(":");
	if (colonIndex == std::string::npos)
		base = lexical_cast<int>(baseString) * 60 * 1000;
	else
	{
		std::string minutes = baseString.substr(0, colonIndex);
		std::string seconds = baseString.substr(colonIndex + 1, baseString.length() - colonIndex - 1);
		base = lexical_cast<int>(minutes) * 60 * 1000 + lexical_cast<int>(seconds) * 1000;
	}

	if (type == TimerType::Incremental)
		increment = lexical_cast<int>(*fieldIter) * 1000;
}

Move WinboardTranslateUserMove(const std::string& moveString)
{
	Square startSquare = CoordinateToSquare(moveString.substr(0,2));
	Square endSquare = CoordinateToSquare(moveString.substr(2, 2));
	Piece promoted = PieceIndex::NoPiece;
	if (moveString.length() > 4)
	{
		switch (moveString[4])
		{
		case 'q':
			if (winboardPlayingWhite) promoted = PieceIndex::BlackQueen;
			else promoted = PieceIndex::WhiteQueen;
			break;
		case 'n':
			if (winboardPlayingWhite) promoted = PieceIndex::BlackKnight;
			else promoted = PieceIndex::WhiteKnight;
			break;
		case 'b':
			if (winboardPlayingWhite) promoted = PieceIndex::BlackBishop;
			else promoted = PieceIndex::WhiteBishop;
			break;
		case 'r':
			if (winboardPlayingWhite) promoted = PieceIndex::BlackRook;
			else promoted = PieceIndex::WhiteRook;
			break;
		}
	}

	for (int i = 0; validMoveList[i] != NoMove; i++)
	{
		Move currentMove = validMoveList[i];
		if (startSquare == Start(currentMove) &&
			endSquare == End(currentMove) &&
			promoted == Promoted(currentMove))
		{
			return currentMove;
		}
	}
	return NoMove;
}