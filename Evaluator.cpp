#include "Include.h"

Bitboard WhiteFrontSpanBitboard[64] = {0};
Bitboard BlackFrontSpanBitboard[64] = {0};
Bitboard SideSpanBitboard[64] = {0};
Bitboard FileSpanBitboard[64] = {0};

const int PieceSquareTable[14][64] =
{
	//White Pawn
	{
		0, 0, 0, 0, 0, 0, 0, 0,
			5, 10, 10, -20, -20, 10, 10, 5,
			5,-5, -10, 0, 0, -10, -5, 5,
			0, 0, 0, 20, 20, 0, 0, 0,
			5, 5, 10, 25, 25, 10, 5, 5,
			10, 10, 20, 30, 30, 20, 10, 10,
			50, 50, 50, 50, 50, 50, 50, 50,
			0, 0, 0, 0, 0, 0, 0, 0
	},
	//Black Pawn
	{
		0, 0, 0, 0, 0, 0, 0, 0,
			50, 50, 50, 50, 50, 50, 50, 50,
			10, 10, 20, 30, 30, 20, 10, 10,
			5, 5, 10, 25, 25, 10, 5, 5,
			0, 0, 0, 20, 20, 0, 0, 0,
			5, -5, -10, 0, 0, -10, -5, 5,
			5, 10, 10, -20, -20, 10, 10, 5,
			0, 0, 0, 0, 0, 0, 0, 0
		},
		//White Knight
		{
			-50, -40, -30, -30, -30, -30, -40, -50,
				-40, -20, 0, 5, 5, 0, -20, -40,
				-30, 5, 10, 15, 15, 10, 5, -30,
				-30, 0, 15, 20, 20, 15, 0, -30,
				-30, 5, 15, 20, 20, 15, 5, -30,
				-30, 0, 10, 15, 15, 10, 0, -30,
				-40, -20, 0, 0, 0, 0, -20, -40,
				-50, -40, -30, -30, -30, -30, -40, -50
		},
		//Black Knight
		{
			-50, -40, -30, -30, -30, -30, -40, -50,
				-40, -20, 0, 0, 0, 0, -20, -40,
				-30, 0, 10, 15, 15, 10, 0, -30,
				-30, 5, 15, 20, 20, 15, 5, -30,
				-30, 0, 15, 20, 20, 15, 0, -30,
				-30, 5, 10, 15, 15, 10, 5, -30,
				-40, -20, 0, 5, 5, 0, -20, -40,
				-50, -40, -30, -30, -30, -30, -40, -50
			},
			//White King Middlegame
			{
				20, 30, 10, 0, 0, 10, 30, 20,
					20, 20, 0, 0, 0, 0, 20, 20,
					-10, -20, -20, -20, -20, -20, -20, -10,
					-20, -30, -30, -40, -40, -30, -30, -20,
					-30, -40, -40, -50, -50, -40, -40, -30,
					-30, -40, -40, -50, -50, -40, -40, -30,
					-30, -40, -40, -50, -50, -40, -40, -30,
					-30, -40, -40, -50, -50, -40, -40, -30
			},
			//Black King Middlegame
			{
				-30, -40, -40, -50, -50, -40, -40, -30,
					-30, -40, -40, -50, -50, -40, -40, -30,
					-30, -40, -40, -50, -50, -40, -40, -30,
					-30, -40, -40, -50, -50, -40, -40, -30,
					-20, -30, -30, -40, -40, -30, -30, -20,
					-10, -20, -20, -20, -20, -20, -20, -10,
					20, 20, 0, 0, 0, 0, 20, 20,
					20, 30, 10, 0, 0, 10, 30, 20
				},
				//White Queen
				{
					-20, -10, -10, -5, -5, -10, -10, -20,
						-10, 0, 5, 0, 0, 0, 0, -10,
						-10, 5, 5, 5, 5, 5, 0, -10,
						0, 0, 5, 5, 5, 5, 0, -5,
						-5, 0, 5, 5, 5, 5, 0, -5,
						-10, 0, 5, 5, 5, 5, 0, -10,
						-10, 0, 0, 0, 0, 0, 0, -10,
						-20, -10, -10, -5, -5, -10, -10, -20
				},
				//Black Queen
				{
					-20, -10, -10, -5, -5, -10, -10, -20,
						-10, 0, 0, 0, 0, 0, 0, -10,
						-10, 0, 5, 5, 5, 5, 0, -10,
						-5, 0, 5, 5, 5, 5, 0, -5,
						0, 0, 5, 5, 5, 5, 0, -5,
						-10, 5, 5, 5, 5, 5, 0, -10,
						-10, 0, 5, 0, 0, 0, 0, -10,
						-20, -10, -10, -5, -5, -10, -10, -20
					},
					//White Rook
					{
						0, 0, 0, 5, 5, 0, 0, 0,
							-5, 0, 0, 0, 0, 0, 0, -5,
							-5, 0, 0, 0, 0, 0, 0, -5,
							-5, 0, 0, 0, 0, 0, 0, -5,
							-5, 0, 0, 0, 0, 0, 0, -5,
							-5, 0, 0, 0, 0, 0, 0, -5,
							5, 10, 10, 10, 10, 10, 10, 5,
							0, 0, 0, 0, 0, 0, 0, 0,
					},
					//Black Rook
					{
						0, 0, 0, 0, 0, 0, 0, 0,
							5, 10, 10, 10, 10, 10, 10, 5,
							-5, 0, 0, 0, 0, 0, 0, -5,
							-5, 0, 0, 0, 0, 0, 0, -5,
							-5, 0, 0, 0, 0, 0, 0, -5,
							-5, 0, 0, 0, 0, 0, 0, -5,
							-5, 0, 0, 0, 0, 0, 0, -5,
							0, 0, 0, 5, 5, 0, 0, 0
						},
						//White Bishop
						{
							-20, -10, -10, -10, -10, -10, -10, -20,
								-10, 5, 0, 0, 0, 0, 5, -10,
								-10, 10, 10, 10, 10, 10, 10, -10,
								-10, 0, 10, 10, 10, 10, 0, -10,
								-10, 5, 5, 10, 10, 5, 5, -10,
								-10, 0, 5, 10, 10, 5, 0, -10,
								-10, 0, 0, 0, 0, 0, 0, -10,
								-20, -10, -10, -10, -10, -10, -10, -20,
						},
						//Black Bishop
						{
							-20, -10, -10, -10, -10, -10, -10, -20,
								-10, 0, 0, 0, 0, 0, 0, -10,
								-10, 0, 5, 10, 10, 5, 0, -10,
								-10, 5, 5, 10, 10, 5, 5, -10,
								-10, 0, 10, 10, 10, 10, 0, -10,
								-10, 10, 10, 10, 10, 10, 10, -10,
								-10, 5, 0, 0, 0, 0, 5, -10,
								-20, -10, -10, -10, -10, -10, -10, -20
							},
							//White King Endgame
							{
								-50, -30, -30, -30, -30, -30, -30, -50,
									-30, -30, 0, 0, 0, 0, -30, -30,
									-30, -10, 20, 30, 30, 20, -10, -30,
									-30, -10, 30, 40, 40, 30, -10, -30,
									-30, -10, 30, 40, 40, 30, -10, -30,
									-30, -10, 20, 30, 30, 20, -10, -30,
									-30, -20, -10, 0, 0, -10, -20, -30,
									-50, -40, -30, -20, -20, -30, -40, -50
							},
							//Black King Endgame
							{
								-50, -40, -30, -20, -20, -30, -40, -50,
									-30, -20, -10, 0, 0, -10, -20, -30,
									-30, -10, 20, 30, 30, 20, -10, -30,
									-30, -10, 30, 40, 40, 30, -10, -30,
									-30, -10, 30, 40, 40, 30, -10, -30,
									-30, -10, 20, 30, 30, 20, -10, -30,
									-30, -30, 0, 0, 0, 0, -30, -30,
									-50, -30, -30, -30, -30, -30, -30, -50
								}
};

