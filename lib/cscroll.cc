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


#include"../include/cscroll.h"

//Pixmaps____________________________________________________________
static const char *rarrow[] = {
  "16 16 2 1",
  " 	c None",
  ".	c #000000",
  "                ",
  "                ",
  "     .          ",
  "     ..         ",
  "     ...        ",
  "     ....       ",
  "     .....      ",
  "     ......     ",
  "     ......     ",
  "     .....      ",
  "     ....       ",
  "     ...        ",
  "     ..         ",
  "     .          ",
  "                ",
  "                "
};

static const char *larrow[] = {
  "16 16 3 1",
  " 	c None",
  ".	c #FFFFFF",
  "+	c #000000",
  "                ",
  "                ",
  "          +     ",
  "         ++     ",
  "        +++     ",
  "       ++++     ",
  "      +++++     ",
  "     ++++++     ",
  "     ++++++     ",
  "      +++++     ",
  "       ++++     ",
  "        +++     ",
  "         ++     ",
  "          +     ",
  "                ",
  "                "
};


static const char *uarrow[] = {
  "16 16 3 1",
  " 	c None",
  ".	c #FFFFFF",
  "+	c #000000",
  "                ",
  "                ",
  "                ",
  "                ",
  "                ",
  "       ++       ",
  "      ++++      ",
  "     ++++++     ",
  "    ++++++++    ",
  "   ++++++++++   ",
  "  ++++++++++++  ",
  "                ",
  "                ",
  "                ",
  "                ",
  "                "
};

static const char *darrow[] = {
  "16 16 3 1",
  " 	c None",
  ".	c #FFFFFF",
  "+	c #000000",
  "                ",
  "                ",
  "                ",
  "                ",
  "                ",
  "  ++++++++++++  ",
  "   ++++++++++   ",
  "    ++++++++    ",
  "     ++++++     ",
  "      ++++      ",
  "       ++       ",
  "                ",
  "                ",
  "                ",
  "                ",
  "                "
};

// CScroll___________________________________________________________

CScroll::CScroll (void)
{
  button1 = new CToolButton;
  button1->SetName ("Button1");
  button1->SetTag (0);
  button1->SetFOwner (this);
  button1->EvMouseButtonPress = EVMOUSEBUTTONPRESS & CScroll::ScrollButtonPress;
  button1->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & CScroll::ScrollButtonRelease;
  button2 = new CToolButton;
  button2->SetName ("Button2");
  button2->SetTag (1);
  button2->SetFOwner (this);
  button2->EvMouseButtonPress = EVMOUSEBUTTONPRESS & CScroll::ScrollButtonPress;
  button2->EvMouseButtonRelease =EVMOUSEBUTTONRELEASE & CScroll::ScrollButtonRelease;
  Type = 0;
  Size = 0;
  SetRange (7);
  SetPosition (3);
  SetWidth (20);
  SetHeight (160);
  SetX (10);
  SetY (10);
  SetType (st_horizontal);
  SetType (st_vertical);
  EvOnChangePosition = NULL;
  SetClass ("CScroll");
//  SetCanFocus (false);
  Move = false;
  CreateChild (button1);
  CreateChild (button2);
};

CScroll::~CScroll (void)
{
};

void
CScroll::Create (CControl * control)
{
  CControl::Create (control);
  if (!ColorSet)
    Color = ColorByName ("gray");
};

void
CScroll::Draw (void)
{
  if ((!Visible)||(Paint == NULL))
    return;
  Paint->InitDraw (this);

  if (Type == st_vertical)
    {
      int hb = button1->GetHeight ();
      int h2;
      Paint->Pen.SetColor (Color);
      Paint->Rectangle ( 0, hb, Width, Height - (2 * hb));
      Paint->Pen.SetColor (ColorByName ("light gray"));

      if (Position == 0)
	{
	  h2 = 0;
	}
      else
	{
	  if (Position == Range - 1)
	    h2 = ((Height - (2 * hb)) - Size);
	  else
	    h2 = (((Height - (2 * hb) - Size) * Position) / Range);
	};

      Paint->Rectangle ( 0, hb + h2, Width, Size);
      Paint->RaiserFrame ( 0, hb + h2, Width, hb + h2 + Size, 1);
    }
  else
    {
      int wb = button1->GetWidth ();
      int w2;

      Paint->Pen.SetColor (Color);
      Paint->Rectangle ( wb, 0, Width - (2 * wb), Height);
      Paint->Pen.SetColor (ColorByName ("light gray"));

      if (Position == 0)
	{
	  w2 = 0;
	}
      else
	{
	  if (Position == Range - 1)
	    w2 = ((Width - (2 * wb)) - Size);
	  else
	    w2 = (((Width - (2 * wb) - Size) * Position) / Range);
	};
      Paint->Rectangle ( wb + w2, 0, Size, Height);
      Paint->RaiserFrame ( wb + w2, 0, wb + w2 + Size, Height);
    };
  CControl::Draw ();
};

