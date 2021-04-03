/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001-2021  Luis Claudio Gamboa Lopes

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


#include"../include/lxutils.h"
#include"../include/cwindow.h"
#include"../include/capplication.h"
#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include <minizip/zip.h>
#include <minizip/unzip.h>

#include"../../lunasvg/include/svgdocument.h"

using namespace lunasvg;

//-------------------------------------------------------------------------

lxTextFile::lxTextFile()
{
 f = NULL;
}

int
lxTextFile::Open(lxString fname)
{
 fn = fname;
 f = fopen (fname.c_str (), "rw");
 if (f)
  return 1;
 else
  return 0;
}

int
lxTextFile::Create(lxString fname)
{
 fn = fname;
 f = fopen (fname.c_str (), "rw");
 if (f)
  return 1;
 else
  return 0;
}

bool
lxTextFile::IsOpened(void)
{
 if (f)
  return 1;
 else
  return 0;
}

void
lxTextFile::Close(void)
{
 fclose (f);
 f = NULL;
}

void
lxTextFile::GoToLine(int l)
{
 //FIXME	
 fseek (f, l, SEEK_SET);
}

FILE *
lxTextFile::GetFd(void)
{
 return f;
}

lxTextFile::operator FILE*() const
{
 return f;
}

void
lxTextFile::Clear(void)
{
 if (f) fclose (f);
 f = fopen (fn.c_str (), "w");
 if (f) fclose (f);
 f = fopen (fn.c_str (), "rw");
}

void
lxTextFile::AddLine(lxString line)
{
 fprintf (f, "%s\n", line.c_str ());
}

//-------------------------------------------------------------------------

lxImage::lxImage(CPWindow * win)
{
 Image = NULL;
}

lxImage::lxImage(Imlib_Image img)
{
 Image = img;
}

lxImage::~lxImage()
{
 if (Image)
  {
   imlib_context_set_image (Image);
   imlib_free_image ();
   Image = NULL;
  }
}

unsigned int
lxImage::GetWidth(void)
{
 imlib_context_set_image (Image);
 return imlib_image_get_width ();
}

unsigned int
lxImage::GetHeight(void)
{
 imlib_context_set_image (Image);
 return imlib_image_get_height ();
}

bool
lxImage::LoadFile(const lxString fname, int orientation, float scalex, float scaley, int useAlpha, double * ret_sx, double * ret_sy)
{
 Destroy ();

 if (fname.Contains (".svg"))
  {
   int width;
   int height;

   SVGDocument document;
   if (document.loadFromFile ((const char *) fname.c_str ()))
    {
     width = document.documentWidth (96.0) * scalex;
     height = document.documentHeight (96.0) * scaley;

     if (ret_sx)
      *ret_sx = ((double) width) / document.documentWidth (96.0);

     if (ret_sy)
      *ret_sy = ((double) height) / document.documentHeight (96.0);

     Bitmap bitmap = document.renderToBitmap (width, height, 96.0, 0);

     const unsigned char * bmp = bitmap.data ();
     int size = bitmap.width () * bitmap.height ();
     unsigned char * data = (unsigned char *) malloc (size * 4);

     //RGBA to  ARGB 
     for (int i = 0; i < size; i++)
      {
       data[(4 * i) + 0] = bmp[(4 * i) + 2];
       data[(4 * i) + 1] = bmp[(4 * i) + 1];
       data[(4 * i) + 2] = bmp[(4 * i) + 0];
       data[(4 * i) + 3] = bmp[(4 * i) + 3];
      }

     Image = imlib_create_image_using_copied_data (bitmap.width (), bitmap.height (), (unsigned int*) data);

     free (data);

     imlib_context_set_image (Image);
     imlib_image_orientate (orientation);

     return 1;
    }

  }
 else //png
  {
   lxImage image = imlib_load_image_immediately ((char *) fname.c_str ());

   if (image)
    {

     imlib_context_set_image (image);
     imlib_image_orientate (orientation);

     Image = imlib_create_cropped_scaled_image (0, 0, imlib_image_get_width (), imlib_image_get_height (),
                                                imlib_image_get_width () * scalex, imlib_image_get_height () * scaley);

     image.Destroy ();
     return 1;
    }
  }

 return 0;

}

