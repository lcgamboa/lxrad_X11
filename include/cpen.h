/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#ifndef CPEN
#define CPEN

#include"cobject.h"

class CControl;

class CPen:public CObject
{
private:
  XColor FgColor;
  XColor BgColor;
  int Width;
  Display * Disp;
  GC *Agc;
public:
    CPen (void)
  {
    SetClass ("CPen");
  };
  void Create (CControl * control, GC * gc);
  void SetColor (XColor color);
  void SetFgColor (XColor color);
  void SetBgColor (XColor color);
  XColor GetFgColor (void);
  XColor GetBgColor (void);
  void SetWidth (unsigned long width);
  void SetPen (int function);
};
#endif
