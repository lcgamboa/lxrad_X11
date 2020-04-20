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
#include"../include/ctimer.h"
#include"../include/capplication.h"
#include <sys/time.h>
#include<unistd.h>

#ifdef HAVE_LIBPTHREAD
#include<pthread.h>
//extern pthread_mutex_t Display_Lock;
#endif

// CTimer___________________________________________________________

CTimer::CTimer (void)
{
  CanFocus = false;
  SetX (10);
  SetY (10);
  SetWidth (10);
  SetHeight (10);
  SetClass ("CTimer");
  Time = 1;
  OverTime=0;
  Run = false;
  EvOnTime = NULL;
  SetVisible (false);
  CanVisible = false;
}

CTimer::~CTimer (void)
{
}

int
CTimer::Create (CControl * control)
{
  return CControl::Create (control);
}

void
CTimer::Destroy (void)
{
  SetRunState (false);
  CControl::Destroy ();
};

void
CTimer::Draw (void)
{
  /*	
  if ((!Visible)||(Paint == NULL))
    return;
  Paint->InitDraw (this);
  Paint->Pen.SetColor (Color);
  Paint->Rectangle ( 2, 2, Width - 4, Height - 4);
  Paint->Pen.SetColor (ColorByName ("black"));
  Paint->LowerFrame ( 0, 0, Width, Height);
  Paint->Text ( 10, 10, "Timer");
  CControl::Draw ();
  */
};

CStringList CTimer::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("Time=" + itoa (GetTime ()) + ";int");
  Context.AddLine ("RunState=" + itoa (GetRunState ()) + ";int");
  Context.AddLine ("OnTime=" + btoa (GetEv ()) + ";event");
  return Context;
};

void
CTimer::SetContext (CStringList context)
{
  Erase ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      String line = Context.GetLine (i);
      String arg;
      eqparse (line, arg);
      if (line.compare ("Time") == 0)
	SetTime (atoi (arg));
      if (line.compare ("RunState") == 0)
	SetRunState (atoi (arg));
      if (line.compare ("OnTime") == 0)
	SetEv (atob (arg));
    };
  Draw ();
};

#ifdef HAVE_LIBPTHREAD	

void *
thread1 (void *arg)
{
  struct timeval  tv1, tv2;
  unsigned long tused;
  pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS,NULL);  
  CTimer *timer = (CTimer *) arg;
 
  //wait for display
  
  Display * disp;
 
  usleep (timer->GetTime ()*1000);
  for (;;)
  {
      disp=Application->GetADisplay();
      if(disp)
      {
        gettimeofday(&tv1, NULL);
        //pthread_mutex_lock (&Display_Lock);
        XLockDisplay(disp);
        timer->on_time ();
       	XUnlockDisplay(disp);   
        //pthread_mutex_unlock (&Display_Lock);
        gettimeofday(&tv2, NULL);	
        tused=((tv2.tv_usec - tv1.tv_usec) + 1000000L*(tv2.tv_sec - tv1.tv_sec));
        
        if((timer->GetTime ()*1000) > tused)
          usleep((timer->GetTime ()*1000) - tused);
	else
          usleep((timer->GetTime ()*1000));
      }
      else
      {
        usleep(10000);
      }
    };
};
#endif


//propiedades

void
CTimer::SetTime (uint time)
{
  Time = time;
};

uint
CTimer::GetTime (void)
{
  return Time;
};

void
CTimer::SetOverTime (uint overtime)
{
  OverTime = overtime;
};

uint
CTimer::GetOverTime (void)
{
  return OverTime;
};

void
CTimer::SetRunState (bool run)
{
  if (Run != run)
    {
      if (run)
      {
#ifdef HAVE_LIBPTHREAD	
	  pthread_create (&Th, NULL, thread1, (void *) this);
#else
	  Application->AddTimer(this);
#endif  
      }
      else
	{
#ifdef HAVE_LIBPTHREAD	
	  pthread_cancel (Th);
	  pthread_join (Th, NULL);
#else
	  Application->RemoveTimer(this);
#endif  
	};
      Run = run;
    };
};

bool
CTimer::GetRunState (void)
{
  return Run;
};

void
CTimer::on_time (void)
{
  if ((FOwner) && (EvOnTime))
    (FOwner->*EvOnTime) (this);
};