void
lxImage::Destroy(void)
{
 if (Image)
  {
   imlib_context_set_image (Image);
   imlib_free_image ();
   Image = NULL;
  }
}

lxImage::operator
Imlib_Image() const
{
 return Image;
}
//-------------------------------------------------------------------------

lxBitmap::~lxBitmap()
{
 if (CPixmap != 0)
  XFreePixmap (Application->GetADisplay (), CPixmap);
}

lxBitmap::lxBitmap(lxImage * img, CWindow *win, int orient)
{
 //FIXME use win parameter
 //
 imlib_context_set_image (*img);
 int w = imlib_image_get_width ();
 int h = imlib_image_get_height ();

 switch (orient)
  {
  case 0:
  case 2:
   CPixmap = XCreatePixmap (Application->GetADisplay (), *Application->GetADefaultRootWindow (), w, h, *(Application->GetADepth ()));
   break;
  case 1:
  case 3:
   CPixmap = XCreatePixmap (Application->GetADisplay (), *Application->GetADefaultRootWindow (), h, w, *(Application->GetADepth ()));
   break;
  }

 imlib_image_orientate (orient);
 imlib_context_set_drawable (CPixmap);
 //imlib_context_set_mask(CMask);
 imlib_render_image_on_drawable (0, 0);

}

lxBitmap::lxBitmap(int width, int height)
{
 CPixmap = XCreatePixmap (Application->GetADisplay (), *Application->GetADefaultRootWindow (), width, height, *(Application->GetADepth ()));
}

lxBitmap::operator
Pixmap() const
{
 return CPixmap;
}

Pixmap
lxBitmap::GetPixmap(void)
{
 return CPixmap;
}

lxSize
lxBitmap::GetSize(void)
{
 Window win;
 lxSize size;
 int x = 0, y = 0;
 unsigned int w = 0, h = 0, b, d;
 XGetGeometry (Application->GetADisplay (), CPixmap, &win, &x, &y, &w, &h, &b, &d);
 size.SetWidth (w);
 size.SetHeight (h);
 return size;
}

unsigned int
lxBitmap::GetWidth(void)
{
 Window win;
 int x = 0, y = 0;
 unsigned int w = 0, h = 0, b, d;
 XGetGeometry (Application->GetADisplay (), CPixmap, &win, &x, &y, &w, &h, &b, &d);

 return w;
}

unsigned int
lxBitmap::GetHeight(void)
{
 Window win;
 int x = 0, y = 0;
 unsigned int w = 0, h = 0, b, d;
 XGetGeometry (Application->GetADisplay (), CPixmap, &win, &x, &y, &w, &h, &b, &d);

 return h;
}

//-------------------------------------------------------------------------

bool
lxSound::Create(lxString fname)
{
 //FIXME	
 printf ("Incomplete: %s -> %s :%i\n", __func__, __FILE__, __LINE__);
 return false;
}

void
lxSound::Stop(void)
{
 printf ("Incomplete: %s -> %s :%i\n", __func__, __FILE__, __LINE__);
}

void
lxSound::Play(int flags)
{
 printf ("Incomplete: %s -> %s :%i\n", __func__, __FILE__, __LINE__);
}
//-------------------------------------------------------------------------

void
lxFileName::Assign(lxString fname)
{
 FName = fname;
}

void
lxFileName::MakeAbsolute(void)
{
 char resolved_path[PATH_MAX];
 realpath (FName, resolved_path);
 FName = resolved_path;
}

lxString
lxFileName::GetFullPath(void)
{
 return FName;
}

//-------------------------------------------------------------------------

lxColor::lxColor() { }

lxColor::lxColor(XColor color)
{
 Color = color;
}

