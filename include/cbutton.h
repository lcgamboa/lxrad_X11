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

#ifndef CBUTTON
#define CBUTTON

#include"ccontrol.h"
#include"clabel.h"
#include"cwindow.h"

class CButton:public CControl
{
protected:
  bool Press;
  CLabel *Text;
  XColor PressColor;
public:
    CButton (void);
   ~CButton (void);
  virtual void Draw (void);
  int Create (CControl * control);
  lxStringList GetContext (void);
  void SetContext (lxStringList context);
  //propiedades
  void SetPress (bool press);
  bool GetPress (void);
  void SetText (lxString t);
  lxString GetText (void);
  void SetWidth (uint width);
  void SetHeight (uint height);
  //eventos
  virtual void key_press (XEvent event);
  void key_release (XEvent event);
  virtual void button_press (XEvent event);
  void button_release (XEvent event);
  void pointer_in (void);
  void pointer_out (void);
  void SetAlign (CAlign align);
  CAlign GetAlign (void);
  void focus_in (void);
  void focus_out (void);
};

#endif
