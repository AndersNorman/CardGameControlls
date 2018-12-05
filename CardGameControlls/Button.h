#pragma once
#include "Graphics.h"

#include "iostream"

#include "list"

#include "string"
class ControllButton {

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
	std::string inputtextbox;
	std::string identifytext;
	ControllButton(Grafik* thegrafik, int thex, int they, int thewidth, int theheight, int red, int green, int blue, std::string starttext);
	void Update();
	void Draw(float x, float y, float width, float height);
};