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

#include"../include/cgauge.h"

CGauge::CGauge (void)
{
  Value=50;
  Range=100;
  Type=0;
  SetClass ("CGauge");
  SetX (10);
  SetY (10);
  SetWidth (60);
  SetHeight (20);
}

int
CGauge::Create (CControl * control)
{
  return CControl::Create (control);
}

void
CGauge::Draw (void)
{
  if((!Visible)||(Paint == NULL))return;
  
  Paint->InitDraw (this);

      Paint->Pen.SetColor (Color);
      Paint->Rectangle ( 0, 0, Width, Height);
      Paint->Pen.SetColor (ColorByRGB(0,0,200));
      unsigned int wg= Width*Value/Range;
      if(wg > Width)wg=Width;
      Paint->Rectangle ( 0, 0, wg , Height);
      Paint->LowerFrame ( 0, 0, Width, Height, Border);
      CControl::Draw ();		
      
};

lxStringList 
CGauge::GetContext (void)
{
//FIXME
 lxStringList sl;
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
 return sl;
}

void 
CGauge::SetContext (lxStringList context)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}

void 
CGauge::SetValue (int value)
{
  if(Value != value)
  {	  
    Value=value;
    Draw();
  }  
}

int 
CGauge::GetValue (void)
{
  return Value;	
}

void 
CGauge::SetRange (int range)
{
  Range=range;	
  Draw();  
}

int 
CGauge::GetRange (void)
{
  return Range;	
}

void 
CGauge::SetType (int type)
{
  Type=type;	
}

int 
CGauge::GetType (void)
{
  return Type;	
}

