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
#include"../include/capplication.h"

#include<time.h>
#include<unistd.h>

#include<pthread.h>
//pthread_mutex_t Display_Lock;

#ifdef HAVE_LIBIMLIB2
#include<Imlib2.h>
#endif

// CApplication__________________________________________________________


CApplication::CApplication (void)
{
  Title = "Program";
  AWindowCount = -1;
  AWindowList = NULL;
  Tag = 0;
  PixelsCount = -1;
  ColorTable = NULL;
  Exit = false;

  ADisplay=NULL;
  HintControl=NULL;
  HintTime=time(NULL);
  HintX=0;
  HintY=0;
  //pthread_mutex_init (&Display_Lock,NULL);
  //pthread_mutex_lock (&Display_Lock);
};

CApplication::~CApplication (void)
{
  PixelsCount = -1;
  if (ColorTable)
    delete[]ColorTable;
  ColorTable = NULL;
  //pthread_mutex_destroy (&Display_Lock);  
};

void
CApplication::Start (void)
{
  Display *adisplay=NULL;	
  
  XInitThreads();
   
  eprint("Application init ...\n");
  for (int i = 0; i != Aargc; i++)
    {
      if ((!strcmp (Aargv[i], "-display")) && ((i + 1) < Aargc))
	{
	  adisplay = XOpenDisplay (Aargv[i + 1]);
	  if (adisplay)
	    break;
	  else
	    {
	      eprint("Error!: Can't open  Display \n");
	      eprint("...Application Finished\n");
	      Exit = true;
	      return;

	    };
	};
    };
  if (adisplay == NULL)
    {
      adisplay = XOpenDisplay (NULL);
    };
  if (adisplay == NULL)
    {
      eprint("Error!: Can't open  Display \n");
      eprint("...Application Finished\n");
      Exit = true;
      return;
    };

  XLockDisplay(adisplay);

  ADisplay=adisplay;
  AScreen = XDefaultScreenOfDisplay (ADisplay);
  ADepth = XDefaultDepthOfScreen (AScreen);
  ABlackColor = BlackPixel (ADisplay, DefaultScreen (ADisplay));
  AWhiteColor = WhitePixel (ADisplay, DefaultScreen (ADisplay));
  ADefaultRootWindow = DefaultRootWindow (ADisplay);
  AWMDeleteWindow = XInternAtom (ADisplay, "WM_DELETE_WINDOW", false);
  AWMTakeFocus = XInternAtom (ADisplay, "WM_TAKE_FOCUS", false);
  AWMProtocols = XInternAtom (ADisplay, "WM_PROTOCOLS", false);


#ifdef _DEBUG
  eprint("synchronize\n");
  XSynchronize (ADisplay, true);
#else
  if (setlocale (LC_CTYPE, "") == NULL)
    {
      eprint("Error : setlocale() !\n");
      Exit = true;
      return;
    };
  if ((IM = XOpenIM (ADisplay, NULL, NULL, NULL)) == NULL)
    {
      eprint("Cannot use XOpenIM() !\n");
    };
#endif


#ifdef HAVE_LIBIMLIB2
   imlib_set_cache_size(2048 * 1024);
   imlib_context_set_display(ADisplay);
   imlib_context_set_visual(DefaultVisual(ADisplay, DefaultScreen(ADisplay)));
   imlib_context_set_colormap(DefaultColormap(ADisplay, DefaultScreen(ADisplay)));
#endif

};

void
CApplication::ACreateWindow (CWindow * AWindow,CWindow *window)
{
  if (Exit)
    {
      AWindow->CControl::Destroy ();
      return;
    };
  if (AWindowCount == -1)
    AWindow->SetVisible (true,false);
  AWindow->WCreate (window);
  AWindowCount++;
  CWindow **WindowList;
  WindowList = new CWindow *[AWindowCount + 1];
  for (int c = 0; c < AWindowCount; c++)
    WindowList[c] = AWindowList[c];
  WindowList[AWindowCount] = AWindow;
  if (AWindowList)
    delete[]AWindowList;
  AWindowList = WindowList;
  AWindow->Draw ();
};

void
CApplication::Draw (void)
{
  if (AWindowCount < 0)
    return;
  else
    for (int i = 0; i <= AWindowCount; i++)
      AWindowList[i]->Draw ();
};

void
CApplication::Update (void)
{
  if (AWindowCount < 0)
    return;
  else
    for (int i = 0; i <= AWindowCount; i++)
      AWindowList[i]->Update ();
};

