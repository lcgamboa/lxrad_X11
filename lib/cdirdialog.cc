/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001-2020  Luis Claudio Gamboa Lopes

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


#include"../include/cdirdialog.h"
#include"../include/cinput.h"
#include"../include/cmessage.h"
#include"../include/cdialog.h"


// CDirDialog__________________________________________________________

CDirDialog::CDirDialog (void)
{
 CanDestroyChilds = false;
 CanFocus = false;
 CanExecuteEvent = false;
 SetClass ("CDirDialog");
 CanVisible = false;

 SetVisible (false, false);
 SetCanExecuteEvent (false);
 SetSaveUnder (true);
 EvOnClose = NULL;

 FileName = "untitled.txt";
 SetX (320);
 SetY (240);
 SetWidth (200);
 SetHeight (250);
 SetName ("Dialog");
 //button1
 button1.SetText ("OK");
 button1.SetX (33);
 button1.SetY (222);
 button1.SetFOwner (this);
 button1.EvMouseButtonRelease =
     EVMOUSEBUTTONRELEASE & CDirDialog::ButtonRelease1;
 CreateChild (&button1);
 //button2
 button2.SetText ("Cancel");
 button2.SetX (106);
 button2.SetY (222);
 button2.SetFOwner (this);
 button2.EvMouseButtonRelease =
     EVMOUSEBUTTONRELEASE & CDirDialog::ButtonRelease2;
 CreateChild (&button2);
 //button3
 button3.SetText ("mkdir");
 button3.SetX (33);
 button3.SetY (192);
 button3.SetFOwner (this);
 button3.EvMouseButtonRelease =
     EVMOUSEBUTTONRELEASE & CDirDialog::ButtonRelease3;
 CreateChild (&button3);
 //button4
 button4.SetText ("rm");
 button4.SetX (106);
 button4.SetY (192);
 button4.SetFOwner (this);
 button4.EvMouseButtonRelease =
     EVMOUSEBUTTONRELEASE & CDirDialog::ButtonRelease4;
 CreateChild (&button4);
 //edit1
 edit1.SetText ("");
 edit1.SetX (20);
 edit1.SetY (160);
 edit1.SetWidth (160);
 CreateChild (&edit1);
 //filelist1
 filelist1.SetX (20);
 filelist1.SetY (20);
 filelist1.SetFOwner (this);
 filelist1.EvFileListSelectFile = EVFILELISTSELECTFILE & CDirDialog::ListOnFileSelected;
 CreateChild (&filelist1);
};

CDirDialog::~CDirDialog (void) { };

int
CDirDialog::Create (CControl * control)
{
 if (control == this)
  CanDestroyChilds = true;

 if (control->GetClass ().compare (GetClass ()) == 0)
  return CWindow::Create (control);
 
 return 1;
}

void
CDirDialog::DestroyChilds (void)
{
 if (CanDestroyChilds)
  CControl::DestroyChilds ();
};

void
CDirDialog::ButtonRelease1 (CControl * control, uint button, uint x, uint y,
                             uint state)
{
 HideExclusive ();
 Result = true;
};

void
CDirDialog::ButtonRelease2 (CControl * control, uint button, uint x, uint y,
                             uint state)
{
 HideExclusive ();
 Result = false;
};

void
CDirDialog::ButtonRelease3 (CControl * control, uint button, uint x, uint y,
                             uint state)
{
 lxString texto = "untitled";
 if (Input ("Dir Name:", texto))
  if (mkdir (texto.c_str (), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
   {
    Message (strerror (errno));
   };
 filelist1.SetDir (filelist1.GetSelectedDir ());
};

void
CDirDialog::ButtonRelease4 (CControl * control, uint button, uint x, uint y,
                             uint state)
{
 if (Dialog ("Remove " + edit1.GetText () + " ?"))
  if (unlink (edit1.GetText ().c_str ()) != 0)
   {
    Message (strerror (errno));
   };
 filelist1.SetDir (filelist1.GetSelectedDir ());
};

void
CDirDialog::ListOnFileSelected (CFileList * flist)
{
 CDirDialog *dialog;
 dialog = dynamic_cast < CDirDialog * >(flist->GetOwner ());
 lxString selected = flist->GetSelectedItem ();
 int pos = selected.find ("/");
 if (pos < 0)
  dialog->edit1.SetText (selected);
 else
  dialog->edit1.SetText (selected.substr (1, selected.size ()));
};

lxString
CDirDialog::GetFileName (void)
{
 return GetDirName () + "/" + FileName;
};

lxString
CDirDialog::GetDirName (void)
{
 return filelist1.GetSelectedDir ();
};

void
CDirDialog::SetFileName (lxString filename)
{
 FileName = filename;
};

void
CDirDialog::SetDir (lxString dir)
{
 filelist1.SetDir (dir);
};

void
CDirDialog::Run (void)
{
 SetTitle (GetName ());
 edit1.SetText (GetFileName ());
 if (!WWindow)
  {
   Application->ACreateWindow (this);
  }
 ShowExclusive ();
 SetFileName (edit1.GetText ());
 WDestroy ();

 if ((FOwner) && (EvOnClose))
  (FOwner->*EvOnClose) (Result);
};

lxStringList
CDirDialog::GetContext (void)
{
 CControl::GetContext ();
 return Context;
};

void
CDirDialog::SetContext (lxStringList context)
{
 Erase ();
 CControl::SetContext (context);
 /*
 for (uint i = 0; i < context.GetLinesCount (); i++)
   {
     lxString line = Context.GetLine (i);
     lxString arg;
     eqparse (line, arg);
   };
  */
 Draw ();
};

void
CDirDialog::SetFilter (lxString filter)
{
 Filter = filter;
 //TODO
}

void
CDirDialog::SetType (int type)
{
 Type = type;
 //TODO
}

int
CDirDialog::GetType (void)
{
 return Type;
}
