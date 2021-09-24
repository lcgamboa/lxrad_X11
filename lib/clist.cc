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

#include"../include/clist.h"

// CList_____________________________________________________________

CList::CList (void)
{
  Scroll = new CScroll;
  Scroll->SetPosition (0);
  Scroll->SetRange (1);
  Scroll->SetFOwner (this);
  Scroll->EvMouseButtonPress = EVMOUSEBUTTONPRESS & CList::ScrollOnButtonPress;
  Scroll->EvOnChangePosition = EVONCHANGEPOSITION & CList::ScrollOnChangePosition;
  Items = NULL;
  ItemsCount = -1;
  ItemsList.Clear ();
  SetX (10);
  SetY (10);
  SetWidth (160);
  SetHeight (130);
  SetClass ("CList");
  SelectedItem = 0;
  CreateChild (Scroll);
  OnChangeItem = NULL;
  ShowSelection = true;
}


CList::~CList (void)
{
  if (Items)
    {
      delete[]Items;
      Items = NULL;
    }
  ItemsCount = -1;
}

int
CList::Create (CControl * control)
{
  if (!ColorSet)
    SetColor ("white");
  return CControl::Create (control);
}

void
CList::Draw ()
{
  if ((!Visible)||(Paint == NULL))
    return;
  int lcount = ItemsList.GetLinesCount ();
  int ssize = Scroll->GetWidth ();

  Paint->InitDraw (this);
  Paint->Pen.SetColor (Color);
  Paint->Rectangle ( 0, 0, Width - ssize, Height);
  Paint->LowerFrame ( 0, 0, Width - ssize, Height);


  if(GChanges)
  {
  DeleteItems (false);
  if ((ItemsCount == -1) && (Height > 20) && (lcount > 0))
    {
      GChanges=false;	  
      AddStringItem (ItemsList.GetLine (0));
      int h = Items[0]->GetTextHeight ();
      int y = 0;
      Items[0]->SetVisible (false, false);
      Items[0]->SetY (y);
      Items[0]->SetVisible (true, false);
      for (int c = 1; (h * c + h) < (int) Height; c++)
	{
	  AddStringItem ("");
	  int y = h * c;
	  Items[c]->SetVisible (false, false);
	  Items[c]->SetY (y);
	  Items[c]->SetVisible (true, false);
	}
      
      Scroll->SetVisible (false, false);
      Scroll->SetX (Width - Scroll->GetWidth ());
      Scroll->SetHeight (Height);
      Scroll->SetVisible (true, false);
    }
  }
      
  Scroll->SetVisible (false, false);
  Scroll->SetRange (lcount - ItemsCount);
  Scroll->SetVisible (true, false);
  
  for (int c = 0; c <= ItemsCount; c++)
    {
      Items[c]->SetVisible (false, false);
      if(c < lcount)
        Items[c]->SetText (ItemsList.GetLine (c + Scroll->GetPosition ()));
      else
        Items[c]->SetText ("");
      Items[c]->SetVisible (true, false);
    }

  CControl::Draw ();

  if (ShowSelection)
    {
      if ((SelectedItem - Scroll->GetPosition () >= 0) &&
	  (SelectedItem - Scroll->GetPosition () <= ItemsCount))
	{
	  SetColor ("dark blue");
	  XColor color =
	    Items[SelectedItem - Scroll->GetPosition ()]->GetColor ();
	  Items[SelectedItem - Scroll->GetPosition ()]->SetColor ("white");
	  Items[SelectedItem - Scroll->GetPosition ()]->Draw ();
	  SetColor ("white");
	  Items[SelectedItem - Scroll->GetPosition ()]->SetColor (color);
	}
      Update ();
    }
}

void
CList::Update ()
{
  CControl::Update ();
}

lxStringList
CList::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("Items=" + GetItems () + ";StringList");
  Context.AddLine ("OnChangeItem=" + btoa (GetEv ()) + ";event");
  return Context;
}

void
CList::SetContext (lxStringList context)
{
  Erase ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      lxString line = Context.GetLine (i);
      lxString arg;
      eqparse (line, arg);
      if (line.compare ("Items") == 0)
	  SetItems (arg);
      if (line.compare ("OnChangeItem") == 0)
	SetEv (atob (arg));
    }
  Draw ();
}


//propriedades

void
CList::SetItems (lxString litens)
{
  int f = 0;
  ItemsList.Clear ();
  while (litens.size () > 1)
    {
      f = litens.find (",");
      ItemsList.AddLine (litens.substr (0, f));
      litens = litens.substr (f + 1, litens.size ());
    }
  Draw ();
}

lxString
CList::GetItems (void)
{
  lxString list = "";
  for (uint c = 0; c < ItemsList.GetLinesCount (); c++)
    {
      list += ItemsList.GetLine (c) + ",";
    }
  return list;
}

void
CList::AddStringItem (lxString text)
{
  CLabel *item = new CLabel;
  item->SetText (text);
  item->SetAlign (CA_LEFT);
  item->SetCanExecuteEvent (true);
  item->SetX (5);
  item->SetY (5);
  item->SetTag (ItemsCount + 1);
  item->SetWidth (Width - Scroll->GetWidth () - 10);
  item->SetVisible (false, false);
  item->SetFOwner (this);
  item->EvMouseButtonPress = EVMOUSEBUTTONPRESS & CList::ItemButtonPress;
  CreateChild (item);
  ItemsCount++;
  CLabel **AItems = new CLabel *[ItemsCount + 1];
  for (int c = 0; c < ItemsCount; c++)
    AItems[c] = Items[c];
  AItems[ItemsCount] = item;
  if (Items)
    delete[]Items;
  Items = AItems;
}

