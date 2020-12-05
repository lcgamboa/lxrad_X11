/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2000-2018  Luis Claudio Gamboa Lopes

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

#include"../include/mstring.h"

#include"../include/lxutils.h"

lxString
eqparse(lxString & str, lxString & arg)
{
 //FIXME substr	
 lxString temp = str;
 int pos1 = str.find ("=");
 str = temp.substr (0, pos1);
 arg = temp.substr ((pos1 + 1), temp.size ());
 temp = arg;
 pos1 = temp.find (";");
 arg = temp.substr (0, pos1);
 lxString stype = temp.substr ((pos1 + 1), temp.size ());
 return stype;
}

lxString
strndel(const lxString & str, uint n)
{
 lxString temp;
 temp = str;
 if ((n > 0) && (n <= str.size ()))
  temp.erase (n - 1, 1);
 return temp;
}

lxString
strnadd(const lxString & str, char c, uint n)
{
 //FIXME substr	
 lxString temp;
 temp = str.substr (0, n) + c + str.substr (n, str.size ());
 return temp;
}

int
atoi(const lxString & str)
{
 return atoi (str.c_str ());
}

float
atof(const lxString & str)
{
 return atof (str.c_str ());
}

bool
atob(const lxString & str)
{
 bool b;
 if (str.compare ("TRUE") == 0)
  b = true;
 else
  b = false;
 return b;
}

lxString
itoa(int n, const lxString & format)
{
 char temp[200];
 sprintf (temp, format.c_str (), n);
 return lxString (temp);
}

lxString
itoa(uint n, const lxString & format)
{
 char temp[200];
 sprintf (temp, format.c_str (), n);
 return lxString (temp);
}

lxString
ftoa(float f, const lxString & format)
{
 char temp[200];
 sprintf (temp, format.c_str (), f);
 return lxString (temp);
}

lxString
btoa(bool b)
{
 lxString str;
 if (b)
  str = "TRUE";
 else
  str = "FALSE";
 return str;
}

lxString
uppercase(const lxString & str)
{
 char *buf = new char[str.length () + 1];
 strcpy (buf, str.c_str ());
 for (uint i = 0; i < str.length (); i++)
  buf[i] = toupper (buf[i]);
 lxString r (buf);
 delete[] buf;
 return r;
}

lxString
lowercase(const lxString & str)
{
 char *buf = new char[str.length () + 1];
 strcpy (buf, str.c_str ());
 for (uint i = 0; i < str.length (); i++)
  buf[i] = tolower (buf[i]);
 lxString r (buf);
 delete[]buf;
 return r;
}

bool
fgetline(FILE* file, lxString & str)
{
 char line[5000];
 line[0] = 0;

 if (file)
  {
   fgets (line, 5000, file);

   if (line[0] != 0)
    {
     line[strlen (line) - 1] = '\0';
    }
   else
    line[0] = '\0';


   str = line;

   if (!feof (file))
    return true;
   else
    return false;
  }
 else
  {
   eprint ("fatal error in function fgetline!\n");
   exit (-1);
  }
}

#ifdef NO_STL
//lxString _____________________________________________________________

lxString::lxString(void)
{
 Str = NULL;
}

lxString::lxString(const lxString & str)
{

 if (str.Str != NULL)
  {
   Str = new char[strlen (str.Str) + 1];

   strcpy (Str, str.Str);
  }
 else
  Str = NULL;
}

lxString::lxString(const char &str)
{
 if (str != '\0')
  {
   Str = new char[2];
   Str[0] = str;
   Str[1] = '\0';
  }
 else
  Str = NULL;
}

lxString::lxString(const char *str)
{
 if (str != NULL)
  {
   Str = new char[strlen (str) + 1];
   strcpy (Str, str);
  }
 else
  Str = NULL;
}

lxString::lxString(const char *str, int size)
{
 if (str)
  {
   Str = new char[size + 1];
   strncpy (Str, str, size);
   Str[size] = '\0';
  }
 else
  Str = NULL;
}

lxString::~lxString(void)
{
 if (Str != NULL)
  {
   delete[]Str;
   Str = NULL;
  }
}

const char *
lxString::c_str(void) const
{
 if (Str)
  return Str;
 else
  return NULL;
}

const char *
lxString::char_str(void) const
{
 if (Str)
  return Str;
 else
  return NULL;
}

int
lxString::Cmp(const char *str) const
{
 if ((Str) && (str))
  return strcmp (Str, str);
 else
  return -1;
}

int
lxString::compare(const char *str) const
{
 if ((Str) && (str))
  return strcmp (Str, str);
 else
  return -1;
}

