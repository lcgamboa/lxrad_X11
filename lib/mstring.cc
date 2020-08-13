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

String
eqparse (String & str, String & arg)
{
  //FIXME substr	
  String temp = str;
  int pos1 = str.find ("=");
  str = temp.substr (0, pos1);
  arg = temp.substr ((pos1 + 1), temp.size ());
  temp = arg;
  pos1 = temp.find (";");
  arg = temp.substr (0, pos1);
  String stype = temp.substr ((pos1 + 1), temp.size ());
  return stype;
};

String
strndel (const String & str, uint n)
{
  String temp;
  temp = str;
  if ((n > 0) && (n <= str.size ()))
    temp.erase (n - 1, 1);
  return temp;
}

String
strnadd (const String & str, char c, uint n)
{
  //FIXME substr	
  String temp;
  temp = str.substr (0, n) + c + str.substr (n, str.size ());
  return temp;
};

int
atoi (const String & str)
{
  return atoi (str.c_str ());
};

float
atof (const String & str)
{
  return atof (str.c_str ());
};

bool
atob (const String & str)
{
  bool b;
  if (str.compare ("TRUE") == 0)
    b = true;
  else
    b = false;
  return b;
};

String
itoa (int n, const String & format)
{
  char temp[200];
  sprintf (temp, format.c_str (), n);
  return String (temp);
};

String
itoa (uint n, const String & format)
{
  char temp[200];
  sprintf (temp, format.c_str (), n);
  return String (temp);
};

String
ftoa (float f, const String & format)
{
  char temp[200];
  sprintf (temp, format.c_str (), f);
  return String (temp);
};

String
btoa (bool b)
{
  String str;
  if (b)
    str = "TRUE";
  else
    str = "FALSE";
  return str;
};

String
uppercase (const String & str)
{
  char *buf = new char[str.length ()+1];
  strcpy(buf,str.c_str());
  for (uint i = 0; i < str.length (); i++)
    buf[i] = toupper (buf[i]);
  String r (buf);
  delete[] buf;
  return r;
}

String
lowercase (const String & str)
{
  char *buf = new char[str.length ()+1];
  strcpy (buf, str.c_str());
  for (uint i = 0; i < str.length (); i++)
    buf[i] = tolower (buf[i]);
  String r (buf);
  delete[]buf;
  return r;
}

	
bool
fgetline (FILE* file, String & str)
{	
  char line[5000];
  line[0]=0; 

  if(file)
  {
  fgets (line,5000,file);
 
  if(line[0] != 0)
  {
    line[strlen(line)-1]='\0';
  }
  else
    line[0]='\0';

  
  str = line;
  
  if(!feof(file))
    return true;
  else
    return false;
  }
  else
  {
     eprint("fatal error in function fgetline!\n");
     exit(-1);
  };
};

#ifdef NO_STL
//String _____________________________________________________________


String::String (void)
{
  Str = NULL;
}

String::String (const String & str)
{
  
   if (str.Str != NULL)
    {
      Str = new char[strlen (str.Str) + 1];
      
      strcpy (Str, str.Str);
    }
  else
    Str = NULL;
}

String::String (const char &str)
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

String::String (const char *str)
{
  if (str != NULL)
    {
      Str = new char[strlen (str) + 1];
      strcpy (Str, str);
    }
  else
    Str = NULL;
}


String::String (const char *str, int size)
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


String::~String (void)
{
  if (Str != NULL)
    {
      delete[]Str;
      Str = NULL;
    }
}

const char *
String::c_str (void) const
{
  if (Str)
    return Str;
  else
    return NULL;
}

const char *
String::char_str (void) const
{
  if (Str)
    return Str;
  else
    return NULL;
}

int
String::Cmp (const char *str) const
{
  if ((Str) && (str))
    return strcmp (Str, str);
  else
    return -1;
};

int
String::compare (const char *str) const
{
  if ((Str) && (str))
    return strcmp (Str, str);
  else
    return -1;
};

int
String::compare (const String & str) const
{
  if ((Str) && (str.Str))
    return strcmp (Str, str.Str);
  else
    return -1;
};

uint
String::size (void) const
{
  if (Str != NULL)
    return strlen (Str);
  else
    return 0;
};

uint
String::length (void) const
{
  if (Str != NULL)
    return strlen (Str);
  else
    return 0;
};

int
String::erase (uint start, int num = 1)
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
};

String
String::substr (uint start, uint len) const
{
  if (start < length ())
    {
      String temp;
      char *tmp;
      if (start+len > length ())
	len = length ()-start;
      tmp = new char[len + 1];
      strncpy (tmp, Str + start, len);
      tmp[len] = '\0';
      temp = tmp;
      delete[]tmp;
      return temp;
    }
  else
    return '\0';
};

int
String::copy (char *str, uint sz) const
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
};

int
String::find (const String & str) const
{
  if ((Str != NULL) && (str.Str != NULL))
    {
      char *temp = strstr (Str, str.Str);
      if (temp)
	return strlen (Str) - strlen (temp);
      else
	return -1;		//strlen (Str) + 1;
    }
  else
    return 0;
}

