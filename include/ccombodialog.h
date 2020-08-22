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

#ifndef CCOMBODIALOG
#define CCOMBODIALOG

#include"cwindow.h"
#include"ccontrol.h"
#include"clist.h"

class CComboDialog:public CWindow
{
public:
  CList list1;
  int Create (CControl * control);
  void SetWidth(unsigned width);
  void SetHeight(unsigned height );
  void SetItems (lxString litens);
  lxString GetItems (void);
  void listchange1 (CList * list);
  void buttonpress1 (CControl * control, uint key, uint x, uint y,uint mask);
  CComboDialog (void);
};

#endif
