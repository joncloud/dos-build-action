#if !defined( __CUSTCNTL_H )
#define __CUSTCNTL_H

/*
 * CUSTOM CONTROL LIBRARY - HEADER FILE
 *
 */

#ifndef RC_INVOKED
#pragma option -a-
#endif

/* general size definitions */
#define 	CTLTYPES		12				/* number of control types */
#define 	CTLDESCR		22				/* size of control menu name */
#define		CTLCLASS	  	20			  	/* max size of class name */
#define		CTLTITLE	  	94			  	/* max size of control text */

/**/

/*
 * CONTROL STYLE DATA STRUCTURE
 *
 * This data structure is used by the class style dialog function
 * to set and/or reset various control attributes.
 *
 */

typedef struct {
	WORD			wX;					 		/* x origin of control */
	WORD			wY;					 		/* y origin of control */
	WORD			wCx;							/* width of control */
	WORD			wCy;							/* height of control */
	WORD			wId;							/* control child id */
	DWORD			dwStyle;						/* control style */
	char			szClass[CTLCLASS];  		/* name of control class */
	char			szTitle[CTLTITLE];  		/* control text */
} CTLSTYLE;

typedef CTLSTYLE *		PCTLSTYLE;
typedef CTLSTYLE FAR *		LPCTLSTYLE;

/**/

/*
 * CONTROL DATA STRUCTURE
 *
 * This data structure is returned by the control options function
 * when enquiring about the capabilities of a particular control.
 * Each control may contain various types (with predefined style
 * bits) under one general class.
 *
 * The width and height fields are used to provide the host
 * application with a suggested size.  The values in these fields
 * could be either in pixels or in rc coordinates.  If it is in pixels,
 * the most sigificant bit(MSB) is on.  If the MSB is off, it is in rc
 * coordinates.
 *
 * The cursor and bitmap handles reference objects which can be
 * used by the dialog editor in the placement and definition of
 * new, user-defined control classes.  However, dialog editor in win30
 * does not use these fields.
 *
 */

typedef struct {
	WORD			wType;						/* type style */
	WORD			wWidth;						/* suggested width */
	WORD			wHeight;						/* suggested height */
	DWORD			dwStyle;						/* default style */
	char			szDescr[CTLDESCR];  		/* menu name */
} CTLTYPE;

typedef struct {
	WORD			wVersion;					/* control version */
	WORD			wCtlTypes;					/* control types */
	char			szClass[CTLCLASS];  		/* control class name */
	char			szTitle[CTLTITLE];  		/* control title */
	char			szReserved[10];			/* reserved for future use */
	CTLTYPE 		Type[CTLTYPES]; 		/* control type list */
} CTLINFO;

typedef CTLINFO *		PCTLINFO;
typedef CTLINFO FAR *		LPCTLINFO;

/* These two function prototypes are used by dialog editor */
typedef DWORD			(FAR PASCAL *LPFNSTRTOID)( LPSTR );
typedef WORD			(FAR PASCAL *LPFNIDTOSTR)( WORD, LPSTR, WORD );


// Resource Workshop extensions follow here


// function  prototypes left out of the original custcntl.h


typedef HANDLE		(FAR PASCAL *LPFNINFO)( void );
typedef BOOL		(FAR PASCAL *LPFNSTYLE)
(
  HWND hWnd,
  HANDLE hCntlStyle,
  LPFNSTRTOID lpfnSID,
  LPFNIDTOSTR lpfnIDS
);

typedef WORD   (FAR PASCAL *LPFNFLAGS)
(
  DWORD   dwStyle,
  LPSTR   lpBuff,
  WORD    wBuffLength
);

/*****************************************************************************

  Resource Workshop has extended the MS Dialog editor's custom control
  API in three main areas:

  1) More than 1 custom control can be placed in a single DLL

  2) The "Info" data structure has been extended to allow custom controls
     to be added to the RW toolbox

  3) The style data structure has been extended to allow custom controls
     access to the CTLDATA field. This field contains up to 255 bytes
     of binary data. A pointer to this data is passed to the control
     in the WM_CREATE message at runtime.


*****************************************************************************/

