#pragma once
#include "wincodec.h"
#include "Graphics.h"

class SpriteSheet {
public:

	Grafik* gfx;
	ID2D1Bitmap* bmp;

	SpriteSheet(LPCWSTR filename, Grafik* gfx);



	~SpriteSheet();

	void Draw(float x, float y, float x2, float y2, float opicity);

};

