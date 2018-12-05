#pragma once
#include "wincodec.h"
#include "Graphics.h"

class SpriteSheet {
	Grafik* gfx;
	ID2D1Bitmap* bmp;

public:
	SpriteSheet(LPCWSTR filename, Grafik* gfx);

	~SpriteSheet();

	void Draw(float x, float y, float x2, float y2, float opicity);

};

