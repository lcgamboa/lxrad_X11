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

#include"../config.h"
#include"../include/cdraw.h"
#include"../include/cmessage.h"
#include"../include/capplication.h"

#include"xevents.cc"

#include<pthread.h>
//extern pthread_mutex_t Display_Lock;

#ifdef HAVE_LIBIMLIB2
#include<Imlib2.h>
//extern ImlibData* AID;
#endif

#ifdef HAVE_LIBXPM    
#include<X11/xpm.h>
#endif

// CDraw_____________________________________________________________

CDraw::CDraw (void)
{
  FileName = "";
  Data = NULL;
  SetX (10);
  SetY (10);
  SetWidth (100);
  SetHeight (100);
  SetClass ("CDraw");
  SetTransparent (false);
  CMask = 0;
  CPixmap = 0;
};

CDraw::~CDraw (void)
{
};

void
CDraw::Create (CControl * control)
{
  if (!ColorSet)
    SetColor ("white");
  CControl::Create (control);
  Canvas.Create(this);
  Canvas.SetDoCalcRXY(false);
  Visible=false;	    
  SetPixmapFileName (FileName);
  SetPixmapData (Data);
  Visible=true;	    
  if(!CPixmap)
  {
    CPixmap =XCreatePixmap (Win->GetADisplay (), Win->GetWWindow (), Width, Height, *(Win->GetADepth()));
    Canvas.SetDrawIn(CPixmap);
    Canvas.Pen.SetColor (Color);
    Canvas.Rectangle ( 0, 0, Width, Height);
  };
 };

void
CDraw::Destroy (void)
{
  if (CPixmap != 0)
    XFreePixmap (Win->GetADisplay (), CPixmap);
  if (CMask != 0)
    XFreePixmap (Win->GetADisplay (), CMask);
  Canvas.Destroy();
  CControl::Destroy ();
};

void 
CDraw::SetWidth (uint width)
{
  if(Paint != NULL)
  {
    Display *disp=Application->GetADisplay();
    if(disp)
    {
      //pthread_mutex_lock (&Display_Lock);
      XLockDisplay(disp);
      int en=XPending(disp);
      XEvent ev;
      for(int i=0; i< en; i++)
      {
	      XNextEvent(disp,&ev); 
              printf("event type %s  %lX  %lX\n",xevent_name(ev.type).c_str(),ev.xany.window,CPixmap);
      }

      if (CPixmap != 0) XFreePixmap (Win->GetADisplay (), CPixmap);
      CPixmap =XCreatePixmap (Win->GetADisplay (), Win->GetWWindow (), width, Height, *(Win->GetADepth()));
      Canvas.SetDrawIn(CPixmap);
      XUnlockDisplay(disp);
      //pthread_mutex_unlock (&Display_Lock);
    }
  };
  CControl::SetWidth(width);
};

void 
CDraw::SetHeight (uint height)
{
  if(Paint != NULL)
  {
    Display *disp=Application->GetADisplay();
    if(disp)
    {
      //pthread_mutex_lock (&Display_Lock);
      XLockDisplay(disp);
      int en=XPending(disp);
      XEvent ev;
      for(int i=0; i< en; i++)
      {
	      XNextEvent(disp,&ev); 
              printf("event type %s  %lX  %lX\n",xevent_name(ev.type).c_str(),ev.xany.window,CPixmap);
      }

      if (CPixmap != 0)XFreePixmap (Win->GetADisplay (), CPixmap);
      CPixmap =XCreatePixmap (Win->GetADisplay (), Win->GetWWindow (), Width, height, *(Win->GetADepth()));
      Canvas.SetDrawIn(CPixmap);
      XUnlockDisplay(disp);
      //pthread_mutex_unlock (&Display_Lock);
    }
  };
  CControl::SetHeight(height);
};
      
