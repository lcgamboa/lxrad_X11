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

#ifndef CFILELIST
#define CFILELIST

#include"clist.h"
#include"cwindow.h"
#include<unistd.h>
#include<dirent.h>

#define EVFILELISTSELECTFILE   (void(CControl::*)(CFileList*))
#define EVFILELISTSELECTDIR   (void(CControl::*)(CFileList*))

class CFileList:public CList
{
protected:
  lxString Dir;
  void dir_selected (void);
  void file_selected (void);
  unsigned char *ItemsType;
public:
    CFileList (void);
   ~CFileList (void);
  void Draw (void);
  int Create (CControl * control);
  lxStringList GetContext (void);
  void SetContext (lxStringList context);
  //propiedades
  void AddItem (char *name, unsigned char dtype);
  void AddStringItem (lxString text);
  void DeleteItems (bool clean=true);
  void SetDir (lxString dir);
  lxString GetSelectedDir (void);
  lxString GetSelectedFile (void);
//events
  void DirButtonClick (CControl * control, uint button, uint x, uint y,
			     uint state);
  void FileButtonRelease (CControl * control, uint button, uint x, uint y,
			  uint state);
  void (CControl::*EvFileListSelectFile) (CFileList * flist);
  void (CControl::*EvFileListSelectDir) (CFileList * flist);
};

#endif
