/*  new.h

    Access to operator new() and newhandler()

    Copyright (c) 1990, 1991 by Borland International
    All Rights Reserved.
*/

#if !defined( __NEW_H )
#define __NEW_H

#if !defined( __DEFS_H )
#include <_defs.h>
#endif

#ifndef __cplusplus
#error Must use C++ with new.h
#endif

typedef void (_FAR * _FARFUNC pvf)();
 
extern void (_FAR * _Cdecl _FARFUNC _new_handler)();
 
void (_FAR * _FARFUNC set_new_handler( void (_FAR * _FARFUNC )() ))();

void _FAR * _FARFUNC operator new( unsigned );

#endif  /* __NEW_H */

