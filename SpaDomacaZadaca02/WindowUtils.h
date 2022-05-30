#pragma once

class WindowUtils
{
private:
	static bool windowFocus;
public:
	static bool isWindowFocused();
	static void setWindowFocus(bool windowFocus);
};
