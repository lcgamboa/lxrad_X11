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

#include"../include/cfilelist.h"

// CFileList_____________________________________________________________

CFileList::CFileList (void)
{
  SetClass ("CFileList");
  char *temp = get_current_dir_name ();
  Dir = temp;
  free(temp);
  EvFileListSelectDir = NULL;
  EvFileListSelectFile = NULL;
  ItemsType = NULL;
};


CFileList::~CFileList (void)
{
  if (ItemsType)
    {
      delete[]ItemsType;
      ItemsType = NULL;
    };
};

int
CFileList::Create (CControl * control)
{
  int ret=CList::Create (control);
  char *temp = get_current_dir_name ();
  SetVisible (false, false);
  SetDir (temp);
  SetVisible (true, false);
  free(temp);
  return ret;
}

int
file_filter (const struct dirent *de)
{
  if ((de->d_type == DT_REG) && (de->d_name[0] != '.'))
    return 1;
  else
    return 0;
};

int
dir_filter (const struct dirent *de)
{
  if ((de->d_type == DT_DIR)
      && ((de->d_name[0] != '.') || (de->d_name[1] == '.')))
    return 1;
  else
    return 0;
};

void
CFileList::SetDir (lxString dir)
{
  struct dirent **dirs = NULL;
  struct dirent **files = NULL;
  int n;

  chdir(dir);
  
  Dir = dir;
  
  Scroll->SetPosition (0);
  ItemsList.Clear ();
  SelectedItem = -1;

  n = scandir (Dir.c_str (), &dirs, dir_filter, alphasort);
  if (n >= 0)
    {
      for (int cnt = 0; cnt < n; ++cnt)
	AddItem (dirs[cnt]->d_name, dirs[cnt]->d_type);
    }
  else
    eprint( "Couldn't open the directory " + Dir +"\n");
  for (int cnt = 0; cnt < n; ++cnt)
    {
      if (dirs[cnt])
	free( dirs[cnt]);
    };
    free(dirs);
  
  dirs = NULL;

  n = scandir (Dir.c_str (), &files, file_filter, alphasort);
  if (n >= 0)
    {
      for (int cnt = 0; cnt < n; ++cnt)
	AddItem (files[cnt]->d_name, files[cnt]->d_type);
    }
  else
    eprint("Couldn't open the directory " + Dir + "\n");
  for (int cnt = 0; cnt < n; ++cnt)
    {
      if (files[cnt])
	free( files[cnt]);
    };
   free(files);
  
  files = NULL;
  
  Draw ();
};

void
CFileList::Draw ()
{
  if((!Visible)||(Paint == NULL))
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
      for (int c = 1;(h * c + h) < (int) Height; c++)
	{
	  AddStringItem ("");
	  int y = h * c;
	  Items[c]->SetVisible (false, false);
	  Items[c]->SetY (y);
	  Items[c]->SetVisible (true, false);
	};
      Scroll->SetVisible (false, false);
      Scroll->SetX (Width - Scroll->GetWidth ());
      Scroll->SetHeight (Height);
      Scroll->SetVisible (true, false);
    };
  }; 
  
  Scroll->SetVisible (false, false);
  Scroll->SetRange (lcount - ItemsCount);
  Scroll->SetVisible (true, false);
  
  for (int c = 0; c <= ItemsCount; c++)
    {
      Items[c]->SetVisible (false, false);
      if(c < lcount)
      {
         Items[c]->SetText (ItemsList.GetLine (c + Scroll->GetPosition ()));
      if (ItemsType[c + Scroll->GetPosition ()] == DT_REG)
	{
	  Items[c]->EvMouseButtonRelease =
	    EVMOUSEBUTTONRELEASE & CFileList::FileButtonRelease;
	  Items[c]->EvMouseButtonClick = NULL;
	  Items[c]->SetColor ("black");
	};
      if (ItemsType[c + Scroll->GetPosition ()] == DT_DIR)
	{
	  Items[c]->EvMouseButtonRelease = NULL;
	  Items[c]->EvMouseButtonClick =
	    EVMOUSEBUTTONCLICK & CFileList::DirButtonClick;
	  Items[c]->SetColor ("blue");
	  Items[c]->SetText ("/" + Items[c]->GetText ());
	};
       }
      else
      {
        Items[c]->SetText(""); 
	Items[c]->EvMouseButtonRelease = NULL;
	Items[c]->EvMouseButtonClick = NULL;
	Items[c]->SetColor ("black");
      };
      Items[c]->SetVisible (true, false);
    };
  CControl::Draw ();

  if ((SelectedItem - Scroll->GetPosition () >= 0) &&
      (SelectedItem - Scroll->GetPosition () <= ItemsCount))
    {
      SetColor ("dark blue");
      lxColor color =
	Items[SelectedItem - Scroll->GetPosition ()]->GetColor ();
      Items[SelectedItem - Scroll->GetPosition ()]->SetColor ("white");
      Items[SelectedItem - Scroll->GetPosition ()]->Draw ();
      SetColor ("white");
      Items[SelectedItem - Scroll->GetPosition ()]->SetColor (color);
    };
  Update ();
};

