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

#ifndef MSTRING
#define MSTRING

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdarg.h>

#define NO_STL

#ifdef NO_STL


class lxString
{
private:
  char *Str;
public:
    lxString (void);
    lxString (const lxString & str);
    lxString (const char &str);
    lxString (const char *str);
    lxString (const char *str, int size);
   ~lxString (void);
  const char *c_str (void) const;
  const char *char_str (void) const;
  int Cmp (const char *str) const;
  int compare (const char *str) const;
  int compare (const lxString & str) const;
  lxString Format(const char *fmt, ...) __attribute__((format (printf, 2, 3)));
  lxString Printf(const char *fmt, ...) __attribute__((format (printf, 2, 3)));
  uint size (void) const;
  uint length (void) const;
  lxString substr (uint start, uint len) const;
  int find (const lxString & str) const;
  int Contains(const lxString & str) const;
  int rfind (const lxString & str) const;
  int erase (uint start, int num);
  static lxString FromAscii(char * str);
  int copy (char *str, uint sz) const;
  bool  operator == (const lxString & str);
  lxString & operator = (const lxString & str);
  lxString & operator = (const char *str);
  lxString operator + (const lxString & str);
  lxString operator + (const char *str);
  lxString operator + (const char &str);
  lxString & operator += (const lxString & str);
  lxString & operator += (const char *str);
  lxString & operator += (const char &str);
  char &operator[] (const uint & index);
  operator const char*() const;
  friend lxString operator + (const char *str1, const lxString & str2);
  friend lxString operator + (const char &str1, const lxString & str2);
//  friend ostream & operator << (ostream & os, const lxString & str);
};

class lxStringList
{
private:
  lxString * Lines;
  int LinesCount;
public:
    lxStringList (void);
    lxStringList (const lxStringList & list);
   ~lxStringList (void);
  void Create ();
  void Clear ();
  void AddLine (const lxString line);
  void Append (const lxString line);
  void InsertLine (lxString line, uint linen);
  void DelLine (uint linen);
  bool LoadFromFile (lxString fname);
  bool SaveToFile (lxString fname);
  bool AppendToFile (lxString fname);
    lxStringList & operator = (const lxStringList & list);
  //propiedades
  lxString GetLine (uint linen) const;
  void SetLine (lxString line, uint linen);
  uint GetLinesCount (void) const;
};


#else

using namespace std;
#include<string>
#include<iostream>
#include<fstream>
#include<list>

#define lxString string

class lxStringList
{
private:
  list < string > List;
  list < string >::iterator itList;
public:
  lxStringList (void);
    lxStringList (const lxStringList & list);
   ~lxStringList (void);
  void Create ();
  void Clear ();
  void AddLine (string line);
  void InsertLine (string line, uint linen);
  void DelLine (uint linen);
  bool LoadFromFile (string fname);
  bool SaveToFile (string fname);
  //propiedades
  string GetLine (uint linen) const;
  void SetLine (string line, uint linen);
  unsigned int GetLinesCount (void) const;
};
#endif
lxString xml_out (lxString name, lxString type, lxString value);
void xml_in (lxString data, lxString & name, lxString & type, lxString & value);

lxString eqparse (lxString & str, lxString & arg);
lxString strndel (const lxString & str, uint n);
lxString strnadd (const lxString & str, char c, uint n);
int atoi (const lxString & str);
float atof (const lxString & str);
bool atob (const lxString & str);
lxString itoa (int n, const lxString & format = "%i");
lxString itoa (uint n, const lxString & format = "%u");
lxString ftoa (float f, const lxString & format = "%f");
lxString btoa (bool b);

lxString uppercase (const lxString & str);
lxString lowercase (const lxString & str);

lxString basename (const lxString & str);
lxString dirname (const lxString & str);

bool fgetline (FILE* file, lxString & str);

#endif
