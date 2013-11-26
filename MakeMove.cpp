#include "Include.h"

void MakeMove(const int& move)
{
#ifdef DEBUG_MODE
	Debugger::searchLine[currentDepth] = move;
#endif
	storeWhiteCastle[currentDepth] = whiteCastle;
	storeBlackCastle[currentDepth] = blackCastle;
	storeEnPassant[currentDepth] = enPassant;
	storeHashKeys[currentDepth] = currentPositionKey;
	storePieceSquareScore[currentDepth] = pieceSquareScore;

	if (enPassant != NoEnPassant)
	{
		currentPositionKey ^= TranspoTable::EnPassantKeys[enPassant];
		enPassant = NoEnPassant;
	}
	currentDepth++;

	repetitionFifty[currentDepth] = repetitionFifty[currentDepth - 1] + 1;

	Square startSquare = Start(move);
	Square endSquare = End(move);
	Piece capturedType = Captured(move);
	Piece promotedType = Promoted(move);
	Piece movedType = Moved(move);

	//General procedures applied to any kind of pieces/moves
	pieceTypes[startSquare] = PieceIndex::NoPiece;
	pieceTypes[endSquare] = movedType;
	Bitboard sourceBit = 1ull << startSquare;
	Bitboard destBit = 1ull << endSquare;
	Bitboard combBit = sourceBit ^ destBit;

	//specific pieces update
	if (whiteToMove)
	{
		switch (movedType)
		{
		case PieceIndex::WhitePawn:
			if (promotedType != PieceIndex::NoPiece)
			{
				pieceBitboards[PieceIndex::WhitePawn] ^= sourceBit;
				whitePawnScore -= PieceScore::PawnScore;

				switch (promotedType)
				{
				case PieceIndex::WhiteQueen:
					pieceBitboards[PieceIndex::WhiteQueen] ^= destBit;
					pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
					pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
					pieceTypes[endSquare] = PieceIndex::WhiteQueen;
					whitePieceScore += PieceScore::QueenScore;
					pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][startSquare];
					pieceSquareScore += PieceSquareTable[PieceIndex::WhiteQueen - 2][endSquare];
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteQueen - 2][endSquare] ^ TranspoTable::ZobristKeys[PieceIndex::WhitePawn - 2][startSquare];
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][startSquare];
					break;
				case PieceIndex::WhiteKnight:
					pieceBitboards[PieceIndex::WhiteKnight] ^= destBit;
					pieceTypes[endSquare] = PieceIndex::WhiteKnight;
					whitePieceScore += PieceScore::KnightScore;
					pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][startSquare];
					pieceSquareScore += PieceSquareTable[PieceIndex::WhiteKnight - 2][endSquare];
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteKnight - 2][endSquare] ^ TranspoTable::ZobristKeys[PieceIndex::WhitePawn - 2][startSquare];
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][startSquare];
					break;
				case PieceIndex::WhiteRook:
					pieceBitboards[PieceIndex::WhiteRook] ^= destBit;
					pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
					pieceTypes[endSquare] = PieceIndex::WhiteRook;
					whitePieceScore += PieceScore::RookScore;
					pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][startSquare];
					pieceSquareScore += PieceSquareTable[PieceIndex::WhiteRook - 2][endSquare];
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteRook - 2][endSquare] ^ TranspoTable::ZobristKeys[PieceIndex::WhitePawn - 2][startSquare];
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][startSquare];
					break;
				case PieceIndex::WhiteBishop:
					pieceBitboards[PieceIndex::WhiteBishop] ^= destBit;
					pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
					pieceTypes[endSquare] = PieceIndex::WhiteBishop;
					whitePieceScore += PieceScore::BishopScore;
					pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][startSquare];
					pieceSquareScore += PieceSquareTable[PieceIndex::WhiteBishop - 2][endSquare];
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteBishop - 2][endSquare] ^ TranspoTable::ZobristKeys[PieceIndex::WhitePawn - 2][startSquare];
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][startSquare];
					break;
				}
			} else
			{
				pieceBitboards[PieceIndex::WhitePawn] ^= combBit;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhitePawn - 2][startSquare] ^ TranspoTable::ZobristKeys[PieceIndex::WhitePawn - 2][endSquare];
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][startSquare] ^ PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][endSquare];
				pieceSquareScore += PieceSquareTable[PieceIndex::WhitePawn - 2][endSquare]
				- PieceSquareTable[PieceIndex::WhitePawn - 2][startSquare];
				if ((endSquare - startSquare) == 16)
				{
					enPassant = endSquare - 8;
					currentPositionKey ^= TranspoTable::EnPassantKeys[enPassant];
				}
			}
			repetitionFifty[currentDepth] = 0;
			break;

		case PieceIndex::WhiteKnight:
			pieceBitboards[PieceIndex::WhiteKnight] ^= combBit;
			currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteKnight - 2][startSquare] ^ TranspoTable::ZobristKeys[PieceIndex::WhiteKnight - 2][endSquare];
			pieceSquareScore += PieceSquareTable[PieceIndex::WhiteKnight - 2][endSquare] - PieceSquareTable[PieceIndex::WhiteKnight - 2][startSquare];
			break;

		case PieceIndex::WhiteBishop:
			pieceBitboards[PieceIndex::WhiteBishop] ^= combBit;
			pieceBitboards[PieceIndex::WhiteBishopQueen] ^= combBit;
			currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteBishop - 2][startSquare] ^ TranspoTable::ZobristKeys[PieceIndex::WhiteBishop - 2][endSquare];
			pieceSquareScore += PieceSquareTable[PieceIndex::WhiteBishop - 2][endSquare] - PieceSquareTable[PieceIndex::WhiteBishop - 2][startSquare];
			break;

		case PieceIndex::WhiteRook:
			pieceBitboards[PieceIndex::WhiteRook] ^= combBit;
			pieceBitboards[PieceIndex::WhiteRookQueen] ^= combBit;
			currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteRook - 2][startSquare] ^ TranspoTable::ZobristKeys[PieceIndex::WhiteRook - 2][endSquare];
			pieceSquareScore += PieceSquareTable[PieceIndex::WhiteRook - 2][endSquare] - PieceSquareTable[PieceIndex::WhiteRook - 2][startSquare];
			if (whiteCastle != NoCastling)
			{
				if (startSquare == Squares::A1 && LongCastle(whiteCastle) != 0)
				{
					currentPositionKey ^= TranspoTable::WhiteLongCastleKey;
					whiteCastle &= 1;
					repetitionFifty[currentDepth] = 0;
				} else if (startSquare == Squares::H1 && ShortCastle(whiteCastle) != 0)
				{
					currentPositionKey ^= TranspoTable::WhiteShortCastleKey;
					whiteCastle &= 2;
					repetitionFifty[currentDepth] = 0;
				}
			}
			break;

		case PieceIndex::WhiteQueen:
			pieceBitboards[PieceIndex::WhiteQueen] ^= combBit;
			pieceBitboards[PieceIndex::WhiteRookQueen] ^= combBit;
			pieceBitboards[PieceIndex::WhiteBishopQueen] ^= combBit;
			currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteQueen - 2][startSquare] ^ TranspoTable::ZobristKeys[PieceIndex::WhiteQueen - 2][endSquare];
			pieceSquareScore += PieceSquareTable[PieceIndex::WhiteQueen - 2][endSquare] - PieceSquareTable[PieceIndex::WhiteQueen - 2][startSquare];
			break;

		case PieceIndex::WhiteKing:
			pieceBitboards[PieceIndex::WhiteKing] ^= combBit;
			currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteKing - 2][startSquare]
			^ TranspoTable::ZobristKeys[PieceIndex::WhiteKing - 2][endSquare];
			pieceSquareScore += PieceSquareTable[PieceIndex::WhiteKing - 2][endSquare] - PieceSquareTable[PieceIndex::WhiteKing - 2][startSquare];

			if (whiteCastle != NoCastling)
			{
				if (endSquare - startSquare == 2)
				{
					pieceBitboards[PieceIndex::WhiteRook] ^= 0xA0l; //0xA0 == '10100000'
					pieceBitboards[PieceIndex::WhiteRookQueen] ^= 0xA0l;
					pieceBitboards[PieceIndex::WhitePieces] ^= 0xA0l;
					pieceBitboards[PieceIndex::AllPieces] ^= 0xA0l;
					pieceTypes[Squares::H1] = PieceIndex::NoPiece;
					pieceTypes[Squares::F1] = PieceIndex::WhiteRook;
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteRook - 2][Squares::H1] ^ TranspoTable::ZobristKeys[PieceIndex::WhiteRook - 2][Squares::F1];
					pieceSquareScore += PieceSquareTable[PieceIndex::WhiteRook - 2][Squares::F1] - PieceSquareTable[PieceIndex::WhiteRook - 2][Squares::H1];
				} else if (endSquare - startSquare == -2)
				{
					pieceBitboards[PieceIndex::WhiteRook] ^= 0x9l; //0x9 == '00001001'
					pieceBitboards[PieceIndex::WhiteRookQueen] ^= 0x9l;
					pieceBitboards[PieceIndex::WhitePieces] ^= 0x9l;
					pieceBitboards[PieceIndex::AllPieces] ^= 0x9l;
					pieceTypes[Squares::A1] = PieceIndex::NoPiece;
					pieceTypes[Squares::D1] = PieceIndex::WhiteRook;
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteRook - 2][Squares::A1] ^ TranspoTable::ZobristKeys[PieceIndex::WhiteRook - 2][Squares::D1];
					pieceSquareScore += PieceSquareTable[PieceIndex::WhiteRook - 2][Squares::D1] - PieceSquareTable[PieceIndex::WhiteRook - 2][Squares::A1];
				}
				if (LongCastle(whiteCastle) != 0)
				{
					currentPositionKey ^= TranspoTable::WhiteLongCastleKey;
				}
				if (ShortCastle(whiteCastle) != 0)
				{
					currentPositionKey ^= TranspoTable::WhiteShortCastleKey;
				}
				whiteCastle = NoCastling;
				repetitionFifty[currentDepth] = 0;
			}
			break;
		}

		pieceBitboards[PieceIndex::WhitePieces] ^= combBit;
		pieceBitboards[PieceIndex::AllPieces] ^= combBit;

		if (capturedType != PieceIndex::NoPiece)
		{
			switch (capturedType)
			{
			case PieceIndex::BlackPawn:
				pieceBitboards[PieceIndex::BlackPawn] ^= destBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				blackPawnScore -= PieceScore::PawnScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][endSquare];
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackPawn - 2][endSquare];
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][endSquare];
				break;

			case PieceIndex::BlackKnight:
				pieceBitboards[PieceIndex::BlackKnight] ^= destBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				blackPieceScore -= PieceScore::KnightScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::BlackKnight - 2][endSquare];
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackKnight - 2][endSquare];
				break;

			case PieceIndex::BlackBishop:
				pieceBitboards[PieceIndex::BlackBishop] ^= destBit;
				pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				blackPieceScore -= PieceScore::BishopScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::BlackBishop - 2][endSquare];
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackBishop - 2][endSquare];
				break;

			case PieceIndex::BlackRook:
				pieceBitboards[PieceIndex::BlackRook] ^= destBit;
				pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				blackPieceScore -= PieceScore::RookScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::BlackRook - 2][endSquare];
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackRook - 2][endSquare];
				if (blackCastle != NoCastling)
				{
					if (endSquare == Squares::A8 && LongCastle(blackCastle) != 0)
					{
						currentPositionKey ^= TranspoTable::BlackLongCastleKey;
						blackCastle &= 1;
					} else if (endSquare == Squares::H8 && ShortCastle(blackCastle) != 0)
					{
						currentPositionKey ^= TranspoTable::BlackShortCastleKey;
						blackCastle &= 2;
					}
				}
				break;

			case PieceIndex::BlackQueen:
				pieceBitboards[PieceIndex::BlackQueen] ^= destBit;
				pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
				pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				blackPieceScore -= PieceScore::QueenScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::BlackQueen - 2][endSquare];
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackQueen - 2][endSquare];
				break;

			case PieceIndex::EnPassant:
				Bitboard enPassantBit = destBit >> 8;
				Square enPassantSquare = endSquare - 8;
				pieceBitboards[PieceIndex::BlackPawn] ^= enPassantBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= enPassantBit;
				pieceBitboards[PieceIndex::AllPieces] ^= enPassantBit;
				pieceTypes[enPassantSquare] = PieceIndex::NoPiece;
				blackPawnScore -= PieceScore::PawnScore;
				pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][enPassantSquare];
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackPawn - 2][enPassantSquare];
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][enPassantSquare];
				break;
			}
			repetitionFifty[currentDepth] = 0;
		}
	} else
	{
		switch (movedType)
		{
		case PieceIndex::BlackPawn:
			if (promotedType != PieceIndex::NoPiece)
			{
				pieceBitboards[PieceIndex::BlackPawn] ^= sourceBit;
				blackPawnScore -= PieceScore::PawnScore;

				switch (promotedType)
				{
				case PieceIndex::BlackQueen:
					pieceBitboards[PieceIndex::BlackQueen] ^= destBit;
					pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
					pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
					pieceTypes[endSquare] = PieceIndex::BlackQueen;
					blackPieceScore += PieceScore::QueenScore;
					pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][startSquare];
					pieceSquareScore -= PieceSquareTable[PieceIndex::BlackQueen - 2][endSquare];
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackQueen - 2][endSquare] ^ TranspoTable::ZobristKeys[PieceIndex::BlackPawn - 2][startSquare];
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][startSquare];
					break;
				case PieceIndex::BlackKnight:
					pieceBitboards[PieceIndex::BlackKnight] ^= destBit;
					pieceTypes[endSquare] = PieceIndex::BlackKnight;
					blackPieceScore += PieceScore::KnightScore;
					pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][startSquare];
					pieceSquareScore -= PieceSquareTable[PieceIndex::BlackKnight - 2][endSquare];
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackKnight - 2][endSquare] ^ TranspoTable::ZobristKeys[PieceIndex::BlackPawn - 2][startSquare];
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][startSquare];
					break;
				case PieceIndex::BlackRook:
					pieceBitboards[PieceIndex::BlackRook] ^= destBit;
					pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
					pieceTypes[endSquare] = PieceIndex::BlackRook;
					blackPieceScore += PieceScore::RookScore;
					pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][startSquare];
					pieceSquareScore -= PieceSquareTable[PieceIndex::BlackRook - 2][endSquare];
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackRook - 2][endSquare] ^ TranspoTable::ZobristKeys[PieceIndex::BlackPawn - 2][startSquare];
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][startSquare];
					break;
				case PieceIndex::BlackBishop:
					pieceBitboards[PieceIndex::BlackBishop] ^= destBit;
					pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
					pieceTypes[endSquare] = PieceIndex::BlackBishop;
					blackPieceScore += PieceScore::BishopScore;
					pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][startSquare];
					pieceSquareScore -= PieceSquareTable[PieceIndex::BlackBishop - 2][endSquare];
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackBishop - 2][endSquare] ^ TranspoTable::ZobristKeys[PieceIndex::BlackPawn - 2][startSquare];
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][startSquare];
					break;
				}
			} else
			{
				pieceBitboards[PieceIndex::BlackPawn] ^= combBit;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackPawn - 2][startSquare] ^ TranspoTable::ZobristKeys[PieceIndex::BlackPawn - 2][endSquare];
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][startSquare] ^ PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][endSquare];
				pieceSquareScore -= PieceSquareTable[PieceIndex::BlackPawn - 2][endSquare]
				- PieceSquareTable[PieceIndex::BlackPawn - 2][startSquare];
				if ((endSquare - startSquare) == -16)
				{
					enPassant = endSquare + 8;
					currentPositionKey ^= TranspoTable::EnPassantKeys[enPassant];
				}
			}
			repetitionFifty[currentDepth] = 0;
			break;

		case PieceIndex::BlackKnight:
			pieceBitboards[PieceIndex::BlackKnight] ^= combBit;
			currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackKnight - 2][startSquare] ^ TranspoTable::ZobristKeys[PieceIndex::BlackKnight - 2][endSquare];
			pieceSquareScore -= PieceSquareTable[PieceIndex::BlackKnight - 2][endSquare] - PieceSquareTable[PieceIndex::BlackKnight - 2][startSquare];
			break;

		case PieceIndex::BlackBishop:
			pieceBitboards[PieceIndex::BlackBishop] ^= combBit;
			pieceBitboards[PieceIndex::BlackBishopQueen] ^= combBit;
			currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackBishop - 2][startSquare] ^ TranspoTable::ZobristKeys[PieceIndex::BlackBishop - 2][endSquare];
			pieceSquareScore -= PieceSquareTable[PieceIndex::BlackBishop - 2][endSquare] - PieceSquareTable[PieceIndex::BlackBishop - 2][startSquare];
			break;

		case PieceIndex::BlackRook:
			pieceBitboards[PieceIndex::BlackRook] ^= combBit;
			pieceBitboards[PieceIndex::BlackRookQueen] ^= combBit;
			currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackRook - 2][startSquare] ^ TranspoTable::ZobristKeys[PieceIndex::BlackRook - 2][endSquare];
			pieceSquareScore -= PieceSquareTable[PieceIndex::BlackRook - 2][endSquare] - PieceSquareTable[PieceIndex::BlackRook - 2][startSquare];
			if (blackCastle != NoCastling)
			{
				if (startSquare == Squares::A8 && LongCastle(blackCastle) != 0)
				{
					currentPositionKey ^= TranspoTable::BlackLongCastleKey;
					blackCastle &= 1;
					repetitionFifty[currentDepth] = 0;
				} else if (startSquare == Squares::H8 && ShortCastle(blackCastle) != 0)
				{
					currentPositionKey ^= TranspoTable::BlackShortCastleKey;
					blackCastle &= 2;
					repetitionFifty[currentDepth] = 0;
				}
			}
			break;

		case PieceIndex::BlackQueen:
			pieceBitboards[PieceIndex::BlackQueen] ^= combBit;
			pieceBitboards[PieceIndex::BlackRookQueen] ^= combBit;
			pieceBitboards[PieceIndex::BlackBishopQueen] ^= combBit;
			currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackQueen - 2][startSquare] ^ TranspoTable::ZobristKeys[PieceIndex::BlackQueen - 2][endSquare];
			pieceSquareScore -= PieceSquareTable[PieceIndex::BlackQueen - 2][endSquare] - PieceSquareTable[PieceIndex::BlackQueen - 2][startSquare];
			break;

		case PieceIndex::BlackKing:
			pieceBitboards[PieceIndex::BlackKing] ^= combBit;
			currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackKing - 2][startSquare]
			^ TranspoTable::ZobristKeys[PieceIndex::BlackKing - 2][endSquare];
			pieceSquareScore -= PieceSquareTable[PieceIndex::BlackKing - 2][endSquare] - PieceSquareTable[PieceIndex::BlackKing - 2][startSquare];

			if (blackCastle != NoCastling)
			{
				if (endSquare - startSquare == 2)
				{
					pieceBitboards[PieceIndex::BlackRook] ^= 0xA000000000000000l;
					pieceBitboards[PieceIndex::BlackRookQueen] ^= 0xA000000000000000l;
					pieceBitboards[PieceIndex::BlackPieces] ^= 0xA000000000000000l;
					pieceBitboards[PieceIndex::AllPieces] ^= 0xA000000000000000l;
					pieceTypes[Squares::H8] = PieceIndex::NoPiece;
					pieceTypes[Squares::F8] = PieceIndex::BlackRook;
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackRook - 2][Squares::H8] ^ TranspoTable::ZobristKeys[PieceIndex::BlackRook - 2][Squares::F8];
					pieceSquareScore -= PieceSquareTable[PieceIndex::BlackRook - 2][Squares::F8] - PieceSquareTable[PieceIndex::BlackRook - 2][Squares::H8];
				} else if (endSquare - startSquare == -2)
				{
					pieceBitboards[PieceIndex::BlackRook] ^= 0x900000000000000l;
					pieceBitboards[PieceIndex::BlackRookQueen] ^= 0x900000000000000l;
					pieceBitboards[PieceIndex::BlackPieces] ^= 0x900000000000000l;
					pieceBitboards[PieceIndex::AllPieces] ^= 0x900000000000000l;
					pieceTypes[Squares::A8] = PieceIndex::NoPiece;
					pieceTypes[Squares::D8] = PieceIndex::BlackRook;
					currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::BlackRook - 2][Squares::A8] ^ TranspoTable::ZobristKeys[PieceIndex::BlackRook - 2][Squares::D8];
					pieceSquareScore -= PieceSquareTable[PieceIndex::BlackRook - 2][Squares::D8] - PieceSquareTable[PieceIndex::BlackRook - 2][Squares::A8];
				}
				if (LongCastle(blackCastle) != 0)
				{
					currentPositionKey ^= TranspoTable::BlackLongCastleKey;
				}
				if (ShortCastle(blackCastle) != 0)
				{
					currentPositionKey ^= TranspoTable::BlackShortCastleKey;
				}
				blackCastle = NoCastling;
				repetitionFifty[currentDepth] = 0;
			}
			break;
		}

		pieceBitboards[PieceIndex::BlackPieces] ^= combBit;
		pieceBitboards[PieceIndex::AllPieces] ^= combBit;

		if (capturedType != PieceIndex::NoPiece)
		{
			switch (capturedType)
			{
			case PieceIndex::WhitePawn:
				pieceBitboards[PieceIndex::WhitePawn] ^= destBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				whitePawnScore -= PieceScore::PawnScore;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhitePawn - 2][endSquare];
				pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][endSquare];
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][endSquare];
				break;

			case PieceIndex::WhiteKnight:
				pieceBitboards[PieceIndex::WhiteKnight] ^= destBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				whitePieceScore -= PieceScore::KnightScore;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteKnight - 2][endSquare];
				pieceSquareScore -= PieceSquareTable[PieceIndex::WhiteKnight - 2][endSquare];
				break;

			case PieceIndex::WhiteBishop:
				pieceBitboards[PieceIndex::WhiteBishop] ^= destBit;
				pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				whitePieceScore -= PieceScore::BishopScore;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteBishop - 2][endSquare];
				pieceSquareScore -= PieceSquareTable[PieceIndex::WhiteBishop - 2][endSquare];
				break;

			case PieceIndex::WhiteRook:
				pieceBitboards[PieceIndex::WhiteRook] ^= destBit;
				pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				whitePieceScore -= PieceScore::RookScore;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteRook - 2][endSquare];
				pieceSquareScore -= PieceSquareTable[PieceIndex::WhiteRook - 2][endSquare];
				if (whiteCastle != NoCastling)
				{
					if (endSquare == Squares::A1 && LongCastle(whiteCastle) != 0)
					{
						currentPositionKey ^= TranspoTable::WhiteLongCastleKey;
						whiteCastle &= 1;
					} else if (endSquare == Squares::H1 && ShortCastle(whiteCastle) != 0)
					{
						currentPositionKey ^= TranspoTable::WhiteShortCastleKey;
						whiteCastle &= 2;
					}
				}
				break;

			case PieceIndex::WhiteQueen:
				pieceBitboards[PieceIndex::WhiteQueen] ^= destBit;
				pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
				pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				whitePieceScore -= PieceScore::QueenScore;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhiteQueen - 2][endSquare];
				pieceSquareScore -= PieceSquareTable[PieceIndex::WhiteQueen - 2][endSquare];
				break;

			case PieceIndex::EnPassant:
				Bitboard enPassantBit = destBit << 8;
				Square enPassantSquare = endSquare + 8;
				pieceBitboards[PieceIndex::WhitePawn] ^= enPassantBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= enPassantBit;
				pieceBitboards[PieceIndex::AllPieces] ^= enPassantBit;
				pieceTypes[enPassantSquare] = PieceIndex::NoPiece;
				whitePawnScore -= PieceScore::PawnScore;
				currentPositionKey ^= TranspoTable::ZobristKeys[PieceIndex::WhitePawn - 2][enPassantSquare];
				pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][enPassantSquare];
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][enPassantSquare];
				break;
			}
			repetitionFifty[currentDepth] = 0;
		}
	}

	//TODO: update history table
	//historyTable[startSquare][endSquare]++;

	whiteToMove = !whiteToMove;
	currentPositionKey ^= TranspoTable::ChangeSideKey;
	repetitionKeyList[movesSinceIrreversible + currentDepth] = currentPositionKey;
	//Chess.Utilities.ShowMove(move);
}