const int BishopMobility[] =
{
	-6, -2, 0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8
};
const int RookMobility[] =
{
	-4, -2, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4
};
const int KnightMobility[] =
{
	-6, -4, 0, 2, 4, 5, 6, 7, 8
};
const int KnightMobilityAdjustmentByPawn[] =
{
	-20, -16, -12, -8, -4, 0, 4, 8, 12
};
const int RookMobilityAdjustmentByPawn[] =
{
	15, 12, 9, 6, 3, 0, -3, -6, -9
};

int MobilityScore()
{
	int score = 0;
	Bitboard pieces;
	Bitboard targets;
	int startSquare;

	/*
	* White Knight Mobility
	*/
	pieces = pieceBitboards[PieceIndex::WhiteKnight];
	while (pieces != 0)
	{
		startSquare = TrailingZeros(pieces);
		ResetLowestBit(pieces);
		targets = KnightMoveBitboard[startSquare] & ~pieceBitboards[PieceIndex::AllPieces];
		score += KnightMobility[PopulationCountFew(targets)];
	}
	/*
	* Black Knight Mobility
	*/
	pieces = pieceBitboards[PieceIndex::BlackKnight];
	while (pieces != 0)
	{
		startSquare = TrailingZeros(pieces);
		ResetLowestBit(pieces);
		targets = KnightMoveBitboard[startSquare] & ~pieceBitboards[PieceIndex::AllPieces];
		score -= KnightMobility[PopulationCountFew(targets)];
	}
	/*
	* White Bishop Mobility
	*/
	pieces = pieceBitboards[PieceIndex::WhiteBishop];
	while (pieces != 0)
	{
		startSquare = TrailingZeros(pieces);
		ResetLowestBit(pieces);
		targets = BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];
		score += BishopMobility[PopulationCountFew(targets)];
	}
	/*
	* Black Bishop Mobility
	*/
	pieces = pieceBitboards[PieceIndex::BlackBishop];
	while (pieces != 0)
	{
		startSquare = TrailingZeros(pieces);
		ResetLowestBit(pieces);
		targets = BishopMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];
		score -= BishopMobility[PopulationCountFew(targets)];
	}
	/*
	* White Rook Mobility
	*/
	pieces = pieceBitboards[PieceIndex::WhiteRook];
	while (pieces != 0)
	{
		startSquare = TrailingZeros(pieces);
		ResetLowestBit(pieces);
		targets = RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];
		score += RookMobility[PopulationCountFew(targets)];
	}
	/*
	* Black Rook Mobility
	*/
	pieces = pieceBitboards[PieceIndex::BlackRook];
	while (pieces != 0)
	{
		startSquare = TrailingZeros(pieces);
		ResetLowestBit(pieces);
		targets = RookMagicBitboard(startSquare, pieceBitboards[PieceIndex::AllPieces]) & ~pieceBitboards[PieceIndex::AllPieces];
		score -= RookMobility[PopulationCountFew(targets)];
	}

	int numWhitePawns = PopulationCountMore(pieceBitboards[PieceIndex::WhitePawn]);
	int numBlackPawns = PopulationCountMore(pieceBitboards[PieceIndex::BlackPawn]);
	score += KnightMobilityAdjustmentByPawn[numWhitePawns];
	score -= KnightMobilityAdjustmentByPawn[numBlackPawns];
	score += RookMobilityAdjustmentByPawn[numWhitePawns];
	score -= RookMobilityAdjustmentByPawn[numBlackPawns];

	return score;
}


