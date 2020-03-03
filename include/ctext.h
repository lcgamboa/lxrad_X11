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

#ifndef CTEXT
#define CTEXT

#include"clxrad.h"
#include"cwindow.h"
#include"cedit.h"

class CText:public CEdit
{
private:
  CStringList Lines;
  uint PLine;
  uint PChar;
  uint CursorLin;
public:
    CText (void);
   ~CText (void);
  void Draw (void);
  void DrawCursor (void);
  void DrawLine (void);
  int Create (CControl * control);
  void Clear ();
  void AddLine (char *line);
  void AddLine (String line);
  void InsertLine (char *line);
  void DelLine (void);
  void LoadFromFile (char *fname);
  void SaveToFile (char *fname);
  void SaveToFile (String fname);
  //propiedades
  void SetText (String t);
  String GetText (void);
  void SetCursorPos (uint cursorpos);
  //void SetReadOnly(bool r);
  //bool GetReadOnly(void);
  //void SetAlign(CAlign align);
  //CAlign GetAlign(void);
  unsigned int GetCountLines (void);
  //eventos
  void key_press (XEvent event);
};

#endif
