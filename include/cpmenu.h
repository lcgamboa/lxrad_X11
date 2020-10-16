/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001-2018  Luis Claudio Gamboa Lopes

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

#ifndef CPMENUW
#define CPMENUW

#include"ccontrol.h"
#include"cwindow.h"
#include"citemmenu.h"

class CPMenu:public CWindow 
{
private:
  bool Press;
  lxString MenuItems;	  
  CItemMenu IMenu;
public:
  uint NextItemX;
  uint NextItemY;
  CPMenu(void);
//   ~CPMenu (void);
  void Draw (void);
  int Create (CControl * control);
  void SetMenuItems (lxString mitens);
  lxString GetMenuItems (void);
  lxStringList GetContext (void);
  void SetContext (lxStringList context);
  void SetText (lxString text);
//events
  void button_press (XEvent event);
  void on_leave (void);
};

#endif
