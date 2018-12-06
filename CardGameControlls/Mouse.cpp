#pragma once
#include "Mouse.h"

void Mouse::Update()
{
	RECT fanster;
	RECT realfanster;


	GetWindowRect(*this->windowHandle, &fanster);
	GetClientRect(*this->windowHandle, &realfanster);


	float border_thickness = GetSystemMetrics(SM_CYCAPTION);

	float realwindowthickness = fanster.right - fanster.left;
	float realwindowheight = fanster.bottom - fanster.top;
	float clientwidth = realfanster.right - realfanster.left;
	float clientheight = realfanster.bottom - realfanster.top;

	 this->titlebar = realwindowheight - clientheight;
	 this->framewidth = realwindowthickness - clientwidth;
	


	POINT pmouse;


	if (GetCursorPos(&pmouse)) {
		

		
		


		if (ScreenToClient(*this->windowHandle, &pmouse)) {
			
			float yfactor = 1080 / clientheight;
			float xfactor = 1920 / clientwidth;
			this->y = (pmouse.y) * yfactor;
			this->x = pmouse.x * xfactor;

		}

		
		
	}
	this->down = (GetAsyncKeyState(VK_LBUTTON));

}
