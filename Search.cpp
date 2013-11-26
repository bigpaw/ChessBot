#include "Include.h"

void StartSearch()
{
	winboardIsEngineSearching = true;

	chessTimer.Start();

	bool openingMoveFound = false;
	currentDepth = 0;
	int score = 0;

	if (numOutOfOpeningBook < NumberOpeningBookTrials)
	{
		Move move = SearchOpeningMove();
		if (move != NoMove)
		{
			openingMoveFound = true;
			PlayEngineMove(move);
		}
	}

	if (!openingMoveFound)
	{
		allotedTime = CalculateAllotedTime();
		for (int searchDepth = 2; searchDepth <= winboardSearchDepthLimit; searchDepth++)
		{
			numNodes = 0;
#ifdef DEBUG_MODE
			Debugger::debugTimer.Start();
			Debugger::numTableHits = 0;
			Debugger::searchDepth = searchDepth;
#ifdef DEBUG_SEARCHTREE
			Debugger::searchTreeSs.clear();
#endif
#endif

			score = SearchRoot(searchDepth, -Infinity, Infinity);
			if (score == ThreadInterruptedValue)
			{
				if (winboardPlayNow) break;
				else return;
			}
			else if (score == StopSearchTimeUpValue) break;

			FillPrincipleVariation(searchDepth);
			if (winboardPostOutput)
				WinboardSendThinkingOutput(GetThinkingOutput(searchDepth, score));
#ifdef DEBUG_MODE
			Debugger::score = score;
			Debugger::numNodes[searchDepth] = numNodes;
			Debugger::numTableFilled = transpoTable->GetNumberElements();
			Debugger::WriteLog(DebugTypeSearchLine);
#ifdef DEBUG_SEARCHTREE
			Debugger::WriteLog(DebugTypeSearchTree);
#endif
#endif
		}
#ifdef DEBUG_MODE
		Debugger::WriteLog(DebugTypeEmptyLine);
#endif
		PlayEngineMove(principalVariation[0]);

	}

	winboardIsEngineSearching = false;
}

void PlayEngineMove(const Move& move)
{
	if (numOutOfOpeningBook < NumberOpeningBookTrials)
		UpdateOpeningBookKey(move); //This order must be maintained

	MakeMove(move);

	if (!whiteToMove) //White just moved
	{
		if (Moved(move) == PieceIndex::WhitePawn || Captured(move) != PieceIndex::NoPiece || (Moved(move) == PieceIndex::WhiteKing && (abs(Start(move) - End(move)) == 2)))
		{
			movesSinceIrreversible = 0;
		}
		else	
			movesSinceIrreversible++;
		repetitionKeyList[movesSinceIrreversible] = currentPositionKey;
	}
	else
	{
		if (Moved(move) == PieceIndex::BlackPawn || Captured(move) != PieceIndex::NoPiece || (Moved(move) == PieceIndex::BlackKing && (abs(Start(move) - End(move)) == 2)))
			movesSinceIrreversible = 0;
		else	
			movesSinceIrreversible++;
		repetitionKeyList[movesSinceIrreversible] = currentPositionKey;
	}
	repetitionFifty[0] = movesSinceIrreversible;

	memset(validMoveList, 0, MaxNumberMoves * sizeof(Move));
	GenerateValidMoveList(validMoveList);
	WinboardSendMove(CoordinateMoveNotation(move));
}

int CalculateAllotedTime()
{
	if (timerType == TimerType::SecondsPerMove)
		return timePerMove;
	else
	{
		return engineTimeLeft / 30 + incrementTime;
	}
}

void FillPrincipleVariation(const int& depth)
{
	memset(principalVariation, 0, sizeof(principalVariation));
	principalVariation[0] = bestRootMove;
	int i;
	for (i = 0; i < depth && principalVariation[i] != NoMove; i++)
	{
		MakeMove(principalVariation[i]);
		principalVariation[i + 1] = GetNextPrincipleVariationMove();

	}
	for (int j = i - 1; j >= 0; j--) UndoMove(principalVariation[j]);
}

Move GetNextPrincipleVariationMove()
{
	TableNode* node = transpoTable->ProbeTableNode();
	if (node->hashKey == currentPositionKey) return node->move;
	else return NoMove;
}

