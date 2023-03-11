// (C) Copyright 1991 by Borland International

#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <dir.h>
#if defined( BWCC)
#include <bwcc.h>
#else
#define IDHELP 210
#endif
#include "rwcdemo.h"
#include "rwcdlg.h"

int FAR PASCAL _export dlgProc(HWND hDlg, WORD msg, WORD wParam, LONG lParam)
{
        static TDialog *TheDialog;

        switch(msg)
        {
                case WM_INITDIALOG:
                        TheDialog = (TDialog*)(void far *)lParam;
                        TheDialog->hWindow = hDlg;
                        TheDialog->setupDialog();
                        return 1;
                case WM_COMMAND:
                        return TheDialog->doCommands(wParam, lParam);
        }
        return 0;
}

TDialog::TDialog(int DlgName)
{
        dialogFunc = MakeProcInstance((FARPROC)::dlgProc, WinApp::hInstance);
        name = MAKEINTRESOURCE(DlgName);
}

TDialog::~TDialog()
{
        FreeProcInstance(dialogFunc);
}

int TDialog::doCommands(WORD wParam, LONG)
{
        switch(wParam)
        {
                case IDOK:
                        retrieveInfo();
                        if (canClose())
                                EndDialog(hWindow, wParam);
                        return 1;
                case IDCANCEL:
                        EndDialog(hWindow, wParam);
                        return 1;

                case IDHELP:
#if defined (BWCC)
                BWCCMessageBox( hWindow,
                  "Call WinHelp here",
                  "Help",
                  MB_OK | MB_ICONINFORMATION);
#else
                MessageBox( hWindow,
                  "Call WinHelp here",
                  "Help",
                  MB_OK | MB_ICONINFORMATION);
#endif
                return 0;

        }
        return 1;
}

#define ScribbleExtension ".SCR"
#define GraphExtension ".GRP"

TFileOpen::TFileOpen(int *aType, char *aFilePath) :
        TDialog(dlg_Open)
{
        filePath = strdup(aFilePath);
        fileType = aType;
}

int TFileOpen::canClose()
{
        WORD pathLen;

  GetDlgItemText(hWindow, id_FName, pathName, fsPathName + 1);
  fnmerge(pathName,"","",pathName,"");
  pathLen = strlen(pathName);
  if (hasWildCards(pathName) ||
      (GetFocus() == GetDlgItem( hWindow, id_DList) ||
      ( pathLen && (pathName[pathLen - 1] == '\\') )

        ))
  {
        if (pathName[pathLen - 1] == '\\')
          strncat(pathName, fileSpec, fsPathName);
        if (!updateListBoxes())
        {
          MessageBeep(0);
          selectFileName();
        }
        return 0;
  }
  strncat(strncat(pathName, "\\", fsPathName), fileSpec, fsPathName);
  if (updateListBoxes())
        return 0;
  pathName[pathLen] = '\0';
  if (getExtension(pathName)[0] == '\0')
        strncat(pathName, extension, fsPathName);
  AnsiLower(strcpy(filePath, pathName));
  updateButtons();
  if (IsDlgButtonChecked(hWindow, id_Text) == 1)
        *fileType = FileWindow;
  else
  if (IsDlgButtonChecked(hWindow, id_Scribble) == 1)
        *fileType = ScribbleWindow;
  else
  if (IsDlgButtonChecked(hWindow, id_Graph) == 1)
        *fileType = GraphWindow;
  else
        return 0;
  return 1;
}

char *TFileOpen::getFileFirst(char *aFilePath)
{
        static char *P;
        char *Q;

        P = getFileName(aFilePath);
        Q = strchr(P, '.');
        if (Q)
                *Q = '\0';
        return P;
}

char *TFileOpen::getExtension(char *aFilePath)
{
  static char *P;
  P = strchr(getFileName(aFilePath), '.');
  if (!P)
        return &(aFilePath[strlen(aFilePath)]);
  else
        return P;
}

char *TFileOpen::getFileName(char *aFilePath)
{
        static char *P;
        P = strchr(strrev(aFilePath), '\\');
        if (!P)
                P = strchr(aFilePath, ':');
        strrev(aFilePath);
        if (!P)
                return aFilePath;
        else
                return (aFilePath + (strlen(aFilePath) - (P - aFilePath)));
}

void TFileOpen::setupDialog()
{
  SendDlgItemMessage(hWindow, id_FName, EM_LIMITTEXT, fsPathName, 0);
  strncpy(pathName, filePath, fsPathName);
  strncpy(extension, getExtension(pathName), fsExtension);
  if (hasWildCards(extension))
        extension[0] = '\0';
  if (!updateListBoxes())
  {
        strcpy(pathName, "*.*");
        updateListBoxes();
  }
  selectFileName();
}

