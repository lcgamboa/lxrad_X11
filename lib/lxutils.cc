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


#include"../include/lxutils.h"
#include"../include/cwindow.h"
#include"../include/capplication.h"
#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include <minizip/zip.h>
#include <minizip/unzip.h>

//-------------------------------------------------------------------------
lxTextFile::lxTextFile()
{
  f=NULL;
}

int 
lxTextFile::Open(String fname)
{
  fn=fname;
  f=fopen(fname.c_str(),"rw");
  if(f)
   return 1;
  else
   return 0;		  
}

int 
lxTextFile::Create(String fname)
{
  fn=fname;
  f=fopen(fname.c_str(),"rw");
  if(f)
   return 1;
  else
   return 0;		  
}
  
bool 
lxTextFile::IsOpened(void)
{
  if(f)
   return 1;
  else
   return 0;		  
}

void 
lxTextFile::Close(void)
{
  fclose(f);
  f=NULL;  
}

void 
lxTextFile::GoToLine(int l)
{
  //FIXME	
  fseek ( f, l, SEEK_SET);
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
 if(f) fclose(f);
 f=fopen(fn.c_str(),"w");
 if(f) fclose(f);
 f=fopen(fn.c_str(),"rw");
}

void 
lxTextFile::AddLine(String line)
{
 fprintf(f,"%s\n",line.c_str ());
}

//-------------------------------------------------------------------------

	
lxImage::lxImage()
{
  Image=NULL;
}

lxImage::~lxImage()
{
  if(Image)
  {
   imlib_context_set_image(Image);
   imlib_free_image();
   Image=NULL;
  }
}


bool
lxImage::LoadFile(String fname)
{
  Destroy();
	
  //Image = imlib_load_image_immediately_without_cache ((char *) fname.c_str ());
  Image = imlib_load_image_immediately ((char *) fname.c_str ());
  if(Image)
    return 0;
  else
    return 1;  
}

void
lxImage::Destroy(void)
{
  if(Image)
  {
   imlib_context_set_image(Image);
   imlib_free_image();
   Image=NULL;
  }
}

lxImage::operator Imlib_Image() const
{
  return Image;
}
//-------------------------------------------------------------------------

lxBitmap::~lxBitmap()
{
  if (CPixmap != 0)
    XFreePixmap (Application->GetADisplay (), CPixmap);
}

lxBitmap::lxBitmap(lxImage img, CWindow *win)
{
//FIXME use win parameter
//
   imlib_context_set_image(img);
   int w = imlib_image_get_width();
   int h = imlib_image_get_height();
                  
   CPixmap=XCreatePixmap (Application->GetADisplay (), *Application->GetADefaultRootWindow (), w, h, *(Application->GetADepth()));

   imlib_context_set_image(img);
   imlib_context_set_drawable(CPixmap);
   //imlib_context_set_mask(CMask);
   imlib_render_image_on_drawable(0,0);

}

lxBitmap::lxBitmap (int width, int height)
{
 CPixmap=XCreatePixmap (Application->GetADisplay (), *Application->GetADefaultRootWindow (), width, height, *(Application->GetADepth()));
}

lxBitmap::operator Pixmap() const 
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
   int x=0,y=0;
   unsigned int w=0,h=0,b,d;

    XGetGeometry(Application->GetADisplay(), CPixmap, &win, &x, &y, &w, &h, &b, &d); 
   
   return {(int)w,(int)h};
}

//-------------------------------------------------------------------------

bool
lxSound::Create(String fname)
{
//FIXME	
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
return false;
}

void
lxSound::Stop(void)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}

void 
lxSound::Play(int flags)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}
//-------------------------------------------------------------------------
void 
lxFileName::Assign(String fname)
{
  FName=fname;
}

void
lxFileName::MakeAbsolute(void)
{
    char resolved_path[PATH_MAX]; 
    realpath(FName, resolved_path); 
    FName=resolved_path;
}

String
lxFileName::GetFullPath(void)
{
return FName;
}

//-------------------------------------------------------------------------
lxColor::lxColor()
{
}

