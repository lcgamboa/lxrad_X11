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

#define string String

class String
{
private:
  char *Str;
public:
    String (void);
    String (const String & str);
    String (const char &str);
    String (const char *str);
    String (const char *str, int size);
   ~String (void);
  const char *c_str (void) const;
  const char *char_str (void) const;
  int Cmp (const char *str) const;
  int compare (const char *str) const;
  int compare (const String & str) const;
  String Format(const char *fmt, ...) __attribute__((format (printf, 2, 3)));
  String Printf(const char *fmt, ...) __attribute__((format (printf, 2, 3)));
  uint size (void) const;
  uint length (void) const;
  String substr (uint start, uint len) const;
  int find (const String & str) const;
  int Contains(const String & str) const;
  int rfind (const String & str) const;
  int erase (uint start, int num);
  static String FromAscii(char * str);
  int copy (char *str, uint sz) const;
  bool  operator == (const String & str);
  String & operator = (const String & str);
  String & operator = (const char *str);
  String operator + (const String & str);
  String operator + (const char *str);
  String operator + (const char &str);
  String & operator += (const String & str);
  String & operator += (const char *str);
  String & operator += (const char &str);
  char &operator[] (const uint & index);
  operator const char*() const;
  friend String operator + (const char *str1, const String & str2);
  friend String operator + (const char &str1, const String & str2);
//  friend ostream & operator << (ostream & os, const String & str);
};

class CStringList
{
private:
  String * Lines;
  int LinesCount;
public:
    CStringList (void);
    CStringList (const CStringList & list);
   ~CStringList (void);
  void Create ();
  void Clear ();
  void AddLine (const String line);
  void InsertLine (String line, uint linen);
  void DelLine (uint linen);
  bool LoadFromFile (String fname);
  bool SaveToFile (String fname);
    CStringList & operator = (const CStringList & list);
  //propiedades
  String GetLine (uint linen) const;
  void SetLine (String line, uint linen);
  uint GetLinesCount (void) const;
};


#else

using namespace std;
#include<string>
#include<iostream>
#include<fstream>
#include<list>

#define String string

class CStringList
{
private:
  list < string > List;
  list < string >::iterator itList;
public:
  CStringList (void);
    CStringList (const CStringList & list);
   ~CStringList (void);
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
String xml_out (String name, String type, String value);
void xml_in (String data, String & name, String & type, String & value);

String eqparse (String & str, String & arg);
String strndel (const String & str, uint n);
String strnadd (const String & str, char c, uint n);
int atoi (const String & str);
float atof (const String & str);
bool atob (const String & str);
String itoa (int n, const String & format = "%i");
String itoa (uint n, const String & format = "%u");
String ftoa (float f, const String & format = "%f");
String btoa (bool b);

String uppercase (const String & str);
String lowercase (const String & str);

String basename (const String & str);
String dirname (const String & str);

bool fgetline (FILE* file, String & str);

#endif
