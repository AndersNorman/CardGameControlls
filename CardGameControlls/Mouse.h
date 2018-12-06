#pragma once
#include "iostream"
#include "list"
#include "Windows.h"

class Mouse{
public:
	HWND* windowHandle;
	int x;
	int factrealx;
	bool down;
	int y;
	int factrealy;
	void Update();
	float titlebar;
	float framewidth;
};