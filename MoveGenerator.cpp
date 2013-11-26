#include "Include.h"

const int BitScanTable[64] = 
{
	63, 30, 3, 32, 59, 14, 11, 33,
	60, 24, 50, 9, 55, 19, 21, 34,
	61, 29, 2, 53, 51, 23, 41, 18,
	56, 28, 1, 43, 46, 27, 0, 35,
	62, 31, 58, 4, 5, 49, 54, 6,
	15, 52, 12, 40, 7, 42, 45, 16,
	25, 57, 48, 13, 10, 39, 8, 44,
	20, 47, 38, 22, 17, 37, 36, 26
};

const Bitboard AttackedByWhitePawnBitboard[64] =
{
	0x0,             0x0,             0x0,             0x0,              0x0,              0x0,              0x0,              0x0,
	0x0,             0x0,             0x0,             0x0,              0x0,              0x0,              0x0,              0x0,
	0x200,           0x500,           0xA00,           0x1400,           0x2800,           0x5000,           0xA000,           0x4000,
	0x20000,         0x50000,         0xA0000,         0x140000,         0x280000,         0x500000,         0xA00000,         0x400000,
	0x2000000,       0x5000000,       0xA000000,       0x14000000,       0x28000000,       0x50000000,       0xA0000000,       0x40000000,
	0x200000000,     0x500000000,     0xA00000000,     0x1400000000,     0x2800000000,     0x5000000000,     0xA000000000,     0x4000000000,
	0x20000000000,   0x50000000000,   0xA0000000000,   0x140000000000,   0x280000000000,   0x500000000000,   0xA00000000000,   0x400000000000,
	0x2000000000000, 0x5000000000000, 0xA000000000000, 0x14000000000000, 0x28000000000000, 0x50000000000000, 0xA0000000000000, 0x40000000000000
};

const Bitboard AttackedByBlackPawnBitboard[64] =
{
	0x200,            0x500,           0xA00,           0x1400,           0x2800,           0x5000,           0xA000,           0x4000,
	0x20000,          0x50000,         0xA0000,         0x140000,         0x280000,         0x500000,         0xA00000,         0x400000,
	0x2000000,        0x5000000,       0xA000000,       0x14000000,       0x28000000,       0x50000000,       0xA0000000,       0x40000000,
	0x200000000,      0x500000000,     0xA00000000,     0x1400000000,     0x2800000000,     0x5000000000,     0xA000000000,     0x4000000000,
	0x20000000000,    0x50000000000,   0xA0000000000,   0x140000000000,   0x280000000000,   0x500000000000,   0xA00000000000,   0x400000000000,
	0x2000000000000,  0x5000000000000, 0xA000000000000, 0x14000000000000, 0x28000000000000, 0x50000000000000, 0xA0000000000000, 0x40000000000000,
	0x0,              0x0,             0x0,             0x0,              0x0,              0x0,              0x0,              0x0,
	0x0,              0x0,             0x0,             0x0,              0x0,              0x0,              0x0,              0x0
};

const Bitboard KnightMoveBitboard[64] =
{
	0x20400,           0x50800,           0xA1100,            0x142200,           0x284400,           0x508800,           0xA01000,           0x402000,
	0x2040004,         0x5080008,         0xA110011,          0x14220022,         0x28440044,         0x50880088,         0xA0100010,         0x40200020,
	0x204000402,       0x508000805,       0xA1100110A,        0x1422002214,       0x2844004428,       0x5088008850,       0xA0100010A0,       0x4020002040,
	0x20400040200,     0x50800080500,     0xA1100110A00,      0x142200221400,     0x284400442800,     0x508800885000,     0xA0100010A000,     0x402000204000,
	0x2040004020000,   0x5080008050000,   0xA1100110A0000,    0x14220022140000,   0x28440044280000,   0x50880088500000,   0xA0100010A00000,   0x40200020400000,
	0x204000402000000, 0x508000805000000, 0xA1100110A000000,  0x1422002214000000, 0x2844004428000000, 0x5088008850000000, 0xA0100010A0000000, 0x4020002040000000,
	0x400040200000000, 0x800080500000000, 0x1100110A00000000, 0x2200221400000000, 0x4400442800000000, 0x8800885000000000, 0x100010A000000000, 0x2000204000000000,
	0x4020000000000,   0x8050000000000,   0x110A0000000000,   0x22140000000000,   0x44280000000000,   0x88500000000000,   0x10A00000000000,   0x20400000000000
};

const Bitboard KingMoveBitboard[64] =
{
	0x302,             0x705,             0xE0A,             0x1C14,             0x3828,             0x7050,             0xE0A0,             0xC040,
	0x30203,           0x70507,           0xE0A0E,           0x1C141C,           0x382838,           0x705070,           0xE0A0E0,           0xC040C0,
	0x3020300,         0x7050700,         0xE0A0E00,         0x1C141C00,         0x38283800,         0x70507000,         0xE0A0E000,         0xC040C000,
	0x302030000,       0x705070000,       0xE0A0E0000,       0x1C141C0000,       0x3828380000,       0x7050700000,       0xE0A0E00000,       0xC040C00000,
	0x30203000000,     0x70507000000,     0xE0A0E000000,     0x1C141C000000,     0x382838000000,     0x705070000000,     0xE0A0E0000000,     0xC040C0000000,
	0x3020300000000,   0x7050700000000,   0xE0A0E00000000,   0x1C141C00000000,   0x38283800000000,   0x70507000000000,   0xE0A0E000000000,   0xC040C000000000,
	0x302030000000000, 0x705070000000000, 0xE0A0E0000000000, 0x1C141C0000000000, 0x3828380000000000, 0x7050700000000000, 0xE0A0E00000000000, 0xC040C00000000000,
	0x203000000000000, 0x507000000000000, 0xA0E000000000000, 0x141C000000000000, 0x2838000000000000, 0x5070000000000000, 0xA0E0000000000000, 0x40C0000000000000
};

Bitboard MagicRookData[MagicRookDataLength] = {0};
Bitboard MagicBishopData[MagicBishopDataLength] = {0};

