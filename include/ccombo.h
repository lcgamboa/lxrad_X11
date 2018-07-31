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

#ifndef CCOMBO
#define CCOMBO

#include"ccontrol.h"
#include"cwindow.h"
#include"cedit.h"
#include"ctoolbutton.h"
#include"ccombodialog.h"

#define EVONCOMBOCHANGE   (void(CControl::*)(CCombo*))

class CCombo:public CControl
{
protected:
  CEdit edit1;
  CToolButton tbutton1;
  CComboDialog dlist1;
public:
    CCombo (void);
   ~CCombo (void);
  void Draw (void);
  void Create (CControl * control);
  void SetWidth(unsigned width);
  void SetHeight(unsigned height );
  void SetItems (String litens);
  String GetItems (void);
  void AddItem (String text);
  void LoadItemsFromFile (String fname);
  void SaveItemsToFile (String fname);
  String GetItem (int item);
  void SetSelectedItem (String item);
  String GetSelectedItem (void);
  int GetItemsCount (void);
  void DeleteItem (int item);
  void DeleteItems (void);
  CStringList GetContext (void);
  void SetContext (CStringList context);
  void SetText(String text);
  String GetText(void);
  void SetReadOnly (bool r);
  //eventos
  void EditKeyPress (CControl * control, const uint key, const uint hk, uint mask);
  void TButtonPress (CControl * control, uint button, uint x, uint y,uint state);
  
  void combo_change (void);
  
  void (CControl::*EvOnComboChange) (CCombo * combo);
};

#endif