void
CDraw::Draw ()
{
	
  if ((!Visible)||(Paint == NULL))return;
  Paint->InitDraw (this);
 
  if(Color.pixel != BColor.pixel)
  {
    if(CPixmap != 0)
    {	  
      XFreePixmap (Win->GetADisplay (), CPixmap);
      CPixmap=0;
    };
    BColor=Color;
  };
  
  if (CPixmap == 0)
    {
      Visible=false;	    
      SetPixmapFileName (FileName);
      SetPixmapData (Data);
      Visible=true;	    
      Canvas.SetDrawIn(CPixmap);
      if(!CPixmap)
      {
        CPixmap =XCreatePixmap (Win->GetADisplay (), Win->GetWWindow (), Width, Height, *(Win->GetADepth()));
        Canvas.SetDrawIn(CPixmap);
        Canvas.Pen.SetColor (Color);
	Canvas.Rectangle ( 0, 0, Width, Height);
      };
    };
  
  if (CPixmap != 0)
    {
      if ((Transparent) && (CMask != 0))
      {
        XGCValues gcvalues;
        gcvalues.clip_x_origin = GetRX ();
        gcvalues.clip_y_origin = GetRY ();
        gcvalues.clip_mask = CMask;
        XChangeGC (Win->GetADisplay (), Paint->Agc, GCClipXOrigin | GCClipYOrigin | GCClipMask, &gcvalues);
      };
      Paint->Rectangle(0, 0, Width, Height);
      Paint->PutPixmap(0,0,Width,Height,CPixmap);
      XSetClipMask (Win->GetADisplay (), Paint->Agc, None);
    };
  
  Paint->LowerFrame ( 0, 0, Width, Height, Border);
  CControl::Draw ();

};

bool
CDraw::GetTransparent (void)
{
  return Transparent;
};

void
CDraw::SetTransparent (bool transparent)
{
  Transparent = transparent;
};

CStringList CDraw::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("PixmapFileName=" + GetPixmapFileName () + ";File");
  return Context;
};

void
CDraw::SetContext (CStringList context)
{
  Eraser ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      String line = Context.GetLine (i);
      String arg;
      eqparse (line, arg);
      if (line.compare ("PixmapFileName") == 0)
      {
	if(arg.size() >  0)
	  SetPixmapFileName (arg);
	else
	  SetPixmapFileName ("");
      };
    };
  Draw ();
};

String
CDraw::GetPixmapFileName (void)
{
  return FileName;
};


bool
CDraw::SetPixmapFileName (String filename)
{
  FileName = filename;
  if (CPixmap != 0)
    XFreePixmap (Win->GetADisplay (), CPixmap);
  CPixmap=0;
 
  if ((Win) && (FileName.size () > 0))
    {
      int rc = -1;
      
#ifdef HAVE_LIBIMLIB2
     Imlib_Image image;
#else
#ifdef HAVE_LIBXPM    
      XpmAttributes xpmAttributes;
      
      xpmAttributes.colormap = XDefaultColormap (Win->GetADisplay (), 0);
      xpmAttributes.closeness = 65535;
      xpmAttributes.valuemask =
	XpmSize | XpmReturnPixels | XpmColormap | XpmCloseness;
#endif
#endif

      CPixmap =
	XCreatePixmap (Win->GetADisplay (), Win->GetWWindow (), Width, Height, *(Win->GetADepth()));
      if (CMask != 0)
	XFreePixmap (Win->GetADisplay (), CMask);
      CMask =
	XCreatePixmap (Win->GetADisplay (), Win->GetWWindow (), Width, Height,
		       1);

#ifdef HAVE_LIBIMLIB2
     image = imlib_load_image_immediately_without_cache ((char *) FileName.c_str ());
     if (image)
     { 
       /* set the image we loaded as the current context image to work on */
       imlib_context_set_image(image);
       imlib_context_set_drawable(CPixmap);
       imlib_context_set_mask(CMask);
       imlib_render_image_on_drawable(0,0);
       imlib_free_image();
       rc=0;
     }
#else     
#ifdef HAVE_LIBXPM    
      rc =
	XpmReadFileToPixmap (Win->GetADisplay (), Win->GetWWindow (),
			     (char *) FileName.c_str (), &CPixmap, &CMask,
			     &xpmAttributes);
      if(rc == 0)
      XpmFreeAttributes (&xpmAttributes);
#endif
#endif

      Canvas.SetDrawIn(CPixmap);
      
      if (rc == 0)
	{
	  Draw ();
	  return true;
	}
      else
	{
	  Message ("Warning: Load pixmap " + FileName + " failed");
	  FileName = "";
          Canvas.SetDrawIn(0);

	  return false;
	};
    };
  return false;
}