//}
//{ Undo Move
void UndoMove(const int& move)
{
	whiteToMove = !whiteToMove;

	Square startSquare = Start(move);
	Square endSquare = End(move);
	Piece capturedType = Captured(move);
	Piece promotedType = Promoted(move);
	Piece movedType = Moved(move);

	currentDepth--;
	enPassant = storeEnPassant[currentDepth];
	whiteCastle = storeWhiteCastle[currentDepth];
	blackCastle = storeBlackCastle[currentDepth];
	currentPositionKey = storeHashKeys[currentDepth];
	pieceSquareScore = storePieceSquareScore[currentDepth];

#ifdef DEBUG_MODE
	Debugger::searchLine[currentDepth] = NoMove;
#endif

	//General procedures applied to any kind of pieces/moves
	pieceTypes[startSquare] = movedType;
	pieceTypes[endSquare] = PieceIndex::NoPiece;
	Bitboard sourceBit = 1ull << startSquare;
	Bitboard destBit = 1ull << endSquare;
	Bitboard combBit = sourceBit ^ destBit;

	if (whiteToMove)
	{
		switch (movedType)
		{
		case PieceIndex::WhitePawn:
			if (promotedType != PieceIndex::NoPiece)
			{
				pieceBitboards[PieceIndex::WhitePawn] ^= sourceBit;
				whitePawnScore += PieceScore::PawnScore;

				switch (promotedType)
				{
				case PieceIndex::WhiteQueen:
					pieceBitboards[PieceIndex::WhiteQueen] ^= destBit;
					pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
					pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
					whitePieceScore -= PieceScore::QueenScore;
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][startSquare];
					break;
				case PieceIndex::WhiteKnight:
					pieceBitboards[PieceIndex::WhiteKnight] ^= destBit;
					whitePieceScore -= PieceScore::KnightScore;
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][startSquare];
					break;
				case PieceIndex::WhiteRook:
					pieceBitboards[PieceIndex::WhiteRook] ^= destBit;
					pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
					whitePieceScore -= PieceScore::RookScore;
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][startSquare];
					break;
				case PieceIndex::WhiteBishop:
					pieceBitboards[PieceIndex::WhiteBishop] ^= destBit;
					pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
					whitePieceScore -= PieceScore::BishopScore;
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][startSquare];
					break;
				}
			} else
			{
				pieceBitboards[PieceIndex::WhitePawn] ^= combBit;
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][startSquare] ^ PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][endSquare];
			}
			break;

		case PieceIndex::WhiteKnight:
			pieceBitboards[PieceIndex::WhiteKnight] ^= combBit;
			break;

		case PieceIndex::WhiteBishop:
			pieceBitboards[PieceIndex::WhiteBishop] ^= combBit;
			pieceBitboards[PieceIndex::WhiteBishopQueen] ^= combBit;
			break;

		case PieceIndex::WhiteRook:
			pieceBitboards[PieceIndex::WhiteRook] ^= combBit;
			pieceBitboards[PieceIndex::WhiteRookQueen] ^= combBit;
			break;

		case PieceIndex::WhiteQueen:
			pieceBitboards[PieceIndex::WhiteQueen] ^= combBit;
			pieceBitboards[PieceIndex::WhiteRookQueen] ^= combBit;
			pieceBitboards[PieceIndex::WhiteBishopQueen] ^= combBit;
			break;

		case PieceIndex::WhiteKing:
			pieceBitboards[PieceIndex::WhiteKing] ^= combBit;

			if (whiteCastle != NoCastling)
			{
				if (endSquare - startSquare == 2)
				{
					pieceBitboards[PieceIndex::WhiteRook] ^= 0xA0l;
					pieceBitboards[PieceIndex::WhiteRookQueen] ^= 0xA0l;
					pieceBitboards[PieceIndex::WhitePieces] ^= 0xA0l;
					pieceBitboards[PieceIndex::AllPieces] ^= 0xA0l;
					pieceTypes[Squares::F1] = PieceIndex::NoPiece;
					pieceTypes[Squares::H1] = PieceIndex::WhiteRook;
				} else if (endSquare - startSquare == -2)
				{
					pieceBitboards[PieceIndex::WhiteRook] ^= 0x9l;
					pieceBitboards[PieceIndex::WhiteRookQueen] ^= 0x9l;
					pieceBitboards[PieceIndex::WhitePieces] ^= 0x9l;
					pieceBitboards[PieceIndex::AllPieces] ^= 0x9l;
					pieceTypes[Squares::D1] = PieceIndex::NoPiece;
					pieceTypes[Squares::A1] = PieceIndex::WhiteRook;
				}
			}
			break;
		}

		pieceBitboards[PieceIndex::WhitePieces] ^= combBit;
		pieceBitboards[PieceIndex::AllPieces] ^= combBit;

		if (capturedType != PieceIndex::NoPiece)
		{
			switch (capturedType)
			{
			case PieceIndex::BlackPawn:
				pieceBitboards[PieceIndex::BlackPawn] ^= destBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				pieceTypes[endSquare] = PieceIndex::BlackPawn;
				blackPawnScore += PieceScore::PawnScore;
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][endSquare];
				break;

			case PieceIndex::BlackKnight:
				pieceBitboards[PieceIndex::BlackKnight] ^= destBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				pieceTypes[endSquare] = PieceIndex::BlackKnight;
				blackPieceScore += PieceScore::KnightScore;
				break;

			case PieceIndex::BlackBishop:
				pieceBitboards[PieceIndex::BlackBishop] ^= destBit;
				pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				pieceTypes[endSquare] = PieceIndex::BlackBishop;
				blackPieceScore += PieceScore::BishopScore;
				break;

			case PieceIndex::BlackRook:
				pieceBitboards[PieceIndex::BlackRook] ^= destBit;
				pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				pieceTypes[endSquare] = PieceIndex::BlackRook;
				blackPieceScore += PieceScore::RookScore;
				break;

			case PieceIndex::BlackQueen:
				pieceBitboards[PieceIndex::BlackQueen] ^= destBit;
				pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
				pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				pieceTypes[endSquare] = PieceIndex::BlackQueen;
				blackPieceScore += PieceScore::QueenScore;
				break;

			case PieceIndex::EnPassant:
				Bitboard enPassantBit = destBit >> 8;
				Square enPassantSquare = endSquare - 8;
				pieceBitboards[PieceIndex::BlackPawn] ^= enPassantBit;
				pieceBitboards[PieceIndex::BlackPieces] ^= enPassantBit;
				pieceBitboards[PieceIndex::AllPieces] ^= enPassantBit;
				pieceTypes[enPassantSquare] = PieceIndex::BlackPawn;
				blackPawnScore += PieceScore::PawnScore;
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][enPassantSquare];
				break;
			}
		}
	} else
	{
		switch (movedType)
		{

		case PieceIndex::BlackPawn:
			if (promotedType != PieceIndex::NoPiece)
			{
				pieceBitboards[PieceIndex::BlackPawn] ^= sourceBit;
				blackPawnScore += PieceScore::PawnScore;
				switch (promotedType)
				{
				case PieceIndex::BlackQueen:
					pieceBitboards[PieceIndex::BlackQueen] ^= destBit;
					pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
					pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
					blackPieceScore -= PieceScore::QueenScore;
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][startSquare];
					break;
				case PieceIndex::BlackKnight:
					pieceBitboards[PieceIndex::BlackKnight] ^= destBit;
					blackPieceScore -= PieceScore::KnightScore;
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][startSquare];
					break;
				case PieceIndex::BlackRook:
					pieceBitboards[PieceIndex::BlackRook] ^= destBit;
					pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
					blackPieceScore -= PieceScore::RookScore;
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][startSquare];
					break;
				case PieceIndex::BlackBishop:
					pieceBitboards[PieceIndex::BlackBishop] ^= destBit;
					pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
					blackPieceScore -= PieceScore::BishopScore;
					pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][startSquare];
					break;
				}
			} else
			{
				pieceBitboards[PieceIndex::BlackPawn] ^= combBit;
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][startSquare] ^ PawnTranspoTable::ZobristKeys[PawnTableBlackIndex][endSquare];
			}
			break;

		case PieceIndex::BlackKnight:
			pieceBitboards[PieceIndex::BlackKnight] ^= combBit;
			break;

		case PieceIndex::BlackBishop:
			pieceBitboards[PieceIndex::BlackBishop] ^= combBit;
			pieceBitboards[PieceIndex::BlackBishopQueen] ^= combBit;
			break;

		case PieceIndex::BlackRook:
			pieceBitboards[PieceIndex::BlackRook] ^= combBit;
			pieceBitboards[PieceIndex::BlackRookQueen] ^= combBit;
			break;

		case PieceIndex::BlackQueen:
			pieceBitboards[PieceIndex::BlackQueen] ^= combBit;
			pieceBitboards[PieceIndex::BlackRookQueen] ^= combBit;
			pieceBitboards[PieceIndex::BlackBishopQueen] ^= combBit;
			break;

		case PieceIndex::BlackKing:
			pieceBitboards[PieceIndex::BlackKing] ^= combBit;

			if (blackCastle != NoCastling)
			{
				if (endSquare - startSquare == 2)
				{
					pieceBitboards[PieceIndex::BlackRook] ^= 0xA000000000000000l;
					pieceBitboards[PieceIndex::BlackRookQueen] ^= 0xA000000000000000l;
					pieceBitboards[PieceIndex::BlackPieces] ^= 0xA000000000000000l;
					pieceBitboards[PieceIndex::AllPieces] ^= 0xA000000000000000l;
					pieceTypes[Squares::F8] = PieceIndex::NoPiece;
					pieceTypes[Squares::H8] = PieceIndex::BlackRook;
				} else if (endSquare - startSquare == -2)
				{
					pieceBitboards[PieceIndex::BlackRook] ^= 0x900000000000000l;
					pieceBitboards[PieceIndex::BlackRookQueen] ^= 0x900000000000000l;
					pieceBitboards[PieceIndex::BlackPieces] ^= 0x900000000000000l;
					pieceBitboards[PieceIndex::AllPieces] ^= 0x900000000000000l;
					pieceTypes[Squares::D8] = PieceIndex::NoPiece;
					pieceTypes[Squares::A8] = PieceIndex::BlackRook;
				}
			}
			break;
		}

		pieceBitboards[PieceIndex::BlackPieces] ^= combBit;
		pieceBitboards[PieceIndex::AllPieces] ^= combBit;

		if (capturedType != PieceIndex::NoPiece)
		{
			switch (capturedType)
			{
			case PieceIndex::WhitePawn:
				pieceBitboards[PieceIndex::WhitePawn] ^= destBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				pieceTypes[endSquare] = PieceIndex::WhitePawn;
				whitePawnScore += PieceScore::PawnScore;
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][endSquare];
				break;

			case PieceIndex::WhiteKnight:
				pieceBitboards[PieceIndex::WhiteKnight] ^= destBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				pieceTypes[endSquare] = PieceIndex::WhiteKnight;
				whitePieceScore += PieceScore::KnightScore;
				break;

			case PieceIndex::WhiteBishop:
				pieceBitboards[PieceIndex::WhiteBishop] ^= destBit;
				pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				pieceTypes[endSquare] = PieceIndex::WhiteBishop;
				whitePieceScore += PieceScore::BishopScore;
				break;

			case PieceIndex::WhiteRook:
				pieceBitboards[PieceIndex::WhiteRook] ^= destBit;
				pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				pieceTypes[endSquare] = PieceIndex::WhiteRook;
				whitePieceScore += PieceScore::RookScore;
				break;

			case PieceIndex::WhiteQueen:
				pieceBitboards[PieceIndex::WhiteQueen] ^= destBit;
				pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
				pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				pieceTypes[endSquare] = PieceIndex::WhiteQueen;
				whitePieceScore += PieceScore::QueenScore;
				break;

			case PieceIndex::EnPassant:
				Bitboard enPassantBit = destBit << 8;
				Square enPassantSquare = endSquare + 8;
				pieceBitboards[PieceIndex::WhitePawn] ^= enPassantBit;
				pieceBitboards[PieceIndex::WhitePieces] ^= enPassantBit;
				pieceBitboards[PieceIndex::AllPieces] ^= enPassantBit;
				pieceTypes[enPassantSquare] = PieceIndex::WhitePawn;
				whitePawnScore += PieceScore::PawnScore;
				pawnStructureKey ^= PawnTranspoTable::ZobristKeys[PawnTableWhiteIndex][enPassantSquare];
				break;
			}
		}
	}
}

void MakeNullMove()
{
	storeEnPassant[currentDepth] = enPassant;
	if (enPassant != NoEnPassant)
	{
		currentPositionKey ^= TranspoTable::EnPassantKeys[enPassant];
		enPassant = NoEnPassant;
	}
	currentDepth++;
	repetitionFifty[currentDepth] = 0;
	whiteToMove = !whiteToMove;
	currentPositionKey ^= TranspoTable::ChangeSideKey;
}

void UndoNullMove()
{
	whiteToMove = !whiteToMove;
	currentDepth--;
	enPassant = storeEnPassant[currentDepth];
	if (enPassant != NoEnPassant)
	{
		currentPositionKey ^= TranspoTable::EnPassantKeys[enPassant];
	}
	currentPositionKey ^= TranspoTable::ChangeSideKey;
}