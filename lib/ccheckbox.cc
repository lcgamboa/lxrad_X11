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


#include"../include/ccheckbox.h"

// CCheckBox___________________________________________________________

CCheckBox::CCheckBox(void)
{
 Text = new CLabel;
 Text->SetText ("checkbox");
 Text->SetFont (FontName);
 Text->SetX (10);
 Text->SetY (0);
 CBox = new CCbox;
 CBox->SetX (0);
 CBox->SetY (5);
 SetX (10);
 SetY (10);
 SetWidth (70);
 SetHeight (20);
 SetAlign (CA_LEFT);
 SetCheck (false);
 SetClass ("CCheckBox");
 CreateChild (Text);
 CreateChild (CBox);
};

CCheckBox::~CCheckBox(void) { };

int
CCheckBox::Create(CControl * control)
{
 return CControl::Create (control);
}

void
CCheckBox::Draw(void)
{
 if ((!Visible) || (Paint == NULL)) return;
 Paint->InitDraw (this);
 Paint->Pen.SetColor (Color);
 Paint->Rectangle (0, 0, Width, Height);
 CControl::Draw ();
};

lxStringList
CCheckBox::GetContext(void)
{
 CControl::GetContext ();
 Context.AddLine ("Text=" + GetText () + ";String");
 Context.AddLine ("Check=" + itoa (GetCheck ()) + ";bool");
 return Context;
};

void
CCheckBox::SetContext(lxStringList context)
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
   if (line.compare ("Check") == 0)
    SetCheck (atoi (arg));
  };
 Draw ();
};

//propiedades

void
CCheckBox::SetCheck(bool check)
{
 CBox->SetCheck (check);
 Draw ();
};

bool
CCheckBox::GetCheck(void)
{
 return CBox->GetCheck ();
};

void
CCheckBox::SetText(lxString t)
{
 Text->SetText (t);
 Draw ();
};

lxString
CCheckBox::GetText(void)
{
 return Text->GetText ();
};

void
CCheckBox::SetWidth(uint width)
{
 Text->SetWidth (width - 10);
 CControl::SetWidth (width);
};

void
CCheckBox::SetHeight(uint height)
{
 Text->SetHeight (height);
 CControl::SetHeight (height);
};

void
CCheckBox::SetAlign(CAlign align)
{
 Text->SetAlign (align);
};

CAlign
CCheckBox::GetAlign(void)
{
 return Text->GetAlign ();
};

//events

void
CCheckBox::button_press(XEvent event)
{
 Update ();
 CControl::button_press (event);

 if ((FOwner) && (EvOnCheckBox))
  (FOwner->*EvOnCheckBox) (this);
}

void
CCheckBox::key_press(XEvent event)
{
 KeySym key;
 char text[10];
 Status status;

 XXLookupString (NULL, &event.xkey, text, 10, &key, &status);
 if (key == XK_space)
  {
   if (CBox->GetCheck ())
    CBox->SetCheck (false);
   else
    {
     CBox->SetCheck (true);
     if ((FOwner) && (EvOnCheckBox))
      (FOwner->*EvOnCheckBox) (this);
    }
   CControl::key_press (event);
  }
 else
  CControl::key_press (event);

 Update ();
 CControl::button_press (event);
};
