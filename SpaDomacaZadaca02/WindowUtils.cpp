#include "WindowUtils.h"

bool WindowUtils::windowFocus = true;

bool WindowUtils::isWindowFocused()
{
	return windowFocus;
}

void WindowUtils::setWindowFocus(bool windowFocus)
{
	WindowUtils::windowFocus = windowFocus;
}