bool
CDraw::SetPixmapData (char **data)
{
  Data = data;
  if ((Win) && (data != NULL))
    {
      int rc = -1;
#ifdef HAVE_LIBXPM    
      XpmAttributes xpmAttributes;
      xpmAttributes.colormap = XDefaultColormap (Win->GetADisplay (), 0);
      xpmAttributes.closeness = 65535;
      xpmAttributes.valuemask =
	XpmSize | XpmReturnPixels | XpmColormap | XpmCloseness;
#endif  

      if (CPixmap != 0)
	XFreePixmap (Win->GetADisplay (), CPixmap);
      CPixmap =
	XCreatePixmap (Win->GetADisplay (), Win->GetWWindow (), Width, Height,
		       *(Win->GetADepth()));
      if (CMask != 0)
	XFreePixmap (Win->GetADisplay (), CMask);
      CMask =
	XCreatePixmap (Win->GetADisplay (), Win->GetWWindow (), Width, Height,
		       1);

#ifdef HAVE_LIBXPM    
      rc =
	XpmCreatePixmapFromData (Win->GetADisplay (), Win->GetWWindow (),
				 data, &CPixmap, &CMask, &xpmAttributes);
      if(rc == 0)XpmFreeAttributes (&xpmAttributes);
#endif
      Canvas.SetDrawIn(CPixmap);
      if (rc == 0)
	{
	  Draw ();
	  return true;
	}
      else
	{
	  eprint("Warning: Load pixmap data failed with rc=" + itoa(rc)+"\n");
	  Message ("Warning: Load pixmap data failed");
	  Data = NULL;
	  return false;
	};
    };
  return false;
};

void
CDraw::WritePixmapToFile (String filename)
{
#ifdef HAVE_LIBXPM    
  XpmWriteFileFromPixmap (Win->GetADisplay (), (char *) filename.c_str (),
			  CPixmap, CMask, 0);
#endif

#ifdef HAVE_LIBIMLIB    
  ImlibImage *im;
  ImlibSaveInfo info;
  im = Imlib_create_image_from_drawable(AID,CPixmap , CMask, 0, 0, Width, Height);
  
  if(filename.find(".xpm") > 0)
    Imlib_save_image_to_ppm(AID,im,(char *) filename.c_str () );
  else
    Imlib_save_image(AID, im, (char *) filename.c_str (), &info);
#endif
};


bool 
CDraw::SetImgFileName(String iname)
{
  return SetPixmapFileName (iname);
}
  
bool 
CDraw::SetImgFileName(String filename, float sx, float sy)
{
  FileName = filename;
  if (CPixmap != 0)
    XFreePixmap (Win->GetADisplay (), CPixmap);
  CPixmap=0;
 
  if ((Win) && (FileName.size () > 0))
    {
      int rc = -1;
      
#ifdef HAVE_LIBIMLIB2
     Imlib_Image image;
#else
#ifdef HAVE_LIBXPM    
      XpmAttributes xpmAttributes;
      
      xpmAttributes.colormap = XDefaultColormap (Win->GetADisplay (), 0);
      xpmAttributes.closeness = 65535;
      xpmAttributes.valuemask =
	XpmSize | XpmReturnPixels | XpmColormap | XpmCloseness;
#endif
#endif

      CPixmap =
	XCreatePixmap (Win->GetADisplay (), Win->GetWWindow (), Width, Height, *(Win->GetADepth()));
      if (CMask != 0)
	XFreePixmap (Win->GetADisplay (), CMask);
      CMask =
	XCreatePixmap (Win->GetADisplay (), Win->GetWWindow (), Width, Height,
		       1);

#ifdef HAVE_LIBIMLIB2
     image = imlib_load_image_immediately_without_cache ((char *) FileName.c_str ());
     if (image)
     { 
       /* set the image we loaded as the current context image to work on */
       imlib_context_set_image(image);
       imlib_context_set_drawable(CPixmap);
       imlib_context_set_mask(CMask);
       imlib_render_image_on_drawable_at_size(0,0,Width,Height);
       imlib_free_image();
       rc=0;
     }
#else     
#ifdef HAVE_LIBXPM    
      rc =
	XpmReadFileToPixmap (Win->GetADisplay (), Win->GetWWindow (),
			     (char *) FileName.c_str (), &CPixmap, &CMask,
			     &xpmAttributes);
      if(rc == 0)
      XpmFreeAttributes (&xpmAttributes);
#endif
#endif

      Canvas.SetDrawIn(CPixmap);
      
      if (rc == 0)
	{
	  Draw ();
	  return true;
	}
      else
	{
	  Message ("Warning: Load pixmap " + FileName + " failed");
	  FileName = "";
          Canvas.SetDrawIn(0);

	  return false;
	};
    };
  return false;
}

void 
CDraw::WriteImgToFile (String filename)
{
//FIXME
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}
