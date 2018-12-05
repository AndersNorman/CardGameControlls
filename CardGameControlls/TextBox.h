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
	bool thisdown;
	bool focused;
	std::string inputtextbox;
	TextBox(Grafik* thegrafik, int thex, int they, int thewidth, int theheight, int red, int green, int blue,std::string starttext);
	std::string ReadKeyboard();
	void Update();
	void DrawTextBox(float x, float y, float width, float height);
};