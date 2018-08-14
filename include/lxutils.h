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


#ifndef LXUTILS_H
#define	LXUTILS_H

#include"clxrad.h"
#include"mstring.h"
#include<math.h>
#include<Imlib2.h>

class CWindow;

//Compatibillity 
#define CPWindow CWindow
#define CImage CDraw
#define lxT
#define _T
#define CCanvas CPaint

struct TXColor
{
  String name;
  XColor color;
  XColor displaycolor;
};

void mprint(String message);

void eprint(String error);

XColor ColorByRGB (unsigned short r, unsigned short g, unsigned short b);

XColor ColorByName (String name);

int XXLookupString (XIC ic, XKeyPressedEvent * event, char *buffer_return,
		    int bytes_buffer, KeySym * keysym_return,
		    Status * status_return);

class lxTextFile
{
  private:	
  FILE * f;
  public:
  lxTextFile();
  int Open(String fname);
  bool IsOpened(void);
  void Close(void);
  void GoToLine(int l);
  FILE * GetFd(void);
  operator FILE*() const;
};


class lxImage
{
private:
Imlib_Image Image;
public:
lxImage();
~lxImage();
bool LoadFile(String fname);
void Destroy();
operator Imlib_Image() const;
};

class lxBitmap
{
private:
Pixmap CPixmap;
public:
~lxBitmap();
lxBitmap(lxImage img, CWindow * win );
Pixmap  GetPixmap(void);
operator Pixmap() const;
};

class lxSound
{
private:
public:
  bool Create(String fname);	
  void Stop(void);
  void Play(int flags);
};

class lxFileName
{
private:
String FName;
public:
  void Assign(String fname);
  void MakeAbsolute(void);
  String GetFullPath(void);
};

#define lxPoint XPoint

class lxColor
{
private:	
  XColor Color;
public:
  lxColor();
  lxColor(XColor color);
  lxColor(const char * name);
  String GetAsString(int flags =0);
  operator XColor() const;
};

class lxCursor
{
public:
  lxCursor();
  lxCursor(int type);
};

class lxFont
{
private:
public:
  lxFont();
  lxFont(int size,int family,int style,int weight);
};

#define lxFONTFAMILY_TELETYPE 0x01
#define lxFONTSTYLE_NORMAL 0x01
#define lxFONTWEIGHT_BOLD 0x01


#define lxCURSOR_CROSS 0x01
#define lxCURSOR_ARROW 0x02
#define lxCURSOR_SIZENWSE 0x04

#define lxFD_OPEN 	0x1
#define lxFD_SAVE 	0x2
#define lxFD_CHANGE_DIR 0x4

#define lxSOUND_ASYNC  0x01
#define lxSOUND_LOOP   0x02

#define lxEXEC_MAKE_GROUP_LEADER 0x01

#define lxConvUTF8 2048

#define lxC2S_HTML_SYNTAX 0x01


void lxMilliSleep(unsigned int time);
void lxSetCursor(lxCursor cursor); 
bool lxFileExists(String fname);
void lxExecute(String cmd,unsigned int flags=0);
String lxGetCwd(void);

String lxGetUserDataDir(String appname);
String lxGetTempDir(String appname);
String lxGetExecutablePath(String appname);

bool lxZipDir(const String &in_dirname, const String &out_filename);
bool lxUnzipDir(const String &in_filename, const String &out_dirname);

bool lxRemoveFile(const char* fname);
bool lxRemoveDir(const char* dirname);
bool lxCreateDir(const char * dirname);


#endif	/* LXUTILS_H */