Bitboard WhiteFrontSpanBitboard[];
Bitboard BlackFrontSpanBitboard[];
Bitboard SideSpanBitboard[];
Bitboard FileSpanBitboard[];
//     Bitboard WhiteAttackSpanBitboard[];
//     Bitboard BlackAttackSpanBitboard[];

int WhitePassedPawnScore[64] = 
{
	0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 5, 5, 5, 5, 5, 5,
	10, 8, 8, 8, 8, 8, 8, 10,
	16, 12, 12, 12, 12, 12, 12, 16,
	22, 16, 16, 16, 16, 16, 16, 22,
	30, 20, 20, 20, 20, 20, 20, 30,
	20, 10, 10, 10, 10, 10, 10, 20, //TODO Calculate here or in piece square tables
	0, 0, 0, 0, 0, 0, 0, 0
};
int BlackPassedPawnScore[64] = 
{
	0, 0, 0, 0, 0, 0, 0, 0,
	20, 10, 10, 10, 10, 10, 10, 20, //TODO Calculate here or in piece square tables
	30, 20, 20, 20, 20, 20, 20, 30,
	22, 16, 16, 16, 16, 16, 16, 22,
	16, 12, 12, 12, 12, 12, 12, 16,
	10, 8, 8, 8, 8, 8, 8, 10,
	5, 5, 5, 5, 5, 5, 5, 5,
	0, 0, 0, 0, 0, 0, 0, 0
};
int WhiteProtectedPassedPawnBonus[64] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	4, 4, 4, 4, 4, 4, 4, 4,
	6, 6, 6, 6, 6, 6, 6, 6,
	10, 10, 10, 10, 10, 10, 10, 10,
	15, 15, 15, 15, 15, 15, 15, 15,
	25, 25, 25, 25, 25, 25, 25, 25,
	0, 0, 0, 0, 0, 0, 0, 0
};
int BlackProtectedPassedPawnBonus[64] = 
{
	0, 0, 0, 0, 0, 0, 0, 0,
	25, 25, 25, 25, 25, 25, 25, 25,
	15, 15, 15, 15, 15, 15, 15, 15,
	10, 10, 10, 10, 10, 10, 10, 10,
	6, 6, 6, 6, 6, 6, 6, 6,
	4, 4, 4, 4, 4, 4, 4, 4,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

int PawnStructureScore()
{
	PawnTableNode* node = pawnTranspoTable->ProbeTableNode();
	if (node->hashKey == pawnStructureKey)
	{
		return node->score;
	}

	int score = 0;

	/*
	* Evaluate for doubled, tripled pawns and isolated doubled, tripled
	* pawns. For both sides.
	*/
	bool whiteIsolatedDoubledPawn[8] = {0};
	bool blackIsolatedDoubledPawn[8] = {0};
	for (int i = 0; i < 8; i++)
	{
		Bitboard filePawnBitboard = ~FileBitboardByIndex[i] & pieceBitboards[PieceIndex::WhitePawn];
		switch (PopulationCountFew(filePawnBitboard))
		{
		case 2:
			score += Evaluation::DoublePawnAdjustment;
			if (IsIsolatedWhitePawn(TrailingZeros(filePawnBitboard)))
			{
				score += Evaluation::IsolatedDoublePawnAdjustment;
				whiteIsolatedDoubledPawn[i] = true;
			}
			break;
		case 3:
			score += Evaluation::TriplePawnAdjustment;
			if (IsIsolatedWhitePawn(TrailingZeros(filePawnBitboard)))
			{
				score += Evaluation::IsolatedTriplePawnAdjustment;
				whiteIsolatedDoubledPawn[i] = true;
			}
			break;
		}

		filePawnBitboard = ~FileBitboardByIndex[i] & pieceBitboards[PieceIndex::BlackPawn];
		switch (PopulationCountFew(filePawnBitboard))
		{
		case 2:
			score -= Evaluation::DoublePawnAdjustment;
			if (IsIsolatedBlackPawn(TrailingZeros(filePawnBitboard)))
			{
				score -= Evaluation::IsolatedDoublePawnAdjustment;
				blackIsolatedDoubledPawn[i] = true;
			}
			break;
		case 3:
			score -= Evaluation::TriplePawnAdjustment;
			if (IsIsolatedBlackPawn(TrailingZeros(filePawnBitboard)))
			{
				score -= Evaluation::IsolatedTriplePawnAdjustment;
				blackIsolatedDoubledPawn[i] = true;
			}
			break;
		}
	}

	/*
	* Goes through each black pawn and adds adjustments bonus/deductions
	* for single isolated pawns, pawn chains, backward pawns. Also
	* generates the
	* blackFrontSpan bitboard needed to adjust for white passed and
	* protected passed pawns.
	*/

	Bitboard blackFrontSpan = 0x0l;
	Bitboard pieces = pieceBitboards[PieceIndex::BlackPawn];
	while (pieces != 0)
	{
		Square square = TrailingZeros(pieces);
		ResetLowestBit(pieces);
		blackFrontSpan |= BlackFrontSpanBitboard[square];
		if (!blackIsolatedDoubledPawn[File(square)] && IsIsolatedBlackPawn(square))
		{
			score -= Evaluation::IsolatedPawnAdjustment;
		}
	}
	Bitboard whitePassedPawnsBitboard = ~blackFrontSpan & pieceBitboards[PieceIndex::WhitePawn];
	while (whitePassedPawnsBitboard != 0)
	{
		Square square = TrailingZeros(whitePassedPawnsBitboard);
		ResetLowestBit(whitePassedPawnsBitboard);
		score += WhitePassedPawnScore[square];
		if (IsProtectedWhitePawn(square))
		{
			score += WhiteProtectedPassedPawnBonus[square];
		}
	}

	/*
	* Same but for white.
	*/
	Bitboard whiteFrontSpan = 0x0l;
	pieces = pieceBitboards[PieceIndex::WhitePawn];
	while (pieces != 0)
	{
		Square square = TrailingZeros(pieces);
		ResetLowestBit(pieces);
		whiteFrontSpan |= WhiteFrontSpanBitboard[square];
		if (!whiteIsolatedDoubledPawn[File(square)] && IsIsolatedWhitePawn(square))
		{
			score += Evaluation::IsolatedPawnAdjustment;
		}
	}
	Bitboard blackPassedPawnsBitboard = ~whiteFrontSpan & pieceBitboards[PieceIndex::BlackPawn];
	while (blackPassedPawnsBitboard != 0)
	{
		Square square = TrailingZeros(blackPassedPawnsBitboard);
		ResetLowestBit(blackPassedPawnsBitboard);
		score -= BlackPassedPawnScore[square];
		if (IsProtectedBlackPawn(square))
		{
			score -= BlackProtectedPassedPawnBonus[square];
		}
	}


	pawnTranspoTable->RecordTableNode(pawnStructureKey, score);
	return score;
}

int PawnStructureScoreDebug()
{
	//Utilities.ShowBoard();
	int wdoubledPawns = 0;
	int wtripledPawns = 0;
	int wisolatedDoubled = 0;
	int wisolatedTripled = 0;
	int wisolatedSingle = 0;
	int wpassedPawns = 0;
	int wprotectedPassers = 0;
	int bdoubledPawns = 0;
	int btripledPawns = 0;
	int bisolatedDoubled = 0;
	int bisolatedTripled = 0;
	int bisolatedSingle = 0;
	int bpassedPawns = 0;
	int bprotectedPassers = 0;

	PawnTableNode* node = pawnTranspoTable->ProbeTableNode();
	if (node->hashKey == pawnStructureKey)
	{
		return node->score;
	}

	int score = 0;

	/*
	* Evaluate for doubled, tripled pawns and isolated doubled, tripled
	* pawns. For both sides.
	*/
	bool whiteIsolatedDoubledPawn[8] = {0};
	bool blackIsolatedDoubledPawn[8] = {0};
	for (int i = 0; i < 8; i++)
	{
		Bitboard filePawnBitboard = ~FileBitboardByIndex[i] & pieceBitboards[PieceIndex::WhitePawn];
		switch (PopulationCountFew(filePawnBitboard))
		{
		case 2:
			score += Evaluation::DoublePawnAdjustment;
			wdoubledPawns++;
			if (IsIsolatedWhitePawn(TrailingZeros(filePawnBitboard)))
			{
				score += Evaluation::IsolatedDoublePawnAdjustment;
				whiteIsolatedDoubledPawn[i] = true;
				wisolatedDoubled++;
			}
			break;
		case 3:
			score += Evaluation::TriplePawnAdjustment;
			wtripledPawns++;
			if (IsIsolatedWhitePawn(TrailingZeros(filePawnBitboard)))
			{
				score += Evaluation::IsolatedTriplePawnAdjustment;
				whiteIsolatedDoubledPawn[i] = true;
				wisolatedTripled++;
			}
			break;
		}

		filePawnBitboard = ~FileBitboardByIndex[i] & pieceBitboards[PieceIndex::BlackPawn];
		switch (PopulationCountFew(filePawnBitboard))
		{
		case 2:
			score -= Evaluation::DoublePawnAdjustment;
			bdoubledPawns++;
			if (IsIsolatedBlackPawn(TrailingZeros(filePawnBitboard)))
			{
				score -= Evaluation::IsolatedDoublePawnAdjustment;
				blackIsolatedDoubledPawn[i] = true;
				bisolatedDoubled++;
			}
			break;
		case 3:
			score -= Evaluation::TriplePawnAdjustment;
			btripledPawns++;
			if (IsIsolatedBlackPawn(TrailingZeros(filePawnBitboard)))
			{
				score -= Evaluation::IsolatedTriplePawnAdjustment;
				blackIsolatedDoubledPawn[i] = true;
				bisolatedTripled++;
			}
			break;
		}
	}

	/*
	* Goes through each black pawn and adds adjustments bonus/deductions
	* for single isolated pawns, pawn chains, backward pawns. Also
	* generates the
	* blackFrontSpan bitboard needed to adjust for white passed and
	* protected passed pawns.
	*/

	Bitboard blackFrontSpan = 0x0l;
	Bitboard pieces = pieceBitboards[PieceIndex::BlackPawn];
	while (pieces != 0)
	{
		Square square = TrailingZeros(pieces);
		ResetLowestBit(pieces);
		blackFrontSpan |= BlackFrontSpanBitboard[square];
		if (!blackIsolatedDoubledPawn[File(square)] && IsIsolatedBlackPawn(square))
		{
			score -= Evaluation::IsolatedPawnAdjustment;
			bisolatedSingle++;
		}
	}
	Bitboard whitePassedPawnsBitboard = ~blackFrontSpan & pieceBitboards[PieceIndex::WhitePawn];
	while (whitePassedPawnsBitboard != 0)
	{
		Square square = TrailingZeros(whitePassedPawnsBitboard);
		ResetLowestBit(whitePassedPawnsBitboard);
		score += WhitePassedPawnScore[square];
		wpassedPawns++;
		if (IsProtectedWhitePawn(square))
		{
			score += WhiteProtectedPassedPawnBonus[square];
			wprotectedPassers++;
		}
	}

	/*
	* Same but for white.
	*/
	Bitboard whiteFrontSpan = 0x0l;
	pieces = pieceBitboards[PieceIndex::WhitePawn];
	while (pieces != 0)
	{
		Square square = TrailingZeros(pieces);
		ResetLowestBit(pieces);
		whiteFrontSpan |= WhiteFrontSpanBitboard[square];
		if (!whiteIsolatedDoubledPawn[File(square)] && IsIsolatedWhitePawn(square))
		{
			score += Evaluation::IsolatedPawnAdjustment;
			wisolatedSingle++;
		}
	}
	Bitboard blackPassedPawnsBitboard = ~whiteFrontSpan & pieceBitboards[PieceIndex::BlackPawn];
	while (blackPassedPawnsBitboard != 0)
	{
		Square square = TrailingZeros(blackPassedPawnsBitboard);
		ResetLowestBit(blackPassedPawnsBitboard);
		score -= BlackPassedPawnScore[square];
		bpassedPawns++;
		if (IsProtectedBlackPawn(square))
		{
			score -= BlackProtectedPassedPawnBonus[square];
			bprotectedPassers++;
		}
	}


	pawnTranspoTable->RecordTableNode(pawnStructureKey, score);

	cout << "White: " << endl;
	cout << "Doubled: " << wdoubledPawns << endl;
	cout << "Tripled: " << wtripledPawns << endl;
	cout << "Isolated Single: " << wisolatedSingle << endl;
	cout << "Isolated Double: " << wisolatedDoubled << endl;
	cout << "Isolated Triple: " << wisolatedTripled << endl;
	cout << "Passed Pawns: " << wpassedPawns << endl;
	cout << "Protected Passers: " << wprotectedPassers << endl << endl;

	cout << "Black: " << endl;
	cout << "Doubled: " << bdoubledPawns << endl;
	cout << "Tripled: " << btripledPawns << endl;
	cout << "Isolated Single: " << bisolatedSingle << endl;
	cout << "Isolated Double: " << bisolatedDoubled << endl;
	cout << "Isolated Triple: " << bisolatedTripled << endl;
	cout << "Passed Pawns: " << bpassedPawns << endl;
	cout << "Protected Passers: " << bprotectedPassers << endl;
	cout << endl;

	return score;
}

/*
* Methods to determine if a pawn is protected by another pawn.
* Equals to (MoveAttackBitboard AttackedBy-SameColor-PawnBitboard) & (Pawn
* Bitboard)
*/
bool IsProtectedWhitePawn(Square square)
{
	return (AttackedByWhitePawnBitboard[square] & pieceBitboards[PieceIndex::WhitePawn]) != 0;
}

bool IsProtectedBlackPawn(Square square)
{
	return (AttackedByBlackPawnBitboard[square] & pieceBitboards[PieceIndex::BlackPawn]) != 0;
}

bool IsIsolatedWhitePawn(Square square)
{
	return (SideSpanBitboard[square] & pieceBitboards[PieceIndex::WhitePawn]) == 0;
}

bool IsIsolatedBlackPawn(Square square)
{
	return (SideSpanBitboard[square] & pieceBitboards[PieceIndex::BlackPawn]) == 0;
}

int Evaluate()
{
	int score = 0;
	score += whitePieceScore - blackPieceScore + whitePawnScore - blackPawnScore;
	score += pieceSquareScore;
	score += MobilityScore();
	score += PawnStructureScore();

	return (whiteToMove ? score : -score);
}

void InitializeEvaluation()
{
	for (int i = 8; i < 56; i++)
	{
		Bitboard frontSpanBitboard = 0x0l;
		frontSpanBitboard |= ~FileBitboardByIndex[File(i)];
		frontSpanBitboard |= ~FileBitboardByIndex[File(i) + 1];
		SideSpanBitboard[i] |= ~FileBitboardByIndex[File(i) + 1];
		if (File(i) - 1 >= 0)
		{
			frontSpanBitboard |= ~FileBitboardByIndex[File(i) - 1];
			SideSpanBitboard[i] |= ~FileBitboardByIndex[File(i) - 1];
		}

		FileSpanBitboard[i] = ~FileBitboardByIndex[File(i)] & (0x1ull << i);
		WhiteFrontSpanBitboard[i] = frontSpanBitboard & (0xFFFFFFFFFFFFFFFFl << ((Rank(i) + 1) * 8));
		BlackFrontSpanBitboard[i] = frontSpanBitboard & (0xFFFFFFFFFFFFFFFFl >> ((8 - Rank(i)) * 8));
	}
}