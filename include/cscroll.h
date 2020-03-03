/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001-2108  Luis Claudio Gamboa Lopes

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

#ifndef CSCROLL
#define CSCROLL

#include"ccontrol.h"
#include"cwindow.h"
#include"ctoolbutton.h"

#define  scrolltype int
#define  st_vertical 0x08
#define  st_horizontal 0x04

#define EVONCHANGEPOSITION   (void(CControl::*)(CControl*))

class CScroll:public CControl
{
protected:
  int Position;
  int Range;
  uint Size;
  bool Move;
  scrolltype Type;
  CToolButton *button1, *button2;
  void ScrollButtonPress (CControl * control, const uint button, const uint x, const uint y,
			  const uint state);
  void ScrollButtonRelease (CControl * control, const uint button, const uint x, const uint y,
			    const uint state);
public:
    CScroll (void);
   ~CScroll (void);
  void Draw (void);
  int Create (CControl * control);
  CStringList GetContext (void);
  void SetContext (CStringList context);
  //propiedades
  void SetPosition (int position);
  int GetPosition (void);
  void SetRange (int range);
  int GetRange (void);
  void SetX (int x);
  void SetY (int y);
  void SetWidth (uint width);
  void SetHeight (uint height);
  void SetType (scrolltype type);
  //eventos
  void pointer_out (void);
  void key_press (XEvent event);
  void key_release (XEvent event);
  void button_press (XEvent event);
  void button_release (XEvent event);
  void mouse_move (XEvent event);
  void (CControl::*EvOnChangePosition) (CControl * control);
};

#endif
