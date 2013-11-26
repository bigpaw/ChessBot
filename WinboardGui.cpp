#include "Include.h"

bool winboardIsEngineSearching = false;
bool winboardIsForcedMode = false;
bool winboardPonderAllowed = true;
bool winboardPlayNow = false;
bool winboardGameEnded = true;
bool winboardPostOutput = true;
bool winboardPlayingWhite = false;
int winboardSearchDepthLimit = 0;
boost::thread winboardSearchThread;

//{ Input

bool WinboardProcessInput(const std::string& inputString)
{
	size_t spaceIndex = inputString.find(" ");
	string command = inputString.substr(0, spaceIndex);
	string parameter = inputString.substr(spaceIndex + 1, inputString.length() - spaceIndex - 1);

	if (command ==  "new")
	{
		winboardGameEnded = false;
		ResetEngine();
		InitializeEngine(NewGameFenString);
	}
	else if (command ==  "usermove")
	{
		WinboardUserMove(parameter);
	}
	else if (command ==  "protover")
	{
		WinboardSendFeature(boost::lexical_cast<int>(parameter));
	}
	else if (command ==  "quit")
	{
		WinboardQuit();
		ResetEngine();
		return true;
	}
	else if (command ==  "force")
	{
		WinboardForce();
		winboardIsForcedMode = true;
	}
	else if (command ==  "xboard")
	{
	}
	else if (command ==  "accepted")
	{
	}
	else if (command ==  "rejected")
	{
	}
	else if (command ==  "variant")
	{
	}
	else if (command ==  "random")
	{
	}
	else if (command ==  "go")
	{
		winboardIsForcedMode = false;
		WinboardGo();
	}
	else if (command ==  "playother")
	{
		winboardIsForcedMode = false;
		WinboardPlayOther();
	}
	else if (command ==  "level")
	{
		WinboardTimeLevel(parameter);
	}
	else if (command ==  "st")
	{
		WinboardTimeSet(boost::lexical_cast<int>(parameter));
	}
	else if (command ==  "sd")
	{
		WinboardSetDepth(boost::lexical_cast<int>(parameter));
	}
	else if (command ==  "time")
	{
		WinboardEngineTime(boost::lexical_cast<int>(parameter));
	}
	else if (command ==  "otim")
	{
		WinboardUserTime(boost::lexical_cast<int>(parameter));
	}
	else if (command ==  "ping")
	{
		WinboardSendPong(boost::lexical_cast<int>(parameter));
	}
	else if (command ==  "?")
	{
		winboardPlayNow = true;
		WinboardPlayNowFx();
		winboardPlayNow = false;
	}
	else if (command ==  "draw")
	{
	}
	else if (command ==  "result")
	{
		winboardGameEnded = true;
	}
	else if (command ==  "setboard")
	{
		WinboardSetBoard(parameter);
	}
	else if (command ==  "edit")
	{
	}
	else if (command ==  "hard")
	{
		winboardPonderAllowed = true;
	}
	else if (command ==  "easy")
	{
		winboardPonderAllowed = false;
	}
	else if (command ==  "hint")
	{
	}
	else if (command ==  "bk")
	{
	}
	else if (command ==  "undo")
	{
	}
	else if (command ==  "remove")
	{
	}
	else if (command ==  "post")
	{
		winboardPostOutput = true;
	}
	else if (command ==  "nopost")
	{
		winboardPostOutput = false;
	}
	else if (command ==  "analyze")
	{
	}
	else if (command ==  "name")
	{
	}
	else if (command ==  "pause")
	{
	}
	else if (command ==  "resume")
	{
	}
	else if (command ==  "computer")
	{
	}
	else if (command ==  "rating")
	{
	}
	else if (command ==  "ics")
	{
	}
	else
	{
		std::cout << "Unknown command" << std::endl;
		throw "Unknown Winboard command.";
	}

	return false;
}

//}

//{ Output