const int MagicRookShift[64] =
{
	52, 53, 53, 53, 53, 53, 53, 52,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 53, 53, 53, 53, 53
};

const unsigned __int64 MagicRookHash[64] =
{
	0x0080001020400080, 0x0040001000200040, 0x0080081000200080, 0x0080040800100080,
	0x0080020400080080, 0x0080010200040080, 0x0080008001000200, 0x0080002040800100,
	0x0000800020400080, 0x0000400020005000, 0x0000801000200080, 0x0000800800100080,
	0x0000800400080080, 0x0000800200040080, 0x0000800100020080, 0x0000800040800100,
	0x0000208000400080, 0x0000404000201000, 0x0000808010002000, 0x0000808008001000,
	0x0000808004000800, 0x0000808002000400, 0x0000010100020004, 0x0000020000408104,
	0x0000208080004000, 0x0000200040005000, 0x0000100080200080, 0x0000080080100080,
	0x0000040080080080, 0x0000020080040080, 0x0000010080800200, 0x0000800080004100,
	0x0000204000800080, 0x0000200040401000, 0x0000100080802000, 0x0000080080801000,
	0x0000040080800800, 0x0000020080800400, 0x0000020001010004, 0x0000800040800100,
	0x0000204000808000, 0x0000200040008080, 0x0000100020008080, 0x0000080010008080,
	0x0000040008008080, 0x0000020004008080, 0x0000010002008080, 0x0000004081020004,
	0x0000204000800080, 0x0000200040008080, 0x0000100020008080, 0x0000080010008080,
	0x0000040008008080, 0x0000020004008080, 0x0000800100020080, 0x0000800041000080,
	0x00FFFCDDFCED714A, 0x007FFCDDFCED714A, 0x003FFFCDFFD88096, 0x0000040810002101,
	0x0001000204080011, 0x0001000204000801, 0x0001000082000401, 0x0001FFFAABFAD1A2
};

const unsigned __int64 MagicRookMask[64] =
{
	0x000101010101017E, 0x000202020202027C, 0x000404040404047A, 0x0008080808080876,
	0x001010101010106E, 0x002020202020205E, 0x004040404040403E, 0x008080808080807E,
	0x0001010101017E00, 0x0002020202027C00, 0x0004040404047A00, 0x0008080808087600,
	0x0010101010106E00, 0x0020202020205E00, 0x0040404040403E00, 0x0080808080807E00,
	0x00010101017E0100, 0x00020202027C0200, 0x00040404047A0400, 0x0008080808760800,
	0x00101010106E1000, 0x00202020205E2000, 0x00404040403E4000, 0x00808080807E8000,
	0x000101017E010100, 0x000202027C020200, 0x000404047A040400, 0x0008080876080800,
	0x001010106E101000, 0x002020205E202000, 0x004040403E404000, 0x008080807E808000,
	0x0001017E01010100, 0x0002027C02020200, 0x0004047A04040400, 0x0008087608080800,
	0x0010106E10101000, 0x0020205E20202000, 0x0040403E40404000, 0x0080807E80808000,
	0x00017E0101010100, 0x00027C0202020200, 0x00047A0404040400, 0x0008760808080800,
	0x00106E1010101000, 0x00205E2020202000, 0x00403E4040404000, 0x00807E8080808000,
	0x007E010101010100, 0x007C020202020200, 0x007A040404040400, 0x0076080808080800,
	0x006E101010101000, 0x005E202020202000, 0x003E404040404000, 0x007E808080808000,
	0x7E01010101010100, 0x7C02020202020200, 0x7A04040404040400, 0x7608080808080800,
	0x6E10101010101000, 0x5E20202020202000, 0x3E40404040404000, 0x7E80808080808000
};

const Bitboard* MagicRookIndex[64] =
{
	MagicRookData + 86016, MagicRookData + 73728, MagicRookData + 36864, MagicRookData + 43008,
	MagicRookData + 47104, MagicRookData + 51200, MagicRookData + 77824, MagicRookData + 94208,
	MagicRookData + 69632, MagicRookData + 32768, MagicRookData + 38912, MagicRookData + 10240,
	MagicRookData + 14336, MagicRookData + 53248, MagicRookData + 57344, MagicRookData + 81920,
	MagicRookData + 24576, MagicRookData + 33792, MagicRookData + 6144,  MagicRookData + 11264,
	MagicRookData + 15360, MagicRookData + 18432, MagicRookData + 58368, MagicRookData + 61440,
	MagicRookData + 26624, MagicRookData + 4096,  MagicRookData + 7168,  MagicRookData + 0,
	MagicRookData + 2048,  MagicRookData + 19456, MagicRookData + 22528, MagicRookData + 63488,
	MagicRookData + 28672, MagicRookData + 5120,  MagicRookData + 8192,  MagicRookData + 1024,
	MagicRookData + 3072,  MagicRookData + 20480, MagicRookData + 23552, MagicRookData + 65536,
	MagicRookData + 30720, MagicRookData + 34816, MagicRookData + 9216,  MagicRookData + 12288,
	MagicRookData + 16384, MagicRookData + 21504, MagicRookData + 59392, MagicRookData + 67584,
	MagicRookData + 71680, MagicRookData + 35840, MagicRookData + 39936, MagicRookData + 13312,
	MagicRookData + 17408, MagicRookData + 54272, MagicRookData + 60416, MagicRookData + 83968,
	MagicRookData + 90112, MagicRookData + 75776, MagicRookData + 40960, MagicRookData + 45056,
	MagicRookData + 49152, MagicRookData + 55296, MagicRookData + 79872, MagicRookData + 98304
};

const int MagicBishopShift[64] =
{
	58, 59, 59, 59, 59, 59, 59, 58,
	59, 59, 59, 59, 59, 59, 59, 59,
	59, 59, 57, 57, 57, 57, 59, 59,
	59, 59, 57, 55, 55, 57, 59, 59,
	59, 59, 57, 55, 55, 57, 59, 59,
	59, 59, 57, 57, 57, 57, 59, 59,
	59, 59, 59, 59, 59, 59, 59, 59,
	58, 59, 59, 59, 59, 59, 59, 58
};

