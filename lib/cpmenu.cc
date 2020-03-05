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


#include"../include/cpmenu.h"
#include"../include/cmenu.h"

//CPMenu----------------------------------------------------------

CPMenu::CPMenu (void)
{
  CanFocus = false;
  PixmapBuffer=true;
  SetBorder (0);
  SetX (1);
  SetY (1);
  SetHeight (10);
  SetWidth (10);
  NextItemY = 0;
  SetClass ("CPMenu");
  SetVisible (false);
  SetOverrideRedirect (true);
  SetSaveUnder (true);
  SetCanExecuteEvent (false);
}

int
CPMenu::Create (CControl * control)
{
  CMenu *Menu = dynamic_cast < CMenu * >(control);
    
  int ret=CWindow::Create (control);
  
  if((IMenu.GetSubMenu() != this)&& Menu)
  {
    IMenu.SetFOwner(FOwner);
    IMenu.SetName(Name);
    IMenu.SetSubMenu(this);
    Menu->CreateChild(&IMenu);
    //Menu->DestroyChild(this);
    //FOwner->CreateChild(this);
  }
  return ret;
}

void
CPMenu::Draw (void)
{

  if ((!Visible)||(Paint == NULL))
    return;
  NextItemY = 0;
  if (ChildCount >= 0)
    Height = ((ChildCount + 1) * (((CItemMenu *) Child[0])->GetTextHeight () + 4)) + 8;
  else
    Height = 1;
  
  for (int a = 0; a <= ChildCount; a++)
    if (((CItemMenu *) Child[a])->GetTextWidth () + 4 > Width)
      Width = ((CItemMenu *) Child[a])->GetTextWidth () + 8;
  if (WWindow != 0)
    {
      Paint->InitDraw (this);
      XResizeWindow (ADisplay, WWindow, Width, Height);
      CreatePixmap();
      Paint->Pen.SetColor (Color);
      Paint->Rectangle ( 2, 2, Width - 2, Height - 2);
      Paint->RaiserFrame ( 0, 0, Width, Height);
      CControl::Draw ();
    };
};

void 
CPMenu::SetMenuItems (String menuitems)
{
  MenuItems=menuitems;
};

void 
CPMenu::SetText (String text)
{
  IMenu.SetText(text);
};


String 
CPMenu::GetMenuItems (void)
{
  return MenuItems;
};


CStringList
CPMenu::GetContext (void)
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
CPMenu::SetContext (CStringList context)
{
  Erase ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      String line = Context.GetLine (i);
      String arg;
      eqparse (line, arg);
      
      if (line.compare ("MenuItens") == 0)
         SetMenuItems(arg);
     };
  Draw ();
};


//events
void
CPMenu::button_press (XEvent event)
{
  HideExclusive ();
  CControl::button_press (event);
};

void
CPMenu::on_leave (void)
{
  HideExclusive ();
  CWindow::on_leave ();
};
