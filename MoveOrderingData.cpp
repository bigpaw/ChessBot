#include "Include.h"

MoveOrderingData::MoveOrderingData() 
{
	hashMove = NoMove;
	moveType = NextMoveType::HashMoveType;
}

Move MoveOrderingData::GetNextMove()
{
	int numMoves;
	switch (moveType)
	{
	case HashMoveType:
		moveType = NextMoveType::GenerateCapturesType;
		if (hashMove != NoMove)
		{
			return hashMove;
		}

	case GenerateCapturesType:
		moveType = NextMoveType::GoodCapturesType;
		numMoves = 0;
		memset(captureList, 0, sizeof(captureList));
		GenerateCaptures(captureList, numMoves);
		memset(moveScore, 0, sizeof(moveScore));
		for (int i = 0; i < numMoves; i++)
		{                    
			moveScore[i] = StaticExchangeScore(captureList[i]);
		}
		OrderMoves(captureList, moveScore, numMoves);

		captureList[numMoves] = NoMove;
		captureIndex = 0;

	case GoodCapturesType:
		int retMove;
		do
		{
			retMove = captureList[captureIndex++];
		} while (retMove != NoMove
			&& retMove == hashMove);

		if (moveScore[captureIndex - 1] >= 0 && retMove != NoMove)
		{
			return retMove;
		} else
		{
			captureIndex--;
			moveType = NextMoveType::KillerOneType;
		}

	case KillerOneType:
		moveType = NextMoveType::KillerTwoType;
		retMove = killerMoves[0][currentDepth];
		if (retMove != NoMove && retMove != hashMove && ValidateNonCapture(retMove))
		{
			return retMove;
		}

	case KillerTwoType:
		moveType = NextMoveType::GenerateNonCapturesType;
		retMove = killerMoves[1][currentDepth];
		if (retMove != NoMove && retMove != hashMove && ValidateNonCapture(retMove))
		{
			return retMove;
		}

	case GenerateNonCapturesType:
		moveType = NextMoveType::NonCapturesType;
		numMoves = 0;
		memset(nonCaptureList, 0, sizeof(nonCaptureList));
		GenerateNonCaptures(nonCaptureList, numMoves);
		memset(moveScore, 0, sizeof(moveScore)); // TODO Order based on history heuritic
		for (int i = 0; i < numMoves; i++)
		{                    
			moveScore[i] = historyHeuristicBoard[Start(nonCaptureList[i])][End(nonCaptureList[i])];
		}
		OrderMoves(nonCaptureList, moveScore, numMoves);
		nonCaptureList[numMoves] = NoMove;
		nonCaptureIndex = 0;

	case NonCapturesType:
		do
		{
			retMove = nonCaptureList[nonCaptureIndex++];
		} while (retMove != NoMove
			&& (retMove == killerMoves[0][currentDepth] || retMove == killerMoves[1][currentDepth] || retMove == hashMove));

		if (retMove != NoMove)
		{
			return retMove;
		} else
		{
			moveType = NextMoveType::BadCapturesType;
		}

	case BadCapturesType:
		do
		{
			retMove = captureList[captureIndex++];
		} while (retMove != NoMove && retMove == hashMove);
		if (retMove != NoMove)
		{
			return retMove;
		}
	}

	return NoMove;
}

void MoveOrderingData::OrderMoves(Move moves[], int scores[], int numMoves) //Shell Sort
{
	int increment = numMoves / 2;

	while (increment > 0)
	{
		for (int i = increment; i < numMoves; i += increment)
		{
			int j = i;
			int tempScore = scores[i];
			int tempMove = moves[i];
			while ((j >= increment) && (scores[j - increment] < tempScore))
			{
				scores[j] = scores[j - increment];
				moves[j] = moves[j - increment];
				j = j - increment;
			}
			scores[j] = tempScore;
			moves[j] = tempMove;
		}

		if (increment == 2)
		{
			increment = 1;
		} else
		{
			increment = (int) (increment / 2.2);
		}
	}
}

Bitboard MoveOrderingData::occupancyBitboards[64][64] = {0};
void MoveOrderingData::InitializeOccupancyBitboards()
{
	for (Square startSquare = 0; startSquare < 64; startSquare++)
	{
		for (Square endSquare = startSquare + 1; endSquare < 64; endSquare++)
		{
			Bitboard occupancy =
				(((RookMagicBitboard(startSquare, 0) & (0x1ull << endSquare)) != 0) ? (RookMagicBitboard(startSquare, 0x1ull << endSquare) & RookMagicBitboard(endSquare, 0x1ull << startSquare)) : 0)
				| (((BishopMagicBitboard(startSquare, 0) & (0x1ull << endSquare)) != 0) ? (BishopMagicBitboard(startSquare, 0x1ull << endSquare) & BishopMagicBitboard(endSquare, 0x1ull << startSquare)) : 0);

			occupancyBitboards[startSquare][endSquare] = occupancy | (0x1ull << endSquare);
			occupancyBitboards[endSquare][startSquare] = occupancy | (0x1ull << startSquare);

		}
	}
}

bool MoveOrderingData::ValidateNonCapture(Move move)
{
	Square startSquare = Start(move);
	Square endSquare = End(move);
	Piece moved = Moved(move);

	if (whiteToMove) {

		if (moved == PieceIndex::WhiteKing) {
			if (endSquare - startSquare == 2) {
				if (!(ShortCastle(whiteCastle) != 0 && ((pieceBitboards[PieceIndex::AllPieces] >> 5) & 0x3) == 0 && !BlackAttack(Squares::E1) && !BlackAttack(Squares::F1) && !BlackAttack(Squares::G1))) {
					return false;
				}
			} else if (endSquare - startSquare == -2) {
				if (!(LongCastle(whiteCastle) != 0 && ((pieceBitboards[PieceIndex::AllPieces] >> 1) & 0x7) == 0 && !BlackAttack(Squares::E1) && !BlackAttack(Squares::D1) && !BlackAttack(Squares::C1))) {
					return false;
				}
			}
		}

	} else if (moved == PieceIndex::BlackKing) {
		if (endSquare - startSquare == 2) {
			if (!(ShortCastle(blackCastle) != 0 && ((pieceBitboards[PieceIndex::AllPieces] >> 61) & 0x3) == 0 && !WhiteAttack(Squares::E8) && !WhiteAttack(Squares::F8) && !WhiteAttack(Squares::G8))) {
				return false;
			}
		} else if (endSquare - startSquare == -2) {
			if (!(LongCastle(blackCastle) != 0 && ((pieceBitboards[PieceIndex::AllPieces] >> 57) & 0x7) == 0 && !WhiteAttack(Squares::E8) && !WhiteAttack(Squares::D8) && !WhiteAttack(Squares::C8))) {
				return false;
			}
		}
	}

	return (moved == pieceTypes[startSquare]) && (occupancyBitboards[startSquare][endSquare] & pieceBitboards[PieceIndex::AllPieces]) == 0;
}

bool MoveOrderingData::ValidateMove(Move move) //TODO: CHANGE !!
{
	int num = 0;
	int moveList[MaxNumberMoves] = {0};
	GenerateCaptures(moveList, num);
	GenerateNonCaptures(moveList, num);
	bool found = false;
	for (int i = 0; i < num; i++)
	{
		if (move == moveList[i])
		{
			found = true;
			break;
		}
	}
	return found;
}