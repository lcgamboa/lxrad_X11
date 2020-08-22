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

#include"../include/cspind.h"

//Pixmap____________________________________________________________

static const char *uarrow[] = {
  "16 10 3 1",
  " 	c None",
  ".	c #FFFFFF",
  "+	c #000000",
  "                ",
  "       ++       ",
  "      ++++      ",
  "     ++++++     ",
  "    ++++++++    ",
  "   ++++++++++   ",
  "  ++++++++++++  ",
  "                ",
  "                ",
  "                "
};

static const char *darrow[] = {
  "16 10 3 1",
  " 	c None",
  ".	c #FFFFFF",
  "+	c #000000",
  "                ",
  "  ++++++++++++  ",
  "   ++++++++++   ",
  "    ++++++++    ",
  "     ++++++     ",
  "      ++++      ",
  "       ++       ",
  "                ",
  "                ",
  "                "
};

// CSpind_____________________________________________________________

CSpind::CSpind (void)
{
  Value=50;				
  Min=0;
  Max=100;
  Inc=1;
  Digits=1;
  EvOnChangeSpinDouble=NULL;
  
  SetClass ("CSpind");
  edit1.SetText ("");
  edit1.SetFont (FontName);
  edit1.SetReadOnly (true);
  edit1.SetFOwner (this);
  edit1.EvKeyboardPress = EVKEYBOARDPRESS & CSpind::EditKeyPress;

  tbutton1.SetPixmapData (uarrow);
  tbutton1.SetFOwner (this);
  tbutton1.EvMouseButtonPress = EVMOUSEBUTTONPRESS & CSpind::TButtonPress1;
  
  tbutton2.SetPixmapData (darrow);
  tbutton2.SetFOwner (this);
  tbutton2.EvMouseButtonPress = EVMOUSEBUTTONPRESS & CSpind::TButtonPress2;

  SetX (10);
  SetY (10);
  SetHeight (20);
  SetWidth (80);
  CreateChild (&edit1);
  CreateChild (&tbutton1);
  CreateChild (&tbutton2);
}

CSpind::~CSpind (void)
{
}

int
CSpind::Create (CControl * control)
{
  return CControl::Create (control);
}

void
CSpind::SetWidth (unsigned width)
{
  edit1.SetX (0);
  edit1.SetWidth (width - GetHeight () - 1);
  tbutton1.SetX (width - GetHeight ());
  tbutton1.SetWidth (GetHeight ());
  tbutton2.SetX (width - GetHeight ());
  tbutton2.SetWidth (GetHeight ());

  CControl::SetWidth (width);
};

void
CSpind::SetHeight (unsigned height)
{
  edit1.SetY (0);
  edit1.SetHeight (height);
  tbutton1.SetY (0);
  tbutton1.SetHeight (height/2);
  tbutton2.SetY (height/2);
  tbutton2.SetHeight (height/2);
  CControl::SetHeight (height);
};

void
CSpind::Draw ()
{
  if ((!Visible)||(Paint == NULL))
    return;
  Paint->InitDraw (this);
  Paint->Pen.SetColor (Color);
  Paint->Rectangle ( 0, 0, Width, Height);
  CControl::Draw ();
};

lxStringList 
CSpind::GetContext (void)
{
  //FIXME	
  lxStringList sl;
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
  return sl;
}

void 
CSpind::SetContext (lxStringList context)
{
 printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}
  
void 
CSpind::SetValue(double v)
{	
  Value=v;
  edit1.SetText(lxString().Printf("%.*lf",Digits,Value));  
}

double 
CSpind::GetValue (void)
{
  return Value;	
}

void 
CSpind::SetMin(double v)
{
  Min=v;	
}

double 
CSpind::GetMin (void)
{
 return Min;	
}

void 
CSpind::SetMax(double v)
{
  Max=v;	
}

double 
CSpind::GetMax (void)
{
  return Max;	
}

void 
CSpind::SetInc(double v)
{
  Inc=v;	
}

double 
CSpind::GetInc (void)
{
  return Inc;	
}

void 
CSpind::SetDigits(int v)
{
  Digits=v;
  SetValue(Value);
}

int 
CSpind::GetDigits(void)
{
  return Digits;	
}

void
CSpind::EditKeyPress (CControl * control, const uint key, const uint hk,  uint mask)
{
  Update ();
};

void
CSpind::TButtonPress1 (CControl * control, uint button, uint x, uint y, uint state)
{
  Value+=Inc;
  if(Value > Max)Value=Max;
  SetValue(Value);
};

void
CSpind::TButtonPress2 (CControl * control, uint button, uint x, uint y, uint state)
{
  Value-=Inc;
  if(Value < Min)Value=Min;
  SetValue(Value);
};
