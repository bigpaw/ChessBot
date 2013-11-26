#include "Include.h"

Square CoordinateToSquare(const std::string& coordinate)
{
	int file = static_cast<int>(coordinate[0]) - 97;
	int row = static_cast<int>(coordinate[1]) - 49;
	return row * 8 + file;
}

void ShowBitboard(const Bitboard& board)
{
	string showString("");
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			if ((1ull << (i * 8 + j) & board) != 0) showString.append("1");
			else showString.append("0");
		}
		showString.append("\n");
	}
	cout << showString << endl;
}

string CoordinateMoveNotation(const Move& move)
{
	string promoteString = "";
	Piece promoted = Promoted(move);
	if (promoted == PieceIndex::WhiteQueen || promoted == PieceIndex::BlackQueen) promoteString = "q";
	else if (promoted == PieceIndex::WhiteKnight || promoted == PieceIndex::BlackKnight) promoteString = "n";
	else if (promoted == PieceIndex::WhiteRook || promoted == PieceIndex::BlackRook) promoteString = "r";
	else if (promoted == PieceIndex::WhiteBishop || promoted == PieceIndex::BlackBishop) promoteString = "b";

	return SquareToCoordinate(Start(move)) + SquareToCoordinate(End(move)) + promoteString;
}

string SquareToCoordinate(const Square& square)
{
	int file = square % 8;
	int row = (square / 8) + 1;
	return string(1, (char)(file + 97)) + (char)(row+48);
}

int MoveNotation(const std::string& moveStringNotation)
{
	int startSquare = CoordinateToSquare(moveStringNotation.substr(0, 2));
	int endSquare = CoordinateToSquare(moveStringNotation.substr(2, 4));
	int movedType = pieceTypes[startSquare];
	int capturedType = pieceTypes[endSquare];
	int promotedType = PieceIndex::NoPiece;
	if (moveStringNotation.length() > 4)
	{
		switch (moveStringNotation[4])
		{
		case 'q':
			promotedType = whiteToMove ? PieceIndex::WhiteQueen : PieceIndex::BlackQueen;
			break;
		case 'n':
			promotedType = whiteToMove ? PieceIndex::WhiteKnight : PieceIndex::BlackKnight;
			break;
		case 'r':
			promotedType = whiteToMove ? PieceIndex::WhiteRook : PieceIndex::BlackRook;
			break;
		case 'b':
			promotedType = whiteToMove ? PieceIndex::WhiteBishop : PieceIndex::BlackBishop;
			break;
		}
	}
	if (movedType == PieceIndex::WhitePawn || movedType == PieceIndex::BlackPawn)
	{
		if ((endSquare - startSquare) % 2 != 0 && capturedType == PieceIndex::NoPiece)
		{
			capturedType = PieceIndex::EnPassant;

		}
	}

	return startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift
		| capturedType << MoveBitShift::CaptureShift
		| promotedType << MoveBitShift::PromoteShift | movedType;
}

void ShowBoard()
    {
        cout << GetBoardString() << endl;;
    }

string GetBoardString() 
{
        char pieceLetters[] = 
        {
            'P', 'p', 'N', 'n', 'K', 'k', 'Q', 'q', 'R', 'r', 'B', 'b'
        };

		stringstream sb;
        string lineString = "";
        for (int i = 63; i >= 0; i--)
        {
            bool noPiece = true;
            for (int j = 0; j < 12; j++)
            {
                if (((pieceBitboards[j + 2] >> i) & 0x1) != 0)
                {
                    lineString = pieceLetters[j] + lineString;
                    noPiece = false;
                    break;
                }
            }
            if (noPiece)
            {
                lineString = "." + lineString;
            }
            if (i % 8 == 0)
            {
                sb << lineString << endl;
                lineString = "";
            }
        }

        return sb.str();
    }