const unsigned __int64 MagicBishopHash[64] =
{
	0x0002020202020200, 0x0002020202020000, 0x0004010202000000, 0x0004040080000000,
	0x0001104000000000, 0x0000821040000000, 0x0000410410400000, 0x0000104104104000,
	0x0000040404040400, 0x0000020202020200, 0x0000040102020000, 0x0000040400800000,
	0x0000011040000000, 0x0000008210400000, 0x0000004104104000, 0x0000002082082000,
	0x0004000808080800, 0x0002000404040400, 0x0001000202020200, 0x0000800802004000,
	0x0000800400A00000, 0x0000200100884000, 0x0000400082082000, 0x0000200041041000,
	0x0002080010101000, 0x0001040008080800, 0x0000208004010400, 0x0000404004010200,
	0x0000840000802000, 0x0000404002011000, 0x0000808001041000, 0x0000404000820800,
	0x0001041000202000, 0x0000820800101000, 0x0000104400080800, 0x0000020080080080,
	0x0000404040040100, 0x0000808100020100, 0x0001010100020800, 0x0000808080010400,
	0x0000820820004000, 0x0000410410002000, 0x0000082088001000, 0x0000002011000800,
	0x0000080100400400, 0x0001010101000200, 0x0002020202000400, 0x0001010101000200,
	0x0000410410400000, 0x0000208208200000, 0x0000002084100000, 0x0000000020880000,
	0x0000001002020000, 0x0000040408020000, 0x0004040404040000, 0x0002020202020000,
	0x0000104104104000, 0x0000002082082000, 0x0000000020841000, 0x0000000000208800,
	0x0000000010020200, 0x0000000404080200, 0x0000040404040400, 0x0002020202020200
};

const unsigned __int64 MagicBishopMask[64] =
{
	0x0040201008040200, 0x0000402010080400, 0x0000004020100A00, 0x0000000040221400,
	0x0000000002442800, 0x0000000204085000, 0x0000020408102000, 0x0002040810204000,
	0x0020100804020000, 0x0040201008040000, 0x00004020100A0000, 0x0000004022140000,
	0x0000000244280000, 0x0000020408500000, 0x0002040810200000, 0x0004081020400000,
	0x0010080402000200, 0x0020100804000400, 0x004020100A000A00, 0x0000402214001400,
	0x0000024428002800, 0x0002040850005000, 0x0004081020002000, 0x0008102040004000,
	0x0008040200020400, 0x0010080400040800, 0x0020100A000A1000, 0x0040221400142200,
	0x0002442800284400, 0x0004085000500800, 0x0008102000201000, 0x0010204000402000,
	0x0004020002040800, 0x0008040004081000, 0x00100A000A102000, 0x0022140014224000,
	0x0044280028440200, 0x0008500050080400, 0x0010200020100800, 0x0020400040201000,
	0x0002000204081000, 0x0004000408102000, 0x000A000A10204000, 0x0014001422400000,
	0x0028002844020000, 0x0050005008040200, 0x0020002010080400, 0x0040004020100800,
	0x0000020408102000, 0x0000040810204000, 0x00000A1020400000, 0x0000142240000000,
	0x0000284402000000, 0x0000500804020000, 0x0000201008040200, 0x0000402010080400,
	0x0002040810204000, 0x0004081020400000, 0x000A102040000000, 0x0014224000000000,
	0x0028440200000000, 0x0050080402000000, 0x0020100804020000, 0x0040201008040200
};

const Bitboard* MagicBishopIndex[64] =
{
	MagicBishopData + 4992, MagicBishopData + 2624, MagicBishopData + 256,  MagicBishopData + 896,
	MagicBishopData + 1280, MagicBishopData + 1664, MagicBishopData + 4800, MagicBishopData + 5120,
	MagicBishopData + 2560, MagicBishopData + 2656, MagicBishopData + 288,  MagicBishopData + 928,
	MagicBishopData + 1312, MagicBishopData + 1696, MagicBishopData + 4832, MagicBishopData + 4928,
	MagicBishopData + 0,    MagicBishopData + 128,  MagicBishopData + 320,  MagicBishopData + 960,
	MagicBishopData + 1344, MagicBishopData + 1728, MagicBishopData + 2304, MagicBishopData + 2432,
	MagicBishopData + 32,   MagicBishopData + 160,  MagicBishopData + 448,  MagicBishopData + 2752,
	MagicBishopData + 3776, MagicBishopData + 1856, MagicBishopData + 2336, MagicBishopData + 2464,
	MagicBishopData + 64,   MagicBishopData + 192,  MagicBishopData + 576,  MagicBishopData + 3264,
	MagicBishopData + 4288, MagicBishopData + 1984, MagicBishopData + 2368, MagicBishopData + 2496,
	MagicBishopData + 96,   MagicBishopData + 224,  MagicBishopData + 704,  MagicBishopData + 1088,
	MagicBishopData + 1472, MagicBishopData + 2112, MagicBishopData + 2400, MagicBishopData + 2528,
	MagicBishopData + 2592, MagicBishopData + 2688, MagicBishopData + 832,  MagicBishopData + 1216,
	MagicBishopData + 1600, MagicBishopData + 2240, MagicBishopData + 4864, MagicBishopData + 4960,
	MagicBishopData + 5056, MagicBishopData + 2720, MagicBishopData + 864,  MagicBishopData + 1248,
	MagicBishopData + 1632, MagicBishopData + 2272, MagicBishopData + 4896, MagicBishopData + 5184
};

