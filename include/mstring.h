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
#include<string>

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
    lxString (const std::string str);
    lxString (const char *str, int size);
   ~lxString (void);
  const char *c_str (void) const;
  const char *char_str (void) const;
  const char *utf8_str (void) const;
  int Cmp (const char *str) const;
  int compare (const char *str) const;
  int compare (const lxString & str) const;
  lxString Format(const char *fmt, ...) __attribute__((format (printf, 2, 3)));
  lxString Printf(const char *fmt, ...) __attribute__((format (printf, 2, 3)));
  unsigned int size (void) const;
  unsigned int length (void) const;
  lxString substr (unsigned int start, unsigned int len) const;
  int find (const lxString & str) const;
  int Contains(const lxString & str) const;
  int rfind (const lxString & str) const;
  int erase (unsigned int start, int num);
  static lxString FromAscii(const char * str);
  static lxString FromUTF8(const char * str);
  static lxString FromUTF8(const std::string str);
  int copy (char *str, unsigned int sz) const;
  bool  operator == (const lxString & str);
  lxString & operator = (const lxString & str);
  lxString & operator = (const char *str);
  lxString operator + (const lxString & str);
  lxString operator + (const char *str);
  lxString operator + (const char &str);
  lxString & operator += (const lxString & str);
  lxString & operator += (const char *str);
  lxString & operator += (const char &str);
  char &operator[] (const unsigned int & index);
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
  void InsertLine (lxString line, unsigned int linen);
  void DelLine (unsigned int linen);
  bool LoadFromFile (lxString fname);
  bool SaveToFile (lxString fname);
  bool AppendToFile (lxString fname);
    lxStringList & operator = (const lxStringList & list);
  //propiedades
  lxString GetLine (unsigned int linen) const;
  void SetLine (lxString line, unsigned int linen);
  unsigned int GetLinesCount (void) const;
  lxString GetBuffer (void);
  void SetBuffer (lxString buff);
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
  void InsertLine (string line, unsigned int linen);
  void DelLine (unsigned int linen);
  bool LoadFromFile (string fname);
  bool SaveToFile (string fname);
  //propiedades
  string GetLine (unsigned int linen) const;
  void SetLine (string line, unsigned int linen);
  unsigned int GetLinesCount (void) const;
};
#endif
lxString xml_out (lxString name, lxString type, lxString value);
void xml_in (lxString data, lxString & name, lxString & type, lxString & value);

lxString eqparse (lxString & str, lxString & arg);
lxString strndel (const lxString & str, unsigned int n);
lxString strnadd (const lxString & str, char c, unsigned int n);
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
