#include "Include.h"

HashKey* openingEnPassantKeys = NULL;
HashKey** openingZobristKeys = NULL;
HashKey* openingChangeSideKey = NULL;
HashKey* openingWhiteShortCastleKey = NULL;
HashKey* openingWhiteLongCastleKey = NULL;
HashKey* openingBlackShortCastleKey = NULL;
HashKey* openingBlackLongCastleKey = NULL;
BookReference* bookReferenceArray = NULL;
int* ReferenceArraySize = NULL;
bool bookVariablesDeleted = true;
int numOutOfOpeningBook = 0;
HashKey openingBookKey = 0;

void LoadOpeningBookReference()
{
	using namespace std;
	using namespace boost;

	ifstream bookRefFile(OpeningBookReference.c_str());
	vector<string> lineVector;

	string line;
	while (getline(bookRefFile, line)) lineVector.push_back(line);
	bookRefFile.close();

	vector<string>::iterator iter = lineVector.begin();

	openingZobristKeys = new HashKey*[12];
	for (int i = 0; i < 12; i++)
	{
		openingZobristKeys[i] = new HashKey[64];
		for (int j = 0; j < 64; j++)
		{
			openingZobristKeys[i][j] = lexical_cast<uint64_t>(*iter++);
		}
	}

	openingEnPassantKeys = new HashKey[64];
	for (int i = 0; i < 64; i++) openingEnPassantKeys[i] = lexical_cast<uint64_t>(*iter++);

	openingChangeSideKey = new HashKey(lexical_cast<uint64_t>(*iter++));
	openingWhiteShortCastleKey = new HashKey(lexical_cast<uint64_t>(*iter++));
	openingWhiteLongCastleKey = new HashKey(lexical_cast<uint64_t>(*iter++));
	openingBlackShortCastleKey = new HashKey(lexical_cast<uint64_t>(*iter++));
	openingBlackLongCastleKey = new HashKey(lexical_cast<uint64_t>(*iter++));

	ReferenceArraySize = new int(lineVector.size() - 12*64 - 64 - 6);
	bookReferenceArray = new BookReference[*ReferenceArraySize];

	for (int i = 0; i < *ReferenceArraySize; i++)
	{
		size_t spaceIndex = iter->find(" ");
		string key = iter->substr(0, spaceIndex);
		string offset = iter->substr(spaceIndex + 1, iter->length() - spaceIndex - 1);
		iter++;

		bookReferenceArray[i] = BookReference(lexical_cast<uint64_t>(key), lexical_cast<int>(offset));
	}

	bookVariablesDeleted = false;
}

Move SearchOpeningMove()
{
	using namespace std;
	using namespace boost;

	int result = SearchBookReference();
	if (result != -1)
	{
		BookReference entry = bookReferenceArray[result];
		ifstream reader(OpeningBookFile.c_str());
		reader.seekg(entry.offset, ios::beg);

		string dataString;
		getline(reader, dataString);

		reader.close();
		char_separator<char> sep(OpeningBookMoveSeparator.c_str());
		tokenizer<char_separator<char> > tokens(dataString, sep);
		tokenizer<char_separator<char> > ::iterator iter = tokens.begin();

		vector<OpeningMoveData> dataVector;

		for (; iter != tokens.end(); iter++)
		{
			char_separator<char> dataSep(OpeningBookDataSeparator.c_str());
			tokenizer<char_separator<char> > tokens(*iter, dataSep);
			tokenizer<char_separator<char> > ::iterator dataIter = tokens.begin();

			Move move = lexical_cast<Move>(*dataIter++);
			int resultScore = lexical_cast<int>(*dataIter++);
			int timesPlayed = lexical_cast<int>(*dataIter++);

			dataVector.push_back(OpeningMoveData(move, resultScore, timesPlayed));
		}

		//Random rand = new Random();
		//int index = rand.Next(moveDataArray.Length);

		return dataVector[0].move;
		//Todo: return random move
	}
	else
	{
		numOutOfOpeningBook++;
		if (numOutOfOpeningBook >= NumberOpeningBookTrials)
		{
			delete[] openingEnPassantKeys;
			for (int i=0; i<12; i++) delete[] openingZobristKeys[i];
			delete[] openingZobristKeys;
			delete openingChangeSideKey;
			delete openingWhiteShortCastleKey;
			delete openingWhiteLongCastleKey;
			delete openingBlackShortCastleKey;
			delete openingBlackLongCastleKey;
			delete[] bookReferenceArray;
			delete ReferenceArraySize;
			bookVariablesDeleted = true;
		}
		return NoMove;
	}
}