lxColor::lxColor(const char * name)
{
 if (name[0] == '#')
  {
   unsigned int r, g, b;
   char tmp[3];
   tmp[2] = 0;
   tmp[0] = name[1];
   tmp[1] = name[2];
   sscanf (tmp, "%02X", &r);
   tmp[0] = name[3];
   tmp[1] = name[4];
   sscanf (tmp, "%02X", &g);
   tmp[0] = name[5];
   tmp[1] = name[6];
   sscanf (tmp, "%02X", &b);

   Color = ColorByRGB (r, g, b);
  }
 else
  {
   Color = ColorByName (name);
  }
}

lxColor::lxColor(unsigned char r, unsigned char g, unsigned char b)
{
 Color = ColorByRGB (r, g, b);
}

lxString
lxColor::GetAsString(int flags)
{
 char cname[20];
 XQueryColor (Application->GetADisplay (), Application->GetAScreen ()->cmap, &Color);

 if (flags == lxC2S_HTML_SYNTAX)
  {
   sprintf (cname, "#%02X%02X%02X", Color.red >> 8, Color.green >> 8, Color.blue >> 8);
  }
 else
  {
   cname[0] = 0;
  }

 return cname;
}

lxColor::operator
XColor() const
{
 return Color;
}
//-------------------------------------------------------------------------

lxCursor::lxCursor()
{
 printf ("Incomplete: %s -> %s :%i\n", __func__, __FILE__, __LINE__);
}

lxCursor::lxCursor(int type)
{
 printf ("Incomplete: %s -> %s :%i\n", __func__, __FILE__, __LINE__);
}

//-------------------------------------------------------------------------

lxFont::lxFont()
{
 printf ("Incomplete: %s -> %s :%i\n", __func__, __FILE__, __LINE__);
}

lxFont::lxFont(int size, int family, int style, int weight)
{
#ifdef _DEBUG 
 printf ("Incomplete: %s -> %s :%i\n", __func__, __FILE__, __LINE__);
#endif
}
//-------------------------------------------------------------------------

void
lxMilliSleep(unsigned int time)
{
 usleep (time * 1000);
}

void
lxSetCursor(lxCursor cursor)
{
 printf ("Incomplete: %s -> %s :%i\n", __func__, __FILE__, __LINE__);
}

bool
lxFileExists(lxString fname)
{
 struct stat sb;

 sb.st_mode = 0;

 stat (fname.c_str (), &sb);

 if (S_ISREG (sb.st_mode))
  return true;
 else
  return false;
}

int
lxExecute(lxString cmd, unsigned int flags, void * arg)
{
 if (flags != lxEXEC_SYNC)
  cmd += lxT (" &");
 return system (cmd.c_str ()) + 1;
}

lxString
lxGetCwd(void)
{
 char cwd[1024];
 if (getcwd (cwd, sizeof (cwd)) != NULL)
  return cwd;
 else
  return "";
}

int
lxSetWorkingDirectory(lxString dir)
{
 return chdir (dir.c_str ());
}

bool
lxLaunchDefaultBrowser(lxString url)
{
 lxString cmd = lxT ("xdg-open ") + url + lxT ("  &");
 return system (cmd.c_str ());
}

bool
lxLaunchDefaultApplication(lxString cmd)
{
 lxString cmd_ = lxT ("xdg-open ") + cmd + lxT ("  &");
 return system (cmd_.c_str ());
}

//-------------------------------------------------------------------------

