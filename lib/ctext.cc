/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001-2020 Luis Claudio Gamboa Lopes

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


#include"../include/ctext.h"

//CText----------------------------------------------------------------

CText::CText (void)
{
  X = 10;
  Y = 10;
  Width = 100;
  Height = 100;
  CursorLin = 1;
  PLine = 0;
  PChar = 0;
  SetClass ("CText");
};

CText::~CText (void)
{
};

void
CText::Draw (void)
{
  if ((!Visible)||(Paint == NULL))
    return;
  Paint->InitDraw (this);
  Paint->Pen.SetColor (Color);
  Paint->Rectangle ( 2, 2, Width - 3, Height - 3);
  Paint->Pen.SetColor (ColorByName ("black"));
  for (unsigned int q = 1; q < Lines.GetLinesCount (); q++)
    Paint->Text (Lines.GetLine(q), 5, 15 + (13 * (q - 1)));
  Paint->LowerFrame ( 0, 0, Width, Height);
  int x =
    XTextWidth (CFont, Lines.GetLine (CursorLin).c_str (), GetCursorPos ());
  int y = 12;
  Paint->Pen.SetColor (Color);
  Paint->Pen.SetPen (GXxor);
  Paint->Line ( 4 + x, (13 * (CursorLin - 1) + 4), 4 + x,
		    (13 * (CursorLin - 1) + 4) + y);
  Paint->Pen.SetPen (GXcopy);
  CControl::Draw ();
};

void
CText::DrawLine (void)
{
  if ((!Visible)||(Paint == NULL))
    return;
  Paint->InitDraw (this);
  Paint->Pen.SetColor (Color);
  Paint->Rectangle ( 4, (13 * CursorLin - 1) - 21, Width, 39);
  Paint->Pen.SetColor (ColorByName ("black"));
  if (CursorLin > 1)
    Paint->Text(Lines.GetLine (CursorLin - 1), 5, 15 + (13 * (CursorLin - 2)));
  Paint->Text (Lines.GetLine (CursorLin),  5, 15 + (13 * (CursorLin - 1)));
  if (CursorLin < Lines.GetLinesCount () - 1)
    Paint->Text (Lines.GetLine (CursorLin + 1), 5, 15 + (13 * (CursorLin)));
  //cursor
  int x =
    XTextWidth (CFont, Lines.GetLine (CursorLin).c_str (), GetCursorPos ());
  int y = 12;
  Paint->Pen.SetColor (Color);
  Paint->Pen.SetPen (GXxor);
  Paint->Line ( 4 + x, (13 * (CursorLin - 1) + 4), 4 + x,
		    (13 * (CursorLin - 1) + 4) + y);
  Paint->Pen.SetPen (GXcopy);
  Paint->LowerFrame ( 0, 0, Width, Height);
  CControl::Draw ();
};

void
CText::DrawCursor (void)
{
  if (Win == NULL)
    return;
/*
  int x = XTextWidth (Font, Lines.GetLine (CursorLin), GetCursorPos ());
  int y = 12;
  Paint->InitDraw (this);
  Paint->Pen.SetPen (GXinvert);
  Paint->Line (this, 0, 0, 0, y - 1);
  Paint->Pen.SetPen (GXcopy);
  CControl::Draw ();
*/
};


int
CText::Create (CControl * control)
{
  int ret=CControl::Create (control);
  Color = ColorByName ("white");
  return ret;
}

void
CText::Clear (void)
{
  Lines.Clear ();
  Draw ();
};

void
CText::AddLine (const char *line)
{
  Lines.AddLine (line);
  Draw ();
}

void
CText::AddLine (const String line)
{
  Lines.AddLine (line.c_str());
  Draw ();
}

void
CText::InsertLine (const char *line)
{
  Lines.InsertLine (line, CursorLin);
  Draw ();
};

void
CText::DelLine (void)
{
  Lines.DelLine (CursorLin);
  Draw ();
};

void
CText::LoadFromFile (const char *fname)
{
  Lines.LoadFromFile (fname);
  Draw ();
};

void
CText::SaveToFile (const char *fname)
{
  Lines.SaveToFile (fname);
};

void
CText::SaveToFile (const String fname)
{
  Lines.SaveToFile (fname.c_str());
};


//propiedades
unsigned int
CText::GetCountLines (void)
{
  return Lines.GetLinesCount ();
};


void
CText::SetText (const String t)
{
//  if(Lines.GetLine(GetCursorLin()) != NULL) Lines.DelLine(CursorLin);
  Lines.SetLine (t, CursorLin);
  SetCursorPos (t.size ());
  //if(Win)DrawLine();
};

String CText::GetText (void)
{
  return Lines.GetLine (CursorLin);
};

void
CText::SetCursorPos (uint cursorpos)
{
  DrawCursor ();
  CursorPos = cursorpos;
  DrawCursor ();
};


//eventos

void
CText::key_press (XEvent event)
{
  if (ReadOnly)
    return;
  KeySym key;
  char text[10];
  XLookupString (&event.xkey, text, 10, &key, NULL);
  switch (key)
    {
    case XK_BackSpace:
      if (GetCursorPos () == 0)
	{
	  eprint( "del preturn\n");
	};
      break;
    case XK_Delete:
      if (GetCursorPos () == Lines.GetLine (CursorLin).size ())
	{
	  //code
	};
      break;
    case XK_Left:
      //code
      break;
    case XK_Right:
      //code
      break;
    };
  key = MEdit (this, event);
  switch (key)
    {
    case XK_Up:
      if (CursorLin > 1)
	{
	  DrawCursor ();
	  CursorLin -= 1;
	  if (GetCursorPos () > GetText ().size ())
	    {
	      DrawCursor ();
	      SetCursorPos (GetText ().size ());
	    }
	  else
	    {
	      DrawCursor ();
	    };
	};
      break;
    case XK_Down:
      if (CursorLin < Lines.GetLinesCount () - 1)
	{
	  DrawCursor ();
	  CursorLin += 1;
	  if (GetCursorPos () > GetText ().size ())
	    {
	      DrawCursor ();
	      SetCursorPos (GetText ().size ());
	    }
	  else
	    {
	      DrawCursor ();
	    };
	};
      break;
    case XK_Return:
      char *str;
      str = new char[GetCursorPos () + 1];
      strncpy (str, Lines.GetLine (CursorLin).c_str (), GetCursorPos ());
      str[GetCursorPos ()] = '\0';
      for (uint a = 1; a <= GetCursorPos (); a++)
	Lines.SetLine (strndel (Lines.GetLine (CursorLin), 1), CursorLin);
      InsertLine (str);
      delete str;
      SetCursorPos (0);
      CursorLin++;
      DrawLine ();
      return;
      break;
    default:
      DrawLine ();
      break;
    };
  CControl::key_press (event);
};
