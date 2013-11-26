#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

int TestSearchScore(const int& searchDepth);
void FillPrincipleVariation(const int& depth);
Move GetNextPrincipleVariationMove();
string GetThinkingOutput(const int& ply, const int& score);
bool DrawRepetitionOrFifty();
int SearchRoot(int depth, int alpha, int beta);
int AlphaBeta(int depth, int alpha, int beta, bool nullMoveAllowed);
int Quiescence(int alpha, int beta);


#endif