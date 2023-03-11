// (C) Copyright 1991 by Borland International

#include <windows.h>


BOOL FAR _export PASCAL BitBnAppDialogProc( HWND hWnd, unsigned message,
WORD wParam, DWORD lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		  if ( HIWORD( lParam) == BN_CLICKED)
			DestroyWindow( hWnd);
		  return TRUE;

		case WM_DESTROY:
		  PostQuitMessage( 0);
		  break;

		case WM_INITDIALOG:
		  return TRUE;


	}
	return FALSE;

}


int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInst, LPSTR lpCommand, int cmdShow)
{
	HWND	hMain;
	MSG	msg;
	HANDLE	hDll;


	hDll = LoadLibrary( "BITBTN.DLL");
	if ( hDll < 32)
	{
		MessageBox( 0,
		  "You must compile BITBTN.DLL before running this program",
		  "Fatal error", MB_OK | MB_ICONHAND);
		return 255;
	}
	hMain = CreateDialog( hInstance, MAKEINTRESOURCE( 100),
		 0, (FARPROC) BitBnAppDialogProc);

	if ( hMain)
	{
	       while (GetMessage(&msg, NULL, 0, 0))
	       {
		       TranslateMessage(&msg);
		       DispatchMessage(&msg);
	       }
	}
	else
	{
	       MessageBox( 0, "Could not create window", "Fatal Error",
		 MB_OK | MB_ICONHAND);
	       return 254;
	}
	FreeLibrary( hDll);

	return msg.wParam;
}
