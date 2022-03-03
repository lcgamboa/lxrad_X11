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

#ifndef CDIALOG
#define CDIALOG

#include"cwindow.h"
#include"cbutton.h"
#include"cedit.h"

class CDialog:public CWindow
{
public:
  CButton button1, button2;
  CLabel string1;
  bool Return;
  void ButtonRelease1 (CControl * control, uint button, uint x, uint y,
		       uint state);
    CDialog (void);
};

bool Dialog (lxString label);
bool Dialog_sz (lxString label, int Width, int Height);

#endif
