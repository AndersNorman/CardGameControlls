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
		int alltogheter = 0;
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_MENU)) {
			alltogheter = 256 + 256;
			alltogheter = alltogheter * 3;
		}
		if (GetAsyncKeyState(VK_SHIFT) && !GetAsyncKeyState(VK_MENU)) {
			alltogheter = alltogheter + 256;
		}
		for (int i = 1; i < 255; ++i) {
			// The bitwise and selects the function behavior (look at doc)
			
			if (i != 164 && i != 160 && i != 16 && i != 18 && i != 162 && i != 17 && i != 8) {
				if (GetAsyncKeyState(i) & 0x07) {

					alltogheter = alltogheter + i;
				}
			}
			if (i == 8) {
				if (GetAsyncKeyState(8) & 0x07) {
					int thesizeofstring = inputtextbox.size();
					if (thesizeofstring > 0) {
						inputtextbox = inputtextbox.substr(0, thesizeofstring - 1);
					}
				}
			}
			
			// if ctrl + alt = 256 + 256 * 3
			// shift = 256


		}

		
		DWORD dwThreadID = GetCurrentThreadId();
		HKL hCurKeyboard = GetKeyboardLayout(dwThreadID);
		SHORT keysdown = VkKeyScanEx('1',hCurKeyboard);
		//inputtextbox = std::to_string(alltogheter) + " " + std::to_string(keysdown);

		for (int i = 1; i < 255; ++i) {
			// The bitwise and selects the function behavior (look at doc)
			keysdown = VkKeyScanEx(i, hCurKeyboard);
			if (std::stoi(std::to_string(keysdown)) == alltogheter) {
				std::string s(1, char(i));
				inputtextbox = inputtextbox + s;
			}
		}

	}

	
	
}

void TextBox::DrawTextBox() {
	thegrafikt->DrawBox(this->x, this->y, this->x + this->width, this->y + this->height, this->r, this->g, this->b, 255);
	thegrafikt->DrawTexts(inputtextbox, this->x, this->y, 0, 0, 0);
}
