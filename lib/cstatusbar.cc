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

#include"../include/cstatusbar.h"

CStatusbar::CStatusbar (void)
{
  CanVisible = false;
  CanFocus = false;
  SetClass ("CStatusbar");
  SetX (0);
  SetY (100);
  SetHeight (25);
  SetWidth (800);
}

CStatusbar::~CStatusbar (void)
{
}

int 
CStatusbar::Create (CControl * control)
{
  return CControl::Create (control);
}

lxStringList 
CStatusbar::GetContext (void)
{
//FIXME	
  lxStringList sl;
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
  return sl;  
}

void 
CStatusbar::SetContext (lxStringList context)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}

void 
CStatusbar::SetFields (lxString litems)
{
  int f = 0;
  Fields.Clear ();
  while (litems.size () > 0)
    {
      f = litems.find (lxT(","));
      if(f < 0)break;
      AddField (litems.substr (0, f));
      litems = litems.substr (f + 1, litems.size () - f -1);
    };
}

lxString 
CStatusbar::GetFields (void)
{
  lxString list = lxT("");
  for (uint c = 0; c < Fields.GetLinesCount (); c++)
    {
      list += Fields.GetLine (c) + lxT(",");
    };
  return list;
}

lxString 
CStatusbar::GetField (int item)
{
if (item < (int) Fields.GetLinesCount ())
    return Fields.GetLine (item);
  else
    return lxT('\0');
}
  
void 
CStatusbar::SetField (int item, lxString sitem)
{
  if (item < (int) Fields.GetLinesCount ())
  {
    Fields.SetLine (sitem, item);
    Draw();
  }
}

int 
CStatusbar::GetFieldsCount (void)
{
  return Fields.GetLinesCount();	
}

void 
CStatusbar::AddField (lxString text)
{
  Fields.AddLine (text);
  Draw();
}

void
CStatusbar::SetX (int x)
{
  X = 0;
}

void
CStatusbar::SetY (int y)
{
  if(Owner != NULL)
  {	  
    if(((CWindow *) Owner)->HasMenu)
      Y = Owner->GetHeight()-90;
    else
      Y = Owner->GetHeight()-30;
  }
}

void
CStatusbar::Draw (void)
{
  if ((!Visible)||(Paint == NULL))return;
  Width = Owner->GetWidth ();
  if(((CWindow *) Owner)->HasMenu)
    Y = Owner->GetHeight()-50;
  else
    Y = Owner->GetHeight()-25;
  Paint->InitDraw (this);
  Paint->Pen.SetColor (Color);
  Paint->Rectangle ( 2, 2, Owner->GetWidth () - 2, Height - 2);
  Paint->RaiserFrame ( 0, 0, Owner->GetWidth (), Height);
  
  Paint->Pen.SetFgColor (ColorByRGB(0,0,0));
  Paint->Pen.SetBgColor (Color);
  for (unsigned int i = 0; i < Fields.GetLinesCount (); i++)
  {
    Paint->ImgText ((Owner->GetWidth()*i/Fields.GetLinesCount())+5 , 15 , Fields.GetLine(i).c_str());
  }

  CControl::Draw ();
};

void
CStatusbar::Update (void)
{
  if (Win == NULL)
    return;
  if ((Owner->GetWidth () == Width))
    CControl::Update ();
  else
    SetWidth (Owner->GetWidth ());
};
