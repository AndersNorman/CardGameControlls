#pragma once
#include "string"
#include "ctime"
#include "iostream"
#include "fstream"
#include "list"
#include "mutex"
#include "TextBox.h"
#include "Button.h"
#include "Graphics.h"
#include "Mouse.h"
#include "Windows.h"
#include "sstream"


class InputControlls {
public:


		bool SetGraphics(Grafik* graphi);
		bool SetTextBoxFocus(int textboxid);
		bool SetWindowHandle(HWND* windowHandle);
		bool RemoveBox(int theboxid);

		bool UpdateBox(int boxid, int x, int y, int width, int height);
		int movecard();

		bool Log(std::string message);
		int CreateTextBox(int thex, int they, int thewidth, int theheight, int red, int green, int blue, std::string starttext);
		int CreateButton(int thex, int they, int thewidth, int theheight, int red, int green, int blue, std::string starttext, std::string textidentify);
		void Update();
		void UpdateFocus();
		void TransferMessage(HWND hwnd, UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		Mouse* ReturnMouse();

		int mousex = 0;
		int mousey = 0;
		int mousedown = -1;
		TextBox* GetTextBox(int textboxid);
		ControllButton* GetButton(int textboxid);
		std::string GetLogPrefix();
		void ButtonPressed(int nValue);

};



