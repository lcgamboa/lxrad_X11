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

#include"../include/medit.h"
#include"../include/cedit.h"
#include"../include/cwindow.h"


//Manipulating Text---------------------------------------------------------------
KeySym
MEdit (CEdit * control, XEvent event)
{
  //buffers 
  uint cursorpos;
  KeySym key;
  char text[10];
  int status;
  XXLookupString (control->GetWin ()->GetIC (), &event.xkey,
		  text, 10, &key, &status);

  if (key == XK_Right)
    {
      if (control->GetCursorPos () < control->GetText ().size ())
	control->SetCursorPos (control->GetCursorPos () + 1);
      return key;
    };
  if (key == XK_Left)
    {
      if (control->GetCursorPos () > 0)
	control->SetCursorPos (control->GetCursorPos () - 1);
      return key;
    };

  if (status == XLookupBoth)
    {
      switch (key)
	{
	case XK_Return:
	  return key;
	  break;
	case XK_Caps_Lock:
	  return key;
	  break;
	case XK_Shift_L:
	  return key;
	  break;
	case XK_Shift_R:
	  return key;
	  break;
	case XK_Down:
	  return key;
	  break;
	case XK_Up:
	  return key;
	  break;
	case XK_BackSpace:
	  if (control->GetText ().size () > 0)
	    {
	      uint cursorpos;
	      cursorpos = control->GetCursorPos ();
	      lxString nstr = strndel (control->GetText ().c_str (), cursorpos);
	      control->SetText (nstr);
	      if (cursorpos > 0)
		control->SetCursorPos (cursorpos - 1);
	      else
		control->SetCursorPos (cursorpos);
	    };
	  return key;
	  break;
	case XK_Delete:
	  if (control->GetText ().size () > 0)
	    {
	      cursorpos = control->GetCursorPos ();
	      lxString nstr =
		strndel (control->GetText ().c_str (), cursorpos + 1);
	      control->SetText (nstr);
	      control->SetCursorPos (cursorpos);
	    };
	  return key;
	  break;
	default:
	  if (status == XLookupChars || status == XLookupBoth)
	    {

	      if (control->GetText ().size () ==
		  (uint) (control->GetMaxLenght ()))
		return key;
	      cursorpos = control->GetCursorPos ();
	      lxString nstr = strnadd (control->GetText (), text[0], cursorpos);
	      control->SetText (nstr);
	      control->SetCursorPos (cursorpos + 1);
	      control->CControl::key_press (event);
	      return key;
	    };
	  break;
	};
    };
  return key;
};
