#ifndef __PBDEVICECONTEXT_H__
#define	__PBDEVICECONTEXT_H__

#include <string>
#include <Windows.h>
#include "PBSprite.h"
#include "PBRect.h"

namespace PB { namespace Windows {

class __declspec(dllexport) device_context : public virtual object
{
private:
	Graphics*				_gdip;
	HWND					_hwnd;
	HDC						_dc, _bufferDC;
	HBITMAP					_bitmap, _prevBitmap;
	HBRUSH					_backgroundBrush;
	RECT					_capturedRect;
	float					_scale;

public:
	device_context(HWND hwnd, COLORREF backgroundColor = RGB(255, 255, 255), float scale = 1.0f);
	~device_context();

public:
	void					drawText(int x, int y, std::string text);
	void					drawRect(const rect& rect);
	bool					draw(image* image, int x, int y, bool rotateVertical = false, bool rotateHorizontal = false);
	bool					draw(image* image, const point& point, bool rotateVertical = false, bool rotateHorizontal = false);
	bool					draw(image* image, const rect& rect, bool rotateVertical = false, bool rotateHorizontal = false);
	void					update();

public:
	operator				HDC () const;

public:
	static void				init();
};

} }

#endif // !__PBDEVICECONTEXT_H__