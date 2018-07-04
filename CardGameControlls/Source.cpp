#include "WinSock2.h"
#include "WS2tcpip.h"
#include "Windows.h"
#include "d2d1_1.h"
#include "iostream"
#include "list"
#include "fstream"
#include "time.h"
#include "Vector"
#include "thread"
#include "tchar.h"
#include "mutex"
#include "conio.h"
#include "Time.h"
#include "gdiplus.h"
#include "assert.h"
#include "InputControlls.h"

#include "Graphics.h"


#pragma comment(lib,"ws2_32.lib")
#pragma comment (lib, "gdiplus.lib")
#pragma comment (lib, "Dwrite.lib")


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}

	/*
	if (uMsg == WM_PAINT) {
	graphics->BeginDraw();

	graphics->ClearScreen(0, 0, 0);
	graphics->DrawCircle(100.0f, 100.0f, 25.0f, 255.0f,255.0f, 255.0f, 255.0f);

	graphics->EndDraw();
	}
	*/

	DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow) {

	WNDCLASSEX windowclass;
	ZeroMemory(&windowclass, sizeof(WNDCLASSEX));
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclass.hInstance = hInstance;
	windowclass.lpfnWndProc = WindowProc;
	windowclass.lpszClassName = L"MainWindow";
	windowclass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&windowclass);
	RECT rect = { 0,0,1920,1080 };
	AdjustWindowRect(&rect, WS_TILEDWINDOW, FALSE);
	DWORD saken = DWORD("MainWindow");
	HWND windowhandle = CreateWindowEx(NULL, L"MainWindow", L"Spel", WS_OVERLAPPEDWINDOW, 10, 10, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);

	if (!windowhandle) return -1;

	Grafik* graphics = new Grafik();
	
	
	if (!graphics->Init(windowhandle)) {
		delete graphics;
		return -1;
	}

	ShowWindow(windowhandle, nCmdShow);



	MSG message;

	message.message = NULL;


	InputControlls* controller = new InputControlls();
	controller->SetGraphics(graphics);

	int TextBoxer = controller->CreateTextBox(0, 0, 250, 50, 255, 255, 255);

	controller->SetTextBoxFocus(TextBoxer);

	while (message.message != WM_QUIT) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&message);
		}
		else {

			graphics->BeginDraw();

			graphics->ClearScreen(0, 0, 0);

			controller->Update();

			graphics->EndDraw();


		}

	}
}