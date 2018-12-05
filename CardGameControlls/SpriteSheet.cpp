#include "SpriteSheet.h"
SpriteSheet::SpriteSheet(LPCWSTR filename, Grafik* gfx) {

	this->gfx = gfx;
	bmp = NULL;
	HRESULT hr;

	IWICImagingFactory *winFactory = NULL;
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&winFactory);


	IWICBitmapDecoder *winDecoder = NULL;

	hr = winFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &winDecoder);

	IWICBitmapFrameDecode* wicFrame = NULL;
	hr = winDecoder->GetFrame(0, &wicFrame);

	IWICFormatConverter* wicConverter = NULL;
	hr = winFactory->CreateFormatConverter(&wicConverter);


	hr = wicConverter->Initialize(
		wicFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL, 0.0, WICBitmapPaletteTypeCustom
	);

	D2D1_POINT_2F imageCenter = D2D1::Point2F(
		50, 50
	);

	gfx->GetRenderTarget()->CreateBitmapFromWicBitmap(
		wicConverter,
		NULL,
		&bmp
	);
	if (winFactory) { winFactory->Release(); }
	if (winDecoder) {
		winDecoder->Release();
	}
	if (wicConverter) {
		wicConverter->Release();
	}
	if (wicFrame) {
		wicFrame->Release();
	}

}

SpriteSheet::~SpriteSheet() {
	bmp->Release();
}

void SpriteSheet::Draw(float x, float y, float x2, float y2, float opicity) {

	gfx->GetRenderTarget()->DrawBitmap(
		bmp,
		D2D1::RectF(x, y, x2, y2),
		opicity,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(0, 0, bmp->GetSize().width, bmp->GetSize().height)
	);
}