string GetThinkingOutput(const int& ply, const int& score)
{
	using namespace std;
	using namespace boost;

	string outputString = lexical_cast<string>(ply);
	outputString.append(" ");
	outputString.append(lexical_cast<string>(score));
	outputString.append(" ");
	outputString.append(lexical_cast<string>(chessTimer.GetElapsedTime()/10));
	outputString.append(" ");
	outputString.append(lexical_cast<string>(numNodes));

	for (int i=0; principalVariation[i] != NoMove && i<MaxSearchDepth; i++)
	{
		outputString.append(" ");
		outputString.append(lexical_cast<string>(CoordinateMoveNotation(principalVariation[i])));
	}

	return outputString;
}

int SearchRoot(int depth, int alpha, int beta)
{
	if (numNodes % InterruptionCheckInterval == 0)
	{
		try
		{
			boost::this_thread::interruption_point();
		}
		catch (boost::thread_interrupted)
		{
			return ThreadInterruptedValue;
		}
		if (chessTimer.GetElapsedTime() > allotedTime) return StopSearchTimeUpValue;
	}
	numNodes++;

	bool inCheck = whiteToMove ? WhiteInCheckNoKing() : BlackInCheckNoKing();

	if (inCheck) depth++;
	//TODO Check extension

	if (DrawRepetitionOrFifty())
	{
		return Outcome::Draw;
	}

	moveDataArray[currentDepth] = MoveOrderingData();
	TableNode* node = transpoTable->ProbeTableNode(); //tODO ?look up table at root?
	if (node->hashKey == currentPositionKey)
	{
		int flag = Flag(node);
		if (flag == HashType::Exact || flag == HashType::LowerBound)
		{
			moveDataArray[currentDepth].hashMove = node->move;
		}
		/*	if (Depth(node) >= depth)
		{
		#ifdef DEBUG_MODE
		Debugger::numTableHits++;
		#endif
		int hashScore = node->score;
		if (abs(hashScore) >= Outcome::Checkmate - MaxSearchDepth)
		{
		if (hashScore > 0) hashScore -= currentDepth;
		else hashScore += currentDepth;
		}

		switch (flag)
		{
		case HashType::Exact:
		bestRootMove = node->move;
		return hashScore;
		case HashType::UpperBound:
		if (hashScore <= alpha)
		{
		return hashScore;
		}
		break;
		case HashType::LowerBound:
		if (hashScore >= beta)
		{
		return hashScore;
		} else
		{
		moveDataArray[currentDepth].hashMove = node->move;
		}
		break;
		}
		} else
		{
		if (flag == HashType::Exact || flag == HashType::LowerBound)
		{
		moveDataArray[currentDepth].hashMove = node->move;
		}
		}*/
	}

	bool validMove = false;
	int bestScore = -Infinity;
	int tt_flag = UpperBound;
	bool zeroWindowSearch = false;
	int curMove;

	while ((curMove = moveDataArray[currentDepth].GetNextMove()) != NoMove)
	{
		/*int numMoves = 0;
		Move moveList[MaxNumberMoves] = {0};
		GenerateCaptures(moveList, numMoves);
		GenerateNonCaptures(moveList, numMoves);
		for (int i=0; i<numMoves; i++)
		{
		curMove = moveList[i];*/
		MakeMove(curMove);
		if (whiteToMove)
		{
			if (BlackInCheck())
			{
				UndoMove(curMove);
				continue;
			}
		} else
		{
			if (WhiteInCheck())
			{
				UndoMove(curMove);
				continue;
			}
		}

		validMove = true;
		int score;
		if (zeroWindowSearch)
		{
			score = -AlphaBeta(depth - 1, -alpha - 1, -alpha, true);
			if (score > alpha && score < beta)
			{
				score = -AlphaBeta(depth - 1, -beta, -alpha, true);
			}
		} else
		{
			score = -AlphaBeta(depth - 1, -beta, -alpha, true);
		}
		if (-score >= StopSearchTimeUpValue)
		{
			UndoMove(curMove);
			return -score;
		}

		UndoMove(curMove);

		if (score > bestScore) 
		{
			bestScore = score;
			if (bestScore > alpha) 
			{
				bestRootMove = curMove;
				if (bestScore >= beta)
				{
					tt_flag = LowerBound;
					if (Captured(bestRootMove) == PieceIndex::NoPiece && Promoted(bestRootMove) == PieceIndex::NoPiece)
					{
						if (bestRootMove == killerMoves[1][currentDepth]
						|| (bestRootMove != killerMoves[0][currentDepth] && bestRootMove != killerMoves[1][currentDepth]))
						{
							killerMoves[1][currentDepth] = killerMoves[0][currentDepth];
							killerMoves[0][currentDepth] = bestRootMove;
						}
					}

					historyHeuristicBoard[Start(curMove)][End(curMove)] += depth * depth;
					break;
				}

				tt_flag = Exact;
				zeroWindowSearch = true;
				alpha = bestScore;
			}
		}
	}

	if (!validMove)
	{
		return inCheck ? -(Outcome::Checkmate - currentDepth) : -Outcome::Stalemate;
	}

	if (abs(bestScore) >= Outcome::Checkmate - MaxSearchDepth)
	{
		int ttScore = bestScore;
		if (ttScore > 0) ttScore += currentDepth;
		else ttScore -= currentDepth;
		transpoTable->RecordTableNode(currentPositionKey, bestRootMove, ttScore, tt_flag, depth);
	}
	else
	{
		transpoTable->RecordTableNode(currentPositionKey, bestRootMove, bestScore, tt_flag, depth);
	}

	return bestScore;
}