void
CList::AddItem (lxString text)
{
  ItemsList.AddLine (text);
  Draw ();
}

lxString CList::GetItem (int item)
{
  if (item <= (int) ItemsList.GetLinesCount ())
    return ItemsList.GetLine (item);
  else
    return '\0';
}

void
CList::SetItem (int item,lxString sitem)
{
  if (item <= (int) ItemsList.GetLinesCount ())
    ItemsList.SetLine (sitem,item);
}

void
CList::SetSelectedItemN (int item)
{
  SelectedItem = item;
  if (ItemsCount != -1)
    {
      Draw ();
    }
  change_item ();
}

void
CList::SetSelectedItem (lxString item)
{

  for (uint c = 0; c < ItemsList.GetLinesCount (); c++)
    {
      if (ItemsList.GetLine (c).compare (item) == 0)
	{
	  SelectedItem = c;
	  if (ItemsCount != -1)
	    {
	      Draw ();
	    }
	  change_item ();

	  break;
	}
    }
}

int
CList::GetSelectedItemN (void)
{
  if (((uint) (SelectedItem) < ItemsList.GetLinesCount ()))
    {
      return SelectedItem;
    }
  else
    {
      return -1;
    }
}

lxString CList::GetSelectedItem (void)
{
  if (((uint) (SelectedItem) < ItemsList.GetLinesCount ()))
    {
      return ItemsList.GetLine (SelectedItem);
    }
  else
    {
      return "";
    }
}

int
CList::GetItemsCount (void)
{
  return ItemsList.GetLinesCount ();
}

void
CList::DeleteItem (int item)
{
  if (item >= 0)
    {
      ItemsList.DelLine (item);
      Scroll->SetVisible (false, false);
      Scroll->SetPosition (0);
      Scroll->SetVisible (true, false);
      Draw ();
    }
}

void
CList::DeleteItems (bool clean)
{
  GChanges=true;	
  if (Items != NULL)
    {
      for (int c = 0; c <= ItemsCount; c++)
	{
	  Items[c]->SetVisible (false, false);
	  DestroyChild (Items[c]);
	  Items[c] = NULL;
	}
      delete[]Items;
      Items = NULL;
      ItemsCount = -1;
    }
  if (clean)
    {
      Scroll->SetPosition (0);
      ItemsList.Clear ();
      SelectedItem = -1;
      Draw ();
    }
}

void
CList::SetX (int x)
{
  Scroll->SetX (Width - Scroll->GetWidth ());
  CControl::SetX (x);
}

void
CList::SetY (int y)
{
  Scroll->SetY (0);
  CControl::SetY (y);
}

void
CList::SetWidth (uint width)
{
  if (ItemsCount >= 0)
    {
      for (int c = 0; c < ItemsCount; c++)
	Items[c]->SetWidth (width - Scroll->GetWidth () - 10);
    }
  Scroll->SetX (width - Scroll->GetWidth ());
  CControl::SetWidth (width);
}

void
CList::SetHeight (uint height)
{
  Scroll->SetHeight (height);
  CControl::SetHeight (height);
}

void
CList::LoadItemsFromFile (lxString fname)
{
  DeleteItems ();
  ItemsList.LoadFromFile (fname);
  Draw();
}

void
CList::SaveItemsToFile (lxString fname)
{
  ItemsList.SaveToFile (fname);
}

void
CList::SetShowSelection (bool show)
{
  ShowSelection = show;
  Draw ();
}

bool CList::GetShowSelection (void)
{
  return ShowSelection;
}

//eventos

void
CList::ScrollOnButtonPress (CControl * control,const uint button,const uint x,const uint y,const uint state)
{
  Update ();
}

void
CList::ScrollOnChangePosition (CControl * scroll)
{
  Draw ();
}

void
CList::ItemButtonPress (CControl * control,const uint button,const uint x,const uint y,const uint state)
{
  if (ShowSelection)
    {
      XColor color;
      if ((SelectedItem - Scroll->GetPosition () >= 0) &&
	  (SelectedItem - Scroll->GetPosition () <= ItemsCount))
	{
	  SetColor ("white");
	  Items[SelectedItem - Scroll->GetPosition ()]->Draw ();
	}
      SetColor ("dark blue");
      color = control->GetColor ();
      control->SetColor ("white");
      control->Draw ();
      SetColor ("white");
      control->SetColor (color);
    }
  SelectedItem = control->GetTag () + Scroll->GetPosition ();
  Update ();
  change_item ();
}

void
CList::change_item (void)
{
  if ((FOwner) && (OnChangeItem))
    (FOwner->*OnChangeItem) (this);
}

void
CList::button_press (XEvent event)
{
  CControl::button_press (event);
}

void
CList::key_press (XEvent event)
{
  CControl::key_press (event);
}

void
CList::focus_in (void)
{
  CControl::focus_in ();
}

void
CList::focus_out (void)
{
  CControl::focus_out ();
}
