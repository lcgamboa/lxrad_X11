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

#include"../include/ccolordialog.h"

  
CColorDialog::CColorDialog (void)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
  EvOnClose=NULL;
  CanVisible = false;
}

CColorDialog::~CColorDialog (void)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}

int
CColorDialog::Create (CControl * control)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
  return 0;
}

void 
CColorDialog::Run (void)
{
//FIXME	
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
return;
}

lxString 
CColorDialog::GetColorName (void)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
return ""; 	
}

void 
CColorDialog::SetColorName (lxString cname)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}

lxColor 
CColorDialog::GetColor (void)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
  return Color;	
}

void 
CColorDialog::SetColor (lxColor c)
{
  Color=c;	
}

void 
CColorDialog::SetColor (uint r, uint g, uint b)
{
  Color=ColorByRGB(r,g,b);	
}

lxStringList 
CColorDialog::GetContext (void)
{
//FIXME	
lxStringList sl;
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
return sl;	
}

void 
CColorDialog::SetContext (lxStringList context)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}
