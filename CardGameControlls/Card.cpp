#include "Card.h"

Card::Card(int id, float xcord2, float ycord2, LPCWSTR texture, int flipped, SpriteSheet* sheet) {
	this->id = id;
	this->xcord = xcord2;
	this->ycord = ycord2;
	this->texture = texture;
	this->flipped = flipped;
	this->spritesheet = sheet;
	this->pickedup = false;
}