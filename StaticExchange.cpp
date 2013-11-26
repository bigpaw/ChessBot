#include "Include.h"

int StaticExchangeScore(Move move)
{
	StaticExchangeData attackerData[16] = {};
	StaticExchangeData defenderData[16] = {};
	int score, targetPieceScore;
	int attackerIndex = 0, defenderIndex = 0;
	int gain[32] = {0};

	Bitboard bitboardsCopy[13];
	memcpy(bitboardsCopy, pieceBitboards+2, sizeof(bitboardsCopy));

	int startSquare = Start(move);
	int endSquare = End(move);
	int capturedType = Captured(move);
	int promotedType = Promoted(move);
	int movedType = Moved(move);

	//Remove first attacking piece from bitboards
	Bitboard startBit = 1ull << startSquare;
	bitboardsCopy[movedType - 2] ^= startBit;
	bitboardsCopy[PieceIndex::AllPieces - 2] ^= startBit;
	targetPieceScore = PieceScoreByIndex[movedType];

	if (capturedType == PieceIndex::EnPassant)
	{
		if (whiteToMove)
		{
			Bitboard enPassantBit = 1ull << (endSquare - 8);
			bitboardsCopy[PieceIndex::BlackPawn - 2] ^= enPassantBit;
			bitboardsCopy[PieceIndex::AllPieces - 2] ^= enPassantBit;
		} else
		{
			Bitboard enPassantBit = 1ull << (endSquare + 8);
			bitboardsCopy[PieceIndex::WhitePawn - 2] ^= enPassantBit;
			bitboardsCopy[PieceIndex::AllPieces - 2] ^= enPassantBit;
		}
		score = PieceScore::PawnScore;
	} else
	{
		score = PieceScoreByIndex[capturedType];
	}

	if (promotedType != PieceIndex::NoPiece)
	{
		score += PieceScoreByIndex[promotedType] - PieceScore::PawnScore;
		targetPieceScore = PieceScoreByIndex[promotedType];
	}

	int numAttackers = PopulateExchangeArray(attackerData, bitboardsCopy, whiteToMove, endSquare);
	int numDefenders = PopulateExchangeArray(defenderData, bitboardsCopy, !whiteToMove, endSquare);

	gain[0] = score;
	while (true)
	{
		//Defender recapture
		if (defenderIndex >= numDefenders)
		{
			break;
		}

		score -= targetPieceScore;
		targetPieceScore = defenderData[defenderIndex].value;
		if (IsPawn(defenderData[defenderIndex].piece) && (endSquare < Squares::B1 || endSquare > Squares::G8))
		{
			score -= PieceScore::QueenScore - PieceScore::PawnScore;
			targetPieceScore = PieceScore::QueenScore;
		}

		if (IsXrayPiece(defenderData[defenderIndex].piece))
		{
			bitboardsCopy[PieceIndex::AllPieces - 2] ^= 1ull << defenderData[defenderIndex].square;
			numDefenders = SearchHiddenPiece(defenderData, bitboardsCopy, !whiteToMove, endSquare, defenderData[defenderIndex].square, numDefenders, defenderIndex);
		}
		gain[defenderIndex * 2 + 1] = -score;
		defenderIndex++;

		//Attacker recapture
		if (attackerIndex >= numAttackers)
		{
			break;
		}

		score += targetPieceScore;
		targetPieceScore = attackerData[attackerIndex].value;
		if (IsPawn(attackerData[attackerIndex].piece) && (endSquare < Squares::B1 || endSquare > Squares::G8))
		{
			score += PieceScore::QueenScore - PieceScore::PawnScore;
			targetPieceScore = PieceScore::QueenScore;
		}
		if (IsXrayPiece(attackerData[attackerIndex].piece))
		{
			bitboardsCopy[PieceIndex::AllPieces - 2] ^= 1ull << attackerData[attackerIndex].square;
			numAttackers = SearchHiddenPiece(attackerData, bitboardsCopy, whiteToMove, endSquare, attackerData[attackerIndex].square, numAttackers, attackerIndex);
		}
		gain[attackerIndex * 2 + 2] = score;
		attackerIndex++;
	}

	for (int i = attackerIndex + defenderIndex; i > 0; i--)
	{
		gain[i - 1] = -max(-gain[i - 1], gain[i]);
	}
	return gain[0];
}