int
String::Contains (const String & str) const
{
  return find(str);	
}

char *
strrstr (const char *str1, const char *str2)
{
  char *temp = NULL;
  char *temp1 = NULL;
  char *temp2 = NULL;
  uint c = 0;
  
  
  if((str1 != NULL )&&(str2 != NULL))
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
	    };
	}
      else
	c = strlen (str1);
    };
   }
  if (temp1)
    delete[]temp1;
  if (temp2)
    delete[]temp2;
  return NULL;
};


int
String::rfind (const String & str) const
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
	return -1;		//length () + 1;
    }
  else
    return -1;			//length () + 1;
};


String & String::operator = (const String & str)
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
    };
  return *this;
};

String & String::operator = (const char *str)
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
};
  
String 
String::Format(const char *fmt, ...)
{
  va_list arg;
  char tmp[256];	
  if (Str != NULL)
    delete[]Str;
  
  if (fmt != NULL)
    {
      va_start(arg, fmt);
      vsprintf(tmp, fmt, arg);
      va_end(arg);
      Str = new char[strlen (tmp) + 1];
      strcpy (Str, tmp);
    }
  else
    Str = NULL;
  return *this;
};
  

String 
String::Printf(const char *fmt, ...) 
{
  va_list arg;
  char tmp[256];	
  if (Str != NULL)
    delete[]Str;
  
  if (fmt != NULL)
    {
      va_start(arg, fmt);
      vsprintf(tmp, fmt, arg);
      va_end(arg);
      Str = new char[strlen (tmp) + 1];
      strcpy (Str, tmp);
    }
  else
    Str = NULL;
  return *this;
}

String
String::FromAscii(char * str)
{
  String temp(str); 	
  return temp;
}

String String::operator + (const String & str)
{
  String
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
    };
  return temp;
};


String String::operator + (const char *str)
{
  String
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
    };
  return temp;
};

String String::operator + (const char &str)
{
  char
    str2[2];
  str2[0] = str;
  str2[1] = '\0';
  String
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
    };

  return temp;
};

String & String::operator += (const String & str)
{
  String temp = Str;

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
    };
  return *this;
};

String & String::operator += (const char *str)
{
  String
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
    };
  return *this;
};

String & String::operator += (const char &str)
{
  char
    str2[2];
  str2[0] = str;
  str2[1] = '\0';
  String
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
    };
  return *this;
};

char &
String::operator[] (const uint & index)
{
//  if((Str)&&(index <= length()))
  return Str[index];
};

String
operator + (const char *str1, const String & str2)
{
  if (str1)
    {
      if (str2.Str)
	return string (str1) + str2;
      else
	return str1;
    }
  else
    {
      if (str2.Str)
	return str2;
    };
  return "";
};

String
operator + (const char &str1, const String & str2)
{
  char temp[2];
  temp[0] = str1;
  temp[1] = '\0';

  if (str2.Str)
     return string (temp) + str2;
  else
     return temp;

}


String::operator const char*() const
{
  return Str;
}
  
bool  String::operator == (const String & str)
{
  if(strcmp(Str,str))
   return false;
  else
   return true;	  
}

/*
ostream & operator << (ostream & os, const String & str)
{
  os << str.Str;
  return os;
};
*/

//CStringList _____________________________________________________________

CStringList::CStringList (void)
{
  Lines = NULL;
  LinesCount = -1;
};

CStringList::CStringList (const CStringList & list)
{
  Lines = NULL;
  LinesCount = -1;
  for (uint c = 0; c < list.GetLinesCount (); c++)
    {
      AddLine (list.GetLine (c));
    };
};

CStringList::~CStringList (void)
{
  if (Lines)
    {
      delete[]Lines;
      Lines = NULL;
    };
  LinesCount = -1;
};

void
CStringList::Create ()
{
};

String
CStringList::GetLine (uint linen) const
{
  if((int)linen <= LinesCount)	
    return Lines[linen];
  else
    return "";
};

void
CStringList::SetLine (String line, uint linen)
{
  Lines[linen] = line;
};


void
CStringList::Clear (void)
{
  if (Lines)
    {
      delete[]Lines;
    };
  Lines = NULL;
  LinesCount = -1;
};

void
CStringList::AddLine (const String line)
{
  if (line.c_str () != NULL)
    {
      LinesCount++;
      String *ALines = new String[LinesCount + 1];
      for (int c = 0; c < LinesCount; c++)
	ALines[c] = Lines[c];
      ALines[LinesCount] = line;
      if (Lines)
	delete[]Lines;
      Lines = ALines;
    }
};

void
CStringList::InsertLine (String line, uint linen)
{
  LinesCount++;
  String *ALines = new String[LinesCount + 1];
  for (uint c = 0; c < linen; c++)
    ALines[c] = Lines[c];
  ALines[linen] = line;
  for (int c = linen + 1; c <= LinesCount; c++)
    ALines[c] = Lines[c - 1];
  if (Lines)
    delete[]Lines;
  Lines = ALines;
};


