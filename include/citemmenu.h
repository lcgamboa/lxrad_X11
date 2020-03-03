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

#ifndef CITEMMENU
#define CITEMMENU

#include"ccontrol.h"
#include"clabel.h"
#include"cwindow.h"

class CPMenu;

class CItemMenu:public CLabel
{
protected:
  CPMenu * SubMenu;
//  bool Press;
public:
  CItemMenu (void);
//propiedade;
  void Draw (void);
  int Create (CControl * control);
  CStringList GetContext (void);
  void SetContext (CStringList context);
  void SetSubMenu (CPMenu * submenu);
  CPMenu * GetSubMenu (void);
//eventos
  void button_press (XEvent event);
  void button_release (XEvent event);
  void focus_out (void);
  void (CControl::*EvMenuActive) (CControl * control);
};

#endif