int
lxString::compare(const lxString & str) const
{
 if ((Str) && (str.Str))
  return strcmp (Str, str.Str);
 else
  return -1;
}

uint
lxString::size(void) const
{
 if (Str != NULL)
  return strlen (Str);
 else
  return 0;
}

uint
lxString::length(void) const
{
 if (Str != NULL)
  return strlen (Str);
 else
  return 0;
}

int
lxString::erase(uint start, int num = 1)
{
 if (start + num <= length ())
  {
   char *tmp;
   tmp = new char[length () - num + 1];
   strncpy (tmp, Str, start);
   tmp[start] = '\0';
   strcat (tmp, (Str + start + num));
   strcpy (Str, tmp);
   delete[]tmp;
   return 1;
  }
 else
  return 0;
}

lxString
lxString::substr(uint start, uint len) const
{
 if (start < length ())
  {
   lxString temp;
   char *tmp;
   if (start + len > length ())
    len = length () - start;
   tmp = new char[len + 1];
   strncpy (tmp, Str + start, len);
   tmp[len] = '\0';
   temp = tmp;
   delete[]tmp;
   return temp;
  }
 else
  return '\0';
}

int
lxString::copy(char *str, uint sz) const
{
 if (Str)
  {
   if (sz < size ())
    {
     strncpy (str, Str, sz);
     str[sz] = '\0';
     return sz;
    }
   else
    {
     strncpy (str, Str, size ());
     str[size ()] = '\0';
     return size ();
    }
  }
 return 0;
}

int
lxString::find(const lxString & str) const
{
 if ((Str != NULL) && (str.Str != NULL))
  {
   char *temp = strstr (Str, str.Str);
   if (temp)
    return strlen (Str) - strlen (temp);
   else
    return -1; //strlen (Str) + 1;
  }
 else
  return -1;
}

int
lxString::Contains(const lxString & str) const
{
 return (find (str) >= 0);
}

char *
strrstr(const char *str1, const char *str2)
{
 char *temp = NULL;
 char *temp1 = NULL;
 char *temp2 = NULL;
 uint c = 0;


 if ((str1 != NULL)&&(str2 != NULL))
  {
   while (strlen (str1) != c)
    {
     temp1 = new char[strlen (str1) + 1 - c];
     strncpy (temp1, str1, strlen (str1) - c);
     temp1[strlen (str1) - c] = '\0';
     temp = strrchr (temp1, str2[0]);
     if (temp)
      {
       temp2 = new char[strlen (str2) + 1];
       strncpy (temp2, temp, strlen (str2));
       temp2[strlen (str2)] = '\0';
       if (strcmp (temp2, str2) == 0)
        {
         c = strlen (temp) - 1;
         delete[]temp1;
         delete[]temp2;
         char *str = new char[strlen (str1) + 1 - c];
         strncpy (str, str1, strlen (str1) - c);
         str[strlen (str1) - c] = '\0';
         return str;
        }
       else
        {
         c += strlen (temp);
         delete[]temp1;
         delete[]temp2;
        }
      }
     else
      c = strlen (str1);
    }
  }
 if (temp1)
  delete[]temp1;
 if (temp2)
  delete[]temp2;
 return NULL;
}

int
lxString::rfind(const lxString & str) const
{
 if ((Str != NULL) && (str.Str != NULL))
  {
   char *temp = strrstr (Str, str.Str);
   if (temp)
    {
     int c = strlen (temp) - 1;
     delete[]temp;
     return c;
    }
   else
    return -1; //length () + 1;
  }
 else
  return -1; //length () + 1;
}

lxString & lxString::operator=(const lxString & str)
{
 if (this != &str)
  {
   if (Str != NULL)
    delete[]Str;
   if (str.Str != NULL)
    {
     Str = new char[strlen (str.Str) + 1];
     strcpy (Str, str.Str);
    }
   else
    Str = NULL;
  }
 return *this;
}

lxString & lxString::operator=(const char *str)
{
 if (Str != NULL)
  delete[]Str;

 if (str != NULL)
  {
   Str = new char[strlen (str) + 1];
   strcpy (Str, str);
  }
 else
  Str = NULL;
 return *this;
}

lxString
lxString::Format(const char *fmt, ...)
{
 va_list arg;
 char tmp[256];
 if (Str != NULL)
  delete[]Str;

 if (fmt != NULL)
  {
   va_start (arg, fmt);
   vsprintf (tmp, fmt, arg);
   va_end (arg);
   Str = new char[strlen (tmp) + 1];
   strcpy (Str, tmp);
  }
 else
  Str = NULL;
 return *this;
}

