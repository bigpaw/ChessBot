#ifndef WINBOARD_H_INCLUDED
#define WINBOARD_H_INCLUDED

extern bool winboardIsEngineSearching;
extern bool winboardIsForcedMode;
extern bool winboardPonderAllowed;
extern bool winboardPlayNow;
extern bool winboardGameEnded;
extern bool winboardPostOutput;
extern bool winboardPlayingWhite;
extern int winboardSearchDepthLimit;
extern boost::thread winboardSearchThread;

bool WinboardProcessInput(const string&);

void WinboardSendMove(const string&);
void WinboardSendFeature(const int&);
void WinboardSendPong(const int&);
void WinboardSendIllegalMove(const string&);
void WinboardSendThinkingOutput(const string&);

void WinboardUserMove(const std::string&);
void WinboardEngineTime(const int&);
void WinboardUserTime(const int&);
void WinboardQuit();
void WinboardForce();
void WinboardGo();
void WinboardPlayOther();
void WinboardPlayNowFx();
void WinboardSetBoard(const std::string&);
void WinboardSetDepth(const int&);
void WinboardTimeLevel(const std::string&);
void WinboardTimeSet(const int&);

Move WinboardTranslateUserMove(const std::string& moveString);
void WinboardParseTimeString(const std::string& timeString, TimerType& type, int& numMoves, int& base, int& increment);


#endif // WINBOARD_H_INCLUDED
