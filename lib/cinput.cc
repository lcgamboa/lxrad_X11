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


#include"../include/cinput.h"


// CInput__________________________________________________________


CInput::CInput (void)
{
  Return = false;
  SetClass ("CInput");
  SetX (310);
  SetY (240);
  SetWidth (285);
  SetHeight (115);
  SetName ("Input");
  //button1
  button1.SetText ("OK");
  button1.SetX (80);
  button1.SetY (75);
  button1.SetTag (1);
  button1.SetFOwner (this);
  button1.EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & CInput::ButtonRelease1;
  CreateChild (&button1);
  //button1
  button2.SetText ("Cancel");
  button2.SetX (150);
  button2.SetY (75);
  button2.SetTag (0);
  button2.SetFOwner (this);
  button2.EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & CInput::ButtonRelease1;
  CreateChild (&button2);
  //string1
  string1.SetText ("");
  string1.SetX (20);
  string1.SetY (20);
  CreateChild (&string1);
  //edit1
  edit1.SetText ("");
  edit1.SetX (100);
  edit1.SetY (20);
  edit1.SetWidth (180);
  CreateChild (&edit1);
};

void
CInput::ButtonRelease1 (CControl * control, uint button, uint x, uint y,
			uint state)
{
  Return = control->GetTag ();
  HideExclusive ();
};

bool
Input (lxString label, lxString & str)
{
  bool ret;
  CInput *winput;
  winput = new CInput;
  winput->string1.SetText (label);
  winput->edit1.SetText (str);
  winput->WCreate ();
  winput->Draw ();
  winput->ShowExclusive ();
  winput->SetCanDestroy (true);
  ret = winput->Return;
  if (ret)
    str = winput->edit1.GetText ();
  winput->WDestroy ();
  delete winput;
  return ret;
};
