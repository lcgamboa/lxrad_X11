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

#ifndef CEDIT
#define CEDIT

#include"ccontrol.h"
#include"clabel.h"
#include"cwindow.h"

class CCursor:public CControl
{
public:
  CCursor (void);
  int Create (CControl * control);
  void Draw (void);
};

class CEdit:public CControl
{
protected:
  uint MaxLenght;
  char PrevKey;
  CLabel *Text;
  CCursor *Cursor;
  uint CursorPos;
  bool ReadOnly;
public:
    CEdit (void);
   ~CEdit (void);
  void Draw (void);
  void DrawCursor (void);
  int Create (CControl * control);
  lxStringList GetContext (void);
  void SetContext (lxStringList context);
  //propiedades
  virtual void SetText (lxString t);
  virtual lxString GetText (void);
  void SetX (int x);
  void SetY (int y);
  void SetWidth (uint width);
  void SetHeight (uint height);
  void SetReadOnly (bool r);
  bool GetReadOnly (void);
  void SetMaxLenght (int l);
  int GetMaxLenght (void);
  void SetAlign (CAlign align);
  CAlign GetAlign (void);
  virtual void SetCursorPos (uint cursorpos);
  uint GetCursorPos (void);
  void SetPrevKey (char prevkey);
  char GetPrevKey (void);
  //eventos
  void button_press (XEvent event);
  void key_press (XEvent event);
  void focus_in (void);
  void focus_out (void);
};

#include"medit.h"

#endif