lxString
lxString::Printf(const char *fmt, ...)
{
 va_list arg;
 char tmp[256];
 if (Str != NULL)
  delete[]Str;

 if (fmt != NULL)
  {
   va_start (arg, fmt);
   vsprintf (tmp, fmt, arg);
   va_end (arg);
   Str = new char[strlen (tmp) + 1];
   strcpy (Str, tmp);
  }
 else
  Str = NULL;
 return *this;
}

lxString
lxString::FromAscii(char * str)
{
 lxString temp (str);
 return temp;
}

lxString lxString::operator+(const lxString & str)
{
 lxString
 temp;

 if (str.Str != NULL)
  {
   temp.Str = new char[size () + strlen (str.Str) + 1];
   if (Str)
    {
     strcpy (temp.Str, Str);
     strcat (temp.Str, str.Str);
    }
   else
    strcpy (temp.Str, str.Str);
  }
 else
  {
   if (Str)
    {
     if (&str != this)
      {
       temp.Str = new char[size () + 1];
       strcpy (temp.Str, Str);
      }
     else
      {
       temp.Str = new char[(2 * size ()) + 1];
       strcpy (temp.Str, Str);
       strcat (temp.Str, Str);
      }
    }
   else
    temp.Str = NULL;
  }
 return temp;
}

lxString lxString::operator+(const char *str)
{
 lxString
 temp;

 if (str != NULL)
  {
   temp.Str = new char[size () + strlen (str) + 1];
   if (Str)
    {
     strcpy (temp.Str, Str);
     strcat (temp.Str, str);
    }
   else
    strcpy (temp.Str, str);
  }
 else
  {
   if (Str)
    {
     temp.Str = new char[size () + 1];
     strcpy (temp.Str, Str);
    }
   else
    temp.Str = NULL;
  }
 return temp;
}

lxString lxString::operator+(const char &str)
{
 char
 str2[2];
 str2[0] = str;
 str2[1] = '\0';
 lxString
 temp;

 if (Str != NULL)
  {
   temp.Str = new char[size () + 2];
   strcpy (temp.Str, Str);
   strcat (temp.Str, str2);
  }
 else
  {
   temp.Str = new char[2];
   strcpy (temp.Str, str2);
  }

 return temp;
}

lxString & lxString::operator+=(const lxString & str)
{
 lxString temp = Str;

 if (Str != NULL)
  {
   delete[] Str;
   Str = NULL;
  }

 if (str.Str != NULL)
  {
   Str = new char[temp.size () + strlen (str.Str) + 1];
   if (temp.Str)
    {
     strcpy (Str, temp.Str);
     strcat (Str, str.Str);
    }
   else
    strcpy (Str, str.Str);
  }
 else
  {
   if (temp.Str)
    {
     if (&str != this)
      {
       Str = new char[temp.size () + 1];
       strcpy (Str, temp.c_str ());
      }
     else
      {
       Str = new char[(2 * temp.size ()) + 1];
       strcpy (Str, temp.c_str ());
       strcat (Str, temp.c_str ());
      }
    }
   else
    Str = NULL;
  }
 return *this;
}

lxString & lxString::operator+=(const char *str)
{
 lxString
 temp =
     Str;
 if (Str != NULL)
  {
   delete[] Str;
  }

 if (str != NULL)
  {
   Str = new char[temp.size () + strlen (str) + 1];
   if (temp.Str)
    {
     strcpy (Str, temp.c_str ());
     strcat (Str, str);
    }
   else
    strcpy (Str, str);
  }
 else
  {
   if (temp.Str)
    {
     Str = new char[temp.size () + 1];
     strcpy (Str, temp.c_str ());
    }
   else
    Str = NULL;
  }
 return *this;
}

lxString & lxString::operator+=(const char &str)
{
 char
 str2[2];
 str2[0] = str;
 str2[1] = '\0';
 lxString
 temp =
     Str;

 if (Str != NULL)
  {
   delete[] Str;
   Str = new char[temp.size () + 2];
   strcpy (Str, temp.c_str ());
   strcat (Str, str2);
  }
 else
  {
   Str = new char[2];
   strcpy (Str, str2);
  }
 return *this;
}

char &
lxString::operator[](const uint & index)
{
 //  if((Str)&&(index <= length()))
 return Str[index];
}

lxString
operator+(const char *str1, const lxString & str2)
{
 if (str1)
  {
   if (str2.Str)
    return lxString (str1) + str2;
   else
    return str1;
  }
 else
  {
   if (str2.Str)
    return str2;
  }
 return "";
}