void InitializeMagicBitboards()
{
    int magicBitboardPosition[64] =
    {
        63, 0, 58, 1, 59, 47, 53, 2,
        60, 39, 48, 27, 54, 33, 42, 3,
        61, 51, 37, 40, 49, 18, 28, 20,
        55, 30, 34, 11, 43, 14, 22, 4,
        62, 57, 46, 52, 38, 26, 32, 41,
        50, 36, 17, 19, 29, 10, 13, 21,
        56, 45, 25, 31, 35, 16, 9, 12,
        44, 24, 15, 8, 23, 7, 6, 5
    };

    Bitboard * magicRookIndex[64] =
    {
        MagicRookData + 86016, MagicRookData + 73728, MagicRookData + 36864, MagicRookData + 43008,
        MagicRookData + 47104, MagicRookData + 51200, MagicRookData + 77824, MagicRookData + 94208,
        MagicRookData + 69632, MagicRookData + 32768, MagicRookData + 38912, MagicRookData + 10240,
        MagicRookData + 14336, MagicRookData + 53248, MagicRookData + 57344, MagicRookData + 81920,
        MagicRookData + 24576, MagicRookData + 33792, MagicRookData + 6144,  MagicRookData + 11264,
        MagicRookData + 15360, MagicRookData + 18432, MagicRookData + 58368, MagicRookData + 61440,
        MagicRookData + 26624, MagicRookData + 4096,  MagicRookData + 7168,  MagicRookData + 0,
        MagicRookData + 2048,  MagicRookData + 19456, MagicRookData + 22528, MagicRookData + 63488,
        MagicRookData + 28672, MagicRookData + 5120,  MagicRookData + 8192,  MagicRookData + 1024,
        MagicRookData + 3072,  MagicRookData + 20480, MagicRookData + 23552, MagicRookData + 65536,
        MagicRookData + 30720, MagicRookData + 34816, MagicRookData + 9216,  MagicRookData + 12288,
        MagicRookData + 16384, MagicRookData + 21504, MagicRookData + 59392, MagicRookData + 67584,
        MagicRookData + 71680, MagicRookData + 35840, MagicRookData + 39936, MagicRookData + 13312,
        MagicRookData + 17408, MagicRookData + 54272, MagicRookData + 60416, MagicRookData + 83968,
        MagicRookData + 90112, MagicRookData + 75776, MagicRookData + 40960, MagicRookData + 45056,
        MagicRookData + 49152, MagicRookData + 55296, MagicRookData + 79872, MagicRookData + 98304
    };

    Bitboard * magicBishopIndex[64] =
    {
        MagicBishopData + 4992, MagicBishopData + 2624, MagicBishopData + 256,  MagicBishopData + 896,
        MagicBishopData + 1280, MagicBishopData + 1664, MagicBishopData + 4800, MagicBishopData + 5120,
        MagicBishopData + 2560, MagicBishopData + 2656, MagicBishopData + 288,  MagicBishopData + 928,
        MagicBishopData + 1312, MagicBishopData + 1696, MagicBishopData + 4832, MagicBishopData + 4928,
        MagicBishopData + 0,    MagicBishopData + 128,  MagicBishopData + 320,  MagicBishopData + 960,
        MagicBishopData + 1344, MagicBishopData + 1728, MagicBishopData + 2304, MagicBishopData + 2432,
        MagicBishopData + 32,   MagicBishopData + 160,  MagicBishopData + 448,  MagicBishopData + 2752,
        MagicBishopData + 3776, MagicBishopData + 1856, MagicBishopData + 2336, MagicBishopData + 2464,
        MagicBishopData + 64,   MagicBishopData + 192,  MagicBishopData + 576,  MagicBishopData + 3264,
        MagicBishopData + 4288, MagicBishopData + 1984, MagicBishopData + 2368, MagicBishopData + 2496,
        MagicBishopData + 96,   MagicBishopData + 224,  MagicBishopData + 704,  MagicBishopData + 1088,
        MagicBishopData + 1472, MagicBishopData + 2112, MagicBishopData + 2400, MagicBishopData + 2528,
        MagicBishopData + 2592, MagicBishopData + 2688, MagicBishopData + 832,  MagicBishopData + 1216,
        MagicBishopData + 1600, MagicBishopData + 2240, MagicBishopData + 4864, MagicBishopData + 4960,
        MagicBishopData + 5056, MagicBishopData + 2720, MagicBishopData + 864,  MagicBishopData + 1248,
        MagicBishopData + 1632, MagicBishopData + 2272, MagicBishopData + 4896, MagicBishopData + 5184
    };

    for (int i = 0; i < 64; i++)
    {
        int squares[64];
        int numsquares = 0;
        Bitboard temp = MagicBishopMask[i];
        while (temp != 0)
        {
            Bitboard bit = LowestBit(temp);
            squares[numsquares++] = magicBitboardPosition[(bit * MagicNumber) >> 58];
            temp ^= bit;
        }
        for (temp = 0; temp < (1ull << numsquares); temp++)
        {
            Bitboard tempocc = InitializeMagicOccupancy(squares, numsquares, temp);
            *(magicBishopIndex[i] + ((tempocc * MagicBishopHash[i]) >> MagicBishopShift[i])) = InitializeBishopMagic(i, tempocc);
        }
    }

    for (int i = 0; i < 64; i++)
    {
        int squares[64];
        int numsquares = 0;
        Bitboard temp = MagicRookMask[i];
        while (temp != 0)
        {
            Bitboard bit = LowestBit(temp);
            squares[numsquares++] = magicBitboardPosition[(bit * MagicNumber) >> 58];
            temp ^= bit;
        }
        for (temp = 0; temp < (1ull << numsquares); temp++)
        {
            Bitboard tempocc = InitializeMagicOccupancy(squares, numsquares, temp);
            *(magicRookIndex[i] + ((tempocc * MagicRookHash[i]) >> MagicRookShift[i])) = InitializeRookMagic(i, tempocc);
        }
    }
}

Bitboard InitializeRookMagic(const Square& square, const Bitboard& occupancy)
{
    Bitboard ret = 0;
    Bitboard bit;
    Bitboard rowbits = 0xFFull << (8 * (square / 8));

    bit = 1ull << square;
    do
    {
        bit <<= 8;
        ret |= bit;
    }
    while (bit != 0 && (bit & occupancy) == 0);
    bit = 1ull << square;
    do
    {
        bit >>= 8;
        ret |= bit;
    }
    while (bit != 0 && (bit & occupancy) == 0);
    bit = 1ull << square;
    do
    {
        bit <<= 1;
        if ((bit & rowbits) != 0) ret |= bit;
        else break;
    }
    while ((bit & occupancy) == 0);
    bit = 1ull << square;
    do
    {
        bit >>= 1;
        if ((bit & rowbits) != 0) ret |= bit;
        else break;
    }
    while ((bit & occupancy) == 0);
    return ret;
}