CStringList 
CScroll::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("OnChangePosition=" + btoa (GetEv ()) + ";event");
  return Context;
};

void
CScroll::SetContext (CStringList context)
{
  Eraser ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      string line = Context.GetLine (i);
      string arg;
      eqparse (line, arg);
      if (line.compare ("OnChangePosition") == 0)
	SetEv (atob (arg));
    };
  Draw ();
};

//propiedades

void
CScroll::SetPosition (int position)
{
  if ((position >= 0) && (position < Range))
    {
      if (Position != position)
	{
	  Position = position;
	  Draw ();
	  if ((FOwner) && (EvOnChangePosition))
	    (FOwner->*EvOnChangePosition) (this);
	}
    }
}

int
CScroll::GetPosition (void)
{
  return Position;
}

void
CScroll::SetRange (int range)
{
  if(range <= 0)range=1;	
  
  Range = range;

  if (Type == st_vertical)
    {
      if (Range > 0)
	Size = (Height - (2 * button1->GetHeight ())) / Range;
      else
	Size = Height - (2 * button1->GetHeight ());
    }
  else
    {
      if (Range > 0)
	Size = (Width - (2 * button1->GetWidth ())) / Range;
      else
	Size = Width - (2 * button1->GetWidth ());
    };
  if (Size < 20)
    Size = 20;
};

int
CScroll::GetRange (void)
{
  return Range;
};


void
CScroll::SetType (scrolltype type)
{
  if (Type != type)
    {
      Type = type;
      /*
      int temp = GetWidth ();
      SetWidth (GetHeight ());
      SetHeight (temp);
      */
      SetWidth (Width);
      SetHeight (Height);
      SetRange (Range);
      if (Type == st_vertical)
	{
	  button1->SetPixmapData ((char **)uarrow);
	  button2->SetPixmapData ((char **)darrow);
	}
      else
	{
	  button1->SetPixmapData ((char **)larrow);
	  button2->SetPixmapData ((char **)rarrow);
	};
      Draw ();
    }
};

void
CScroll::SetX (int x)
{
  if (Type == st_vertical)
    {
      button1->SetX (0);
      button2->SetX (0);
    }
  else
    {
      button1->SetX (0);
      button2->SetX (Width - button2->GetWidth ());
    };
  CControl::SetX (x);
};

void
CScroll::SetY (int y)
{
  if (Type == st_vertical)
    {
      button1->SetY (0);
      button2->SetY (Height - button2->GetHeight ());
    }
  else
    {
      button1->SetY (0);
      button2->SetY (0);
    };
  CControl::SetY (y);
};

void
CScroll::SetWidth (uint width)
{
  if (Type == st_vertical)
    {
      button1->SetWidth (width);
      button1->SetHeight (width);
      button2->SetWidth (width);
      button2->SetHeight (width);
    };
  CControl::SetWidth (width);
  SetX (GetX ());
  SetY (GetY ());
  SetRange (Range);
};

void
CScroll::SetHeight (uint height)
{
  if (Type == st_horizontal)
    {
      button1->SetWidth (height);
      button1->SetHeight (height);
      button2->SetWidth (height);
      button2->SetHeight (height);
    };
  CControl::SetHeight (height);
  SetX (GetX ());
  SetY (GetY ());
  SetRange (Range);
};


//events

void
CScroll::pointer_out (void)
{
  Move = false;
  CControl::pointer_out ();
};

void
CScroll::key_press (XEvent event)
{
  KeySym key;
  char text[10];
  Status status;

  XXLookupString (Win->GetIC (), &event.xkey, text, 10, &key, &status);

  switch (key)
    {
    case XK_Left:
      SetPosition (GetPosition () - 1);
      break;
    case XK_Up:
      SetPosition (GetPosition () - 1);
      break;
    case XK_Right:
      SetPosition (GetPosition () + 1);
      break;
    case XK_Down:
      SetPosition (GetPosition () + 1);
      break;
    case XK_Page_Up:
      SetPosition (GetPosition () - ((Range / 10) + 1));
      break;
    case XK_Page_Down:
      SetPosition (GetPosition () + ((Range / 10) + 1));
      break;
    default:
      CControl::key_press (event);
    };
};

void
CScroll::key_release (XEvent event)
{
};

