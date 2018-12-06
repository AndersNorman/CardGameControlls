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
#include "Connections.h"
#include "mutex"
#include "Mouse.h"
#include "Commands.h"
#include "MousePointers.h";




#pragma comment(lib,"ws2_32.lib")
#pragma comment (lib, "gdiplus.lib")
#pragma comment (lib, "Dwrite.lib")


int gamemode = 0;
InputControlls* controller = new InputControlls();
std::list<int> boxes = std::list<int>();
bool tocloseornot = false;
std::mutex thelock;
std::mutex ShutDown;

KBDLLHOOKSTRUCT kbdStruct;
HHOOK _hook;
Connections* NetWorkConnectionSource;
CardController* CardControllSource = new CardController();
std::list<Card*> toupdate = std::list<Card*>();
std::list<MousePointers*> mousepointers = std::list<MousePointers*>();
Card* currentselectedcard = nullptr;
bool istrueman = false;
std::list<Commands> tosend = std::list<Commands>();
int writemessage = -1;
Mouse* musen = nullptr;
int cursorwidth = 16;
int cursorheight = 20;

/*

The reason im doing this very slowly is cause of low motivation!
Im always abit slow. JK :)

*/

void ClientResize(HWND hWnd, int nWidth, int nHeight)
{

}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {

	if (uMsg == WM_DESTROY) {
		if (NetWorkConnectionSource != NULL) {
			ShutDown.lock();
			tocloseornot = true;
			ShutDown.unlock();
		}
		PostQuitMessage(0);
		return 0;
	}

	controller->TransferMessage(hwnd, uMsg, wParam, lParam);
	

	if (uMsg == WM_CHAR && writemessage == -1) {
		std::string s(1, char(wParam));
		if (s == "F" || s == "f") {
			thelock.lock();
			CardControllSource->FlipCommand();
			thelock.unlock();
		}
		if (s == "X" || s == "x") {
			thelock.lock();
			CardControllSource->UnFlipCommand();
			thelock.unlock();

		}
		if (s == "P" || s == "p") {

			CardControllSource->PileState();

		}

		controller->Log("Title bar is = " + std::to_string(musen->titlebar));
		controller->Log("Width bar is = " + std::to_string(musen->framewidth));

		if (s == "A" || s == "a") {

			CardControllSource->AllMode();
		}

		if (s == "N" || s == "n") {

			CardControllSource->CenterMode();

		}

	}





	
	
	
	if (uMsg == WM_KEYDOWN) {

		

		if (wParam == VK_RETURN) {

			if (gamemode == 2) {

				if (writemessage != -1) {
					TextBox* themessagebox = controller->GetTextBox(writemessage);


					std::string theinputmessage = themessagebox->inputtextbox;
					if (theinputmessage.find(";") == std::string::npos && theinputmessage.find(":") == std::string::npos) {

						Commands newcommand;
						newcommand.command = "SENDMESSAGE";
						newcommand.variables = std::list<std::string>();
						newcommand.variables.push_back(theinputmessage);
						thelock.lock();

						tosend.push_back(newcommand);

						thelock.unlock();
						


					}
					controller->RemoveBox(writemessage);
					writemessage = -1;

				}

				else {

					writemessage = controller->CreateTextBox(50, 50, 250, 50, 255, 255, 255, "");
					TextBox* theit = controller->GetTextBox(writemessage);
					theit->focused = true;

				}

			}


		}

	}

	
	if (uMsg == 256) {
			
	}
	if (uMsg == 1050) {

	}

	if (uMsg == WM_SIZING) {

		


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



void refreshme() {


	Commands updatelist;
	updatelist.command = "CARDUPDATECLIENT";
	updatelist.variables = std::list<std::string>();
	Commands updatemouselist;
	updatemouselist.command = "MOUSEUPDATECLIENT";
	updatemouselist.variables = std::list<std::string>();
	while (true) {
		ShutDown.lock();
		bool decide = tocloseornot;
		ShutDown.unlock();
		if (decide) {
			controller->Log("Thread made close command!");
			NetWorkConnectionSource->CloseConnection();
			delete controller;
			delete NetWorkConnectionSource;
			delete CardControllSource;


			return;

		}
		try {
			thelock.lock();
			std::list<Commands> toupdateis = tosend;
			std::list<MousePointers*> mouses = std::list<MousePointers*>();
			tosend = std::list<Commands>();
			tosend.push_front(updatemouselist);
			tosend.push_front(updatelist);
			Commands updatemousetoserver;
			updatemousetoserver.command = "MOUSEUPDATESERVER";
			updatemousetoserver.variables = std::list<std::string>();

			if (musen != nullptr) {

				updatemousetoserver.variables.push_back(std::to_string(musen->x));
				updatemousetoserver.variables.push_back(std::to_string(musen->y));
				tosend.push_back(updatemousetoserver);

			}

			std::list<Card*> allcardupdatecheck = CardControllSource->getlist();

			thelock.unlock();

			for (Card* ix : allcardupdatecheck) {

				if (ix->pickedup == true) {
					// Making command to send update notice to server - this way we save alot of bandwith and performance! =-)

					thelock.lock();

					Commands thecommandtomake;
					thecommandtomake.command = "UPDATECARD";
					thecommandtomake.variables = std::list<std::string>();


					thecommandtomake.variables.push_back(std::to_string(ix->id));
					thecommandtomake.variables.push_back(std::to_string(ix->xcord));
					thecommandtomake.variables.push_back(std::to_string(ix->ycord));
					thecommandtomake.variables.push_back(std::to_string(ix->flipped));
					toupdateis.push_back(thecommandtomake);


					thelock.unlock();
						

				}

			}

			
			std::list<Commands> itreturned = NetWorkConnectionSource->RegularUpdate(toupdateis);

			std::list<Card*> newversion = std::list<Card*>();
			for (Commands x : itreturned) {
				int cardid;
				float cardx;
				float cardy;
				int cardflipped;
				
				if (x.command == "UPDATEMOUSE") {
					MousePointers* muspekare = new MousePointers;
					int at = 0;
					for (std::string ix : x.variables) {
						
						if (at == 0) {
							muspekare->mousename = ix;
						}
						else {
							if (at == 1) {
								muspekare->mousex = std::stof(ix);
							}
							else {
								muspekare->mousey = std::stof(ix);
							}
							mouses.push_back(muspekare);
						}
						
						at = at + 1;
					}


					

				}



				if (x.command == "UPDATECARD") {
					int at = 0;
					for (std::string x : x.variables) {
						
						if (at == 0) {

							cardid = std::stoi(x);

						}

					
						if (at == 1) {
							cardx = std::stof(x);
						}
						if (at == 2) {
							cardy = std::stof(x);
						}
						if (at == 3) {
							cardflipped = std::stoi(x);

							Card* updatedcardrecieved = CardControllSource->GetCard(cardid);

							if (updatedcardrecieved != NULL) {

								if (updatedcardrecieved->pickedup == false) {
									updatedcardrecieved->xcord = cardx;

									updatedcardrecieved->ycord = cardy;
									updatedcardrecieved->flipped = cardflipped;
								}
								newversion.push_back(updatedcardrecieved);


							}
						}
						

					
						




						at = at + 1;

					}

					

				}

			}
			thelock.lock();
			mousepointers = mouses;
			thelock.unlock();
			if (newversion.size() > 0) {
				CardControllSource->SetList(newversion);
			}


		}
		catch (int e) {
			NetWorkConnectionSource->CloseConnection();
			controller->Log("Server crash!");
		}

	}


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
	HWND windowhandle = CreateWindowExW(NULL, L"MainWindow", L"Spel", WS_OVERLAPPEDWINDOW, 0, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
	ShowCursor(false);
	if (!windowhandle) return -1;

	Grafik* graphics = new Grafik();
	
	
	if (!graphics->Init(windowhandle)) {
		delete graphics;
		return -1;
	}

	ShowWindow(windowhandle, nCmdShow);



	MSG message;

	message.message = NULL;


	controller->SetGraphics(graphics);
	int InfoBoxesWidth = 250;
	int IpBox = controller->CreateTextBox(0, 0, InfoBoxesWidth, 50, 255, 255, 255,"127.0.0.1");
	int PortBox = controller->CreateTextBox(0, 0, InfoBoxesWidth, 50, 255, 255, 255, "54000");
	int NickBox = controller->CreateTextBox(0, 0, InfoBoxesWidth, 50, 255, 255, 255, "Nickname");

	int donebutton = controller->CreateButton(0, 255, InfoBoxesWidth, 50, 255, 255, 255, "Done","ConnectPlayerIpPortNick");
	boxes.push_back(IpBox);
	boxes.push_back(PortBox);
	boxes.push_back(NickBox);
	boxes.push_back(donebutton);
	controller->SetTextBoxFocus(IpBox);
	controller->SetWindowHandle(&windowhandle);	
	int cardwidth = 0;
	int cardheight = 0;

	musen = controller->ReturnMouse();
	std::string theflippedtexture = "card/card_back.png";
	std::string mousetexture = "card/cursor.png";
	SpriteSheet* flippedspritesheet = new SpriteSheet(std::wstring(theflippedtexture.begin(), theflippedtexture.end()).c_str(), graphics);
	SpriteSheet* Pointer = new SpriteSheet(std::wstring(mousetexture.begin(), mousetexture.end()).c_str(), graphics);
	while (message.message != WM_QUIT) {
		
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else {
			graphics->BeginDraw();

			graphics->ClearScreen(0, 0, 0);
			if (gamemode == 0) {
				graphics->DrawTexts("X = " + std::to_string(musen->x),0,0,255,0,0,24);
				graphics->DrawTexts("Y = " + std::to_string(musen->y),0,50,255,0,0,24);

				TranslateMessage(&message);
				DispatchMessage(&message);
				int theheight = 50;
				for (std::list<int>::iterator it = boxes.begin(); it != boxes.end();it++) {
					int thex = *it;
					controller->UpdateBox(thex, rect.right / 2 - (InfoBoxesWidth / 2), theheight, InfoBoxesWidth, 50);
					theheight = theheight + 100;
				}

			}
			if (gamemode == 2) {
				musen->Update();
				// Draw circle where mouse is for dev

				if (musen->down) {
					std::list<Card*> korten = CardControllSource->UpdateMouse(CardControllSource->CardWidth, CardControllSource->CardHeight, musen->x, musen->y);
				
				}
				else {					
					CardControllSource->FreeMouse();
				}
				thelock.lock();
			
				std::list<MousePointers*> mouses = mousepointers;
				std::list<Card*> thecards = CardControllSource->getlist();
				thelock.unlock();
				for (MousePointers* ix : mouses) {
					Pointer->Draw(ix->mousex, ix->mousey, ix->mousex + cursorwidth, ix->mousey + cursorheight, 1);

				}
				graphics->DrawCircle(musen->factrealx, musen->factrealy, 2, 0, 255, 0, 1);
				
				
				for (Card* thecard : thecards) {
					if (thecard->flipped == 0) {
						// None flipped card
						thecard->spritesheet->Draw(thecard->xcord, thecard->ycord, thecard->xcord + CardControllSource->CardWidth, thecard->ycord + CardControllSource->CardHeight, 255);
					}
					else {
						if (thecard->flipped == NetWorkConnectionSource->clientid) {
							// Player flipped card
							thecard->spritesheet->Draw(thecard->xcord, thecard->ycord, thecard->xcord + CardControllSource->CardWidth, thecard->ycord + CardControllSource->CardHeight, 0.3);

						}
						else {
							// Flipped card
							if (thecard->flipped == -1) {
								flippedspritesheet->Draw(thecard->xcord, thecard->ycord, thecard->xcord + CardControllSource->CardWidth, thecard->ycord + CardControllSource->CardHeight, 1);
							}
							else {
								flippedspritesheet->Draw(thecard->xcord, thecard->ycord, thecard->xcord + CardControllSource->CardWidth, thecard->ycord + CardControllSource->CardHeight, 0.3);
							}

						}

					}

				}
				

			


			

			}
			if (gamemode == 1) {
				try {
					gamemode = 2;

					//Connections(std::string, std::string, CardController *, std::mutex *);
					NetWorkConnectionSource = new Connections(controller->GetTextBox(IpBox)->inputtextbox, controller->GetTextBox(PortBox)->inputtextbox, CardControllSource, &thelock);
					if (NetWorkConnectionSource->didconnect) {



						controller->Log("Valid connection!");
						NetWorkConnectionSource->SendCommand("SET PLAYERNAME");
						controller->Log(NetWorkConnectionSource->SendCommand(controller->GetTextBox(NickBox)->inputtextbox));
						std::string commandreturnwidth = NetWorkConnectionSource->SendCommand("CARD WIDTH");
						std::string commandreturnheight = NetWorkConnectionSource->SendCommand("CARD HEIGHT");

						CardControllSource->CardHeight = stoi(commandreturnwidth);
						CardControllSource->CardWidth = stoi(commandreturnheight);
						controller->Log("Card height = " + commandreturnheight);
						controller->Log("Card width = " + commandreturnwidth);
						NetWorkConnectionSource->createlist(graphics);

						std::list<Card*> thecards = CardControllSource->getalllist();
						for (std::list<Card*>::iterator it = thecards.begin(); it != thecards.end();it++) {

							Card* thecard = *it;
							controller->Log("Card with the id " + std::to_string(thecard->id));
						}
						CardControllSource->ourclientid = NetWorkConnectionSource->clientid;

						controller->Log("Client id is == " + std::to_string(CardControllSource->ourclientid));

						std::list<int> copy = boxes;

						boxes = std::list<int>();
						for (std::list<int>::iterator it = copy.begin(); it != copy.end();it++) {
							int thex = *it;
							controller->RemoveBox(thex);
						}

						std::thread* nytttrad = new std::thread(refreshme);




					}
					else {

						controller->Log("Invalid connection!");
						gamemode = 0;
					}
				}
				catch(int e){
					controller->GetTextBox(IpBox)->inputtextbox = "";
					controller->GetTextBox(PortBox)->inputtextbox = "";
					controller->GetTextBox(NickBox)->inputtextbox = "";
				}
				
			}
			

			graphics->DrawTexts("X = " + std::to_string(musen->x), 0, 0, 255, 0, 0, 24);
			graphics->DrawTexts("Y = " + std::to_string(musen->y), 0, 50, 255, 0, 0, 24);
			if (CardControllSource->allmodeis == 1) {
				graphics->DrawTexts("Multiple select on", 0, 100, 255, 255, 255, 24);
			}

			if (CardControllSource->allmodeis == 0) {
				graphics->DrawTexts("Single select on", 0, 100, 255, 255, 255, 24);

			}



			controller->Update();


			Pointer->Draw(musen->x, musen->y, musen->x + cursorwidth, musen->y + cursorheight, 1);

			graphics->EndDraw();


		}

	}

}


void InputControlls::ButtonPressed(int x) {
	
	// Currently the only button i have is a connection button....
	ControllButton* thebutton = controller->GetButton(x);
	if (thebutton->identifytext == "ConnectPlayerIpPortNick") {

		controller->Log("ConnectPlayerIpPortNick was pressed!");
		gamemode = 1;



	}
		

}