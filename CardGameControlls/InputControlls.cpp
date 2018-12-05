#include "InputControlls.h"

Mouse mouse;
std::mutex mtxlock;
HWND* WindowHandle = nullptr;
std::thread nytradd;
std::ofstream LogFile("Logs.txt");
std::list<TextBox*> textboxes = std::list<TextBox*>();
std::list<ControllButton*> buttones = std::list<ControllButton*>();
int amount = 0;
int keyregister = 0;
bool doregisterkey = true;
Grafik* graphics = NULL;
bool lastdown = false;
int cardheld = -1;
std::string allowcharacters = "1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM.";

bool InputControlls::SetGraphics(Grafik* graphi) {	
	graphics = graphi;
	return true;
}

bool InputControlls::SetWindowHandle(HWND* Windowhandle) {
	mouse.x = 0;
	mouse.y = 0;
	mouse.windowHandle = Windowhandle;
	WindowHandle = Windowhandle;
	return true;
}

int realx( int currentx)
{

	if (WindowHandle != nullptr) {

		RECT fanster;
		RECT realfanster;

		GetWindowRect(*WindowHandle, &fanster);
		GetClientRect(*WindowHandle, &realfanster);

		int border_thickness = GetSystemMetrics(SM_CXSIZEFRAME);



		float topframe = (fanster.bottom - fanster.top) - (realfanster.bottom - realfanster.top) - 10;
		float realx = (float)currentx - (float)fanster.left;

		float thewidth = (float)fanster.right - (float)fanster.left;
		float theheight = (float)fanster.bottom - (float)fanster.top;

		float widthfactor = 1920 / thewidth;

		float heightfactor = 1080 / theheight;



		realx = realx * widthfactor;


		return realx;

	}
	return -1;
}

int realy( int currentx)
{
	if (WindowHandle != nullptr) {
		RECT fanster;
		RECT realfanster;

		GetWindowRect(*WindowHandle, &fanster);
		GetClientRect(*WindowHandle, &realfanster);

		int border_thickness = GetSystemMetrics(SM_CXSIZEFRAME);



		float topframe = (fanster.bottom - fanster.top) - (realfanster.bottom - realfanster.top) - 10;
		float realy = (float)currentx - (float)fanster.top;
		realy = realy - topframe;

		float thewidth = (float)fanster.right - (float)fanster.left;
		float theheight = (float)fanster.bottom - (float)fanster.top;

		float widthfactor = 1920 / thewidth;

		float heightfactor = 1080 / theheight;

		realy = realy * heightfactor;



		return realy;

	}
	return -1;
}

int InputControlls::CreateTextBox(int thex, int they, int thewidth, int theheight, int red, int green, int blue, std::string starttext) {

	TextBox* newtextbox = new TextBox(graphics, thex, they, thewidth, theheight, red, green, blue, starttext);
	newtextbox->boxid = amount;
	amount++;
	textboxes.push_back(newtextbox);
	this->Log("Created Textbox!");

	return newtextbox->boxid;
}

int InputControlls::CreateButton(int thex, int they, int thewidth, int theheight, int red, int green, int blue, std::string starttext,std::string textidentify) {

	ControllButton* newtextbox = new ControllButton(graphics, thex, they, thewidth, theheight, red, green, blue, starttext);
	newtextbox->boxid = amount;
	newtextbox->identifytext = textidentify;
	amount++;
	buttones.push_back(newtextbox);
	this->Log("Created button!");

	return newtextbox->boxid;
}

TextBox* InputControlls::GetTextBox(int textboxid) {

	for (std::list<TextBox*>::iterator it = textboxes.begin(); it != textboxes.end();it++) {
		TextBox* theit = *it;
		if (theit->boxid == textboxid) {
			return theit;
		}
	}
	TextBox* failed = NULL;
	return failed;

	// User failed to give valid id if its here!
}

ControllButton* InputControlls::GetButton(int textboxid) {

	for (std::list<ControllButton*>::iterator it = buttones.begin(); it != buttones.end();it++) {
		ControllButton* theit = *it;
		if (theit->boxid == textboxid) {
			return theit;
		}
	}
	ControllButton* failed = NULL;
	return failed;

	// User failed to give valid id if its here!
}

std::string InputControlls::GetLogPrefix() {
	std::string prefix;
	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);
	prefix = std::to_string((now->tm_year + 1900)) + "-" + std::to_string((now->tm_mon + 1)) + "-" + std::to_string((now->tm_mday));
	prefix = prefix + " " + std::to_string(now->tm_hour);
	prefix = prefix + ":" + std::to_string(now->tm_min);
	prefix = prefix + ":" + std::to_string(now->tm_sec);
	return prefix;
}



