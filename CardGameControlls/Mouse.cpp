#pragma once
#include "Mouse.h"

void Mouse::Update()
{
	RECT fanster;
	RECT realfanster;

	GetWindowRect(*this->windowHandle, &fanster);
	GetClientRect(*this->windowHandle, &realfanster);

	int border_thickness = GetSystemMetrics(SM_CXSIZEFRAME);


	POINT pmouse;

	if (GetCursorPos(&pmouse)) {
		

		if (ScreenToClient(*this->windowHandle, &pmouse)) {

			float realx = pmouse.x;
			float realy = pmouse.y;

			float screenwidth = realfanster.right - realfanster.left;
			float screenheight = realfanster.bottom - realfanster.top;

			float screenwidthfactor = 1920 / screenwidth;
			float screenheightfactor = 1080 / screenheight;






			this->factrealx = realx;
				this->factrealy = realy;


				realx = realx * screenwidthfactor;

				realy = realy * screenheightfactor;

				this->x = realx;
				this->y = realy;

				this->down = (GetAsyncKeyState(VK_LBUTTON));
		}
	}

}
