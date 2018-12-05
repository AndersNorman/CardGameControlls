#include "Windows.h"
#include "SpriteSheet.h"
#pragma once

class Card {

public:
	int id;
	float xcord;
	float ycord;
	LPCWSTR texture;
	int flipped = 0;
	SpriteSheet* spritesheet;
	bool pickedup;
	float xoff;
	float yoff;
	Card(int id, float xcord2, float ycord2, LPCWSTR texture, int flipped, SpriteSheet*);



};