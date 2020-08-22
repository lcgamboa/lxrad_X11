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
  int  Create (CControl * control);
  void SetWidth(unsigned width);
  void SetHeight(unsigned height );
  void SetItems (lxString litens);
  lxString GetItems (void);
  void AddItem (lxString text);
  void LoadItemsFromFile (lxString fname);
  void SaveItemsToFile (lxString fname);
  lxString GetItem (int item);
  void SetSelectedItem (lxString item);
  lxString GetSelectedItem (void);
  int GetItemsCount (void);
  void DeleteItem (int item);
  void DeleteItems (void);
  lxStringList GetContext (void);
  void SetContext (lxStringList context);
  void SetText(lxString text);
  lxString GetText(void);
  void SetReadOnly (bool r);
  bool GetReadOnly (void);
  //eventos
  void EditKeyPress (CControl * control, const uint key, const uint hk, uint mask);
  void TButtonPress (CControl * control, uint button, uint x, uint y,uint state);
  
  void combo_change (void);
  
  void (CControl::*EvOnComboChange) (CCombo * combo);
};

#endif