int AlphaBeta(int depth, int alpha, int beta, bool nullMoveAllowed)
{
	DrawRepetitionOrFifty();
#ifdef DEBUG_SEARCHTREE
	for (int i = 1; i < currentDepth; i++)
		Debugger::searchTreeSs << "\t";
	for (int i=0; i<currentDepth; i++) 
		Debugger::searchTreeSs << CoordinateMoveNotation(Debugger::searchLine[i]) << "  ";
	Debugger::searchTreeSs << "\t" << alpha << "\t" << beta << endl;
#endif
	if (numNodes % InterruptionCheckInterval == 0)
	{
		try
		{
			boost::this_thread::interruption_point();
		}
		catch (boost::thread_interrupted)
		{
			return ThreadInterruptedValue;
		}
		if (chessTimer.GetElapsedTime() > allotedTime) return StopSearchTimeUpValue;
	}
	numNodes++;

	bool inCheck = whiteToMove ? WhiteInCheckNoKing() : BlackInCheckNoKing();

	if (inCheck) depth++;
	//TODO Check extension

	if (DrawRepetitionOrFifty())
	{
		//cout << movesSinceIrreversible << " " << repetitionFifty[currentDepth] << endl;
		//for (int i=0; i<currentDepth; i++) 
		//	cout << CoordinateMoveNotation(Debugger::searchLine[i]) << "  ";
		//cout << endl;
		return Outcome::Draw;
	}

	moveDataArray[currentDepth] =  MoveOrderingData();
	TableNode* node = transpoTable->ProbeTableNode();
	if (node->hashKey == currentPositionKey)
	{
		int flag = Flag(node);
		if (Depth(node) >= depth)
		{
#ifdef DEBUG_MODE
			Debugger::numTableHits++;
#ifdef DEBUG_SEARCHTREE
			if (CoordinateMoveNotation(Debugger::searchLine[0]) == "c2d2" && CoordinateMoveNotation(Debugger::searchLine[1]) == "f3f2" && CoordinateMoveNotation(Debugger::searchLine[2]) == "d2e1") {
				for (int i = 1; i < currentDepth; i++)
				{
					Debugger::searchTreeSs << "\t";
				}
				Debugger::searchTreeSs << "Hash: " << node->score << " move:" << CoordinateMoveNotation(node->move) << "  " << flag << endl;
			}
#endif
#endif

			int hashScore = node->score;
			if (abs(hashScore) >= Outcome::Checkmate - MaxSearchDepth)
			{
				if (hashScore > 0) hashScore -= currentDepth;
				else hashScore += currentDepth;
			}
			switch (flag)
			{
			case HashType::Exact:

				return hashScore;
			case HashType::UpperBound:
				if (hashScore <= alpha)
				{
					return hashScore;
				}
				break;
			case HashType::LowerBound:
				if (hashScore >= beta)
				{
					return hashScore;
				} else
				{
					moveDataArray[currentDepth].hashMove = node->move;
				}
				break;
			}
		} else
		{
			if (flag == HashType::Exact || flag == HashType::LowerBound)
			{
				moveDataArray[currentDepth].hashMove = node->move;
			}
		}
	}

	if (depth == 0)
	{
		return Quiescence(alpha, beta);
	}

	if (depth > Searching::NullMoveShallowLimit
		&& nullMoveAllowed
		&& !inCheck
		&& (whiteToMove ? whitePieceScore > Evaluation::EndgameMaterialScoreLimit : blackPieceScore > Evaluation::EndgameMaterialScoreLimit))
	{
		MakeNullMove();
		int R = (depth > Searching::NullMoveDeeptLimit) ? Searching::NullMoveDeepPruning : Searching::NullMoveShallowPruning;

		int score = -AlphaBeta(depth - R - 1, -beta, -beta + 1, false);
		UndoNullMove();

		if (score >= beta)
		{
			return beta;
		}
	}

	bool futilityPruningAllowed = (depth <= Searching::FutilityPruningDepthLimit
		&& !inCheck
		&& Evaluate() + FutilityPruningMarginByDepth[depth] <= alpha);


	bool validMove = false;
	int bestMove = NoMove;
	int bestScore = -Infinity;	
	int tt_flag = UpperBound;
	bool zeroWindowSearch = false;
	int curMove;

	while ((curMove = moveDataArray[currentDepth].GetNextMove()) != NoMove)
	{

		//int numMoves = 0;
		//Move moveList[MaxNumberMoves] = {0};
		//GenerateCaptures(moveList, numMoves);
		//GenerateNonCaptures(moveList, numMoves);
		//	for (int i=0; i<numMoves; i++)
		//	{
		//		curMove = moveList[i];

		if (moveDataArray[currentDepth].moveType == NextMoveType::BadCapturesType && depth >= 4) depth -= 2;
		MakeMove(curMove);

		if (futilityPruningAllowed
			&& Captured(curMove) == PieceIndex::NoPiece
			&& Promoted(curMove) == PieceIndex::NoPiece
			&& !(whiteToMove ? WhiteInCheckNoKing() : BlackInCheckNoKing()))
		{
			UndoMove(curMove);
			continue;
		}

		if (whiteToMove)
		{
			if (BlackInCheck())
			{
				UndoMove(curMove);
				continue;
			}
		} else
		{
			if (WhiteInCheck())
			{
				UndoMove(curMove);
				continue;
			}
		}

		validMove = true;
		int score;
		if (zeroWindowSearch && depth > Searching::ZeroWindowSearchDepthLimit)
		{
			score = -AlphaBeta(depth - 1, -alpha - 1, -alpha, true);
			if (score > alpha && score < beta)
			{
				score = -AlphaBeta(depth - 1, -beta, -alpha, true);
			}
		} else
		{
			score = -AlphaBeta(depth - 1, -beta, -alpha, true);
		}

#ifdef DEBUG_SEARCHTREE
		for (int i = 1; i < currentDepth; i++)
		{
			Debugger::searchTreeSs << "\t";
		}
		Debugger::searchTreeSs << "Score: " << score << endl;
		if (score == -1693) {
			for (int i=0; i<currentDepth; i++) 
				cout << CoordinateMoveNotation(Debugger::searchLine[i]) << "  ";
			cout << endl;
		}
#endif

		if (-score >= StopSearchTimeUpValue)
		{
			UndoMove(curMove);
			return -score;
		}

		UndoMove(curMove);

		if (score > bestScore) 
		{
			bestScore = score;
			if (bestScore > alpha) 
			{
				bestMove = curMove;
				if (bestScore >= beta)
				{
					tt_flag = LowerBound;
					if (Captured(bestMove) == PieceIndex::NoPiece && Promoted(bestMove) == PieceIndex::NoPiece)
					{
						if (bestMove == killerMoves[1][currentDepth]
						|| (bestMove != killerMoves[0][currentDepth] && bestMove != killerMoves[1][currentDepth]))
						{
							killerMoves[1][currentDepth] = killerMoves[0][currentDepth];
							killerMoves[0][currentDepth] = bestMove;
						}
					}

					historyHeuristicBoard[Start(curMove)][End(curMove)] += depth * depth;
					break;
				}

				tt_flag = Exact;
				zeroWindowSearch = true;
				alpha = bestScore;
			}
		}
	}

	if (!validMove)
	{
		return inCheck ? -(Outcome::Checkmate - currentDepth) : -Outcome::Stalemate;
	}

	if (abs(bestScore) >= Outcome::Checkmate - MaxSearchDepth)
	{
		int ttScore = bestScore;
		if (ttScore > 0) ttScore += currentDepth;
		else ttScore -= currentDepth;
		transpoTable->RecordTableNode(currentPositionKey, bestMove, ttScore, tt_flag, depth);
	}
	else
	{
		transpoTable->RecordTableNode(currentPositionKey, bestMove, bestScore, tt_flag, depth);
	}
	return bestScore;
}

