#pragma once
#include "Graphics.h"

#include "iostream"

#include "list"

#include "string"
class TextBox {

public:
	int boxid;

	int width;
	int height;

	int x;
	int y;

	int r;
	int g;
	int b;
	bool focused;

	TextBox(Grafik* thegrafik, int thex, int they, int thewidth, int theheight, int red, int green, int blue);
	void Update();
	void DrawTextBox();
};