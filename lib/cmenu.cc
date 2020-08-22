/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001-2018  Luis Claudio Gamboa Lopes

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


#include"../include/cmenu.h"

//CMenu-----------------------------------------------------------

CMenu::CMenu (void)
{
  CanFocus = false;
  NextItemX = 0;
  SetClass ("CMenu");
  SetX (0);
  SetY (0);
  SetHeight (25);
  SetWidth (800);
  CanVisible = false;
}

void
CMenu::Draw (void)
{
  if ((!Visible)||(Paint == NULL))return;
  Width = Owner->GetWidth ();
  Paint->InitDraw (this);
  Paint->Pen.SetColor (Color);
  Paint->Rectangle ( 2, 2, Owner->GetWidth () - 2, Height - 2);
  Paint->RaiserFrame ( 0, 0, Owner->GetWidth (), Height);
  NextItemX = 5;
  CControl::Draw ();
}

void
CMenu::Update (void)
{
  if (Win == NULL)
    return;
  if ((Owner->GetWidth () == Width))
    CControl::Update ();
  else
    SetWidth (Owner->GetWidth ());
};

int
CMenu::Create (CControl * control)
{
  ((CWindow *) control)->HasMenu=1;
  return CControl::Create (control);
}

void
CMenu::SetX (int x)
{
  X = 0;
};

void
CMenu::SetY (int y)
{
  Y = 0;
};

lxStringList
CMenu::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("MenuItems=" + GetMenuItems() + ";MenuItems");
  
  for (uint i = 0; i < Context.GetLinesCount(); i++)
  {
  if(Context.GetLine(i).find("PopupMenu") == 0)
    Context.DelLine(i);
  };
  return Context;
};

void
CMenu::SetContext (lxStringList context)
{
  Erase ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      lxString line = Context.GetLine (i);
      lxString arg;
      eqparse (line, arg);
      
      if (line.compare ("MenuItems") == 0)
         SetMenuItems(arg);
      };
  Draw ();
};
  

void 
CMenu::SetMenuItems (lxString menuitems)
{
  MenuItems=menuitems;
};

lxString 
CMenu::GetMenuItems (void)
{
  return MenuItems;
};

