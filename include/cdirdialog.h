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

#ifndef CDIRDIALOG
#define CDIRDIALOG

#include<sys/stat.h>
#include<errno.h>
#include"cwindow.h"
#include"cbutton.h"
#include"cedit.h"
#include"cfilelist.h"
#include"capplication.h"


class CDirDialog:public CWindow
{
private:
  bool CanDestroyChilds;
  bool Result;
  lxString Filter;
  int Type;
  lxString FileName;
  CButton button1, button2, button3, button4;
  CEdit edit1;
  CFileList filelist1;
public:
    CDirDialog (void);
   ~CDirDialog (void);
  int Create(CControl* control);
  void Run (void);
  //propriedades
  lxString GetFileName (void);
  void SetFileName (lxString filename);
  lxString GetDirName (void);
  void SetDir (lxString dir);
  void DestroyChilds (void);
  lxStringList GetContext (void);
  void SetContext (lxStringList context);
  void SetFilter(lxString filter);
  void SetType(int type);
  int GetType(void);
  //eventos
  void (CControl::*EvOnClose) (int retId);
  void ButtonRelease1 (CControl * control, uint button, uint x, uint y, uint state);
  void ButtonRelease2 (CControl * control, uint button, uint x, uint y, uint state);
  void ButtonRelease3 (CControl * control, uint button, uint x, uint y, uint state);
  void ButtonRelease4 (CControl * control, uint button, uint x, uint y, uint state);
  void ListOnFileSelected (CFileList * flist);
};

#endif
