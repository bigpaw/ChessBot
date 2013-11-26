#ifndef INCLUDE_H_INCLUDED
#define INCLUDE_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <random>
using namespace std;

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <boost/timer.hpp>
#include <boost/format.hpp>

typedef int Move;
typedef unsigned __int64 Bitboard;
typedef int Square;
typedef int Piece;
typedef unsigned __int64 HashKey;
typedef unsigned int PawnHashKey;

#include "Constants.h"
#include "Structs.h"
#include "OpeningBook.h"
#include "Winboard.h"
#include "ChessTimer.h"
#include "Utilities.h"
#include "Debugger.h"
#include "TranspoTable.h"
#include "PawnTranspoTable.h"
#include "StaticExchange.h"
#include "MoveOrderingData.h"
#include "Engine.h"
#include "MoveGenerator.h"
#include "MakeMove.h"
#include "Evaluator.h"
#include "Search.h"

#endif