int SearchBookReference()
{
	int low = 0;
	int high = *ReferenceArraySize - 2;

	while (low <= high)
	{
		int mid = (low + high) >> 1;
		HashKey midVal = bookReferenceArray[mid].key;

		if (midVal < openingBookKey)
			low = mid + 1;
		else if (midVal > openingBookKey)
			high = mid - 1;
		else
			return mid; // key found
	}
	return -1;  // key not found.
}

void UpdateOpeningBookKey(const Move& move)
{
	//storeWhiteCastle[currentDepth] = whiteCastle;
	//storeBlackCastle[currentDepth] = blackCastle;
	//storeEnPassant[currentDepth] = enPassant;
	//storeHashKeys[currentDepth] = openingBookKey;
	//StorePieceSquareScore[currentDepth] = //pieceSquareScore;

	if (enPassant != NoEnPassant)
	{
		openingBookKey ^= openingEnPassantKeys[enPassant];
		//enPassant = NoEnPassant;
	}
	//currentDepth++;

	Square startSquare = Start(move);
	Square endSquare = End(move);
	Piece capturedType = Captured(move);
	Piece promotedType = Promoted(move);
	Piece movedType = Moved(move);

	//General procedures applied to any kind of pieces/moves
	//pieceTypes[startSquare] = PieceIndex::NoPiece;
	//pieceTypes[endSquare] = movedType;
	//Bitboard sourceBit = 1ul << startSquare;
	//Bitboard destBit = 1ul << endSquare;
	//Bitboard combBit = sourceBit ^ destBit;

	//specific pieces update
	if (whiteToMove)
	{
		switch (movedType)
		{
		case PieceIndex::WhitePawn:
			if (promotedType != PieceIndex::NoPiece)
			{
				//pieceBitboards[PieceIndex::WhitePawn] ^= sourceBit;
				openingBookKey ^= openingZobristKeys[PieceIndex::WhitePawn - 2][startSquare];

				switch (promotedType)
				{
				case PieceIndex::WhiteQueen:
					//pieceBitboards[PieceIndex::WhiteQueen] ^= destBit;
					//pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
					//pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
					//pieceTypes[endSquare] = PieceIndex::WhiteQueen;
					//materialScore += QueenValue - PawnValue;
					//pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][startSquare];
					//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteQueen - 2][endSquare];
					openingBookKey ^= openingZobristKeys[PieceIndex::WhiteQueen - 2][endSquare] ^ openingZobristKeys[PieceIndex::WhitePawn - 2][startSquare];
					break;
				case PieceIndex::WhiteKnight:
					//pieceBitboards[PieceIndex::WhiteKnight] ^= destBit;
					//pieceTypes[endSquare] = PieceIndex::WhiteKnight;
					//materialScore += KnightValue - PawnValue;
					//pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][startSquare];
					//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteKnight - 2][endSquare];
					openingBookKey ^= openingZobristKeys[PieceIndex::WhiteKnight - 2][endSquare] ^ openingZobristKeys[PieceIndex::WhitePawn - 2][startSquare];
					break;
				case PieceIndex::WhiteRook:
					//pieceBitboards[PieceIndex::WhiteRook] ^= destBit;
					//pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
					//pieceTypes[endSquare] = PieceIndex::WhiteRook;
					//materialScore += RookValue - PawnValue;
					//pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][startSquare];
					//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteRook - 2][endSquare];
					openingBookKey ^= openingZobristKeys[PieceIndex::WhiteRook - 2][endSquare] ^ openingZobristKeys[PieceIndex::WhitePawn - 2][startSquare];
					break;
				case PieceIndex::WhiteBishop:
					//pieceBitboards[PieceIndex::WhiteBishop] ^= destBit;
					//pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
					//pieceTypes[endSquare] = PieceIndex::WhiteBishop;
					//materialScore += BishopValue - PawnValue;
					//pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][startSquare];
					//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteBishop - 2][endSquare];
					openingBookKey ^= openingZobristKeys[PieceIndex::WhiteBishop - 2][endSquare] ^ openingZobristKeys[PieceIndex::WhitePawn - 2][startSquare];
					break;
				}
			}
			else
			{
				//pieceBitboards[PieceIndex::WhitePawn] ^= combBit;
				openingBookKey ^= openingZobristKeys[PieceIndex::WhitePawn - 2][startSquare] ^ openingZobristKeys[PieceIndex::WhitePawn - 2][endSquare];
				if ((endSquare - startSquare) == 16)
				{
					//enPassant = endSquare - 8;
					openingBookKey ^= openingEnPassantKeys[endSquare - 8];
					//pieceSquareScore += PieceSquareTable[PieceIndex::WhitePawn - 2][endSquare] -
					//PieceSquareTable[PieceIndex::WhitePawn - 2][startSquare];
				}
			}
			break;

		case PieceIndex::WhiteKnight:
			//pieceBitboards[PieceIndex::WhiteKnight] ^= combBit;
			openingBookKey ^= openingZobristKeys[PieceIndex::WhiteKnight - 2][startSquare] ^ openingZobristKeys[PieceIndex::WhiteKnight - 2][endSquare];
			//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteKnight - 2][endSquare] - PieceSquareTable[PieceIndex::WhiteKnight - 2][startSquare];
			break;

		case PieceIndex::WhiteBishop:
			//pieceBitboards[PieceIndex::WhiteBishop] ^= combBit;
			//pieceBitboards[PieceIndex::WhiteBishopQueen] ^= combBit;
			openingBookKey ^= openingZobristKeys[PieceIndex::WhiteBishop - 2][startSquare] ^ openingZobristKeys[PieceIndex::WhiteBishop - 2][endSquare];
			//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteBishop - 2][endSquare] - PieceSquareTable[PieceIndex::WhiteBishop - 2][startSquare];
			break;

		case PieceIndex::WhiteRook:
			//pieceBitboards[PieceIndex::WhiteRook] ^= combBit;
			//pieceBitboards[PieceIndex::WhiteRookQueen] ^= combBit;
			openingBookKey ^= openingZobristKeys[PieceIndex::WhiteRook - 2][startSquare] ^ openingZobristKeys[PieceIndex::WhiteRook - 2][endSquare];
			//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteRook - 2][endSquare] - PieceSquareTable[PieceIndex::WhiteRook - 2][startSquare];
			if (whiteCastle != NoCastling)
			{
				if (startSquare == Squares::A1 && LongCastle(whiteCastle) != 0)
				{
					openingBookKey ^= *openingWhiteLongCastleKey;
					//whiteCastle &= 1;
				}
				else if (startSquare == Squares::H1 && ShortCastle(whiteCastle) != 0)
				{
					openingBookKey ^= *openingWhiteShortCastleKey;
					//whiteCastle &= 2;
				}
			}
			break;

		case PieceIndex::WhiteQueen:
			//pieceBitboards[PieceIndex::WhiteQueen] ^= combBit;
			//pieceBitboards[PieceIndex::WhiteRookQueen] ^= combBit;
			//pieceBitboards[PieceIndex::WhiteBishopQueen] ^= combBit;
			openingBookKey ^= openingZobristKeys[PieceIndex::WhiteQueen - 2][startSquare] ^ openingZobristKeys[PieceIndex::WhiteQueen - 2][endSquare];
			//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteQueen - 2][endSquare] - PieceSquareTable[PieceIndex::WhiteQueen - 2][startSquare];
			break;

		case PieceIndex::WhiteKing:
			//pieceBitboards[PieceIndex::WhiteKing] ^= combBit;
			openingBookKey ^= openingZobristKeys[PieceIndex::WhiteKing - 2][startSquare] ^
				openingZobristKeys[PieceIndex::WhiteKing - 2][endSquare];
			//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteKing - 2][endSquare] - PieceSquareTable[PieceIndex::WhiteKing - 2][startSquare];

			if (whiteCastle != NoCastling)
			{
				if (endSquare - startSquare == 2)
				{
					//pieceBitboards[PieceIndex::WhiteRook] ^= 0xA0; //0xA0 == '10100000'
					//pieceBitboards[PieceIndex::WhiteRookQueen] ^= 0xA0;
					//pieceBitboards[PieceIndex::WhitePieces] ^= 0xA0;
					//pieceBitboards[PieceIndex::AllPieces] ^= 0xA0;
					//pieceTypes[Squares::H1] = PieceIndex::NoPiece;
					//pieceTypes[Squares::F1] = PieceIndex::WhiteRook;
					openingBookKey ^= openingZobristKeys[PieceIndex::WhiteRook - 2][Squares::H1] ^ openingZobristKeys[PieceIndex::WhiteRook - 2][Squares::F1];
					//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteRook - 2][Squares::F1] - PieceSquareTable[PieceIndex::WhiteRook - 2][Squares::H1];
				}
				else if (endSquare - startSquare == -2)
				{
					//pieceBitboards[PieceIndex::WhiteRook] ^= 0x9; //0x9 == '00001001'
					//pieceBitboards[PieceIndex::WhiteRookQueen] ^= 0x9;
					//pieceBitboards[PieceIndex::WhitePieces] ^= 0x9;
					//pieceBitboards[PieceIndex::AllPieces] ^= 0x9;
					//pieceTypes[Squares::A1] = PieceIndex::NoPiece;
					//pieceTypes[Squares::D1] = PieceIndex::WhiteRook;
					openingBookKey ^= openingZobristKeys[PieceIndex::WhiteRook - 2][Squares::A1] ^ openingZobristKeys[PieceIndex::WhiteRook - 2][Squares::D1];
					//pieceSquareScore += PieceSquareTable[PieceIndex::WhiteRook - 2][Squares::D1] - PieceSquareTable[PieceIndex::WhiteRook - 2][Squares::A1];
				}
				if (LongCastle(whiteCastle) != 0) openingBookKey ^= *openingWhiteLongCastleKey;
				if (ShortCastle(whiteCastle) != 0) openingBookKey ^= *openingWhiteShortCastleKey;
				//whiteCastle = NoCastling;
			}
			break;
		}

		//pieceBitboards[PieceIndex::WhitePieces] ^= combBit;
		//pieceBitboards[PieceIndex::AllPieces] ^= combBit;

		if (capturedType != PieceIndex::NoPiece)
		{
			switch (capturedType)
			{
			case PieceIndex::BlackPawn:
				//pieceBitboards[PieceIndex::BlackPawn] ^= destBit;
				//pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				//materialScore += PawnValue;
				//pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][endSquare];
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackPawn - 2][endSquare];
				break;

			case PieceIndex::BlackKnight:
				//pieceBitboards[PieceIndex::BlackKnight] ^= destBit;
				//pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				//materialScore += KnightValue;
				//pieceSquareScore += PieceSquareTable[PieceIndex::BlackKnight - 2][endSquare];
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackKnight - 2][endSquare];
				break;

			case PieceIndex::BlackBishop:
				//pieceBitboards[PieceIndex::BlackBishop] ^= destBit;
				//pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
				//pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				//materialScore += BishopValue;
				//pieceSquareScore += PieceSquareTable[PieceIndex::BlackBishop - 2][endSquare];
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackBishop - 2][endSquare];
				break;

			case PieceIndex::BlackRook:
				//pieceBitboards[PieceIndex::BlackRook] ^= destBit;
				//pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
				//pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				//materialScore += RookValue;
				//pieceSquareScore += PieceSquareTable[PieceIndex::BlackRook - 2][endSquare];
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackRook - 2][endSquare];
				if (blackCastle != NoCastling)
				{
					if (endSquare == Squares::A8 && LongCastle(blackCastle) != 0)
					{
						openingBookKey ^= *openingBlackLongCastleKey;
						//blackCastle &= 1;
					}
					else if (endSquare == Squares::H8 && ShortCastle(blackCastle) != 0)
					{
						openingBookKey ^= *openingBlackShortCastleKey;
						//blackCastle &= 2;
					}
				}
				break;

			case PieceIndex::BlackQueen:
				//pieceBitboards[PieceIndex::BlackQueen] ^= destBit;
				//pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
				//pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
				//pieceBitboards[PieceIndex::BlackPieces] ^= destBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				//materialScore += QueenValue;
				//pieceSquareScore += PieceSquareTable[PieceIndex::BlackQueen - 2][endSquare];
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackQueen - 2][endSquare];
				break;

			case PieceIndex::EnPassant:
				//Bitboard enPassantBit = destBit >> 8;
				Square enPassantSquare = endSquare - 8;
				//pieceBitboards[PieceIndex::BlackPawn] ^= enPassantBit;
				//pieceBitboards[PieceIndex::BlackPieces] ^= enPassantBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= enPassantBit;
				//pieceTypes[enPassantSquare] = PieceIndex::NoPiece;
				//materialScore += PawnValue;
				//pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][enPassantSquare];
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackPawn - 2][enPassantSquare];
				break;
			}
		}
	}
	else
	{
		switch (movedType)
		{

		case PieceIndex::BlackPawn:
			if (promotedType != PieceIndex::NoPiece)
			{
				//pieceBitboards[PieceIndex::BlackPawn] ^= sourceBit;
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackPawn - 2][startSquare];

				switch (promotedType)
				{
				case PieceIndex::BlackQueen:
					//pieceBitboards[PieceIndex::BlackQueen] ^= destBit;
					//pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
					//pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
					//pieceTypes[endSquare] = PieceIndex::BlackQueen;
					//materialScore -= QueenValue - PawnValue;
					//pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][startSquare];
					//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackQueen - 2][endSquare];
					openingBookKey ^= openingZobristKeys[PieceIndex::BlackQueen - 2][endSquare] ^ openingZobristKeys[PieceIndex::BlackPawn - 2][startSquare];
					break;
				case PieceIndex::BlackKnight:
					//pieceBitboards[PieceIndex::BlackKnight] ^= destBit;
					//pieceTypes[endSquare] = PieceIndex::BlackKnight;
					//materialScore -= KnightValue - PawnValue;
					//pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][startSquare];
					//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackKnight - 2][endSquare];
					openingBookKey ^= openingZobristKeys[PieceIndex::BlackKnight - 2][endSquare] ^ openingZobristKeys[PieceIndex::BlackPawn - 2][startSquare];
					break;
				case PieceIndex::BlackRook:
					//pieceBitboards[PieceIndex::BlackRook] ^= destBit;
					//pieceBitboards[PieceIndex::BlackRookQueen] ^= destBit;
					//pieceTypes[endSquare] = PieceIndex::BlackRook;
					//materialScore -= RookValue - PawnValue;
					//pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][startSquare];
					//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackRook - 2][endSquare];
					openingBookKey ^= openingZobristKeys[PieceIndex::BlackRook - 2][endSquare] ^ openingZobristKeys[PieceIndex::BlackPawn - 2][startSquare];
					break;
				case PieceIndex::BlackBishop:
					//pieceBitboards[PieceIndex::BlackBishop] ^= destBit;
					//pieceBitboards[PieceIndex::BlackBishopQueen] ^= destBit;
					//pieceTypes[endSquare] = PieceIndex::BlackBishop;
					//materialScore -= BishopValue - PawnValue;
					//pieceSquareScore += PieceSquareTable[PieceIndex::BlackPawn - 2][startSquare];
					//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackBishop - 2][endSquare];
					openingBookKey ^= openingZobristKeys[PieceIndex::BlackBishop - 2][endSquare] ^ openingZobristKeys[PieceIndex::BlackPawn - 2][startSquare];
					break;
				}
			}
			else
			{
				//pieceBitboards[PieceIndex::BlackPawn] ^= combBit;
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackPawn - 2][startSquare] ^ openingZobristKeys[PieceIndex::BlackPawn - 2][endSquare];
				if ((endSquare - startSquare) == -16)
				{
					//enPassant = endSquare + 8;
					openingBookKey ^= openingEnPassantKeys[endSquare + 8];
					//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackPawn - 2][endSquare] -
					//PieceSquareTable[PieceIndex::BlackPawn - 2][startSquare];
				}
			}
			break;

		case PieceIndex::BlackKnight:
			//pieceBitboards[PieceIndex::BlackKnight] ^= combBit;
			openingBookKey ^= openingZobristKeys[PieceIndex::BlackKnight - 2][startSquare] ^ openingZobristKeys[PieceIndex::BlackKnight - 2][endSquare];
			//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackKnight - 2][endSquare] - PieceSquareTable[PieceIndex::BlackKnight - 2][startSquare];
			break;

		case PieceIndex::BlackBishop:
			//pieceBitboards[PieceIndex::BlackBishop] ^= combBit;
			//pieceBitboards[PieceIndex::BlackBishopQueen] ^= combBit;
			openingBookKey ^= openingZobristKeys[PieceIndex::BlackBishop - 2][startSquare] ^ openingZobristKeys[PieceIndex::BlackBishop - 2][endSquare];
			//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackBishop - 2][endSquare] - PieceSquareTable[PieceIndex::BlackBishop - 2][startSquare];
			break;

		case PieceIndex::BlackRook:
			//pieceBitboards[PieceIndex::BlackRook] ^= combBit;
			//pieceBitboards[PieceIndex::BlackRookQueen] ^= combBit;
			openingBookKey ^= openingZobristKeys[PieceIndex::BlackRook - 2][startSquare] ^ openingZobristKeys[PieceIndex::BlackRook - 2][endSquare];
			//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackRook - 2][endSquare] - PieceSquareTable[PieceIndex::BlackRook - 2][startSquare];
			if (blackCastle != NoCastling)
			{
				if (startSquare == Squares::A8 && LongCastle(blackCastle) != 0)
				{
					openingBookKey ^= *openingBlackLongCastleKey;
					//blackCastle &= 1;
				}
				else if (startSquare == Squares::H8 && ShortCastle(blackCastle) != 0)
				{
					openingBookKey ^= *openingBlackShortCastleKey;
					//blackCastle &= 2;
				}
			}
			break;

		case PieceIndex::BlackQueen:
			//pieceBitboards[PieceIndex::BlackQueen] ^= combBit;
			//pieceBitboards[PieceIndex::BlackRookQueen] ^= combBit;
			//pieceBitboards[PieceIndex::BlackBishopQueen] ^= combBit;
			openingBookKey ^= openingZobristKeys[PieceIndex::BlackQueen - 2][startSquare] ^ openingZobristKeys[PieceIndex::BlackQueen - 2][endSquare];
			//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackQueen - 2][endSquare] - PieceSquareTable[PieceIndex::BlackQueen - 2][startSquare];
			break;

		case PieceIndex::BlackKing:
			//pieceBitboards[PieceIndex::BlackKing] ^= combBit;
			openingBookKey ^= openingZobristKeys[PieceIndex::BlackKing - 2][startSquare] ^
				openingZobristKeys[PieceIndex::BlackKing - 2][endSquare];
			//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackKing - 2][endSquare] - PieceSquareTable[PieceIndex::BlackKing - 2][startSquare];

			if (blackCastle != NoCastling)
			{
				if (endSquare - startSquare == 2)
				{
					//pieceBitboards[PieceIndex::BlackRook] ^= 0xA000000000000000;
					//pieceBitboards[PieceIndex::BlackRookQueen] ^= 0xA000000000000000;
					//pieceBitboards[PieceIndex::BlackPieces] ^= 0xA000000000000000;
					//pieceBitboards[PieceIndex::AllPieces] ^= 0xA000000000000000;
					//pieceTypes[Squares::H8] = PieceIndex::NoPiece;
					//pieceTypes[Squares::F8] = PieceIndex::BlackRook;
					openingBookKey ^= openingZobristKeys[PieceIndex::BlackRook - 2][Squares::H8] ^ openingZobristKeys[PieceIndex::BlackRook - 2][Squares::F8];
					//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackRook - 2][Squares::F8] - PieceSquareTable[PieceIndex::BlackRook - 2][Squares::H8];
				}
				else if (endSquare - startSquare == -2)
				{
					//pieceBitboards[PieceIndex::BlackRook] ^= 0x900000000000000;
					//pieceBitboards[PieceIndex::BlackRookQueen] ^= 0x900000000000000;
					//pieceBitboards[PieceIndex::BlackPieces] ^= 0x900000000000000;
					//pieceBitboards[PieceIndex::AllPieces] ^= 0x900000000000000;
					//pieceTypes[Squares::A8] = PieceIndex::NoPiece;
					//pieceTypes[Squares::D8] = PieceIndex::BlackRook;
					openingBookKey ^= openingZobristKeys[PieceIndex::BlackRook - 2][Squares::A8] ^ openingZobristKeys[PieceIndex::BlackRook - 2][Squares::D8];
					//pieceSquareScore -= PieceSquareTable[PieceIndex::BlackRook - 2][Squares::D8] - PieceSquareTable[PieceIndex::BlackRook - 2][Squares::A8];
				}
				if (LongCastle(blackCastle) != 0) openingBookKey ^= *openingBlackLongCastleKey;
				if (ShortCastle(blackCastle) != 0) openingBookKey ^= *openingBlackShortCastleKey;
				//blackCastle = NoCastling;
			}
			break;
		}

		//pieceBitboards[PieceIndex::BlackPieces] ^= combBit;
		//pieceBitboards[PieceIndex::AllPieces] ^= combBit;

		if (capturedType != PieceIndex::NoPiece)
		{
			switch (capturedType)
			{
			case PieceIndex::WhitePawn:
				//pieceBitboards[PieceIndex::WhitePawn] ^= destBit;
				//pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				//materialScore -= PawnValue;
				openingBookKey ^= openingZobristKeys[PieceIndex::WhitePawn - 2][endSquare];
				//pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][endSquare];

				break;

			case PieceIndex::WhiteKnight:
				//pieceBitboards[PieceIndex::WhiteKnight] ^= destBit;
				//pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				//materialScore -= KnightValue;
				openingBookKey ^= openingZobristKeys[PieceIndex::WhiteKnight - 2][endSquare];
				//pieceSquareScore -= PieceSquareTable[PieceIndex::WhiteKnight - 2][endSquare];
				break;

			case PieceIndex::WhiteBishop:
				//pieceBitboards[PieceIndex::WhiteBishop] ^= destBit;
				//pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
				//pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				//materialScore -= BishopValue;
				openingBookKey ^= openingZobristKeys[PieceIndex::WhiteBishop - 2][endSquare];
				//pieceSquareScore -= PieceSquareTable[PieceIndex::WhiteBishop - 2][endSquare];
				break;

			case PieceIndex::WhiteRook:
				//pieceBitboards[PieceIndex::WhiteRook] ^= destBit;
				//pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
				//pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				//materialScore -= RookValue;
				openingBookKey ^= openingZobristKeys[PieceIndex::WhiteRook - 2][endSquare];
				//pieceSquareScore -= PieceSquareTable[PieceIndex::WhiteRook - 2][endSquare];
				if (whiteCastle != NoCastling)
				{
					if (endSquare == Squares::A1 && LongCastle(whiteCastle) != 0)
					{
						openingBookKey ^= *openingWhiteLongCastleKey;
						//whiteCastle &= 1;
					}
					else if (endSquare == Squares::H1 && ShortCastle(whiteCastle) != 0)
					{
						openingBookKey ^= *openingWhiteShortCastleKey;
						// whiteCastle &= 2;
					}
				}
				break;

			case PieceIndex::WhiteQueen:
				//pieceBitboards[PieceIndex::WhiteQueen] ^= destBit;
				//pieceBitboards[PieceIndex::WhiteRookQueen] ^= destBit;
				//pieceBitboards[PieceIndex::WhiteBishopQueen] ^= destBit;
				//pieceBitboards[PieceIndex::WhitePieces] ^= destBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= destBit;
				//materialScore -= QueenValue;
				openingBookKey ^= openingZobristKeys[PieceIndex::WhiteQueen - 2][endSquare];
				//pieceSquareScore -= PieceSquareTable[PieceIndex::WhiteQueen - 2][endSquare];
				break;

			case PieceIndex::EnPassant:
				//Bitboard enPassantBit = destBit << 8;
				Square enPassantSquare = endSquare + 8;
				//pieceBitboards[PieceIndex::WhitePawn] ^= enPassantBit;
				//pieceBitboards[PieceIndex::WhitePieces] ^= enPassantBit;
				//pieceBitboards[PieceIndex::AllPieces] ^= enPassantBit;
				//pieceTypes[enPassantSquare] = PieceIndex::NoPiece;
				//materialScore -= PawnValue;
				openingBookKey ^= openingZobristKeys[PieceIndex::WhitePawn - 2][enPassantSquare];
				//pieceSquareScore -= PieceSquareTable[PieceIndex::WhitePawn - 2][enPassantSquare];
				break;
			}
		}
	}

	//update history table
	//historyTable[startSquare][endSquare]++;

	//whiteToMove = !whiteToMove;
	openingBookKey ^= *openingChangeSideKey;

	//Chess.Utilities.ShowMove(move);
}

