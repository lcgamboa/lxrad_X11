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


#include"../include/citemmenu.h"
#include"../include/cpmenu.h"
#include"../include/cmenu.h"
#include"../include/capplication.h"


//CItemMenu------------------------------------------------------

CItemMenu::CItemMenu (void)
{
  SetCanExecuteEvent (true);
  SetClass ("CItemMenu");
  SubMenu = NULL;
  EvMenuActive= NULL;
};


void
CItemMenu::Draw (void)
{
  if ((!Visible)||(Paint == NULL)) return;

  Width = GetTextWidth ()+2;
  Height = GetTextHeight ();

  CPMenu *PMenu = dynamic_cast < CPMenu * >(Owner);
  CMenu *Menu = dynamic_cast < CMenu * >(Owner);

  if (Menu != NULL)
    {

      Y = 2;
      X = Menu->NextItemX;
      Menu->NextItemX += Width + 20;
      Height = Menu->GetHeight () - 4;
    };

  if (PMenu != NULL)
    {
      X = 4;
      Y = PMenu->NextItemY + 4;
      PMenu->NextItemY += Height + 4;
    };
  CLabel::Draw ();
};


int 
CItemMenu::Create (CControl * control)
{
  return CLabel::Create (control);
}

void
CItemMenu::SetSubMenu (CPMenu * submenu)
{
  SubMenu = submenu;
};
  
CPMenu * 
CItemMenu::GetSubMenu (void)
{
  return SubMenu;
}

CStringList
CItemMenu::GetContext (void)
{
  CObject::GetContext ();

  Context.AddLine ("Text=" + GetText () + ";String");

  if (SubMenu)
    Context.AddLine ("SubMenu=" + SubMenu->GetName () + ";SubMenu");
  else
    Context.AddLine ("SubMenu=NULL;SubMenu");
 
  Context.AddLine ("MouseButtonPress=" + btoa (GetEv (true)) + ";event");
  
  /*
  for (uint i = 0; i < Context.GetLinesCount(); i++)
  {
  if(Context.GetLine(i).find("PopupMenu") == 0)
    Context.DelLine(i);
  };
  */
  return Context;
};

void
CItemMenu::SetContext (CStringList context)
{
  Erase ();
  CObject::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      String line = Context.GetLine (i);
      String arg;
      eqparse (line, arg);
      if (line.compare ("Text") == 0)
	 SetText(arg);     
      if (line.compare ("SubMenu") == 0)
	if (arg.compare ("NULL") != 0)
	  SetSubMenu (dynamic_cast < CPMenu * >(Win->GetChildByName (arg)));
      if (line.compare ("MouseButtonPress") == 0)
	SetEv (atob (arg),true);
    };
  Draw ();
};


//eventos

void
CItemMenu::button_press (XEvent event)
{
  CPMenu *PMenu = dynamic_cast < CPMenu * >(Owner);
  if (PMenu != NULL)
    {
      PMenu->HideExclusive ();
      //XSync(GetWin()->GetADisplay(),false);
      XFlush(GetWin()->GetADisplay());
      CControl::button_press (event);
      
      if ((FOwner) && (EvMenuActive))
      {
         (FOwner->*EvMenuActive) (this);
      }
      return;
    };

  if ((SubMenu != NULL) && ((SubMenu->GetChildCount ()) != -1))
    {
      int x, y;
      Window child;
      if (!SubMenu->GetWWindow () != 0)
	{
	  //Application.ACreateWindow (SubMenu,Win);
	  Application->ACreateWindow (SubMenu);
	};
      XTranslateCoordinates (Win->GetADisplay (), Win->GetWWindow (),
			     SubMenu->GetWWindow (), X, Y, &x, &y, &child);
      SubMenu->SetX (SubMenu->GetX () + x);
      SubMenu->SetY (SubMenu->GetY () + y + Height);
      SubMenu->Show ();
      SubMenu->Draw ();
      while (SubMenu->GetVisible ())
	{
	  Application->ProcessEvents (SubMenu);
	};
    };
  //Owner->GetWin ()->Update ();
  CControl::button_press (event);
  
};

void
CItemMenu::button_release (XEvent event)
{
  if (SubMenu != NULL)
    {
      SubMenu->WDestroy ();
    };
  CControl::button_release (event);


};

void
CItemMenu::focus_out (void)
{
  if (SubMenu != NULL)
    {
      SubMenu->WDestroy ();
    };
  CControl::focus_out ();
};

