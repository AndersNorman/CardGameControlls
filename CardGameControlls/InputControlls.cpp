#include "InputControlls.h"
std::list<TextBox*> textboxes = std::list<TextBox*>();
Grafik* graphics;
int itemsids = 0;

bool InputControlls::SetGraphics(Grafik* graphi) {
	graphics = graphi;
	return true;
}

int InputControlls::CreateTextBox(int thex, int they, int thewidth, int theheight, int red, int green, int blue) {

	TextBox* newtextbox = new TextBox(graphics, thex, they, thewidth, theheight, red, green, blue);
	newtextbox->boxid = itemsids;
	itemsids = itemsids + 1;
	textboxes.push_back(newtextbox);

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

bool InputControlls::RemoveTextBox(int textboxid) {
	TextBox* theit = GetTextBox(textboxid);
	if (theit != NULL) {
		textboxes.remove(theit);
		return true;
	}

	return false;
}

void InputControlls::Update() {
	for (std::list<TextBox*>::iterator it = textboxes.begin(); it != textboxes.end();it++) {
		TextBox* theit = *it;
		theit->DrawTextBox();
		theit->Update();
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
		OutputDebugString(L"ERROR!");
	}
	return false;
}