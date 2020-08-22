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


#include"../include/ctoolbutton.h"


// CToolButton___________________________________________________________

CToolButton::CToolButton (void)
{
  Xpm = new CDraw;
  Xpm->SetBorder (0);
  Xpm->SetCanExecuteEvent (false);
  Xpm->SetTransparent (true);
  Xpm->SetX (2);
  Xpm->SetY (2);
  SetX (10);
  SetY (10);
  SetWidth (26);
  SetHeight (26);
  Press = false;
  SetClass ("CToolButton");
  CanFocus=false;
  DestroyChilds ();
  CreateChild (Xpm);
};

CToolButton::~CToolButton (void)
{
};

int
CToolButton::Create (CControl * control)
{
  int ret=CButton::Create (control);
  Xpm->SetColor (GetColor ());
  return ret;
}

void
CToolButton::Draw (void)
{
  if ((!Visible)||(Paint == NULL))return;
  Paint->InitDraw (this);
  if (Press && PointerOn)
    {
      Paint->Pen.SetColor (PressColor);
      Paint->Rectangle ( 0, 0, Width, Height);
      Xpm->SetColor (PressColor);
      Paint->LowerFrame ( 0, 0, Width, Height, Border);
    }
  else
    {
      Paint->Pen.SetColor (Color);
      Paint->Rectangle ( 0, 0, Width, Height);
      Xpm->SetColor (Color);
      Paint->RaiserFrame ( 0, 0, Width, Height, Border);
    };
  CControl::Draw ();
};

lxStringList CToolButton::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("PixmapFileName=" + GetPixmapFileName () + ";File");
  return Context;
};

void
CToolButton::SetContext (lxStringList context)
{
  Erase ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      lxString line = Context.GetLine (i);
      lxString arg;
      eqparse (line, arg);
      if (line.compare ("PixmapFileName") == 0)
      {
	if(arg.size() >  0)
	  SetPixmapFileName (arg);
	else
	  SetPixmapFileName ("");
      }
    };
  Draw ();
};

//propiedades

void
CToolButton::SetPixmapData (const char **data)
{
  Xpm->SetPixmapData ((char **)data);
  Draw ();
}

void
CToolButton::SetImgData (const char **data)
{
  Xpm->SetPixmapData ((char **)data);
  Draw ();
}

void
CToolButton::SetPixmapFileName (lxString filename)
{
  Xpm->SetPixmapFileName (filename);
  Draw ();
}

void
CToolButton::SetImgFileName (lxString filename)
{
  Xpm->SetPixmapFileName (filename);
  Draw ();
}

lxString
CToolButton::GetPixmapFileName (void)
{
  return Xpm->GetPixmapFileName ();
};

void
CToolButton::SetWidth (uint width)
{
  CControl::SetWidth (width);
  Xpm->SetWidth (width - 4);
};

void
CToolButton::SetHeight (uint height)
{
  CControl::SetHeight (height);
  Xpm->SetHeight (height - 4);
};
