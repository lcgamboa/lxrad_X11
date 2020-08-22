/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001  Luis Claudio Gamboa Lopes

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

#include"../include/ccombodialog.h"

//CComboDialog-------------------------------------------------------

CComboDialog::CComboDialog (void)
{
  SetClass ("CComboDialog");
  SetX (0);
  SetY (0);
  SetWidth (80);
  SetHeight (100);
  SetName ("ComboDialog");

  SetVisible (false);
  SetOverrideRedirect (true);
  SetSaveUnder (true);
  SetPixmapBuffer(true);

  //list
  list1.SetX (0);
  list1.SetY (0);
  list1.SetFOwner (this);
  list1.OnChangeItem = ONCHANGEITEM & CComboDialog::listchange1;
  list1.EvMouseButtonPress = EVMOUSEBUTTONPRESS & CComboDialog::buttonpress1;
  CreateChild (&list1);
};

int
CComboDialog::Create (CControl * control)
{
  return CWindow::Create (control);
}

void
CComboDialog::SetWidth (unsigned width)
{
  list1.SetWidth (width);
  CControl::SetWidth (width);
};

void
CComboDialog::SetHeight (unsigned height)
{
  list1.SetHeight (height);
  CControl::SetHeight (height);
};
  
void 
CComboDialog::SetItems (lxString litems)
{
  list1.SetItems(litems);
};

lxString 
CComboDialog::GetItems (void)
{
  return list1.GetItems();
};

void
CComboDialog::listchange1 (CList * list)
{
  HideExclusive ();
};

void
CComboDialog::buttonpress1 (CControl * control, uint key, uint x, uint y,
			    uint mask)
{
  HideExclusive ();
};