uint
CStringList::GetLinesCount (void) const
{
  return LinesCount + 1;
};

void
CStringList::DelLine (uint linen)
{
  LinesCount--;
  String *ALines = new String[LinesCount + 1];
  for (uint c = 0; c < linen; c++)
    ALines[c] = Lines[c];
  for (int c = linen; c < LinesCount + 1; c++)
    ALines[c] = Lines[c + 1];
  if (Lines)
    delete[]Lines;
  Lines = ALines;
};

bool CStringList::LoadFromFile (String fname)
{
  FILE* file;
  file=fopen(fname.c_str (),"r");
  if (file)
    {
      String line;
      while (fgetline (file, line))
	AddLine (line);
      fclose(file);
      return true;
    }
  else
    eprint( "File not found!\n");
  return false;
};


bool CStringList::SaveToFile (String fname)
{
  	
  FILE* file;
  file=fopen(fname.c_str (),"w");
  if (file)
    {
      for (int c = 0; c <= LinesCount; c++)
	fprintf(file,"%s\n",GetLine (c).c_str());
    fclose(file);
    }
  else
    eprint( "File not create!\n");
  return false;
  
}

bool CStringList::AppendToFile (String fname)
{
  	
  FILE* file;
  file=fopen(fname.c_str (),"a");
  if (file)
    {
      for (int c = 0; c <= LinesCount; c++)
	fprintf(file,"%s\n",GetLine (c).c_str());
    fclose(file);
    }
  else
    eprint( "File not appended create!\n");
  return false;
  
}

CStringList & CStringList::operator = (const CStringList & list)
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
	};
    };
  return *this;
};

#else
//CStringList _____________________________________________________________

CStringList::CStringList (void)
{
};

CStringList::CStringList (const CStringList & list)
{
  for (uint c = 0; c <= list.GetLinesCount (); c++)
    {
      AddLine (list.GetLine (c));
    };
};

CStringList::~CStringList (void)
{
  List.erase (List.begin (), List.end ());
};

void
CStringList::Create ()
{
};

string CStringList::GetLine (uint linen) const
{
  list < string > List2 (List);
  list < string >::iterator itList2;
  itList2 = List2.begin ();
  for (uint a = 0; a < linen; a++)
    itList2++;
  return *itList2;
};

void
CStringList::SetLine (string line, uint linen)
{
  itList = List.begin ();
  for (uint a = 0; a < linen; a++)
    itList++;
  *itList = line;
};


void
CStringList::Clear (void)
{
  List.erase (List.begin (), List.end ());
};

void
CStringList::AddLine (string line)
{
  List.push_back (line);
};

void
CStringList::InsertLine (string line, uint linen)
{
  itList = List.begin ();
  for (uint a = 0; a < linen; a++)
    itList++;
  List.insert (itList, line);
};


unsigned int
CStringList::GetLinesCount (void) const
{
  return List.size ();
};

void
CStringList::DelLine (uint linen)
{
  itList = List.begin ();
  for (uint a = 0; a < linen; a++)
    itList++;
  List.erase (itList);
};

bool CStringList::LoadFromFile (string fname)
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
    eprint( "File not found!\n");
};

bool CStringList::SaveToFile (string fname)
{
  ofstream
  file (fname.c_str ());
  if (file)
    {
      for (itList = List.begin (); itList != List.end (); itList++)
	file << *itList << endl;
    }
  else
    eprint( "File not create!\n");
};
#endif




String
xml_out (String name, String type, String value)
{
  return (lxT("  <") + name + lxT(" type=\"") + type + lxT("\">") + value + lxT("</") + name + lxT(">"));
};

void
xml_in (String data, String & name, String & type, String & value)
{
  int p0, p1, p2, p3;
  if(data.size() > 0)
  {

  p0 = data.find (lxT("<"));
  p1 = data.find (lxT("type="));
  p2 = data.find (lxT("\">"));
  p3 = data.find (lxT("</"));
  name = data.substr (p0 + 1, p1 - p0 - 2);
  type = data.substr (p1 + 6, p2 - p1 - 6);
  value = data.substr (p2 + 2, p3 - p2 - 2);
  }
  else
  {
  name=lxT("");
  type=lxT("");
  value=lxT("");
  }
};


String
basename (const String & str)
{
#ifndef __WXMSW__
  int pos = str.rfind (lxT("/"));
#else
  int pos = str.rfind (lxT("\\"));
  if(pos == -1) 
     pos = str.rfind (lxT("/"));
#endif
  return str.substr (pos + 1, str.size () - pos - 1);
};

String
dirname (const String & str)
{
#ifndef __WXMSW__
  int pos = str.rfind (lxT("/"));
#else
  int pos = str.rfind (lxT("\\"));
  if(pos == -1) 
     pos = str.rfind (lxT("/"));
#endif
  return str.substr (0, pos + 1);
};