void TFileOpen::selectFileName()
{
  SendDlgItemMessage(hWindow, id_FName, EM_SETSEL, 0, 0x7FFF0000L);
  SetFocus(GetDlgItem(hWindow, id_FName));
}

void TFileOpen::updateFileName()
{
  SetDlgItemText(hWindow, id_FName, AnsiLower(pathName));
  SendDlgItemMessage(hWindow, id_FName, EM_SETSEL, 0, 0x7FFF0000L);
  updateButtons();
}

void TFileOpen::updateButtons()
{
  char *P;
  int WhichButton;

  P = getExtension(pathName);
  if (P)
  {
        if (!stricmp(P, ScribbleExtension))
          WhichButton = id_Scribble;
        else
        if (!stricmp(P, GraphExtension))
          WhichButton = id_Graph;
        else
          WhichButton = id_Text;
        SendDlgItemMessage(hWindow, id_Text, BM_SETCHECK, 0, 0);
        SendDlgItemMessage(hWindow, id_Graph, BM_SETCHECK, 0, 0);
        SendDlgItemMessage(hWindow, id_Scribble, BM_SETCHECK, 0, 0);
        SendDlgItemMessage(hWindow, WhichButton, BM_SETCHECK, 1, 0);
  }
}

int TFileOpen::updateListBoxes()
{
  int Result;
  char Path[fsPathName+1];

  if (GetDlgItem(hWindow, id_FList))
  {
        strcpy(Path, pathName);
        Result = DlgDirList(hWindow, Path, id_FList, id_FPath, 0);
        if (Result)
                DlgDirList(hWindow, "*.*", id_DList, 0, 0x0C010);
  }
  else
  {
        strncpy(Path, pathName, getFileName(pathName) - pathName);
        strncat(Path, "*.*", fsPathName);
        Result = DlgDirList(hWindow, Path, id_DList, id_FPath, 0x0C010);
  }

  if (Result)
  {
        strncpy(fileSpec, getFileName(pathName), fsFileSpec);
        strcpy(pathName, fileSpec);
        updateFileName();
  }
  return Result;
}

int TFileOpen::doCommands(WORD wParam, LONG lParam)
{
        switch(wParam)
        {
                case id_FName:
                        if (HIWORD(lParam) == EN_CHANGE)
                                EnableWindow(GetDlgItem(hWindow, IDOK),
                                        (WORD) SendMessage(LOWORD(lParam), WM_GETTEXTLENGTH, 0, 0));
                        return 0;

                case id_FList:
                        switch(HIWORD(lParam))
                        {
                                case LBN_DBLCLK:
                                        PostMessage(hWindow, WM_COMMAND, IDOK, 0);
                                        //Fall through
                                case LBN_SELCHANGE:
                                        DlgDirSelect(hWindow, pathName, id_FList);
                                        updateFileName();
                                        return 0;
                                case LBN_KILLFOCUS:
                                        SendMessage(LOWORD(lParam), LB_SETCURSEL, -1, 0);
                                        return 0;
                        }
                case id_DList:
                        switch(HIWORD(lParam))
                        {
                                case LBN_SELCHANGE:
                                case LBN_DBLCLK:
                                        DlgDirSelect(hWindow, pathName, id_DList);
                                        strcat(pathName, fileSpec);
                                        if (HIWORD(lParam) == LBN_DBLCLK)
                                                updateListBoxes();
                                        else
                                                updateFileName();
                                        return 0;
                                case LBN_KILLFOCUS:
                                        SendMessage(LOWORD(lParam), LB_SETCURSEL, -1, 0);
                                        return 0;
                        }

                case id_Scribble:
                        strcat(strcpy(pathName,getFileFirst(pathName)), ScribbleExtension);
                        updateFileName();
                        return 0;
                case id_Graph:
                        strcat(strcpy(pathName,getFileFirst(pathName)), GraphExtension);
                        updateFileName();
                        return 0;
                case id_Text:
                        if (strcmp(getExtension(pathName),"."))
                        {
                                strcat(strcpy(pathName,getFileFirst(pathName)), ".TXT");
                                updateFileName();
                        }
                        return 0;
                default:
                        return TDialog::doCommands(wParam, lParam);
        }
}

void TFileNew::setupDialog()
{
        TDialog::setupDialog();
        SetFocus(GetDlgItem(hWindow, id_Text));
        SendDlgItemMessage(hWindow, id_Text, BM_SETCHECK, 1, 0);
}

int TFileNew::canClose()
{
        if (IsDlgButtonChecked(hWindow, id_Text))
                *fileType = FileWindow;
        else
        if (IsDlgButtonChecked(hWindow, id_Scribble))
                *fileType = ScribbleWindow;
        else
        if (IsDlgButtonChecked(hWindow, id_Graphics))
                *fileType = GraphWindow;
        else
                return 0;
        return 1;
}