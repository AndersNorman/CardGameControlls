#pragma once
#include "list"
#include "iostream"
#include "string"
#include "Card.h"
#include "Mouse.h"
class CardController {


public:
	CardController();

	int ourclientid;
	std::list<Card*> getlist();

	std::list<Card*> getalllist();
	int allmodeis;
	std::list<Card*>UpdateMouse(int cardwidth, int cardheight, float mousex, float mousey);

	

	void SetList(std::list<Card*> korten);

	Card* GetCard(int id);

	void FreeMouse();

	void updateList(bool remove, Card* kortet);


	void AddToAllList(Card* kortet);

	void CenterMode();

	void PileState();

	void AllMode();

	void FlipCommand();

	void UnFlipCommand();

	int CardWidth = 0;
	int CardHeight = 0;


};