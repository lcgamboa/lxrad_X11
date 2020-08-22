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


#include"../include/cmessage.h"
#include"../include/capplication.h"


// CMessage__________________________________________________________


CMessage::CMessage (void)
{
  SetClass ("CMessage");
  SetX (320);
  SetY (240);
  SetWidth (285);
  SetHeight (115);
  SetName ("Message");
  //button1
  button1.SetText ("OK");
  button1.SetX (110);
  button1.SetY (70);
  button1.SetFOwner (this);
  button1.EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & CMessage::ButtonRelease1;
  CreateChild (&button1);
  //string1
  string1.SetText ("OK");
  string1.SetX (0);
  string1.SetY (0);
  string1.SetWidth (285);
  string1.SetHeight (75);
  CreateChild (&string1);
}

void
CMessage::ButtonRelease1 (CControl * control, uint button, uint x, uint y,
			  uint state)
{
  HideExclusive ();
}

void
Message (lxString str)
{
  int width, height;  
  CMessage *wmessage = new CMessage;
  width=XDisplayWidth(Application->GetADisplay(),DefaultScreen(Application->GetADisplay()));
  height=XDisplayHeight(Application->GetADisplay(),DefaultScreen(Application->GetADisplay()));
  wmessage->SetX ((width  - wmessage->GetWidth  ())/2);
  wmessage->SetY ((height - wmessage->GetHeight ())/2);
  
  wmessage->string1.SetText (str);
  wmessage->WCreate ();
  wmessage->Draw ();
  wmessage->ShowExclusive ();
  wmessage->SetCanDestroy (true);
  wmessage->WDestroy ();
  delete wmessage;
}

void
Message_sz (lxString str, int Width, int Height)
{
  int width, height;  
  CMessage *wmessage = new CMessage;
  
  wmessage->SetWidth (Width);
  wmessage->SetHeight (Height);
  wmessage->button1.SetX ((Width- wmessage->button1.GetWidth())/2);
  wmessage->button1.SetY (Height-65);
  wmessage->string1.SetWidth (Width-5);
  wmessage->string1.SetHeight (Height-80);

  width=XDisplayWidth(Application->GetADisplay(),DefaultScreen(Application->GetADisplay()));
  height=XDisplayHeight(Application->GetADisplay(),DefaultScreen(Application->GetADisplay()));

  wmessage->SetX ((width  - wmessage->GetWidth  ())/2);
  wmessage->SetY ((height - wmessage->GetHeight ())/2);
  
  wmessage->string1.SetText (str);
  wmessage->WCreate ();
  wmessage->Draw ();
  wmessage->ShowExclusive ();
  wmessage->SetCanDestroy (true);
  wmessage->WDestroy ();
  delete wmessage;
}