Bitboard InitializeBishopMagic(const Square& square, const Bitboard& occupancy)
{
    Bitboard ret = 0;
    Bitboard bit;
    Bitboard bit2;
    Bitboard rowbits = 0xFFull << (8 * (square / 8));

    bit = 1ull << square;
    bit2 = bit;
    do
    {
        bit <<= 8 - 1;
        bit2 >>= 1;
        if ((bit2 & rowbits) != 0) ret |= bit;
        else break;
    }
    while (bit != 0 && (bit & occupancy) == 0);
    bit = 1ull << square;
    bit2 = bit;
    do
    {
        bit <<= 8 + 1;
        bit2 <<= 1;
        if ((bit2 & rowbits) != 0) ret |= bit;
        else break;
    }
    while (bit != 0 && (bit & occupancy) == 0);
    bit = 1ull << square;
    bit2 = bit;
    do
    {
        bit >>= 8 - 1;
        bit2 <<= 1;
        if ((bit2 & rowbits) != 0) ret |= bit;
        else break;
    }
    while (bit != 0 && (bit & occupancy) == 0);
    bit = 1ull << square;
    bit2 = bit;
    do
    {
        bit >>= 8 + 1;
        bit2 >>= 1;
        if ((bit2 & rowbits) != 0) ret |= bit;
        else break;
    }
    while (bit != 0 && (bit & occupancy) == 0);
    return ret;
}

Bitboard InitializeMagicOccupancy(const int* squares, const int& numSquares, const Bitboard& linocc)
{
    int i;
    Bitboard ret = 0;
    for (i = 0; i < numSquares; i++)
    {
        if ((linocc & 1ull << i) != 0) ret |= 1ull << squares[i];
    }
    return ret;
}

//}

//{ In Check / Square Attack

bool WhiteAttack(const Square& square)
{
	if ((pieceBitboards[PieceIndex::WhiteKnight] & KnightMoveBitboard[square]) != 0) return true;
	if ((pieceBitboards[PieceIndex::WhitePawn] & AttackedByWhitePawnBitboard[square]) != 0) return true;
	if ((pieceBitboards[PieceIndex::WhiteRookQueen] & RookMagicBitboard(square, pieceBitboards[PieceIndex::AllPieces])) != 0) return true;
	if ((pieceBitboards[PieceIndex::WhiteBishopQueen] & BishopMagicBitboard(square, pieceBitboards[PieceIndex::AllPieces])) != 0) return true;
	if ((pieceBitboards[PieceIndex::WhiteKing] & KingMoveBitboard[square]) != 0) return true;
	return false;
}

bool BlackAttack(const Square& square)
{
	if ((pieceBitboards[PieceIndex::BlackKnight] & KnightMoveBitboard[square]) != 0) return true;
	if ((pieceBitboards[PieceIndex::BlackPawn] & AttackedByBlackPawnBitboard[square]) != 0) return true;
	if ((pieceBitboards[PieceIndex::BlackRookQueen] & RookMagicBitboard(square, pieceBitboards[PieceIndex::AllPieces])) != 0) return true;
	if ((pieceBitboards[PieceIndex::BlackBishopQueen] & BishopMagicBitboard(square, pieceBitboards[PieceIndex::AllPieces])) != 0) return true;
	if ((pieceBitboards[PieceIndex::BlackKing] & KingMoveBitboard[square]) != 0) return true;
	return false;
}

bool WhiteAttackNoKing(const Square& square)
{
	if ((pieceBitboards[PieceIndex::WhiteKnight] & KnightMoveBitboard[square]) != 0) return true;
	if ((pieceBitboards[PieceIndex::WhitePawn] & AttackedByWhitePawnBitboard[square]) != 0) return true;
	if ((pieceBitboards[PieceIndex::WhiteRookQueen] & RookMagicBitboard(square, pieceBitboards[PieceIndex::AllPieces])) != 0) return true;
	if ((pieceBitboards[PieceIndex::WhiteBishopQueen] & BishopMagicBitboard(square, pieceBitboards[PieceIndex::AllPieces])) != 0) return true;
	return false;
}

bool BlackAttackNoKing(const Square& square)
{
	if ((pieceBitboards[PieceIndex::BlackKnight] & KnightMoveBitboard[square]) != 0) return true;
	if ((pieceBitboards[PieceIndex::BlackPawn] & AttackedByBlackPawnBitboard[square]) != 0) return true;
	if ((pieceBitboards[PieceIndex::BlackRookQueen] & RookMagicBitboard(square, pieceBitboards[PieceIndex::AllPieces])) != 0) return true;
	if ((pieceBitboards[PieceIndex::BlackBishopQueen] & BishopMagicBitboard(square, pieceBitboards[PieceIndex::AllPieces])) != 0) return true;
	return false;
}