bool InputControlls::RemoveBox(int textboxid) {
	ControllButton* theit = GetButton(textboxid);
	if (theit != NULL) {
		buttones.remove(theit);
		return true;
	}

	TextBox* theit2 = GetTextBox(textboxid);
	if (theit2 != NULL) {
		textboxes.remove(theit2);
		return true;
	}

	return false;
}


Mouse* InputControlls::ReturnMouse() {
	return &mouse;
}

void InputControlls::Update() {
	int currentgap = 50;
	for (std::list<TextBox*>::iterator it = textboxes.begin(); it != textboxes.end();it++) {
		TextBox* theit = *it;
	


			theit->DrawTextBox(theit->x, theit->y, theit->width, theit->height);

		currentgap = currentgap + 50;
		theit->Update();
	}
	for (std::list<ControllButton*>::iterator it = buttones.begin(); it != buttones.end();it++) {
		ControllButton* theit = *it;

		theit->Draw(theit->x, theit->y, theit->width, theit->height);
		theit->Update();
	}
	mouse.Update();
	
	UpdateFocus();



}

int InputControlls::movecard() {

	if (cardheld == -1) {
		// Find a card to use

	}

	return -1; // No card at all!
}

bool InputControlls::UpdateBox(int textboxid, int x, int y, int width, int height) {

	TextBox* theit = GetTextBox(textboxid);
	if (theit != NULL) {
		theit->x = x;
		theit->y = y;
		theit->width = width;
		theit->height = height;
		return true;
	}
	else {
		
		
	}

	ControllButton* thebutton = GetButton(textboxid);
	if (thebutton != NULL) {
		thebutton->x = x;
		thebutton->y = y;
		thebutton->width = width;
		thebutton->height = height;
		return true;
	}
	Log("ERROR!");
	return false;

}

void InputControlls::UpdateFocus() {

	if (mouse.down) {
		for (std::list<TextBox*>::iterator it = textboxes.begin(); it != textboxes.end();it++) {
			TextBox* theit = *it;
			if (mouse.x > theit->x && mouse.x < theit->x + theit->width) {
				if (mouse.y > theit->y && mouse.y < theit->y + theit->height) {
					SetTextBoxFocus(theit->boxid);
				}
			}
		}
	}
	if (mouse.down) {
		for (std::list<ControllButton*>::iterator it = buttones.begin(); it != buttones.end();it++) {
			ControllButton* theit = *it;
			if (mouse.x > theit->x && mouse.x < theit->x + theit->width) {
				if (mouse.y > realy(theit->y) && mouse.y < theit->y + theit->height) {
					this->ButtonPressed(theit->boxid);
				}
			}
		}
	}
}

bool InputControlls::SetTextBoxFocus(int textboxid) {
	for (std::list<TextBox*>::iterator it = textboxes.begin(); it != textboxes.end();it++) {
		TextBox* theit = *it;
		theit->focused = false;
	}
	TextBox* thetextbox = GetTextBox(textboxid);
	if (thetextbox != NULL) {
		thetextbox->focused = true;
		return true;
	}
	else {
		Log("Error!");
	}
	return false;
}

bool InputControlls::Log(std::string message) {

	OutputDebugString(std::wstring(message.begin(), message.end()).c_str());
	OutputDebugString(L"\n");
	LogFile << (GetLogPrefix() + " " + message) << std::endl;
	LogFile.flush();
	return true;
}



void InputControlls::TransferMessage(HWND hwnd, UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	//this->Log(std::to_string(uMsg));
	
	if (uMsg == WM_KEYDOWN) {
		lastdown = true;
	}
	if (uMsg == WM_KEYUP) {
		lastdown = false;
	}
		
		if (uMsg == WM_CHAR && char(wParam) != VK_TAB && char(wParam) != VK_RETURN) {
			for (std::list<TextBox*>::iterator it = textboxes.begin(); it != textboxes.end();it++) {
				TextBox* theit = *it;
				if (theit->focused && lastdown) {
					lastdown = false;

					int theparam = wParam;
					int thesizeofstring = theit->inputtextbox.size();
					if (theparam == 8 && thesizeofstring > 0) {
						theit->inputtextbox = theit->inputtextbox.substr(0, thesizeofstring - 1);
					}
					else {

						std::string s(1, char(wParam));
						if (allowcharacters.find(s) != std::string::npos) {
							theit->inputtextbox = theit->inputtextbox + s;
						}
					}
				

				}
			}
			
		}
		
		
		
	

}