#include "CardController.h"
bool iskeylastdown = false;
bool firstmouseclick = true;
std::list<Card*> lastselected = std::list<Card*>();
std::list<Card*> AlLCards = std::list<Card*>();
std::list<Card*> selectedcardsare = std::list<Card*>();

Card* selectedcard = nullptr;
bool havewecard = false;
int currentmode = 0;
CardController::CardController() {

}

int flippingstate = 0;

float mousexstart = 0;
float mouseystart = 0;

void CardController::FreeMouse() {
	selectedcard = nullptr;
	firstmouseclick = true;
	havewecard = false;
	for (Card* ix : selectedcardsare) {

		ix->pickedup = false;
	}
	selectedcardsare = std::list<Card*>();
	flippingstate = 0;
	mousexstart = 0;
	mouseystart = 0;
}

std::list<Card*> CardController::UpdateMouse(int cardwidth, int cardheight,float mousex,float mousey) {
	

	if (selectedcardsare.size() == 0) {
		
		// No cards have been picked up
		std::list<Card*> acopy = lastselected;

		acopy.reverse();

		for (Card* x : acopy) {

			if (mousex > x->xcord && mousex < x->xcord + cardwidth) {

				// X check has been passed!

				if (mousey > x->ycord && mousey < x->ycord + cardheight) {

					x->pickedup = true;
					x->xoff = mousex - x->xcord;
					x->yoff = mousey - x->ycord;
					selectedcardsare.push_back(x);
					if (this->allmodeis == 0) {
						break;
					}

				}

			}
			


		}


	}
	if (selectedcardsare.size() > 0 && flippingstate == 0) {
		mousexstart = mousex;
		mouseystart = mousey;
		for (Card* x : selectedcardsare) {

			x->xcord = mousex - x->xoff;
			x->ycord = mousey - x->yoff;

		}


	}

	if (selectedcardsare.size() > 0 && flippingstate == 1) {

		// Time to pile up good!

		float multiplier = (mousex - mousexstart) / selectedcardsare.size();

		
		float currentmul = 0;




		for (Card* x : selectedcardsare) {

			// Here stock piling will be done! :)


			x->xcord = mousexstart + currentmul;




			currentmul = currentmul + multiplier;




		}








	}




	return selectedcardsare;
}


Card* CardController::GetCard(int id) {

	for (Card* x : AlLCards) {

		if (x->id == id) {
			return x;
		}

	}



	return NULL;
}
std::list<Card*> CardController::getlist() {
	return lastselected;
}

std::list<Card*> CardController::getalllist() {
	return AlLCards;
}

void CardController::SetList(std::list<Card*> korten) {
	lastselected = korten;
}

void CardController::updateList(bool remove, Card* kortet) {
	if (remove) {
		lastselected.remove(kortet);

	}
	else {
		lastselected.push_back(kortet);
	}
}

void CardController::AddToAllList(Card* kortet) {
	AlLCards.push_back(kortet);
}

void CardController::AllMode() {
		
	switch (this->allmodeis) {

	case 0:

		this->allmodeis = 1;
		break;
	
	case 1:
		this->allmodeis = 0;
		break;
	default:
		this->allmodeis = 0;
		break;

	};

	if(this->allmodeis == 0 && selectedcardsare.size() > 1){
		// Need to pick card;

		selectedcardsare = std::list<Card*>();
		

	}
}

void CardController::FlipCommand() {

	for (Card* selectedcard : selectedcardsare) {
			int isnow = selectedcard->flipped;
			if (isnow == ourclientid) {
				selectedcard->flipped = 0;
			}
			if (isnow == 0) {
				selectedcard->flipped = ourclientid;
			}
			if (isnow != ourclientid && isnow != 0) {
				selectedcard->flipped = ourclientid;
			}
		
	}
}

void CardController::UnFlipCommand() {
	for (Card* selectedcard : selectedcardsare) {

				selectedcard->flipped = -1;
			
	}
}

void CardController::CenterMode() {

	for (Card* x : selectedcardsare) {

		x->xoff = 0;
		x->yoff = 0;

	}

}

void CardController::PileState() {

	int wasat = flippingstate;
	if (wasat == 0) {
		flippingstate = 1;
		this->CenterMode();
	}
	if (wasat == 1) {
		for (Card* x : selectedcardsare) {

			x->xoff = 0;
			x->yoff = 0;

		}
		flippingstate = 0;
		
	}

	


}