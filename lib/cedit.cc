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

#include"../include/cedit.h"

CCursor::CCursor ()
{
  SetWidth (1);
  SetHeight (1);
  SetVisible (false);
};

int 
CCursor::Create (CControl * control)
{
  return CControl::Create (control);
}

void
CCursor::Draw (void)
{
  if ((!Visible)||(Paint == NULL))return;
  Paint->InitDraw (this);
  Paint->Rectangle (0,0, Width, Height);
  CControl::Draw ();
};

// CEdit_____________________________________________________________

CEdit::CEdit (void)
{
  Text = new CLabel;
  Text->SetText ("");
  Text->SetAlign (CA_LEFT);
  Text->SetFont (FontName);
  Text->SetX (0);
  Text->SetY (0);

  Cursor = new CCursor;

  SetX (10);
  SetY (10);
  SetWidth (60);
  SetHeight (20);
  SetReadOnly (false);
  SetMaxLenght (255);
  CursorPos = 0;
  SetClass ("CEdit");
  CreateChild (Text);
  CreateChild (Cursor);
};


CEdit::~CEdit (void)
{
};

int
CEdit::Create (CControl * control)
{
  if (!ColorSet)
    SetColor ("white");
  return CControl::Create (control);
}

void
CEdit::Draw ()
{
  if ((!Visible)||(Paint == NULL))return;
  Paint->InitDraw (this);
  Paint->Pen.SetColor (Color);
  Paint->Rectangle ( 2, 2, Width - 3, Height - 3);
  Paint->LowerFrame ( 0, 0, Width, Height);
  DrawCursor ();
  CControl::Draw ();
};

void
CEdit::DrawCursor (void)
{
  if ((Win == NULL) || !(GetFocus ()))
    return;
  int x = 0;

  x = XTextWidth (CFont, Text->GetText ().c_str (),  GetCursorPos ()) - Text->GetTextPosition ();

/*
  cout << "x              =" << x << endl;
  cout << "width          =" << Width << endl;
  cout << "Cursorposition =" << GetCursorPos () << endl;
  cout << "Textposition   =" << Text->GetTextPosition () << endl;
  cout << "Textpointer    =" << Text->GetTextPointer () << endl;
  cout << "TextMaxDisplay =" << Text->GetTextMaxDisplay () << endl;
  cout << "---------------\n";
*/
  /*
     if (x < -5)
     Text->DecTextPointer ();

     if ((x+6) > ((int) (Width)))
     Text->IncTextPointer ();
   */
  if (x < 2) x=2;
  if (x > (int)(Width - 2)) x=Width-2;
  
  Cursor->SetVisible (false);
  Cursor->SetX (x);
  Cursor->SetY ((Height-(Text->GetTextHeight ()+2))/2);
  Cursor->SetHeight (Text->GetTextHeight ()+2);
  Cursor->SetVisible (true);
};

CStringList CEdit::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("Text=" + GetText () + ";String");
  return Context;
};

void
CEdit::SetContext (CStringList context)
{
  Erase ();
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


//propriedades

void
CEdit::SetText (String t)
{
  Text->SetText (t);
  if (t.size () == 0)
    SetCursorPos (0);
  Draw ();
};

String CEdit::GetText (void)
{
  return Text->GetText ();
};

void
CEdit::SetReadOnly (bool r)
{
  ReadOnly = r;
};

bool CEdit::GetReadOnly (void)
{
  return ReadOnly;
};

void
CEdit::SetMaxLenght (int l)
{
  MaxLenght = l;
};

int
CEdit::GetMaxLenght (void)
{
  return MaxLenght;
};


void
CEdit::SetAlign (CAlign align)
{
  Text->SetAlign (align);
};

CAlign CEdit::GetAlign (void)
{
  return Text->GetAlign ();
};

void
CEdit::SetCursorPos (uint cursorpos)
{
  CursorPos = cursorpos;
  if (((int) Text->GetText ().size () - (int) Text->GetTextMaxDisplay ()) > 0)
    {
      int mdisp = (int) Text->GetTextMaxDisplay ();
      int pointer = (int) CursorPos - mdisp;

      if (pointer > (int) Text->GetTextPointer ())
	{
	  Text->SetTextPointer (pointer);
	}
      else
	{
	  if (Text->GetTextPointer () == CursorPos + 1)
	    Text->SetTextPointer (Text->GetTextPointer () - 1);
	};
    }
  else
    {
      Text->SetTextPointer (0);
    };
  Draw ();
};

uint CEdit::GetCursorPos (void)
{
  return CursorPos;
};


void
CEdit::SetPrevKey (char prevkey)
{
  PrevKey = prevkey;
};

char
CEdit::GetPrevKey (void)
{
  return PrevKey;
};

void
CEdit::SetX (int x)
{
  Cursor->SetX (Cursor->GetX () + (X - x));
  CControl::SetX (x);
};

void
CEdit::SetY (int y)
{
  Cursor->SetY (Cursor->GetY () + (Y - y));
  CControl::SetY (y);
};

void
CEdit::SetWidth (uint width)
{
  Text->SetWidth (width);
  CControl::SetWidth (width);
};

void
CEdit::SetHeight (uint height)
{
  Text->SetHeight (height);
  CControl::SetHeight (height);
};


//eventos

void
CEdit::button_press (XEvent event)
{
  int n;
  String buffer;
  switch (event.xbutton.button)
    {
    case 1:
      n = GetText ().size ();
      if (n)
	XStoreBuffer (Win->GetADisplay (), GetText ().c_str (), n, 0);
      break;
    case 2:
      char *buf = XFetchBuffer (Win->GetADisplay (), &n, 0);
      buffer = buf;
      if (buf)
	delete[]buf;
      if (n)
	SetText (buffer);
      break;
    };
  CControl::button_press (event);
};

void
CEdit::key_press (XEvent event)
{
  int key;
  if (ReadOnly)
    return;
  key = MEdit (this, event);
  CControl::key_press (event);
  if (key == XK_Return)
    focus_out ();
};

void
CEdit::focus_in (void)
{
  DrawCursor ();
  CControl::focus_in ();
};

void
CEdit::focus_out (void)
{
  Cursor->SetVisible (false);
  CControl::focus_out ();
};