lxString
operator+(const char &str1, const lxString & str2)
{
 char temp[2];
 temp[0] = str1;
 temp[1] = '\0';

 if (str2.Str)
  return lxString (temp) + str2;
 else
  return temp;

}

lxString::operator const char*() const
{
 return Str;
}

bool lxString::operator==(const lxString & str)
{
 if (strcmp (Str, str))
  return false;
 else
  return true;
}

/*
ostream & operator << (ostream & os, const lxString & str)
{
  os << str.Str;
  return os;
}
 */

//ClxStringList _____________________________________________________________

lxStringList::lxStringList(void)
{
 Lines = NULL;
 LinesCount = -1;
}

lxStringList::lxStringList(const lxStringList & list)
{
 Lines = NULL;
 LinesCount = -1;
 for (uint c = 0; c < list.GetLinesCount (); c++)
  {
   AddLine (list.GetLine (c));
  }
}

lxStringList::~lxStringList(void)
{
 if (Lines)
  {
   delete[]Lines;
   Lines = NULL;
  }
 LinesCount = -1;
}

void
lxStringList::Create() { }

lxString
lxStringList::GetLine(uint linen) const
{
 if ((int) linen <= LinesCount)
  return Lines[linen];
 else
  return "";
}

void
lxStringList::SetLine(lxString line, uint linen)
{
 Lines[linen] = line;
}

void
lxStringList::Clear(void)
{
 if (Lines)
  {
   delete[]Lines;
  }
 Lines = NULL;
 LinesCount = -1;
}

void
lxStringList::AddLine(const lxString line)
{
 if (line.c_str () != NULL)
  {
   LinesCount++;
   lxString *ALines = new lxString[LinesCount + 1];
   for (int c = 0; c < LinesCount; c++)
    ALines[c] = Lines[c];
   ALines[LinesCount] = line;
   if (Lines)
    delete[]Lines;
   Lines = ALines;
  }
}

void
lxStringList::Append(const lxString line)
{
 char buff[1024];
 char * line_;
 char * ptr = NULL;

 strncpy (buff, line.c_str (), 1023);
 line_ = buff;

 if (line_ != NULL)
  {
   if ((ptr = strchr (line_, '\n')))
    {

     do
      {
       int pos = ptr - line_;
       line_[pos] = 0;
       if ((pos > 0) &&(line_[pos - 1] == '\r'))
        {
         line_[pos - 1] = 0;
        }
       if (LinesCount >= 0)
        {
         Lines[LinesCount] += line_;
         AddLine ("");
        }
       else
        {
         AddLine (line_);
         AddLine ("");
        }
       line_ += pos + 1;
      }
     while ((ptr = strchr (line_, '\n')));

     if (strlen (line_))
      {
       Lines[LinesCount] += line_;
      }
    }
   else
    {
     if (LinesCount >= 0)
      {
       Lines[LinesCount] += line_;
      }
     else
      {
       AddLine (line_);
      }
    }
  }
}

void
lxStringList::InsertLine(lxString line, uint linen)
{
 LinesCount++;
 lxString *ALines = new lxString[LinesCount + 1];
 for (uint c = 0; c < linen; c++)
  ALines[c] = Lines[c];
 ALines[linen] = line;
 for (int c = linen + 1; c <= LinesCount; c++)
  ALines[c] = Lines[c - 1];
 if (Lines)
  delete[]Lines;
 Lines = ALines;
}

uint
lxStringList::GetLinesCount(void) const
{
 return LinesCount + 1;
}

void
lxStringList::DelLine(uint linen)
{
 LinesCount--;
 lxString *ALines = new lxString[LinesCount + 1];
 for (uint c = 0; c < linen; c++)
  ALines[c] = Lines[c];
 for (int c = linen; c < LinesCount + 1; c++)
  ALines[c] = Lines[c + 1];
 if (Lines)
  delete[]Lines;
 Lines = ALines;
}

bool
lxStringList::LoadFromFile(lxString fname)
{
 FILE* file;
 file = fopen (fname.c_str (), "r");
 if (file)
  {
   lxString line;
   while (fgetline (file, line))
    AddLine (line);
   fclose (file);
   return true;
  }
 else
  eprint ("File not found!\n");
 return false;
}

bool
lxStringList::SaveToFile(lxString fname)
{

 FILE* file;
 file = fopen (fname.c_str (), "w");
 if (file)
  {
   for (int c = 0; c <= LinesCount; c++)
    fprintf (file, "%s\n", GetLine (c).c_str ());
   fclose (file);
  }
 else
  eprint ("File not create!\n");
 return false;

}