void
CScroll::button_press (XEvent event)
{
  if (event.xbutton.button == 1)
    {
      if (Type == st_vertical)
	{
	  int hb = button1->GetHeight ();
	  int h1;
	  if (Position == 0)
	    {
	      h1 = 0;
	    }
	  else
	    {
	      if (Position == Range - 1)
		h1 = ((Height - (2 * hb)) - Size);
	      else
		h1 = (((Height - (2 * hb) - Size) * Position) / Range);
	    };
	  int y = event.xbutton.y - hb - GetRY();
	  if ((y >= h1) && (y <= int (h1 + Size)))
	    Move = true;
	}
      else
	{
	  int wb = button1->GetWidth ();
	  int w1;
	  if (Position == 0)
	    {
	      w1 = 0;
	    }
	  else
	    {
	      if (Position == Range - 1)
		w1 = ((Width - (2 * wb)) - Size);
	      else
		w1 = (((Width - (2 * wb) - Size) * Position) / Range);
	    };
	  int w = event.xbutton.x - wb - GetRX ();
	  if ((w >= w1) && (w <= int (w1 + Size)))
	    Move = true;
	};
      CControl::button_press (event);
    };
};

void
CScroll::button_release (XEvent event)
{
  Move = false;
  CControl::button_release (event);
};


void
CScroll::mouse_move (XEvent event)
{
  XEvent Nevent = event;

  while (XEventsQueued (Win->GetADisplay (), QueuedAfterReading) > 0)
    {
      XPeekEvent (Win->GetADisplay (), &Nevent);
      if ((Nevent.type == MotionNotify) || (Nevent.type == NoExpose))
	{
	  if (Nevent.type == MotionNotify)
	    XNextEvent (Win->GetADisplay (), &event);
	  else
	    XNextEvent (Win->GetADisplay (), &Nevent);
	}
      else
	{
	  break;
	};
    };

  if (Move)
    {
      if (Type == st_vertical)
	{
	  int h1, hb = button1->GetHeight ();
	  int y = event.xbutton.y - hb - GetRY ();
	  int s;
	  if(Range != 1)
	    s = (Range / (Height - 2 * hb - Size));
	  else
	    s=0;	  
	  s++;
	  
	  for (int c = 0; c < Range; c++)
	    {

	      if (c == 0)
		{
		  h1 = 0;
		}
	      else
		{
		  if (c == Range - 1)
		    h1 = ((Height - (2 * hb)) - Size);
		  else
		    h1 = (((Height - (2 * hb) - Size) * c * s) / Range);
		};

	      if ((y > h1 + 5)
		  && (y < int (h1 + Size - 5)) &&(GetPosition () != c))
		{
		  SetPosition (c * s);
		  return;
		};
	    }
	}
      else
	{
	  int w1, wb = button1->GetWidth ();
	  int w = event.xbutton.x - wb - GetRX ();
	  int s;
	  if(Range != 1)
	    s= (Range / (Width - 2 * wb - Size));
	  else
	    s=0;	  
	  s++;

	  for (int c = 0; c < Range; c++)
	    {
	      if (c == 0)
		{
		  w1 = 0;
		}
	      else
		{
		  if (c == Range - 1)
		    w1 = ((Width - (2 * wb)) - Size);
		  else
		    w1 = (((Width - (2 * wb) - Size) * c * s) / Range);
		};
	      if ((w >= w1 + 5)
		  && (w <= int (w1 + Size - 5)) &&(GetPosition () != c))
		{
		  SetPosition (c * s);
		  return;
		};
	    };
	};
    };
  CControl::mouse_move (event);
};

void
CScroll::ScrollButtonPress (CControl * control, uint button, uint x, uint y,
			    uint status)
{
  Update ();
  if ((FOwner) && (EvMouseButtonPress))
    (FOwner->*EvMouseButtonPress) (this, button, x, y, status);
};

void
CScroll::ScrollButtonRelease (CControl * control, uint button, uint x, uint y,
			      uint status)
{
  CToolButton *bt;
  CScroll *sc;
  bt = dynamic_cast < CToolButton * >(control);
  sc = dynamic_cast < CScroll * >(bt->GetOwner ());
  int pos;
  if (bt->GetTag ())
    {
      pos = sc->GetPosition () + 1;
      if (pos != Range)
	sc->SetPosition (pos);
      else
	sc->SetPosition (pos - 1);
    }
  else
    {
      pos = sc->GetPosition () - 1;
      if (pos >= 0)
	sc->SetPosition (pos);
      else
	sc->SetPosition (pos + 1);
    };
  Update ();

};
