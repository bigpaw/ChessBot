#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

int CoordinateToSquare(const std::string& coordinate);
void ShowBitboard(const Bitboard& board);
string CoordinateMoveNotation(const Move& move);
string SquareToCoordinate(const Square& square);
int MoveNotation(const std::string& moveStringNotation);
void ShowBoard();
string GetBoardString();

#endif