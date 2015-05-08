#ifndef _SPECI_WINDOW_H_
#define  _SPECI_WINDOW_H_
#define ALLOC_UNIT  100	
HRGN BitmapToRegion (HWND hWnd, HBITMAP hBmp, COLORREF cTransparentColor, int mousex, int mousey);
HRESULT EnableBlurBehindWindow(HWND window,
                               bool enable = true,
                               HRGN region = 0,
                               bool transitionOnMaximized = false);

#endif