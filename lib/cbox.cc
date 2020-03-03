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


#include"../include/cbox.h"

// CCbox___________________________________________________________

CCbox::CCbox (void)
{
  CanFocus = false;
  SetX (10);
  SetY (10);
  SetWidth (10);
  SetHeight (10);
  Check = false;
  SetClass ("CCbox");
};

CCbox::~CCbox (void)
{
};

int
CCbox::Create (CControl * control)
{
  if (!ColorSet)
    SetColor ("white");
  int ret=CControl::Create (control);
  PressColor = ColorByName ("black");
  return ret;
};


void
CCbox::Event (XEvent event)
{
  CControl::Event (event);
  SetCanExecuteEvent (false);
  GetOwner ()->Event (event);
  SetCanExecuteEvent (true);
};

void
CCbox::Draw (void)
{
  if ((!Visible)||(Paint == NULL))
    return;
  Paint->InitDraw (this);
  if (Check)
    {
      Paint->Pen.SetColor (PressColor);
      Paint->Rectangle ( 0, 0, Width, Height);
    }
  else
    {
      Paint->Pen.SetColor (Color);
      Paint->Rectangle ( 0, 0, Width, Height);
    };
  Paint->Pen.SetColor (ColorByName ("black"));
  Paint->LowerFrame ( 0, 0, Width, Height);
  CControl::Draw ();
};

CStringList CCbox::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("Check=" + btoa (GetCheck ()) + ";bool");
  return Context;
};

void
CCbox::SetContext (CStringList context)
{
  Eraser ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      String line = Context.GetLine (i);
      String arg;
      eqparse (line, arg);
      if (line.compare ("Check") == 0)
	SetCheck (atob (arg));
    };
  Draw ();
};

//propiedades

void
CCbox::SetCheck (bool check)
{
  Check = check;
  Draw ();
};

bool
CCbox::GetCheck (void)
{
  return Check;
};

//events
void
CCbox::button_press (XEvent event)
{
  if (Check)
    Check = false;
  else
    Check = true;
  Draw ();
  CControl::button_press (event);
};

