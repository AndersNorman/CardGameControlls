#include "TextBox.h"
#include "conio.h"
std::string inputtextbox = "";
Grafik* thegrafikt;
TextBox::TextBox(Grafik* thegrafik, int thex, int they, int thewidth, int theheight, int red, int green, int blue) {
	this->width = thewidth;
	this->height = theheight;
	this->x = thex;
	this->y = they;
	this->g = green;
	this->r = red;
	this->focused = false;
	this->b = blue;
	thegrafikt = thegrafik;
}



int getkey() {

	int respond = -1;
		// This checks if the window is focused
	if (GetForegroundWindow() != GetConsoleWindow()) {

		for (int i = 1; i < 255; ++i) {
			// The bitwise and selects the function behavior (look at doc)
			if (GetAsyncKeyState(i) & 0x07) {
				respond = i;
			}

		}
	}

	return respond;
}

void TextBox::Update() {
	if (this->focused) {
		int thekey = getkey();
		if (thekey >= 0) {
			std::string s(1, char(thekey));
			OutputDebugString(std::wstring(s.begin(), s.end()).c_str());
			if (thekey != 8 && thekey > 32 && thekey < 126) {
				if (!s.empty()) {
					inputtextbox = inputtextbox + s;
				}
			}
			if (thekey == 8) {
				int thesizeofstring = inputtextbox.size();
				if (thesizeofstring > 0) {
					inputtextbox = inputtextbox.substr(0, thesizeofstring - 1);
				}
			}
		}
	}
}

void TextBox::DrawTextBox() {
	thegrafikt->DrawBox(this->x, this->y, this->x + this->width, this->y + this->height, this->r, this->g, this->b, 255);
	thegrafikt->DrawTexts(inputtextbox, this->x, this->y, 0, 0, 0);
}