/*****************************************************************************

  Two new fields have been added to the CTLTYPE data structure to
  make the RWCTLTYPE structure:

  hToolBit is a handle to a 24X24 bitmap which is added to the
  RW toolbox. If this field is 0, no button will be added for this style,
  and it will only be selectable via the Custom control dialog. This bitmap
  is "owned" by RW, and will be freed by RW when the dialog editor is
  unloaded.

  hDropCurs is a handle to a cursor which is used by RW when a user
  selects the control from the toolbox. If 0, a cross cursor will be used.


*****************************************************************************/

#define TOOLBIT_SIZE  24

typedef struct
{
   WORD	      wType;		      /* type style */
   WORD	      wWidth;		      /* suggested width */
   WORD	      wHeight;		      /* suggested height */
   DWORD      dwStyle;		      /* default style */
   char	      szDescr[CTLDESCR];      /* dialog name */
   HBITMAP    hToolBit;               // Toolbox bitmap
   HCURSOR    hDropCurs;              // Drag and drop cursor

} RWCTLTYPE, FAR * LPRWCTLTYPE;

/*****************************************************************************

  This structure reflects the RWCTLTYPE data structure

*****************************************************************************/


typedef struct
{
   WORD	      wVersion;		     /* control version */
   WORD	      wCtlTypes;	     /* control types */
   char	      szClass[CTLCLASS];     /* control class name */
   char	      szTitle[CTLTITLE];     /* control title */
   char	      szReserved[10];	     /* reserved for future use */
   RWCTLTYPE  Type[CTLTYPES]; 	     /* Resource Workshop control type list */

} RWCTLINFO;

typedef RWCTLINFO *		PRWCTLINFO;
typedef RWCTLINFO FAR *		LPRWCTLINFO;

/*****************************************************************************

  Two new fields have been added to the CTLSTYLE data structure to make
  the RWCTLSTYLE data structure:

  CtlDataSize is the size of
  CtlData, which is an array of bytes passed to the control in the
  WM_CREATE message.


*****************************************************************************/

#define CTLDATALENGTH 255            // 255 bytes + Length Byte

typedef struct {
  WORD	    wX;			     /* x origin of control */
  WORD	    wY;			     /* y origin of control */
  WORD	    wCx;		     /* width of control */
  WORD	    wCy;		     /* height of control */
  WORD	    wId;		     /* control child id */
  DWORD	    dwStyle;		     /* control style */
  char	    szClass[CTLCLASS];       /* name of control class */
  char	    szTitle[CTLTITLE];       /* control text */
  BYTE      CtlDataSize;             // Control data Size
  BYTE      CtlData[ CTLDATALENGTH]; // Control data

} RWCTLSTYLE;

typedef RWCTLSTYLE *		  PRWCTLSTYLE;
typedef RWCTLSTYLE FAR *	  LPRWCTLSTYLE;

/*****************************************************************************

  In order to use RW's extensions to the custom controls, a custom
  control DLL *must* implement the ListClasses function. This function
  returns a global memory handle to an initialized CTLCLASSLIST data
  structure. All function pointers *must* point to valid functions.


*****************************************************************************/

typedef struct
{
  LPFNINFO  fnRWInfo;           // RW Info function
  LPFNSTYLE fnRWStyle;          // RW Style function
  LPFNFLAGS fnFlags;            // Flags function
  char  szClass[ CTLCLASS];

} RWCTLCLASS, FAR *LPRWCTLCLASS;

typedef struct
{
  short       nClasses;
#if defined (__cplusplus)
  RWCTLCLASS Classes[1];
#else
  RWCTLCLASS Classes[];
#endif

} CTLCLASSLIST, FAR *LPCTLCLASSLIST;

typedef HANDLE		(FAR PASCAL *LPFNLOADRES)( LPSTR szType, LPSTR szId);
typedef BOOL      (FAR PASCAL *LPFNEDITRES)( LPSTR szType, LPSTR szId);

typedef HANDLE		(FAR PASCAL *LPFNLIST)
(
  LPSTR       szAppName,
  WORD        wVersion,
  LPFNLOADRES fnLoad,
  LPFNEDITRES fnEdit
);

#define DLGCUSTCLASSNAME   "_BorDlg_DlgEditChild"
#define DLGTESTCLASSNAME   "_BorDlg_DlgEditTest"

#ifndef RC_INVOKED
#pragma option -a.
#endif

#endif	/* __CUSTCNTL_H */

