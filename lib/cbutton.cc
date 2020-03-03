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


#include"../include/cbutton.h"

// CButton___________________________________________________________

CButton::CButton (void)
{
  Text = new CLabel;
  Text->SetText ("Button");
  Text->SetFont (FontName);
  Text->SetX (0);
  Text->SetY (0);
  SetX (10);
  SetY (10);
  SetWidth (60);
  SetHeight (20);
  Press = false;
  SetClass ("CButton");
  CreateChild (Text);
};

CButton::~CButton (void)
{
};

int
CButton::Create (CControl * control)
{
  int ret=CControl::Create (control);
  PressColor = ColorByName ("gray");
  return ret;
}

void
CButton::Draw (void)
{
  XColor TColor;
	
  if((!Visible)||(Paint == NULL))return;
  
  Paint->InitDraw (this);

  if (Press && PointerOn)
    {
      Paint->Pen.SetColor (PressColor);
      Paint->Rectangle ( 0, 0, Width, Height);
      Paint->LowerFrame ( 0, 0, Width, Height, Border);
      TColor=GetColor();
      SetColor(PressColor);
      CControl::Draw ();		
      SetColor(TColor);
    }
  else
    {
      Paint->Pen.SetColor (Color);
      Paint->Rectangle ( 0, 0, Width, Height);
      Paint->RaiserFrame ( 0, 0, Width, Height, Border);
      CControl::Draw ();		
    };
};

CStringList CButton::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("Text=" + GetText () + ";String");
  return Context;
};

void
CButton::SetContext (CStringList context)
{
  Eraser ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      String line = Context.GetLine (i);
      String arg;
      eqparse (line, arg);
      if (line.compare ("Text") == 0)
	SetText (arg);
    };
  Draw ();
};

//propiedades

void
CButton::SetPress (bool press)
{
  Press = press;
  Draw ();
};

bool
CButton::GetPress (void)
{
  return Press;
};

void
CButton::SetText (String t)
{
  Text->SetText (t);
  Draw ();
};

String CButton::GetText (void)
{
  return Text->GetText ();
};

void
CButton::SetWidth (uint width)
{
  Text->SetWidth (width);
  CControl::SetWidth (width);
};

void
CButton::SetHeight (uint height)
{
  Text->SetHeight (height);
  CControl::SetHeight (height);
};

//eventos

void
CButton::key_press (XEvent event)
{
  KeySym key = 0;
  char text[10];
  Status status;

  XXLookupString (NULL, &event.xkey, text, 10, &key, &status);
  if (key == XK_Return)
    {
      Press = true;
      Draw ();
      CControl::button_press (event);
    }
  else
    CControl::key_press (event);
};

void
CButton::key_release (XEvent event)
{
  KeySym key = 0;
  char text[10];
  Status status;

  XXLookupString (NULL, &event.xkey, text, 10, &key, &status);
  if (key == XK_Return)
    {
      Press = false;
      Draw ();
      CControl::button_release (event);
    }
  else
    CControl::key_release (event);
};

void
CButton::button_press (XEvent event)
{
  if (event.xbutton.button == 1)
    {
      Press = true;
      Draw ();
      CControl::button_press (event);
    };
};

void
CButton::button_release (XEvent event)
{
  if (event.xbutton.button == 1)
    {
      Press = false;
      Draw ();
      CControl::button_release (event);
    };
};

void
CButton::pointer_in (void)
{
  CControl::pointer_in ();
};

void
CButton::pointer_out (void)
{
  CControl::pointer_out ();
  if (Press)
    {
      Draw ();
      Press = false;
    };
};

void
CButton::SetAlign (CAlign align)
{
  Text->SetAlign (align);
};

CAlign CButton::GetAlign (void)
{
  return Text->GetAlign ();
};

void
CButton::focus_in (void)
{
  Draw ();
  CControl::focus_out ();
};

void
CButton::focus_out (void)
{
  Draw ();
  CControl::focus_out ();
};
