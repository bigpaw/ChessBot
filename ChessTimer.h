#ifndef CHESSTIMER_H_INCLUDED
#define CHESSTIMER_H_INCLUDED

class ChessTimer
{
public:
	ChessTimer();
    void Start();
    int GetElapsedTime();

private:
    boost::timer timer;
};

#endif