#include "Include.h"

static inline Bitboard LowestBit(const Bitboard& board)
    {
        return board & -board;
    }

    static inline int TrailingZeros(Bitboard board)
    {
        board ^= (board - 1);
        int folded = (int)(board ^ (board >> 32));
        return BitScanTable[folded * 0x78291ACF >> 26];
    }

    static inline void ResetLowestBit(Bitboard& board)
    {
        board &= board - 1;
    }

    static inline int ShortCastle(const int& castle)
    {
        return castle & 0x1;
    }

    static inline int LongCastle(const int& castle)
    {
        return castle & 0x2;
    }

    static inline Square Start(const Move& move)
    {
        return move >> MoveBitShift::StartShift & 0x3F;
    }

    static inline Square End(const Move& move)
    {
        return move >> MoveBitShift::EndShift & 0x3F;
    }

    static inline Piece Captured(const Move& move)
    {
        return move >> MoveBitShift::CaptureShift & 0xF;
    }

    static inline Piece Promoted(const Move& move)
    {
        return move >> MoveBitShift::PromoteShift & 0xF;
    }

    static inline Piece Moved(const Move& move)
    {
        return move & 0xF;
    }

    static inline int Score (const TableNode* node)
    {
        return node->data >> HashBitShift::ScoreShift & 0xFFF;
    }

    static inline int Flag (const TableNode* node)
    {
        return node->data >> HashBitShift::FlagShift & 0x7;
    }

    static inline int Depth (const TableNode* node)
    {
        return node->data >> HashBitShift::DepthShift & 0xFF;
    }

    static inline int Sequence (const TableNode* node)
    {
        return node->data & 0x7;
    }

    static inline Bitboard RookMagicBitboardOccupancyMasked(const Square& square, const Bitboard& occupancy)
    {
        return *(MagicRookIndex[square] + ((occupancy * MagicRookHash[square]) >> MagicRookShift[square]));
    }

    static inline Bitboard BishopMagicBitboardOccupancyMasked(const Square& square, const Bitboard& occupancy)
    {
        return *(MagicBishopIndex[square] + ((occupancy * MagicBishopHash[square]) >> MagicBishopShift[square]));
    }

    static inline Bitboard RookMagicBitboard(const Square& square, const Bitboard& occupancy)
    {
        return *(MagicRookIndex[square] + (((occupancy & MagicRookMask[square]) * MagicRookHash[square]) >> MagicRookShift[square]));
    }

    static inline Bitboard BishopMagicBitboard(const Square& square, const Bitboard& occupancy)
    {
        return *(MagicBishopIndex[square] + (((occupancy & MagicBishopMask[square]) * MagicBishopHash[square]) >> MagicBishopShift[square]));
    }

    inline bool WhiteInCheck()
    {
        return BlackAttack(TrailingZeros(pieceBitboards[PieceIndex::WhiteKing]));
    }

    inline bool BlackInCheck()
    {
        return WhiteAttack(TrailingZeros(pieceBitboards[PieceIndex::BlackKing]));
    }

    inline bool WhiteInCheckNoKing()
    {
        return BlackAttackNoKing(TrailingZeros(pieceBitboards[PieceIndex::WhiteKing]));
    }

    inline bool BlackInCheckNoKing()
    {
        return WhiteAttackNoKing(TrailingZeros(pieceBitboards[PieceIndex::BlackKing]));
    }

    static inline bool IsXrayPiece(const Piece& piece)
    {
        return piece > 7 || piece < 4;
    }

    static inline bool IsPawn(const Piece& piece)
    {
        return piece < 4;
    }

     int File(int square) {
        return square & 7;
    }

     int Rank(int square) {
        return square >> 3;
    }