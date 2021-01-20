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


#ifdef HAVE_LIBIMLIB2
#include<Imlib2.h>
#endif

// CApplication__________________________________________________________

CApplication::CApplication (void)
{
 Title = "Program";
 AWindowCount = -1;
 AWindowList = NULL;
 TimerCount = -1;
 TimerList = NULL;
 ThreadCount = -1;
 ThreadList = NULL;
 Tag = 0;
  PixelsCount = -1;
  ColorTable = NULL;
  Exit = false;

  IM=NULL;
  ADisplay=NULL;
 HintControl = NULL;
 HintTime = time(NULL);
 HintX = 0;
 HintY = 0;
}

CApplication::~CApplication (void)
{
  PixelsCount = -1;
  if (ColorTable)
    delete[]ColorTable;
  ColorTable = NULL;
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
     eprint ("...Application Finished\n");
	  if (IM)
	  {
	    XCloseIM (IM);
	    IM=NULL;
	  }
#ifdef _DEBUG
	  eprint("synchronize\n");
	  XSynchronize (ADisplay, false);
#endif
          XFlush (ADisplay);
          XSetCloseDownMode (ADisplay, DestroyAll);
	  XCloseDisplay (ADisplay);
	  ADisplay=NULL;
          Exit = true;
	  exit(0); //avoid thread and timers crash
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
  if (Exit)return;
  
  XUnlockDisplay(ADisplay);

 if (AWindowCount == -1)
    {
     eprint ("No Windows!\n");
     eprint ("...Application Finished\n");
     if (IM)
      {
	XCloseIM (IM);
        IM=NULL;
      }
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
  FWindow = AWindowList[wn]->GetWWindow ();
  for (; AWindowList != NULL;)
    {
     ProcessEvents ();
    }
}

bool
CApplication::ProcessEvents (void)
{
  int wn = 0;
  int ec;   //events in queue
 
 struct timeval tv;
 long int elapsed;
 static int trun=0;

   if((!trun)&&(!Exit))
   {
     trun = 1;	   

   gettimeofday (&tv, NULL);
   //printf("---------------------\n");
   for (int t = 0; t <= TimerCount; t++)
    {
     elapsed = (((tv.tv_usec - TimerList[t]->tv.tv_usec) / 1000L) + 1000L * (tv.tv_sec - TimerList[t]->tv.tv_sec));

     //printf("Elapsed %i = %lu de %lu\n",t,elapsed,TimerList[t]->GetTime());
     if (elapsed >= TimerList[t]->GetTime ())
      {
       //printf("===>>Timer %i reseted\n",t);	       
       TimerList[t]->tv = tv;
       TimerList[t]->SetOverTime (elapsed - TimerList[t]->GetTime ());
       TimerList[t]->on_time ();
      }
    }

#ifndef HAVE_LIBPTHREAD
     for (int t = 0; t <= ThreadCount; t++)
     {
      ThreadList[t]->on_run ();
     }
#endif

     trun =0;
   }


     while(1){
     //wait hint loop	    
     //ec=XEventsQueued(ADisplay,QueuedAlready );
     if(!ADisplay)return 1;
     ec=XEventsQueued(ADisplay,QueuedAfterFlush );
     if(ec ==  0 )
     {
	usleep(1000); //idle 
	//ec=XEventsQueued(ADisplay,QueuedAfterFlush );
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
        return false;
     }
     
     HintControl=NULL;
      

      XNextEvent (ADisplay, &AEvent);
      
      
      if (IM)
	if (XFilterEvent (&AEvent, 0) == true)  return 0;

      
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
		return 1;
	      break;
	    };
     }
  return 0;
}

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

//properties

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

lxString
CApplication::GetTitle (void)
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
CApplication::SetHintControl (CControl* hcontrol, int x, int y)
{
 HintControl = hcontrol;
 HintTime = time (NULL);
 HintX = x;
 HintY = y;
};

void
CApplication::AddToColorTable (lxString colorname, XColor color,
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

bool CApplication::XSearchInColorTable (lxString name, XColor * color)
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




void
CApplication::AddTimer (CTimer * tm)
{
 TimerCount++;
 CTimer **TList;
 TList = new CTimer *[TimerCount + 1];
 for (int c = 0; c < TimerCount; c++)
  TList[c] = TimerList[c];
 TList[TimerCount] = tm;
 if (TimerList)
  delete[]TimerList;
 TimerList = TList;
 gettimeofday (&tm->tv, NULL);
 //printf("Timer %i added: %s\n",TimerCount,tm->GetName().c_str()); 
}

void
CApplication::RemoveTimer (CTimer *tm)
{
 if (TimerCount >= 0)
  {
   int n = -1;
   for (int f = 0; f <= TimerCount; f++)
    if (TimerList[f] == tm)
     n = f;
   if (n != -1)
    {
     for (int c = n; c < TimerCount; c++)
      TimerList[c] = TimerList[c + 1];
     TimerList[TimerCount] = NULL;
     TimerCount--;
     if(TimerCount == -1)
     {
       if (TimerList)
         delete[]TimerList;
     }
    }
   //printf("Timer %i Removed: %s\n",TimerCount,tm->GetName().c_str()); 
  }
}

#ifndef HAVE_LIBPTHREAD

void
CApplication::AddThread (CThread * td)
{
 ThreadCount++;
 CThread **TList;
 TList = new CThread *[ThreadCount + 1];
 for (int c = 0; c < ThreadCount; c++)
  TList[c] = ThreadList[c];
 TList[ThreadCount] = td;
 if (ThreadList)
  delete[]ThreadList;
 ThreadList = TList;
}

void
CApplication::RemoveThread (CThread *td)
{
 if (ThreadCount >= 0)
  {
   int n = -1;
   for (int f = 0; f <= ThreadCount; f++)
    if (ThreadList[f] == td)
     n = f;
   if (n != -1)
    {
     for (int c = n; c < ThreadCount; c++)
      ThreadList[c] = ThreadList[c + 1];
     ThreadList[ThreadCount] = NULL;
     ThreadCount--;
     if(ThreadCount == -1)
     {
       if (ThreadList)
         delete[]ThreadList;
     }
    }
  }
}
#endif
