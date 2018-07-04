#pragma once
#pragma once

//#include "Windows.h"
#include "iostream"
#include "string"
#include "d2d1_1.h"
#include "iostream"
#include "string"
#include "Dwrite.h"
#include "wchar.h"
class Grafik
{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
public:
	Grafik();
	~Grafik();

	bool Init(HWND windowHandle);

	void BeginDraw() { renderTarget->BeginDraw(); }
	void EndDraw() { renderTarget->EndDraw(); }
	ID2D1RenderTarget* GetRenderTarget() {
		return renderTarget;
	}

	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void DrawBox(float x, float y, float x2, float y2, float r, float g, float b, float a);
	void DrawTexts(std::string message, int x, int y, int green, int blue, int red);

};