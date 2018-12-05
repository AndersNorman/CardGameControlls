#include "TextBox.h"
#include "conio.h"
Grafik* thegrafikt;
int lastpressed = 0;
TextBox::TextBox(Grafik* thegrafik, int thex, int they, int thewidth, int theheight, int red, int green, int blue,std::string starttext) {
	this->width = thewidth;
	this->height = theheight;
	this->x = thex;
	this->y = they;
	this->g = green;
	this->r = red;
	this->focused = false;
	this->b = blue;
	this->inputtextbox = starttext;
	this->thisdown = false;
	thegrafikt = thegrafik;
}


std::string TextBox::ReadKeyboard() {
	

	//inputtextbox = std::to_string(alltogheter) + " " + std::to_string(keysdown);
	/*
	for (int i = 1; i < 255; ++i) {
		// The bitwise and selects the function behavior (look at doc)
		keysdown = VkKeyScanEx(i, hCurKeyboard);
		if (std::stoi(std::to_string(keysdown)) == alltogheter) {
			std::string s(1, char(i));
			this->inputtextbox = this->inputtextbox + s;
			return s;
		}
	}
	// Don't know why this wont work, you would think GetAsyncKeyState wouldn't return 0 if the keys were down.
	this->thisdown = false;
	*/
	return "";
}

void TextBox::Update() {
	

	if (this->focused) {
		ReadKeyboard();
	}

	
	
}

void TextBox::DrawTextBox(float x, float y, float width, float height) {
	thegrafikt->DrawBox(x, y, x + width, y + height, this->r, this->g, this->b, 255);
	if (this->focused) {

		thegrafikt->DrawTexts(inputtextbox, x, y, 0, 0, 0,24);


	}
	else {
		thegrafikt->DrawTexts(inputtextbox, x, y,(float)0.41,(float)0.41, (float)0.41,20);

	}

}