lxColor::lxColor(XColor color)
{
  Color=color;
}

lxColor::lxColor(const char * name)
{
  if(name[0] == '#' )
  {
    unsigned int r,g,b;
    char tmp[3];
    tmp[2]=0;
    tmp[0]=name[1];
    tmp[1]=name[2];
    sscanf(tmp,"%02X",&r);
    tmp[0]=name[3];
    tmp[1]=name[4];
    sscanf(tmp,"%02X",&g);
    tmp[0]=name[5];
    tmp[1]=name[6];
    sscanf(tmp,"%02X",&b);
    
    Color=ColorByRGB(r,g,b);
  }	  
  else
  { 	  
    Color=ColorByName(name);
  }
}

String
lxColor::GetAsString(int flags)
{
   char cname[20];
   XQueryColor(Application->GetADisplay (), Application->GetAScreen ()->cmap, &Color);

   if(flags ==  lxC2S_HTML_SYNTAX )
   {
     sprintf(cname,"#%02X%02X%02X",Color.red>>8,Color.green>>8,Color.blue>>8);
   }
   else
   {
     cname[0]=0;
   }

   return cname;	
}

lxColor::operator XColor() const 
{ 
  return Color; 
}
//-------------------------------------------------------------------------

lxCursor::lxCursor()
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}

lxCursor::lxCursor(int type)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}

//-------------------------------------------------------------------------

lxFont::lxFont()
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}

lxFont::lxFont(int size,int family,int style,int weight)
{
#ifdef _DEBUG	
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
#endif
}
//-------------------------------------------------------------------------
void lxMilliSleep(unsigned int time)
{
  usleep(time *1000);
}

void lxSetCursor(lxCursor cursor)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}

bool lxFileExists(String fname)
{
  struct stat sb;
  
  sb.st_mode=0; 

  stat(fname.c_str(), &sb);
          
  if(S_ISREG(sb.st_mode))
    return true;
  else
    return false;
}

void lxExecute(String cmd,unsigned int flags, void * arg)
{
 if(flags != lxEXEC_SYNC)
    cmd+=lxT(" &");	
  system(cmd.c_str());
}

String 
lxGetCwd(void)
{
   char cwd[1024];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
     return cwd;
   else
    return "";
}

int 
lxSetWorkingDirectory(String dir)
{
  return chdir(dir.c_str ());
}

bool 
lxLaunchDefaultBrowser(String url)
{
 String cmd= lxT("xdg-open ")+url+lxT("  &");
 return system(cmd.c_str ());
}

bool 
lxLaunchDefaultApplication(String cmd)
{
 String cmd_= lxT("xdg-open ")+cmd+lxT("  &");
 return system(cmd_.c_str ());
}

//-------------------------------------------------------------------------
bool
lxUnzipDir (const String &in_filename, const String &out_dirname) {
 
 char fname[1024];
 unzFile uzf =unzOpen (in_filename.c_str ());
 
 if(uzf != NULL)
 {
   if(unzGoToFirstFile(uzf) == UNZ_OK)
    {
     do
      {
      unz_file_info finfo; 
      unzGetCurrentFileInfo(uzf,&finfo,fname,1024,NULL,0,NULL,0);
      
      char *buff= (char *)malloc(finfo.uncompressed_size);
      unzOpenCurrentFile(uzf);
      unzReadCurrentFile(uzf,buff,finfo.uncompressed_size);
      unzCloseCurrentFile(uzf);
      
      String dname=out_dirname;
      dname+=dirname(fname);
      
      if(dname.length () > 0)
       {
        lxCreateDir(dname);
       }
      
      String name=out_dirname;
      name+=fname;
      
      FILE * fout=fopen(name.c_str(),"w");
      if(fout)
       {
        fwrite(buff,finfo.uncompressed_size,1, fout);
        fclose(fout);
       }
      free(buff);
      }
      while(unzGoToNextFile(uzf) == UNZ_OK);
    }
   
   unzClose(uzf);
 }
 
 return 0;
 }

