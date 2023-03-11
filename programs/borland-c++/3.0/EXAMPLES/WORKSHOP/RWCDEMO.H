// (C) Copyright 1991 by Borland International

#define max(a,b)	(((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#include "rwcdemoc.h"
#include "rwcwnd.h"
struct WinApp
{
	static HANDLE	hInstance;
	static HANDLE	hPrevInst;
	static WORD    cmdShow;
	static HANDLE	hWndClient;
	static TMDIChildWindow *currentWindow;
};

long FAR PASCAL _export FrameWndProc(HWND hwnd, WORD message, WORD wParam, LONG lParam);
