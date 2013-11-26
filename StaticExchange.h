#ifndef STATICEXCHANGE_H_INCLUDED
#define STATICEXCHANGE_H_INCLUDED

 int StaticExchangeScore(Move move);
 int SearchHiddenPiece(StaticExchangeData data[], Bitboard bitboards[], bool white, Square square, Square prevPieceSquare, int numPieces, int currentIndex);
 void InsertStaticExchangeData(StaticExchangeData data[], StaticExchangeData newData, int currentIndex, int numPieces);
 int PopulateExchangeArray(StaticExchangeData data[], Bitboard bitboards[], bool white, Square square);

#endif