void GenerateCaptures(int moveList[], int& moveIndex)
{
	Bitboard pieces;
	Bitboard targets;
	int startSquare;
	int endSquare;
	int promoteCommon;

	if (whiteToMove)
	{

		/*******************************************************
		*	       WHITE PAWN CAPTURES & PROMOTIONS			   *
		*******************************************************/
		//promotions (without capture)
		targets = pieceBitboards[PieceIndex::WhitePawn] << 8 & RankBitboard::RankEight & ~pieceBitboards[PieceIndex::AllPieces];
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			promoteCommon = (endSquare - 8) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhitePawn;
			moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteQueen << MoveBitShift::PromoteShift;
			moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteKnight << MoveBitShift::PromoteShift;
			moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteRook << MoveBitShift::PromoteShift;
			moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteBishop << MoveBitShift::PromoteShift;
		}

		//Captures towards north-east
		targets = pieceBitboards[PieceIndex::WhitePawn] << 9 & FileBitboard::FileOne & pieceBitboards[PieceIndex::BlackPieces];
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			if (endSquare > Squares::H7)
			{
				promoteCommon = (endSquare - 9) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::WhitePawn;
				moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteQueen << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteKnight << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteRook << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteBishop << MoveBitShift::PromoteShift;
			} else
			{
				moveList[moveIndex++] = (endSquare - 9) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::WhitePawn;
			}
		}

		//Captures towards north-west
		targets = pieceBitboards[PieceIndex::WhitePawn] << 7 & FileBitboard::FileEight & pieceBitboards[PieceIndex::BlackPieces];
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			if (endSquare > Squares::H7)
			{
				promoteCommon = (endSquare - 7) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::WhitePawn;
				moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteQueen << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteKnight << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteRook << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::WhiteBishop << MoveBitShift::PromoteShift;
			} else
			{
				moveList[moveIndex++] = (endSquare - 7) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::WhitePawn;
			}
		}

		//En passant captures
		if (enPassant != NoEnPassant)
		{
			pieces = pieceBitboards[PieceIndex::WhitePawn] & ((1ull << (enPassant - 7) & FileBitboard::FileOne) | (1ull << (enPassant - 9) & FileBitboard::FileEight));
			while (pieces != 0)
			{
				startSquare = TrailingZeros(pieces);
				ResetLowestBit(pieces);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | enPassant << MoveBitShift::EndShift | PieceIndex::EnPassant << MoveBitShift::CaptureShift | PieceIndex::WhitePawn;
			}
		}

		/*******************************************************
		*					    		WHITE KNIGHT CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::WhiteKnight];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = KnightMoveBitboard[startSquare] & pieceBitboards[PieceIndex::BlackPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::WhiteKnight;
			}
		}

		/*******************************************************
		*							    WHITE KING CAPTURES								*
		*******************************************************/
		startSquare = TrailingZeros(pieceBitboards[PieceIndex::WhiteKing]);
		targets = KingMoveBitboard[startSquare] & pieceBitboards[PieceIndex::BlackPieces];

		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::WhiteKing;
		}


		/*******************************************************
		*							    WHITE QUEEN CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::WhiteQueen];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = (RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) | BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces])) & pieceBitboards[PieceIndex::BlackPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::WhiteQueen;
			}
		}


		/*******************************************************
		*						    	WHITE ROOK CAPTURES								*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::WhiteRook];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & pieceBitboards[PieceIndex::BlackPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::WhiteRook;
			}
		}



		/*******************************************************
		*						    	WHITE BISHOP CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::WhiteBishop];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & pieceBitboards[PieceIndex::BlackPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::WhiteBishop;
			}
		}


	} else
	{
		/*******************************************************
		*					BLACK PAWN CAPTURES	& PROMOTIONS					*
		*******************************************************/
		//promotions (without capture)
		targets = pieceBitboards[PieceIndex::BlackPawn] >> 8 & RankBitboard::RankOne & ~pieceBitboards[PieceIndex::AllPieces];
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			promoteCommon = (endSquare + 8) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackPawn;
			moveList[moveIndex++] = promoteCommon | PieceIndex::BlackQueen << MoveBitShift::PromoteShift;
			moveList[moveIndex++] = promoteCommon | PieceIndex::BlackKnight << MoveBitShift::PromoteShift;
			moveList[moveIndex++] = promoteCommon | PieceIndex::BlackRook << MoveBitShift::PromoteShift;
			moveList[moveIndex++] = promoteCommon | PieceIndex::BlackBishop << MoveBitShift::PromoteShift;
		}

		//Captures towards south-west
		targets = pieceBitboards[PieceIndex::BlackPawn] >> 9 & FileBitboard::FileEight & pieceBitboards[PieceIndex::WhitePieces];
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			if (endSquare < Squares::A2)
			{
				promoteCommon = (endSquare + 9) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::BlackPawn;
				moveList[moveIndex++] = promoteCommon | PieceIndex::BlackQueen << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::BlackKnight << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::BlackRook << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::BlackBishop << MoveBitShift::PromoteShift;
			} else
			{
				moveList[moveIndex++] = (endSquare + 9) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::BlackPawn;
			}
		}

		//Captures towards south-east
		targets = pieceBitboards[PieceIndex::BlackPawn] >> 7 & FileBitboard::FileOne & pieceBitboards[PieceIndex::WhitePieces];
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			if (endSquare < Squares::A2)
			{
				promoteCommon = (endSquare + 7) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::BlackPawn;
				moveList[moveIndex++] = promoteCommon | PieceIndex::BlackQueen << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::BlackKnight << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::BlackRook << MoveBitShift::PromoteShift;
				moveList[moveIndex++] = promoteCommon | PieceIndex::BlackBishop << MoveBitShift::PromoteShift;
			} else
			{
				moveList[moveIndex++] = (endSquare + 7) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::BlackPawn;
			}
		}

		//en passant captures
		if (enPassant != NoEnPassant)
		{
			pieces = pieceBitboards[PieceIndex::BlackPawn] & ((1ull << (enPassant + 7) & FileBitboard::FileEight) | (1ull << (enPassant + 9) & FileBitboard::FileOne));
			while (pieces != 0)
			{
				startSquare = TrailingZeros(pieces);
				ResetLowestBit(pieces);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | enPassant << MoveBitShift::EndShift | PieceIndex::EnPassant << MoveBitShift::CaptureShift | PieceIndex::BlackPawn;
			}
		}


		/*******************************************************
		*								BLACK KNIGHT CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::BlackKnight];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = KnightMoveBitboard[startSquare] & pieceBitboards[PieceIndex::WhitePieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::BlackKnight;
			}
		}


		/*******************************************************
		*								BLACK KING CAPTURES								*
		*******************************************************/
		startSquare = TrailingZeros(pieceBitboards[PieceIndex::BlackKing]);
		targets = KingMoveBitboard[startSquare] & pieceBitboards[PieceIndex::WhitePieces];

		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::BlackKing;
		}


		/*******************************************************
		*								BLACK QUEEN CAPTURES								*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::BlackQueen];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = (RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) | BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces])) & pieceBitboards[PieceIndex::WhitePieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::BlackQueen;
			}
		}


		/*******************************************************
		*								BLACK ROOK CAPTURES								*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::BlackRook];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & pieceBitboards[PieceIndex::WhitePieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::BlackRook;
			}
		}

		/*******************************************************
		*								BLACK BISHOP CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::BlackBishop];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & pieceBitboards[PieceIndex::WhitePieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | pieceTypes[endSquare] << MoveBitShift::CaptureShift | PieceIndex::BlackBishop;
			}
		}

	}
}

void GenerateNonCaptures(int moveList[], int& moveIndex)
{
	Bitboard pieces;
	Bitboard targets;
	int startSquare;
	int endSquare;

	if (whiteToMove)
	{

		/*******************************************************
		*							WHITE PAWN NON CAPTURES							*
		*******************************************************/
		//two square pawn advance
		targets = (pieceBitboards[PieceIndex::WhitePawn] & RankBitboard::RankTwo) << 16 & ~pieceBitboards[PieceIndex::AllPieces] << 8 & ~pieceBitboards[PieceIndex::AllPieces];
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = (endSquare - 16) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhitePawn;
		}

		//one square pawn advance (no promotions)
		targets = pieceBitboards[PieceIndex::WhitePawn] << 8 & ~pieceBitboards[PieceIndex::AllPieces] & ~RankBitboard::RankEight;
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = (endSquare - 8) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhitePawn;
		}


		/*******************************************************
		*							WHITE KNIGHT NON CAPTURES						*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::WhiteKnight];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = KnightMoveBitboard[startSquare] & ~pieceBitboards[PieceIndex::AllPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhiteKnight;
			}
		}


		/*******************************************************
		*							WHITE KING NON CAPTURES							*
		*******************************************************/
		if (whiteCastle != NoCastling)
		{
			if (ShortCastle(whiteCastle) != 0 && (pieceBitboards[PieceIndex::AllPieces] >> 5 & 0x3) == 0 && !BlackAttack(Squares::E1) && !BlackAttack(Squares::F1) && !BlackAttack(Squares::G1))
			{
				moveList[moveIndex++] = Squares::E1 << MoveBitShift::StartShift | Squares::G1 << MoveBitShift::EndShift | PieceIndex::WhiteKing;
			}
			if (LongCastle(whiteCastle) != 0 && (pieceBitboards[PieceIndex::AllPieces] >> 1 & 0x7) == 0 && !BlackAttack(Squares::E1) && !BlackAttack(Squares::D1) && !BlackAttack(Squares::C1))
			{
				moveList[moveIndex++] = Squares::E1 << MoveBitShift::StartShift | Squares::C1 << MoveBitShift::EndShift | PieceIndex::WhiteKing;
			}
		}

		startSquare = TrailingZeros(pieceBitboards[PieceIndex::WhiteKing]);
		targets = KingMoveBitboard[startSquare] & ~pieceBitboards[PieceIndex::AllPieces];

		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhiteKing;
		}

		/*******************************************************
		*							WHITE QUEEN NON CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::WhiteQueen];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = (RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) | BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces])) & ~pieceBitboards[PieceIndex::AllPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhiteQueen;
			}
		}


		/*******************************************************
		*							WHITE ROOK NON CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::WhiteRook];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhiteRook;
			}
		}



		/*******************************************************
		*							WHITE BISHOP NON CAPTURES						*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::WhiteBishop];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhiteBishop;
			}
		}

	} else
	{

		/*******************************************************
		*							BLACK PAWN NON CAPTURES							*
		*******************************************************/
		//two square pawn advance
		targets = (pieceBitboards[PieceIndex::BlackPawn] & RankBitboard::RankSeven) >> 16 & ~pieceBitboards[PieceIndex::AllPieces] >> 8 & ~pieceBitboards[PieceIndex::AllPieces];
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = (endSquare + 16) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackPawn;
		}

		//one square pawn advance (no promotions)
		targets = (pieceBitboards[PieceIndex::BlackPawn]) >> 8 & ~pieceBitboards[PieceIndex::AllPieces] & ~RankBitboard::RankOne;
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = (endSquare + 8) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackPawn;
		}


		/*******************************************************
		*							BLACK KNIGHT NON CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::BlackKnight];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = KnightMoveBitboard[startSquare] & ~pieceBitboards[PieceIndex::AllPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackKnight;
			}
		}



		/*******************************************************
		*							BLACK KING NON CAPTURES							*
		*******************************************************/
		if (blackCastle != NoCastling)
		{
			if (ShortCastle(blackCastle) != 0 && (pieceBitboards[PieceIndex::AllPieces] >> 61 & 0x3) == 0 && !WhiteAttack(Squares::E8) && !WhiteAttack(Squares::F8) && !WhiteAttack(Squares::G8))
			{
				moveList[moveIndex++] = Squares::E8 << MoveBitShift::StartShift | Squares::G8 << MoveBitShift::EndShift | PieceIndex::BlackKing;
			}
			if (LongCastle(blackCastle) != 0 && (pieceBitboards[PieceIndex::AllPieces] >> 57 & 0x7) == 0 && !WhiteAttack(Squares::E8) && !WhiteAttack(Squares::D8) && !WhiteAttack(Squares::C8))
			{
				moveList[moveIndex++] = Squares::E8 << MoveBitShift::StartShift | Squares::C8 << MoveBitShift::EndShift | PieceIndex::BlackKing;
			}
		}

		startSquare = TrailingZeros(pieceBitboards[PieceIndex::BlackKing]);
		targets = KingMoveBitboard[startSquare] & ~pieceBitboards[PieceIndex::AllPieces];

		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackKing;
		}


		/*******************************************************
		*							BLACK QUEEN NON CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::BlackQueen];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = (RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) | BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces])) & ~pieceBitboards[PieceIndex::AllPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackQueen;
			}
		}


		/*******************************************************
		*							BLACK ROOK NON CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::BlackRook];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackRook;
			}
		}



		/*******************************************************
		*							BLACK BISHOP NON CAPTURES							*
		*******************************************************/
		pieces = pieceBitboards[PieceIndex::BlackBishop];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackBishop;
			}
		}

	}
}