bool
lxUnzipDir(const lxString &in_filename, const lxString &out_dirname)
{

 char fname[1024];
 unzFile uzf = unzOpen (in_filename.c_str ());

 if (uzf != NULL)
  {
   if (unzGoToFirstFile (uzf) == UNZ_OK)
    {
     do
      {
       unz_file_info finfo;
       unzGetCurrentFileInfo (uzf, &finfo, fname, 1024, NULL, 0, NULL, 0);

       char *buff = (char *) malloc (finfo.uncompressed_size);
       unzOpenCurrentFile (uzf);
       unzReadCurrentFile (uzf, buff, finfo.uncompressed_size);
       unzCloseCurrentFile (uzf);

       lxString dname = out_dirname;
       dname += dirname (fname);

       if (dname.length () > 0)
        {
         lxCreateDir (dname);
        }

       lxString name = out_dirname;
       name += fname;

       FILE * fout = fopen (name.c_str (), "w");
       if (fout)
        {
         fwrite (buff, finfo.uncompressed_size, 1, fout);
         fclose (fout);
        }
       free (buff);
      }
     while (unzGoToNextFile (uzf) == UNZ_OK);
    }

   unzClose (uzf);
  }

 return 0;
}

bool
lxZipDir(const lxString &in_dirname, const lxString &out_filename)
{
 lxStringList paths = lxListDirRec (in_dirname);

 lxString dname = basename (in_dirname.substr (0, in_dirname.length () - 1));

 if (paths.GetLinesCount () == 0)
  {
   return 1;
  }


 zipFile zf = zipOpen (out_filename.c_str (), APPEND_STATUS_CREATE);
 if (zf == NULL)
  {
   return 1;
  }

 bool _return = true;


 for (size_t i = 0; i < paths.GetLinesCount (); i++)
  {
   FILE * file = fopen (paths.GetLine (i).c_str (), "r");
   if (file)
    {
     fseek (file, 0, SEEK_END);
     long size = ftell (file);
     fseek (file, 0, SEEK_SET);

     char * buffer = (char*) malloc (size);
     if (size == 0 || fread (buffer, size, 1, file))
      {
       zip_fileinfo zfi = {0};
       lxString fileName = dname + paths.GetLine (i).substr (in_dirname.length (), paths.GetLine (i).length () - in_dirname.length ());

       if (0 == zipOpenNewFileInZip (zf, fileName.c_str (), &zfi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION))
        {
         if (zipWriteInFileInZip (zf, size == 0 ? "" : buffer, size))
          _return = false;

         if (zipCloseFileInZip (zf))
          _return = false;

         fclose (file);
         free (buffer);
         continue;
        }
      }
     fclose (file);
    }
   _return = false;
  }

 if (zipClose (zf, NULL))
  return 3;

 if (!_return)
  return 4;
 return 0;

}

bool
lxRemoveFile(const char * fname)
{
 return remove (fname);
}

bool
lxRenameFile(lxString oldfname, lxString newfname)
{
 return rename (oldfname.c_str (), newfname.c_str ());
}

bool
lxRemoveDir(const char* dirname)
{
 DIR *dp;
 struct dirent *dent;
 struct stat sb;
 char fname[1024];

 dp = opendir (dirname);

 if (dp)
  {
   while ((dent = readdir (dp)) != NULL)
    {
     snprintf (fname, 1024, "%s/%s", dirname, dent->d_name);
     stat (fname, &sb);

     if (S_ISREG (sb.st_mode))
      {
       lxRemoveFile (fname);
      }
     else if (S_ISDIR (sb.st_mode))
      {
       if (!(!strcmp (dent->d_name, ".") || !strcmp (dent->d_name, "..")))
        {
         lxRemoveDir (fname);
        }
      }
    }
   closedir (dp);
   return rmdir (dirname);
  }
 return 0;
}

bool
lxCreateDir(const char * dirname)
{
 DIR *dp;
 dp = opendir (dirname);

 if (dp)
  {
   return closedir (dp);
  }
 else
  {
#ifndef _WIN_
   return mkdir (dirname, S_IWUSR | S_IRUSR | S_IXUSR | S_IRGRP | S_IROTH);
#else     
   return mkdir (dirname);
#endif
  }
 return 0;
}