bool
lxZipDir (const String &in_dirname, const String &out_filename)
{
CStringList paths =lxListDirRec(in_dirname);
   
String dname= basename(in_dirname.substr(0,in_dirname.length ()-1));

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
   FILE * file =fopen(paths.GetLine (i).c_str (), "r");
   if (file)
    {
     fseek (file,0,SEEK_END);
     long size = ftell (file);
     fseek (file, 0, SEEK_SET);

     char * buffer= (char*)malloc(size);
     if (size == 0 || fread (buffer, size,1,file))
      {
       zip_fileinfo zfi = {0};
       String fileName = dname+paths.GetLine (i).substr(in_dirname.length (),paths.GetLine (i).length ()-in_dirname.length ());

       if (0 == zipOpenNewFileInZip (zf, fileName.c_str (), &zfi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION))
        {
         if (zipWriteInFileInZip (zf, size == 0 ? "" : buffer, size))
          _return = false;

         if (zipCloseFileInZip (zf))
          _return = false;

         fclose (file);
         free(buffer);
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

bool lxRemoveFile(const char * fname)
{
    return remove(fname);
}

bool 
lxRenameFile(String oldfname, String newfname)
{
 return rename(oldfname.c_str(),newfname.c_str());
}

bool lxRemoveDir(const char* dirname)
{	
     DIR  *dp;
     struct dirent *dent;
     struct stat sb;
     char fname[1024];
     
     dp = opendir (dirname);

     if(dp)
     {
       while((dent=readdir(dp))!=NULL)
       {
          snprintf(fname,1024,"%s/%s",dirname,dent->d_name);  
          stat(fname, &sb);
          
          if(S_ISREG(sb.st_mode))
          {
            lxRemoveFile(fname);
          }
          else if(S_ISDIR(sb.st_mode))
          {
            if(!(!strcmp(dent->d_name,".") || !strcmp(dent->d_name,"..")))
            {    
              lxRemoveDir(fname);
            }
          }
       }
       closedir(dp);
       return rmdir(dirname);
     }
   return 0;
}    

bool 
lxCreateDir(const char * dirname)
{
   DIR  *dp; 
   dp = opendir (dirname);

   if(dp)
   {
     return closedir(dp);
   }
   else
   {
#ifndef _WIN_
     return mkdir(dirname, S_IWUSR| S_IRUSR | S_IXUSR | S_IRGRP | S_IROTH ); 
#else     
     return mkdir(dirname);
#endif
   }
   return 0;
}

CStringList
lxListDirRec (const String & dirname)
{
 DIR *dp;
 struct dirent *dent;
 struct stat sb;
 char fname[1024];

 CStringList list;

 list.Clear ();

 dp = opendir (dirname);

 if (dp)
  {
   while ((dent = readdir (dp)) != NULL)
    {
     snprintf (fname, 1024, "%s/%s", dirname.c_str(), dent->d_name);
     stat (fname, &sb);

     if (S_ISREG (sb.st_mode))
      {
       list.AddLine (fname);
      }
     else if (S_ISDIR (sb.st_mode))
      {
       if (!(!strcmp (dent->d_name, ".") || !strcmp (dent->d_name, "..")))
        {
          CStringList list2=lxListDirRec (fname);
          for(unsigned int i=0;i<list2.GetLinesCount ();i++)
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

String 
lxGetUserDataDir(String appname)
{
  return String("/home/")+getlogin()+"/."+appname;
}

String 
lxGetTempDir(String appname)
{
  return "/tmp/";
}

String 
lxGetExecutablePath(String appname)
{
  return "/usr/bin/";
}



// Useful functions_______________________________________________________

void mprint(String message)
{
  fprintf(stdout,"%s",message.c_str());
};

void eprint(String error)
{
  fprintf(stderr,"%s",error.c_str());
};

XColor
ColorByRGB (unsigned short r, unsigned short g, unsigned short b)
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
ColorByName (String name)
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
XXLookupString (XIC ic, XKeyPressedEvent * event, char *buffer_return,
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

  strcpy (buffer_return, XKeysymToString (*keysym_return));
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
    };

#endif
}