void InitializeBookKey(string fenString)
{
	openingBookKey = 0;

	using namespace boost;

	char_separator<char> fieldSep(" ");
	char_separator<char> rankSep("/");
	tokenizer<char_separator<char> > fieldToken(fenString, fieldSep);
	tokenizer<char_separator<char> >::iterator fieldIter = fieldToken.begin();

	tokenizer<char_separator<char> > rankToken(*fieldIter, rankSep);
	tokenizer<char_separator<char> >::iterator rankIter = rankToken.begin();

	//{ Piece Positions

	int currentSquare = 56;
	for (; rankIter != rankToken.end(); rankIter++, currentSquare -= 16)
	{
		for (int i = 0; i < rankIter->size(); i++)
		{
			switch ((*rankIter)[i])
			{
			case 'P':
				openingBookKey ^= openingZobristKeys[PieceIndex::WhitePawn - 2][currentSquare];
				currentSquare++;
				break;
			case 'N':
				openingBookKey ^= openingZobristKeys[PieceIndex::WhiteKnight - 2][currentSquare];
				currentSquare++;
				break;
			case 'K':
				openingBookKey ^= openingZobristKeys[PieceIndex::WhiteKing - 2][currentSquare];
				currentSquare++;
				break;
			case 'B':
				openingBookKey ^= openingZobristKeys[PieceIndex::WhiteBishop - 2][currentSquare];
				currentSquare++;
				break;
			case 'R':
				openingBookKey ^= openingZobristKeys[PieceIndex::WhiteRook - 2][currentSquare];
				currentSquare++;
				break;
			case 'Q':
				openingBookKey ^= openingZobristKeys[PieceIndex::WhiteQueen - 2][currentSquare];
				currentSquare++;
				break;
			case 'p':
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackPawn - 2][currentSquare];
				currentSquare++;
				break;
			case 'n':
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackKnight - 2][currentSquare];
				currentSquare++;
				break;
			case 'k':
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackKing - 2][currentSquare];
				currentSquare++;
				break;
			case 'b':
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackBishop - 2][currentSquare];
				currentSquare++;
				break;
			case 'r':
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackRook - 2][currentSquare];
				currentSquare++;
				break;
			case 'q':
				openingBookKey ^= openingZobristKeys[PieceIndex::BlackQueen - 2][currentSquare];
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
	fieldIter++;
	//}

	//{ Side to Play

	if (*fieldIter == "b")
	{
		openingBookKey ^= *openingChangeSideKey;
	}
	fieldIter++;

	//{ Castling Rights

	for (int i = 0; i < fieldIter->size(); i++)
	{
		switch ((*fieldIter)[i])
		{
		case 'K':
			openingBookKey ^= *openingWhiteShortCastleKey;
			break;
		case 'Q':
			openingBookKey ^= *openingWhiteLongCastleKey;
			break;
		case 'k':
			openingBookKey ^= *openingBlackShortCastleKey;
			break;
		case 'q':
			openingBookKey ^= *openingBlackLongCastleKey;
			break;
		case '-':
			break;
		default:
			break;
			//throw new Exception("Invalid FEN Notation (Castling).");
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
			openingBookKey ^= openingEnPassantKeys[square];
		} 		 catch (...)
		{
			throw "Invalid FEN Notation (En passant).";
		}
	}

	//}
}