int SearchHiddenPiece(StaticExchangeData data[], Bitboard bitboards[], bool white, Square square, Square prevPieceSquare, int numPieces, int currentIndex)
{
	if (white)
	{
		//Rank or File X-ray attack
		if ((prevPieceSquare - square) % 2 == 0)
		{
			Bitboard targets = bitboards[PieceIndex::WhiteRook - 2] & RookMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
			if (targets != 0)
			{
				int startSquare = TrailingZeros(targets);
				StaticExchangeData newData(PieceIndex::WhiteRook, PieceScore::RookScore, startSquare);
				bitboards[PieceIndex::WhiteRook - 2] ^= 1ull << startSquare;
				InsertStaticExchangeData(data, newData, currentIndex, numPieces);
				numPieces++;
				return numPieces;
			}

			targets = bitboards[PieceIndex::WhiteQueen - 2] & BishopMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]) |
				bitboards[PieceIndex::WhiteQueen - 2] & RookMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
			if (targets != 0)
			{
				int startSquare = TrailingZeros(targets);
				StaticExchangeData newData(PieceIndex::WhiteQueen, PieceScore::QueenScore, startSquare);
				bitboards[PieceIndex::WhiteQueen - 2] ^= 1ull << startSquare;
				InsertStaticExchangeData(data, newData, currentIndex, numPieces);
				numPieces++;
			}
		} //Diagonal X-ray attack
		else
		{
			Bitboard targets = bitboards[PieceIndex::WhiteBishop - 2] & BishopMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
			if (targets != 0)
			{
				int startSquare = TrailingZeros(targets);
				StaticExchangeData newData(PieceIndex::WhiteBishop, PieceScore::BishopScore, startSquare);
				bitboards[PieceIndex::WhiteBishop - 2] ^= 1ull << startSquare;
				InsertStaticExchangeData(data, newData, currentIndex, numPieces);
				numPieces++;
				return numPieces;
			}

			targets = bitboards[PieceIndex::WhiteQueen - 2] & BishopMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]) |
				bitboards[PieceIndex::WhiteQueen - 2] & RookMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
			if (targets != 0)
			{
				int startSquare = TrailingZeros(targets);
				StaticExchangeData newData(PieceIndex::WhiteQueen, PieceScore::QueenScore, startSquare);
				bitboards[PieceIndex::WhiteQueen - 2] ^= 1ull << startSquare;
				InsertStaticExchangeData(data, newData, currentIndex, numPieces);
				numPieces++;
			}
		}
	} else
	{
		//Rank or File X-ray attack
		if ((prevPieceSquare - square) % 2 == 0)
		{
			Bitboard targets = bitboards[PieceIndex::BlackRook - 2] & RookMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
			if (targets != 0)
			{
				int startSquare = TrailingZeros(targets);
				StaticExchangeData newData(PieceIndex::BlackRook, PieceScore::RookScore, startSquare);
				bitboards[PieceIndex::BlackRook - 2] ^= 1ull << startSquare;
				InsertStaticExchangeData(data, newData, currentIndex, numPieces);
				numPieces++;
				return numPieces;
			}

			targets = bitboards[PieceIndex::BlackQueen - 2] & BishopMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]) |
				bitboards[PieceIndex::BlackQueen - 2] & RookMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
			if (targets != 0)
			{
				int startSquare = TrailingZeros(targets);
				StaticExchangeData newData(PieceIndex::BlackQueen, PieceScore::QueenScore, startSquare);
				bitboards[PieceIndex::BlackQueen - 2] ^= 1ull << startSquare;
				InsertStaticExchangeData(data, newData, currentIndex, numPieces);
				numPieces++;
			}
		} //Diagonal X-ray attack
		else
		{
			Bitboard targets = bitboards[PieceIndex::BlackBishop - 2] & BishopMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
			if (targets != 0)
			{
				int startSquare = TrailingZeros(targets);
				StaticExchangeData newData(PieceIndex::BlackBishop, PieceScore::BishopScore, startSquare);
				bitboards[PieceIndex::BlackBishop - 2] ^= 1ull << startSquare;
				InsertStaticExchangeData(data, newData, currentIndex, numPieces);
				numPieces++;
				return numPieces;
			}

			targets = bitboards[PieceIndex::BlackQueen - 2] & BishopMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]) |
				bitboards[PieceIndex::BlackQueen - 2] & RookMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
			if (targets != 0)
			{
				int startSquare = TrailingZeros(targets);
				StaticExchangeData newData(PieceIndex::BlackQueen, PieceScore::QueenScore, startSquare);
				bitboards[PieceIndex::BlackQueen - 2] ^= 1ull << startSquare;
				InsertStaticExchangeData(data, newData, currentIndex, numPieces);
				numPieces++;
			}
		}
	}
	return numPieces;
}

void InsertStaticExchangeData(StaticExchangeData data[], StaticExchangeData newData, int currentIndex, int numPieces)
{
	int i;
	for (i = currentIndex + 1; i < numPieces; i++)
	{
		if (newData.value < data[i].value)
		{
			for (int j = i; j < numPieces; j++)
			{
				data[j + 1] = data[j];
			}
			break;
		}
	}
	data[i] = newData;
}