void
CApplication::ADestroyWindow (CWindow * AWindow)
{
  if (AWindowCount >= 0)
    {
      int wn = 0;
      if (AWindowList[0]->GetWWindow () == AWindow->GetWWindow ())
	{
	  if (AWindowList[0]->GetDynamic ())
	    delete AWindowList[0];
	  for (int r = AWindowCount; r > 0; r--)
	    {
	      AWindowList[r]->SetCanDestroy (true);
	      AWindowList[r]->WDestroy ();
	    };
	  delete[]AWindowList;
	  AWindowList = NULL;
	  eprint("...Application Finished\n");
	  if (IM)
	    XCloseIM (IM);
#ifdef _DEBUG
	  eprint("synchronize\n");
	  XSynchronize (ADisplay, false);
#endif
          XFlush (ADisplay);
          XSetCloseDownMode (ADisplay, DestroyAll);
	  XCloseDisplay (ADisplay);
	  ADisplay=NULL;
	  Exit = true;
	  return;
	}
      else
	{
	  for (int f = 0; f <= AWindowCount; f++)
	    if (AWindowList[f] == AWindow)
	      wn = f;
	  if (wn != 0)
	    {
	      if (AWindowList[wn]->GetDynamic ())
		delete AWindowList[wn];
	      for (int c = wn; c < AWindowCount; c++)
		AWindowList[c] = AWindowList[c + 1];
	      AWindowList[AWindowCount] = NULL;
	      AWindowCount--;
	    };
	};
    };
};


bool
CApplication::ProcessEvents (CWindow * AWindow)
{
  if (AWindow != NULL)
    {
      int wn = -1;
      int ec;
      
     //wait hint loop	    
     ec=XEventsQueued(ADisplay,QueuedAlready );
     while(ec ==  0 )
     {
	usleep(50000);
	ec=XEventsQueued(ADisplay,QueuedAfterFlush );
	if((HintControl)&&(time(NULL)-HintTime > 1))
	{
          if(HintControl->GetHint().size() >0)
	  {
	  WHint(HintControl->GetHint(),
    	    HintX+HintControl->GetWin()->GetX(),
     	    HintY+HintControl->GetWin()->GetY());
	  };
	  HintControl=NULL;
        }
     };
      HintControl=NULL;
      
      
      XNextEvent (ADisplay, &AEvent);

      if (IM)
	if (XFilterEvent (&AEvent, 0) == true)
	  return false;
      
      FWindow = AEvent.xany.window;
      for (int e = 0; e <= AWindowCount; e++)
	{
	  if (FWindow == AWindowList[e]->GetWWindow ())
	    {
	      wn = e;
	      break;
	    };
	};

      if (((AEvent.type == EnterNotify) || (AEvent.type == Expose))
	  && (AWindowCount >= 0) && (wn >= 0))
	{
	  AWindowList[wn]->WEvents (AEvent);
	}
      else
       {
      if (AEvent.xany.window == AWindow->GetWWindow ())
	{
           if (AEvent.type != DestroyNotify)
	    {
	      AWindow->WEvents (AEvent);
	      return true;
	    }
	  else
	    return false;
	}
        else
	{
           if (AEvent.type == MotionNotify)
	    {
	      AWindow->WEvents (AEvent);
	      return true;
	    }
	};
		
       };
    };
  return false;
};




void
CApplication::Load (void)
{
  if (Exit)
    return;
  
  XUnlockDisplay(ADisplay);
  //pthread_mutex_unlock (&Display_Lock);

  if (AWindowCount == -1)
    {
      eprint("No Windows!\n");
      eprint("...Application Finished\n");
      if (IM)
	XCloseIM (IM);
#ifdef _DEBUG
      eprint("synchronize\n");
      XSynchronize (ADisplay, false);
#endif
      XFlush (ADisplay);
      XSetCloseDownMode (ADisplay, DestroyAll);
      XCloseDisplay (ADisplay);
      return;
    };

  int wn = 0;
  int ec;   //events in queue

  FWindow = AWindowList[wn]->GetWWindow ();
  for (; AWindowList != NULL;)
    {

     //wait hint loop	    
     ec=XEventsQueued(ADisplay,QueuedAlready );
     while(ec ==  0 )
     {
	usleep(50000);
	ec=XEventsQueued(ADisplay,QueuedAfterFlush );
	if((HintControl)&&(time(NULL)-HintTime > 1))
	{
          if(HintControl->GetHint().size() >0)
	  {
	  WHint(HintControl->GetHint(),
    	    HintX+HintControl->GetWin()->GetX(),
     	    HintY+HintControl->GetWin()->GetY());
	  };
	  HintControl=NULL;
        }
     };
      HintControl=NULL;
      

      XNextEvent (ADisplay, &AEvent);
      
      
      if (IM)
	if (XFilterEvent (&AEvent, 0) == true)  continue;

      
      FWindow = AEvent.xany.window;

      wn = -1;
      for (int e = 0; e <= AWindowCount; e++)
	if (FWindow == AWindowList[e]->GetWWindow ())
	  {
	    wn = e;
	    break;
	  };
      if (wn >= 0)
	AWindowList[wn]->WEvents (AEvent);

      if (AEvent.type == DestroyNotify)
	for (int p = 0; p <= AWindowCount; p++)
	  if (AEvent.xdestroywindow.window == AWindowList[p]->GetWWindow ())
	    {
	      ADestroyWindow (AWindowList[p]);
	      wn = -1;
	      //Update ();
	      if (Exit)
		return;
	      break;
	    };
      
    };
};

