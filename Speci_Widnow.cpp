#include "stdafx.h"
#include "Speci_Window.h"
HRGN BitmapToRegion (HWND hWnd, HBITMAP hBmp, COLORREF cTransparentColor, int mousex, int mousey)
{//本来黙认是黑色，但考虑到多方面，便改成了取第一点
	HRGN hRgn = NULL;

	if(hBmp)
	{
		HDC hMemDC = CreateCompatibleDC(NULL);
		if(hMemDC)
		{
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);// 获得图片大小
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
					sizeof(BITMAPINFOHEADER),	// biSize 
					bm.bmWidth,					// biWidth; 
					bm.bmHeight,				// biHeight; 
					1,							// biPlanes; 
					32,							// biBitCount 
					BI_RGB,						// biCompression; 
					0,							// biSizeImage; 
					0,							// biXPelsPerMeter; 
					0,							// biYPelsPerMeter; 
					0,							// biClrUsed; 
					0							// biClrImportant; 
			};

			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if(hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);	
				HDC hDC = CreateCompatibleDC(hMemDC);
				if(hDC)
				{	
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;	
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);				
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pObject = (RGNDATA *)GlobalLock(hData);
					pObject->rdh.dwSize = sizeof(RGNDATAHEADER);
					pObject->rdh.iType = RDH_RECTANGLES;
					pObject->rdh.nCount = pObject->rdh.nRgnSize = 0;
					SetRect(&pObject->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);				
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
//关键：
					for(int y = 0; y < bm.bmHeight; y++)
					{
						RGBQUAD* Linia32RGBA = (RGBQUAD* )p32;

						for(int x = 0; x < bm.bmWidth; x++)
						{
							int x0 = x;
							LONG *p = (LONG *)p32 + x;

							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								BYTE c = *p32;

								if(x != 0)
									b = (GetRValue(*p) + GetRValue(*(p - 1)) + GetRValue(*(p + 1))) / 3;
								// 简单模糊渲染
								if(cTransparentColor > -1)
								{
									if(b == cTransparentColor)
									break;
								}
								else if(c == b)// 这个点是“透明”的     			
									break;
								RECT rect;
								GetWindowRect(hWnd, &rect);
								if(x == mousex - rect.left && y== mousey - rect.top)
									break;//请注意，这一点是点透的关键
								p++;
								x++;
							}
							if(x > x0)
							{	
								if(pObject->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pObject = (RGNDATA *)GlobalLock(hData);
								}

								RECT *pr = (RECT *)&pObject->Buffer;
								SetRect(&pr[pObject->rdh.nCount], x0, y, x, y+1);
								if(x0 < pObject->rdh.rcBound.left)
									pObject->rdh.rcBound.left = x0;
								if(y < pObject->rdh.rcBound.top)
									pObject->rdh.rcBound.top = y;
								if(x > pObject->rdh.rcBound.right)
									pObject->rdh.rcBound.right = x;
								if(y+1 > pObject->rdh.rcBound.bottom)
									pObject->rdh.rcBound.bottom = y+1;
                                pObject->rdh.nCount++;
								if(pObject->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pObject);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pObject->rdh.nCount = 0;
									SetRect(&pObject->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}
						p32 -= bm32.bmWidthBytes;
					}

		  
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pObject);
					if (hRgn)
					{
				
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
				}
				DeleteObject(SelectObject(hMemDC, holdBmp));
			}
			DeleteDC(hMemDC);//注意一定要释放
		}	
	}
	return hRgn;
}
/*
bool IsCompositionEnabled()
{
    HMODULE library = ::LoadLibrary("dwmapi.dll");
    bool result = false;
    if (0 != library)
    {
        if (0 != ::GetProcAddress(library, 
                                  "DwmIsCompositionEnabled"))
        {
            BOOL enabled = FALSE;
            result = SUCCEEDED(::DwmIsCompositionEnabled(&enabled)) && enabled;
        }
        VERIFY(::FreeLibrary(library));
    }
    return result;
}


HRESULT EnableBlurBehindWindow(HWND window,
                               bool enable = true,
                               HRGN region = 0,
                               bool transitionOnMaximized = false)
{
	DllImport("dwmapi.dll");
    DWM_BLURBEHIND blurBehind = { 0 };
    blurBehind.dwFlags = DWM_BB_ENABLE | DWM_BB_TRANSITIONONMAXIMIZED;
    blurBehind.fEnable = enable;
    blurBehind.fTransitionOnMaximized = transitionOnMaximized;
    if (enable && 0 != region)
    {
        blurBehind.dwFlags |= DWM_BB_BLURREGION;
        blurBehind.hRgnBlur = region;
    }
    return ::DwmEnableBlurBehindWindow(window,
                                       &blurBehind);
}*/