int PopulateExchangeArray(StaticExchangeData data[], Bitboard bitboards[], bool white, Square square)
{
	int numPieces = 0;

	if (white)
	{
		Bitboard targets = bitboards[PieceIndex::WhitePawn - 2] & AttackedByWhitePawnBitboard[square];
		while (targets != 0)
		{
			data[numPieces] = StaticExchangeData(PieceIndex::WhitePawn, PieceScore::PawnScore, TrailingZeros(targets));
			ResetLowestBit(targets);
			numPieces++;
		}

		targets = bitboards[PieceIndex::WhiteKnight - 2] & KnightMoveBitboard[square];
		while (targets != 0)
		{
			data[numPieces] = StaticExchangeData(PieceIndex::WhiteKnight, PieceScore::KnightScore, TrailingZeros(targets));
			ResetLowestBit(targets);
			numPieces++;
		}

		targets = bitboards[PieceIndex::WhiteBishop - 2] & BishopMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
		while (targets != 0)
		{
			int startSquare = TrailingZeros(targets);
			data[numPieces] = StaticExchangeData(PieceIndex::WhiteBishop, PieceScore::BishopScore, startSquare);
			ResetLowestBit(targets);
			bitboards[PieceIndex::WhiteBishop - 2] ^= 1ull << startSquare;
			numPieces++;
		}

		targets = bitboards[PieceIndex::WhiteRook - 2] & RookMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
		while (targets != 0)
		{
			int startSquare = TrailingZeros(targets);
			data[numPieces] = StaticExchangeData(PieceIndex::WhiteRook, PieceScore::RookScore, startSquare);
			ResetLowestBit(targets);
			bitboards[PieceIndex::WhiteRook - 2] ^= 1ull << startSquare;
			numPieces++;
		}

		targets = bitboards[PieceIndex::WhiteQueen - 2] & BishopMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]) |
			bitboards[PieceIndex::WhiteQueen - 2] & RookMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
		while (targets != 0)
		{
			int startSquare = TrailingZeros(targets);
			data[numPieces] = StaticExchangeData(PieceIndex::WhiteQueen, PieceScore::QueenScore, startSquare);
			ResetLowestBit(targets);
			bitboards[PieceIndex::WhiteQueen - 2] ^= 1ull << startSquare;
			numPieces++;
		}

		targets = bitboards[PieceIndex::WhiteKing - 2] & KingMoveBitboard[square];
		while (targets != 0)
		{
			data[numPieces] = StaticExchangeData(PieceIndex::WhiteKing, PieceScore::KingScore, TrailingZeros(targets));
			ResetLowestBit(targets);
			numPieces++;
		}
	} else
	{
		Bitboard targets = bitboards[PieceIndex::BlackPawn - 2] & AttackedByBlackPawnBitboard[square];
		while (targets != 0)
		{
			data[numPieces] = StaticExchangeData(PieceIndex::BlackPawn, PieceScore::PawnScore, TrailingZeros(targets));
			ResetLowestBit(targets);
			numPieces++;
		}

		targets = bitboards[PieceIndex::BlackKnight - 2] & KnightMoveBitboard[square];
		while (targets != 0)
		{
			data[numPieces] = StaticExchangeData(PieceIndex::BlackKnight, PieceScore::KnightScore, TrailingZeros(targets));
			ResetLowestBit(targets);
			numPieces++;
		}

		targets = bitboards[PieceIndex::BlackBishop - 2] & BishopMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
		while (targets != 0)
		{
			int startSquare = TrailingZeros(targets);
			data[numPieces] = StaticExchangeData(PieceIndex::BlackBishop, PieceScore::BishopScore, startSquare);
			ResetLowestBit(targets);
			bitboards[PieceIndex::BlackBishop - 2] ^= 1ull << startSquare;
			numPieces++;
		}

		targets = bitboards[PieceIndex::BlackRook - 2] & RookMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
		while (targets != 0)
		{
			int startSquare = TrailingZeros(targets);
			data[numPieces] = StaticExchangeData(PieceIndex::BlackRook, PieceScore::RookScore, startSquare);
			ResetLowestBit(targets);
			bitboards[PieceIndex::BlackRook - 2] ^= 1ull << startSquare;
			numPieces++;
		}

		targets = bitboards[PieceIndex::BlackQueen - 2] & BishopMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]) |
			bitboards[PieceIndex::BlackQueen - 2] & RookMagicBitboard(square, bitboards[PieceIndex::AllPieces - 2]);
		while (targets != 0)
		{
			int startSquare = TrailingZeros(targets);
			data[numPieces] = StaticExchangeData(PieceIndex::BlackQueen, PieceScore::QueenScore, startSquare);
			ResetLowestBit(targets);
			bitboards[PieceIndex::BlackQueen - 2] ^= 1ull << startSquare;
			numPieces++;
		}

		targets = bitboards[PieceIndex::BlackKing - 2] & KingMoveBitboard[square];
		while (targets != 0)
		{
			data[numPieces] = StaticExchangeData(PieceIndex::BlackKing, PieceScore::KingScore, TrailingZeros(targets));
			ResetLowestBit(targets);
			numPieces++;
		}
	}

	return numPieces;
}