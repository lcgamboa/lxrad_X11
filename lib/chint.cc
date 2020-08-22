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


#include"../include/chint.h"


// CHint__________________________________________________________


CHint::CHint (void)
{
  SetClass ("CHint");
  SetName ("Hint");
  SetOverrideRedirect (true);
  SetSaveUnder (true);
  SetColor("light yellow");
  SetFOwner (this);
  EvMouseMove = EVMOUSEMOVE & CHint::MouseMove1;
  //SetPixmapBuffer(true);
  
  //string1
  string1.SetText ("OK");
  string1.SetX (0);
  string1.SetY (0);
  CreateChild (&string1);
};

void
CHint::MouseMove1 (CControl * control,uint button, uint x, uint y, uint state)
{
  HideExclusive ();
};

void
WHint (lxString str,int x, int y)
{
  CHint *whint = new CHint;
  
  whint->string1.SetText (str);
  
  whint->SetVisible(false,false);
  whint->WCreate ();
  whint->SetVisible(true,false);
  whint->string1.SetWidth(6+whint->string1.GetTextWidth()); 
  whint->string1.SetHeight(2+whint->string1.GetTextHeight()); 
  whint->SetWidth(whint->string1.GetWidth()); 
  whint->SetHeight(2+whint->string1.GetHeight()); 
  whint->SetX(x-5);
  whint->SetY(y-whint->GetHeight()+1);
  
  whint->string1.Draw ();
  whint->ShowExclusive ();
  whint->SetCanDestroy (true);
  whint->WDestroy ();
  delete whint;
};
