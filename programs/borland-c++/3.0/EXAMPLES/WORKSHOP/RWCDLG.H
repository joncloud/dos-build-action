// (C) Copyright 1991 by Borland International

#define fsPathName 80
#define fsExtension 3
#define fsFileSpec fsPathName+fsExtension

class TDialog
{
public:
	HWND hWindow;
	FARPROC dialogFunc;
	char far *name;

	TDialog(int DlgName);
	~TDialog();
	virtual void setupDialog() {};
	virtual void retrieveInfo() {};
	virtual int doCommands(WORD wParam, LONG lParam);
	virtual int canClose() {return 1;};
};


class TFileNew : public TDialog
{
public:
  int *fileType;
  TFileNew(int *aFileType) : TDialog(dlg_FileNew) { fileType = aFileType;};
  int canClose();
  void setupDialog();
};

class TFileOpen : public TDialog
{
public:
	char *filePath;
	char pathName[fsPathName + 1];
	char extension[fsExtension + 1];
	char fileSpec[fsFileSpec + 1];
	int *fileType;

	TFileOpen(int *aType, char *aFilePath);
	~TFileOpen() {free(filePath);};
	int canClose();
	int hasWildCards(char *aFilePath) { return strchr(aFilePath, '*') || strchr(aFilePath, '?');};
	char *getExtension(char *aFilePath);
	char *getFileName(char *aFilePath);
	char *getFileFirst(char *aFilePath);
	void setupDialog();
	int doCommands(WORD wParam, LONG lParam);
	void selectFileName();
	void updateFileName();
	void updateButtons();
	int updateListBoxes();
};