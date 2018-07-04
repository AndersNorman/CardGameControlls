#pragma once
#include "string"
#include "iostream"
#include "list"
#include "TextBox.h"
#include "Graphics.h"
class InputControlls {

private:
	
	public:
		bool SetGraphics(Grafik* graphi);
		int CreateTextBox(int thex, int they, int thewidth, int theheight, int red, int green, int blue);
		bool RemoveTextBox(int theboxid);
		TextBox* GetTextBox(int textboxid);
		bool SetTextBoxFocus(int textboxid);
		void Update();

};