#
# Borland C++ - (C) Copyright 1991 by Borland International
# 
CC = BCC
AS = TASM 
PASCAL = TPC
RC = RC
.asm.obj:
      $(AS) $(AFLAGS) $&.asm

.c.exe:
      $(CC) $(CFLAGS) $&.c

.c.obj:
      $(CC) $(CFLAGS) /c $&.c

.cpp.obj:
      $(CC) $(CFLAGS) /c $&.cpp

.pas.exe:
      $(PASCAL) $(PFLAGS) $&.pas

.rc.res:
      $(RC) $(RFLAGS) /r $&

.SUFFIXES: .exe .obj .asm .c .pas .res .rc 