lxStringList
lxListDirRec(const lxString & dirname)
{
 DIR *dp;
 struct dirent *dent;
 struct stat sb;
 char fname[1024];

 lxStringList list;

 list.Clear ();

 dp = opendir (dirname);

 if (dp)
  {
   while ((dent = readdir (dp)) != NULL)
    {
     snprintf (fname, 1024, "%s/%s", dirname.c_str (), dent->d_name);
     stat (fname, &sb);

     if (S_ISREG (sb.st_mode))
      {
       list.AddLine (fname);
      }
     else if (S_ISDIR (sb.st_mode))
      {
       if (!(!strcmp (dent->d_name, ".") || !strcmp (dent->d_name, "..")))
        {
         lxStringList list2 = lxListDirRec (fname);
         for (unsigned int i = 0; i < list2.GetLinesCount (); i++)
          {
           list.AddLine (list2.GetLine (i));
          }
        }
      }
    }
   closedir (dp);
  }
 return list;

}

lxString
lxGetUserDataDir(lxString appname)
{
 return lxString ("/home/") + getlogin () + "/." + appname;
}

lxString
lxGetTempDir(lxString appname)
{
 return "/tmp/";
}

lxString
lxGetExecutablePath(lxString appname)
{
 char buff[1024];
 int size = readlink ("/proc/self/exe", buff, 1023);
 if (size == -1)
  {
   return "";
  }
 buff[size] = 0;
 return buff;
}



// Useful functions_______________________________________________________

void
mprint(lxString message)
{
 fprintf (stdout, "%s", message.c_str ());
};

void
eprint(lxString error)
{
 fprintf (stderr, "%s", error.c_str ());
};

XColor
ColorByRGB(unsigned short r, unsigned short g, unsigned short b)
{
 XColor
 colorA,
     colorB;
 colorA.red = r * 256;
 colorA.green = g * 256;
 colorA.blue = b * 256;
 colorA.flags = DoRed | DoGreen | DoBlue;
 colorA.pad = '@';
 colorB = colorA;
 if (Application->XSearchInColorTable (&colorA))
  return colorA;
 XAllocColor (Application->GetADisplay (), Application->GetAScreen ()->cmap,
              &colorB);
 Application->AddToColorTable ("RGB", colorA, colorB);
 return colorB;
};

XColor
ColorByName(lxString name)
{
 XColor
 colorA,
     colorB;
 if (Application->XSearchInColorTable (name, &colorA))
  return colorA;
 XAllocNamedColor (Application->GetADisplay (),
                   Application->GetAScreen ()->cmap, name.c_str (), &colorA,
                   &colorB);
 Application->AddToColorTable (name, colorA, colorB);
 return colorB;
};

int
XXLookupString(XIC ic, XKeyPressedEvent * event, char *buffer_return,
               int bytes_buffer, KeySym * keysym_return,
               Status * status_return)
{
#ifdef _DEBUG
 *status_return = XLookupBoth;
 switch ((event->state & ShiftMask) + (event->state & LockMask))
  {
  case ShiftMask:
   *keysym_return = XLookupKeysym (event, 1);
   break;
  case LockMask:
   *keysym_return = XLookupKeysym (event, 1);
   break;
  default:
   *keysym_return = XLookupKeysym (event, 0);
  };

 strcpy (buffer_return, XKeysymTolxString (*keysym_return));
 return 1;

#else
 if (ic)
  {
   return XmbLookupString (ic, event, buffer_return, bytes_buffer,
                           keysym_return, status_return);
  }
 else
  {
   return XLookupString (event, buffer_return, bytes_buffer, keysym_return,
                         NULL);
  }

#endif
}

lxBitmap *
lxGetBitmapRotated(lxImage *image, CWindow * win, int orientation)
{
 return new lxBitmap (image, win, orientation);
}

unsigned int
lxGetDisplayWidth(int disp)
{
 return XDisplayWidth (Application->GetADisplay (), DefaultScreen (Application->GetADisplay ()));
}

unsigned int
lxGetDisplayHeight(int disp)
{
 return XDisplayHeight (Application->GetADisplay (), DefaultScreen (Application->GetADisplay ()));
}
