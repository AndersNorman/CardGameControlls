#include "Button.h"
Grafik* thegrafiker = NULL;
ControllButton::ControllButton(Grafik* thegrafik, int thex, int they, int thewidth, int theheight, int red, int green, int blue, std::string starttext) {
	this->width = thewidth;
	this->height = theheight;
	this->x = thex;
	this->y = they;
	this->g = green;
	this->r = red;
	this->focused = false;
	this->b = blue;
	this->inputtextbox = starttext;
	thegrafiker = thegrafik;
}

void ControllButton::Update() {
	
}

void ControllButton::Draw(float x, float y, float width, float height) {
	thegrafiker->DrawBox(x, y, x + width, y + height, this->r, this->g, this->b, 255);
	thegrafiker->DrawTexts(inputtextbox, x, y, 0, 0, 0,24);
}