int Quiescence(int alpha, int beta)
{
	if (numNodes % InterruptionCheckInterval == 0)
	{
		try
		{
			boost::this_thread::interruption_point();
		}
		catch (boost::thread_interrupted)
		{
			return ThreadInterruptedValue;
		}
		if (chessTimer.GetElapsedTime() > allotedTime) return StopSearchTimeUpValue;
	}
	numNodes++;

	int bestScore = -Infinity;
	int score = Evaluate();
	if (score >= beta)
	{
		return score;
	}
	if (score > bestScore)
	{
		bestScore = score;
	}
	if (bestScore > alpha)
	{
		alpha = bestScore;
	}

	int quiescentMoveList[MaxNumberMoves] = {0};
	int quiescentMoveScore[MaxNumberMoves] = {0};
	int numMoves = 0;
	GenerateCaptures(quiescentMoveList, numMoves);
	for (int i = 0; i < numMoves; i++)
	{
		quiescentMoveScore[i] = StaticExchangeScore(quiescentMoveList[i]);
	}
	MoveOrderingData::OrderMoves(quiescentMoveList, quiescentMoveScore, numMoves);
	GenerateChecks(quiescentMoveList, numMoves);

	//if (numMoves == 0) //TODO: WRONG! CORRECT OR LEAVE OUT
	//{
	//	bool inCheck = whiteToMove ? WhiteInCheckNoKing() : BlackInCheckNoKing();
	//	return inCheck ? -(Outcome::Checkmate - currentDepth) : -Outcome::Stalemate;
	//}

	for (int i = 0; quiescentMoveList[i] != NoMove; i++)
	{
		int curMove = quiescentMoveList[i];

		/* int testMoveList = new int[100];
		int testNumMoves = 0;
		testNumMoves = MoveGenerator.GenerateCaptures(state, testMoveList, testNumMoves);
		testNumMoves = MoveGenerator.GenerateNonCaptures(state, testMoveList, testNumMoves);
		bool illegal = true;
		for (int j = 0; j < 100 && testMoveList[j] != 0; j++)
		{
		if (curMove == testMoveList[j])
		{
		illegal = false;
		break;
		}
		}
		if (illegal)
		{
		System.out.println("Illegal Move: " + Utilities.CoordinateMoveNotation(curMove));
		}*/

		MakeMove(curMove);

		if (whiteToMove)
		{
			if (BlackInCheck())
			{
				UndoMove(curMove);
				continue;
			}
		} else
		{
			if (WhiteInCheck())
			{
				UndoMove(curMove);
				continue;
			}
		}

		score = -Quiescence(-beta, -alpha);
		if (-score >= StopSearchTimeUpValue)
		{
			UndoMove(curMove);
			return -score;
		}

		UndoMove(curMove);
		if (score >= beta)
		{
			return score;
		}
		if (score > bestScore)
		{
			bestScore = score;
		}
		if (bestScore > alpha)
		{
			alpha = bestScore;
		}
	}

	return bestScore;
}

bool DrawRepetitionOrFifty() 
{
	//Checking for 50-move rule.
	int halfMoveClock = repetitionFifty[currentDepth];
	if (halfMoveClock >= 100) {
		return true;
	}

	//Checking for 3-fold repetition. We only need to go as far as the last 50-move rule reset. We return 1 repetition as a draw/contempt if it happens in the search tree.
	int start = movesSinceIrreversible + currentDepth;
	int numRepeats = 0;
	for (int i = start - 2; i >= start - halfMoveClock && i >= 0; i -= 2) 
	{
		if (repetitionKeyList[i] == currentPositionKey) 
		{
			//if (i < start - currentDepth)
			//{
			//	cout << movesSinceIrreversible << " " << halfMoveClock << endl;
			//	for (int i=0; i<currentDepth; i++) 
			//		cout << CoordinateMoveNotation(Debugger::searchLine[i]) << "  ";
			//	cout << endl;
			//}
			if (i < start - currentDepth) 
			{
				numRepeats++;
				if (numRepeats > 1) return true;
			}
			else 
				return true;
		}
	}
	return false;
}

