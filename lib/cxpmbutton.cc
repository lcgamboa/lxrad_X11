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


#include"../include/cxpmbutton.h"


// CXpmButton___________________________________________________________

CXpmButton::CXpmButton (void)
{
  Xpm = new CDraw;
  Xpm->SetBorder (0);
  Xpm->SetCanExecuteEvent (false);
  Xpm->SetTransparent (true);
  Xpm->SetX (2);
  Xpm->SetY (2);
  Text->SetX (18);
  Text->SetY (0);
  SetX (10);
  SetY (10);
  SetWidth (60);
  SetHeight (20);
  SetClass ("CXpmButton");
  CreateChild (Xpm);
};

CXpmButton::~CXpmButton (void)
{
};

int
CXpmButton::Create (CControl * control)
{
  int ret=CButton::Create (control);
  Xpm->SetColor (Color);
  return ret;
}

void
CXpmButton::Draw (void)
{
  XColor TColor;
	
  if ((!Visible)||(Paint == NULL))return;
  Paint->InitDraw (this);
  if (Press && PointerOn)
    {
      Xpm->SetColor (PressColor);
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
      Xpm->SetColor (Color);
      Paint->Pen.SetColor (Color);
      Paint->Rectangle ( 0, 0, Width, Height);
      Paint->RaiserFrame ( 0, 0, Width, Height, Border);
      CControl::Draw ();
    };
};

lxStringList CXpmButton::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("Text=" + GetText () + ";String");
  Context.AddLine ("PixmapFileName=" + GetPixmapFileName () + ";File");
  return Context;
};

void
CXpmButton::SetContext (lxStringList context)
{
  Erase ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      lxString line = Context.GetLine (i);
      lxString arg;
      eqparse (line, arg);
      if (line.compare ("Text") == 0)
	SetText (arg);
      if (line.compare ("PixmapFileName") == 0)
      {
	if(arg.size() >  0)
	  SetPixmapFileName (arg);
	else
	  SetPixmapFileName ("");
      };
    };
  Draw ();
};

//propiedades

void
CXpmButton::SetPixmapData (char **data)
{
  Xpm->SetPixmapData (data);
  Draw ();
};

void
CXpmButton::SetPixmapFileName (lxString filename)
{
  Xpm->SetPixmapFileName (filename);
};

lxString
CXpmButton::GetPixmapFileName (void)
{
  return Xpm->GetPixmapFileName ();
};

void
CXpmButton::SetWidth (uint width)
{
  CControl::SetWidth (width);
  Xpm->SetWidth (16);
  Text->SetWidth (width - 22);
};

void
CXpmButton::SetHeight (uint height)
{
  CControl::SetHeight (height);
  Xpm->SetHeight (16);
  Text->SetHeight (height);
};
