#include "graphics.h"

IDWriteFactory* pDWriteFactory = NULL;
std::mutex mtx;
int allticks = 0;
int ithinkdoit = 0;
HRESULT res;

void TimeLoop() {
	
		mtx.lock();
		allticks = allticks + 1;

		mtx.unlock();
		Sleep(10);
		TimeLoop();
	
}

std::thread* nytttrad = new std::thread(TimeLoop);





Grafik::Grafik() {
	factory = NULL;
	renderTarget = NULL;
}

Grafik::~Grafik() {
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (nytttrad) nytttrad->detach();
}

bool Grafik::Init(HWND windowHandle) {
	res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) {
		return false;
	}
	RECT rect;
	GetClientRect(windowHandle, &rect);
	factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget);

	res = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_ISOLATED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory)
	);


	if (res != S_OK) return false;
	
	return true;
}


int Grafik::GetTicks() {
	mtx.lock();
	int toreturn = allticks;
	mtx.unlock();
	return toreturn;
}

void Grafik::ClearScreen(float r, float g, float b) {
	renderTarget->Clear(D2D1::ColorF(r, g, b));

}
void Grafik::DrawCircle(float x, float y, float radius, float r, float g, float b, float a) {

	ID2D1SolidColorBrush* brush;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush, 3.0f);

	brush->Release();
}

void Grafik::DrawBox(float x, float y, float x2, float y2, float r, float g, float b, float a) {

	ID2D1SolidColorBrush* brush;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
	D2D1_RECT_F rectangle = D2D1::RectF(
		x, y, x2, y2
	);
	renderTarget->FillRectangle(&rectangle, brush);
	brush->Release();
}

void Grafik::DrawTexts(std::string message, int x, int y, float green, float blue, float red,float fontsize) {
	// IDWriteFactory
	IDWriteTextFormat* thetextformat;
	res = pDWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontsize, L"en-us", &thetextformat);
	std::wstring theunicode(message.begin(), message.end());
	ID2D1SolidColorBrush* brush;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(red, green, blue, 1), &brush);
	D2D1_RECT_F rectangle = D2D1::RectF(
		x, y, x + 1920, y + 1080
	);
	const wchar_t* szName = theunicode.c_str();
	D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();

	renderTarget->DrawTextW(szName, message.size() + 1, thetextformat, rectangle, brush);

}