Display *
CApplication::GetADisplay (void)
{
  return ADisplay;
};

Screen *
CApplication::GetAScreen (void)
{
  return AScreen;
};

bool
CApplication::GetExit (void)
{
  return Exit;
};

//propierties

int
CApplication::GetAWindowCount (void)
{
  return AWindowCount;
};

CWindow *
CApplication::GetAWindow (uint window)
{
  return AWindowList[window];
};

int
CApplication::GetTag ()
{
  return Tag;
};

void
CApplication::SetTag (int x)
{
  Tag = x;
};

String CApplication::GetTitle (void)
{
  return Title;
};

Window *
CApplication::GetADefaultRootWindow (void)
{
  return &ADefaultRootWindow;
};

int *
CApplication::GetADepth (void)
{
  return &ADepth;
};

unsigned int *
CApplication::GetAWhiteColor (void)
{
  return &AWhiteColor;
};

unsigned int *
CApplication::GetABlackColor (void)
{
  return &ABlackColor;
};

Atom *
CApplication::GetAWMProtocols (void)
{
  return &AWMProtocols;
};

Atom *
CApplication::GetAWMDeleteWindow (void)
{
  return &AWMDeleteWindow;
};

Atom *
CApplication::GetAWMTakeFocus (void)
{
  return &AWMTakeFocus;
};

XIM CApplication::GetIM (void)
{
  return IM;
};

/*
void
CApplication::OnHintTime(CControl* control)
{
  if((HTimer.GetTag()==2)&&(HintControl)&&(HintControl->GetHint().size()>0))
  {
    	  
    //WHint(HintControl->GetHint(),
    //	  HTimer.GetX()+HintControl->GetWin()->GetX(),
    // 	  HTimer.GetY()+HintControl->GetWin()->GetY());
   
    XClientMessageEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.send_event=1;
    xev.display=ADisplay;
    xev.window=FWindow;
    xev.message_type = AShowHint;
    xev.format=32;
    xev.data.s[0] = CurrentTime; 
    
    //XSendEvent(ADisplay, FWindow, false,0l, (XEvent *) &xev);
    printf("%s\n",HintControl->GetHint().c_str());	  
    HTimer.SetTag(0);   
    HintControl=NULL;
  };

  if( HTimer.GetTag() == 1 )
    HTimer.SetTag(2);   
  else  
    HTimer.SetTag(0);   
  
};
*/


void 
CApplication::SetHintControl(CControl* hcontrol,int x,int y)
{
  HintControl=hcontrol;
  HintTime=time(NULL);
  HintX=x;
  HintY=y;
};

void
CApplication::AddToColorTable (String colorname, XColor color,
			       XColor displaycolor)
{
  PixelsCount++;
  TXColor *TempTable;
  TempTable = new TXColor[PixelsCount + 1];

  for (int c = 0; c < PixelsCount; c++)
    TempTable[c] = ColorTable[c];

  TempTable[PixelsCount].name = colorname;
  TempTable[PixelsCount].color = color;
  TempTable[PixelsCount].displaycolor = displaycolor;
  if (ColorTable)
    delete[]ColorTable;
  ColorTable = TempTable;
};

bool CApplication::XSearchInColorTable (String name, XColor * color)
{
  for (int c = 0; c <= PixelsCount; c++)
    if (ColorTable[c].name.compare (name) == 0)
      {
	*color = ColorTable[c].color;
	return true;
      };

  return false;
};

bool CApplication::XSearchInColorTable (XColor * color)
{
  for (int c = 0; c <= PixelsCount; c++)
    if ((ColorTable[c].color.red == color->red)
	&& (ColorTable[c].color.green == color->green)
	&& (ColorTable[c].color.blue == color->blue))
      {
	*color = ColorTable[c].displaycolor;
	return true;
      };
  return false;
};

bool
CApplication::ProcessEvents (void)
{
//FIXME
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
return 0;
}