void GenerateChecks(int moveList[], int& moveIndex)
{
	Bitboard pieces;
	Bitboard targets;
	Bitboard kingAttackedBitboard;
	int startSquare;
	int endSquare;

	if (whiteToMove)
	{

		int kingSquare = TrailingZeros(pieceBitboards[PieceIndex::BlackKing]);

		/* PAWN */
		kingAttackedBitboard = ((pieceBitboards[PieceIndex::BlackKing] >> 9 & FileBitboard::FileEight) | (pieceBitboards[PieceIndex::BlackKing] >> 7 & FileBitboard::FileOne)) & ~pieceBitboards[PieceIndex::AllPieces];

		//two squares pawn advances
		targets = (pieceBitboards[PieceIndex::WhitePawn] & RankBitboard::RankTwo) << 16 & ~pieceBitboards[PieceIndex::AllPieces] << 8 & kingAttackedBitboard;
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = (endSquare - 16) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhitePawn;
		}

		//one square pawn advance (no promotions)
		targets = pieceBitboards[PieceIndex::WhitePawn] << 8 & kingAttackedBitboard;
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = (endSquare - 8) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhitePawn;
		}


		/* KNIGHT */
		kingAttackedBitboard = KnightMoveBitboard[kingSquare] & ~pieceBitboards[PieceIndex::AllPieces];
		pieces = pieceBitboards[PieceIndex::WhiteKnight];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = KnightMoveBitboard[startSquare] & kingAttackedBitboard;

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhiteKnight;
			}
		}

		/* QUEEN */
		kingAttackedBitboard = (RookMagicBitboard(kingSquare, pieceBitboards[PieceIndex::AllPieces]) | BishopMagicBitboard(kingSquare, pieceBitboards[PieceIndex::AllPieces])) & ~pieceBitboards[PieceIndex::AllPieces];
		pieces = pieceBitboards[PieceIndex::WhiteQueen];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = (RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) | BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces])) & kingAttackedBitboard;

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhiteQueen;
			}
		}


		/* ROOK */
		kingAttackedBitboard = RookMagicBitboard(kingSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];
		pieces = pieceBitboards[PieceIndex::WhiteRook];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & kingAttackedBitboard;

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhiteRook;
			}
		}

		/* BISHOPS */
		kingAttackedBitboard = BishopMagicBitboard(kingSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];
		pieces = pieceBitboards[PieceIndex::WhiteBishop];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & kingAttackedBitboard;

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::WhiteBishop;
			}
		}
	} else
	{
		int kingSquare = TrailingZeros(pieceBitboards[PieceIndex::WhiteKing]);

		/* PAWN */
		kingAttackedBitboard = ((pieceBitboards[PieceIndex::WhiteKing] << 9 & FileBitboard::FileOne) | (pieceBitboards[PieceIndex::WhiteKing] << 7 & FileBitboard::FileEight)) & ~pieceBitboards[PieceIndex::AllPieces];

		//two squares pawn advances
		targets = (pieceBitboards[PieceIndex::BlackPawn] & RankBitboard::RankTwo) >> 16 & ~pieceBitboards[PieceIndex::AllPieces] >> 8 & kingAttackedBitboard;
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = (endSquare + 16) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackPawn;
		}

		//one square pawn advance (no promotions)
		targets = pieceBitboards[PieceIndex::BlackPawn] >> 8 & kingAttackedBitboard;
		while (targets != 0)
		{
			endSquare = TrailingZeros(targets);
			ResetLowestBit(targets);
			moveList[moveIndex++] = (endSquare + 8) << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackPawn;
		}


		/* KNIGHT */
		kingAttackedBitboard = KnightMoveBitboard[kingSquare] & ~pieceBitboards[PieceIndex::AllPieces];
		pieces = pieceBitboards[PieceIndex::BlackKnight];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = KnightMoveBitboard[startSquare] & kingAttackedBitboard;

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackKnight;
			}
		}

		/* QUEEN */
		kingAttackedBitboard = (RookMagicBitboard(kingSquare, pieceBitboards[PieceIndex::AllPieces]) | BishopMagicBitboard(kingSquare, pieceBitboards[PieceIndex::AllPieces])) & ~pieceBitboards[PieceIndex::AllPieces];
		pieces = pieceBitboards[PieceIndex::BlackQueen];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = (RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) | BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces])) & kingAttackedBitboard;

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackQueen;
			}
		}


		/* ROOK */
		kingAttackedBitboard = RookMagicBitboard(kingSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];
		pieces = pieceBitboards[PieceIndex::BlackRook];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & kingAttackedBitboard;

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackRook;
			}
		}

		/* BISHOPS */
		kingAttackedBitboard = BishopMagicBitboard(kingSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];
		pieces = pieceBitboards[PieceIndex::BlackBishop];
		while (pieces != 0)
		{
			startSquare = TrailingZeros(pieces);
			ResetLowestBit(pieces);
			targets = BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & kingAttackedBitboard;

			while (targets != 0)
			{
				endSquare = TrailingZeros(targets);
				ResetLowestBit(targets);
				moveList[moveIndex++] = startSquare << MoveBitShift::StartShift | endSquare << MoveBitShift::EndShift | PieceIndex::BlackBishop;
			}
		}
	}

}

void GenerateValidMoveList(int moveList[])
{
	int currentMove;
	int numMoves = 0;
	GenerateCaptures(moveList, numMoves);	
	GenerateNonCaptures(moveList, numMoves);

	if (whiteToMove)
	{
		for (int i = 0; (currentMove = moveList[i]) != NoMove; i++)
		{
			MakeMove(currentMove);
			if (WhiteInCheck())
			{
				moveList[i] = moveList[--numMoves];
				moveList[numMoves] = NoMove;
				i--;
			}
			UndoMove(currentMove);
		}
	} else
	{
		for (int i = 0; (currentMove = moveList[i]) != NoMove; i++)
		{
			MakeMove(currentMove);
			if (BlackInCheck())
			{
				moveList[i] = moveList[--numMoves];
				moveList[numMoves] = NoMove;
				i--;
			}
			UndoMove(currentMove);
		}
	}
}