bool
lxStringList::AppendToFile(lxString fname)
{

 FILE* file;
 file = fopen (fname.c_str (), "a");
 if (file)
  {
   for (int c = 0; c <= LinesCount; c++)
    fprintf (file, "%s\n", GetLine (c).c_str ());
   fclose (file);
  }
 else
  eprint ("File not appended create!\n");
 return false;

}

lxStringList & lxStringList::operator=(const lxStringList & list)
{
 if (this != &list)
  {
   if (Lines != NULL)
    delete[]Lines;

   if (list.Lines != NULL)
    {
     Lines = NULL;
     LinesCount = -1;
     for (uint c = 0; c < list.GetLinesCount (); c++)
      {
       AddLine (list.GetLine (c));
      }
    }
   else
    {
     Lines = NULL;
     LinesCount = -1;
    }
  }
 return *this;
}

#else
//ClxStringList _____________________________________________________________

lxStringList::lxStringList(void) { }

lxStringList::lxStringList(const lxStringList & list)
{
 for (uint c = 0; c <= list.GetLinesCount (); c++)
  {
   AddLine (list.GetLine (c));
  }
}

lxStringList::~lxStringList(void)
{
 List.erase (List.begin (), List.end ());
}

void
lxStringList::Create() { }

lxString
lxStringList::GetLine(uint linen) const
{
 list < string > List2 (List);
 list < string >::iterator itList2;
 itList2 = List2.begin ();
 for (uint a = 0; a < linen; a++)
  itList2++;
 return *itList2;
}

void
lxStringList::SetLine(string line, uint linen)
{
 itList = List.begin ();
 for (uint a = 0; a < linen; a++)
  itList++;
 *itList = line;
}

void
lxStringList::Clear(void)
{
 List.erase (List.begin (), List.end ());
}

void
lxStringList::AddLine(string line)
{
 List.push_back (line);
}

void
lxStringList::InsertLine(string line, uint linen)
{
 itList = List.begin ();
 for (uint a = 0; a < linen; a++)
  itList++;
 List.insert (itList, line);
}

unsigned int
lxStringList::GetLinesCount(void) const
{
 return List.size ();
}

void
lxStringList::DelLine(uint linen)
{
 itList = List.begin ();
 for (uint a = 0; a < linen; a++)
  itList++;
 List.erase (itList);
}

bool
lxStringList::LoadFromFile(string fname)
{
 ifstream
 file (fname.c_str ());
 if (file)
  {
   string
   line;
   while (getline (file, line))
    List.push_back (line);
  }
 else
  eprint ("File not found!\n");
}

bool
lxStringList::SaveToFile(string fname)
{
 ofstream
 file (fname.c_str ());
 if (file)
  {
   for (itList = List.begin (); itList != List.end (); itList++)
    file << *itList << endl;
  }
 else
  eprint ("File not create!\n");
}
#endif

lxString
xml_out(lxString name, lxString type, lxString value)
{
 return (lxT ("  <") + name + lxT (" type=\"") + type + lxT ("\">") + value + lxT ("</") + name + lxT (">"));
}

void
xml_in(lxString data, lxString & name, lxString & type, lxString & value)
{
 int p0, p1, p2, p3;
 if (data.size () > 0)
  {

   p0 = data.find (lxT ("<"));
   p1 = data.find (lxT ("type="));
   p2 = data.find (lxT ("\">"));
   p3 = data.find (lxT ("</"));
   name = data.substr (p0 + 1, p1 - p0 - 2);
   type = data.substr (p1 + 6, p2 - p1 - 6);
   value = data.substr (p2 + 2, p3 - p2 - 2);
  }
 else
  {
   name = lxT ("");
   type = lxT ("");
   value = lxT ("");
  }
}

lxString
basename(const lxString & str)
{
#ifndef __WXMSW__
 int pos = str.rfind (lxT ("/"));
#else
 int pos = str.rfind (lxT ("\\"));
 if (pos == -1)
  pos = str.rfind (lxT ("/"));
#endif
 return str.substr (pos + 1, str.size () - pos - 1);
}

lxString
dirname(const lxString & str)
{
#ifndef __WXMSW__
 int pos = str.rfind (lxT ("/"));
#else
 int pos = str.rfind (lxT ("\\"));
 if (pos == -1)
  pos = str.rfind (lxT ("/"));
#endif
 return str.substr (0, pos + 1);
}