lxStringList 
CFileList::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("OnChangeItem=" + btoa (GetEv ()) + ";event");
  Context.AddLine ("OnFileSelected=" + btoa (GetEv ()) + ";event");
  Context.AddLine ("OnDirSelected=" + btoa (GetEv ()) + ";event");
  return Context;
};

void
CFileList::SetContext (lxStringList context)
{
  Erase ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      lxString line = Context.GetLine (i);
      lxString arg;
      eqparse (line, arg);
      if (line.compare ("OnChangeItem") == 0)
	SetEv (atob (arg));
      if (line.compare ("OnFileSelected") == 0)
	SetEv (atob (arg));
      if (line.compare ("OnDirSelected") == 0)
	SetEv (atob (arg));
    };
  Draw ();
};


//propriedades

void
CFileList::AddStringItem (lxString text)
{
  CLabel *item;
  item = new CLabel;
  item->SetText (text);
  item->SetAlign (CA_LEFT);
  item->SetCanExecuteEvent (true);
  item->SetX (5);
  item->SetY (5);
  item->SetWidth (Width - Scroll->GetWidth () - 10);
  item->SetTag (ItemsCount + 1);
  item->SetVisible (false, false);
  item->SetFOwner (this);
  item->EvMouseButtonPress = EVMOUSEBUTTONPRESS & CFileList::ItemButtonPress;
  CreateChild (item);
  ItemsCount++;
  CLabel **AItems = new CLabel *[ItemsCount + 1];
  for (int c = 0; c < ItemsCount; c++)
    AItems[c] = Items[c];
  AItems[ItemsCount] = item;
  if (Items)
    delete[]Items;
  Items = AItems;
};

void
CFileList::AddItem (char *name, unsigned char dtype)
{
  int ICount = ItemsList.GetLinesCount ();
  unsigned char *AItemsType = new unsigned char[ICount + 1];
  for (int c = 0; c < ICount; c++)
    AItemsType[c] = ItemsType[c];
  AItemsType[ICount] = dtype;
  if (ItemsType)
    delete[]ItemsType;
  ItemsType = AItemsType;
  
  ItemsList.AddLine (name);
};

void
CFileList::DeleteItems (bool clean)
{
  CList::DeleteItems (clean);
  if ((ItemsType) && (clean))
    {
      delete[]ItemsType;
      ItemsType = NULL;
    };
};


lxString
CFileList::GetSelectedDir (void)
{
  return Dir;
};

lxString
CFileList::GetSelectedFile (void)
{
  return (Dir +"/"+ ItemsList.GetLine(SelectedItem));
};

void
CFileList::DirButtonClick (CControl * control, uint button, uint x,
			   uint y, uint state)
{
  CLabel *item;
  lxString dir = GetSelectedDir ();

  item = dynamic_cast < CLabel * >(control);

  SetVisible (false, false);
  if (item->GetText ().compare ("/..") != 0)
    {
      if (dir.size () != 1)
	SetDir (dir + item->GetText ());
      else
	SetDir (item->GetText ());
    }
  else
    {
      int pos = dir.rfind ("/");
      if (pos > 0)
	SetDir (dir.substr (0, pos));
      else
	SetDir ("/");
    };
  SetVisible (true, false);
  Draw ();
  dir_selected ();
  change_item();
};

void
CFileList::dir_selected (void)
{
  if ((FOwner) && (EvFileListSelectDir))
    (FOwner->*EvFileListSelectDir) (this);
};

void
CFileList::FileButtonRelease (CControl * control, uint button, uint x, uint y,
			      uint state)
{
  file_selected ();
  change_item();
};

void
CFileList::file_selected (void)
{
  if ((FOwner) && (EvFileListSelectFile))
    (FOwner->*EvFileListSelectFile) (this);
};
