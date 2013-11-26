#include "Include.h"

ChessTimer::ChessTimer() 
{
}

void ChessTimer::Start()
{
	timer = boost::timer();
}

int ChessTimer::GetElapsedTime()
{
	return static_cast<int>(timer.elapsed() * 1000);
}

