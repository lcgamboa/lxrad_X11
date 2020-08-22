/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2015-2018  Luis Claudio Gamboa Lopes

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



#include"../include/cthread.h"
#include"../include/capplication.h"
#include"../config.h"
#include<unistd.h>

#ifdef HAVE_LIBPTHREAD
#include<pthread.h>
#endif

CThread::CThread (void)
{
  CanFocus = false;
  CanVisible = false;
  SetClass (lxT("CThread"));
  runstate=false;
  tdestroy=false;
  SetVisible (false);
  EvThreadRun=NULL;
  EvThreadEnd=NULL;
}

CThread::~CThread (void)
{
}

int
CThread::Create (CControl * control)
{
  Win = control->GetWin ();

  CanExecuteEvent=false;
  int ret=CControl::Create (control);
  CanExecuteEvent=true;
  return ret;
}

void 
CThread::Destroy (void)
{
#ifdef HAVE_LIBPTHREAD
  tdestroy=1;
  while(runstate) 
  {
      usleep(100);
  }      
  pthread_join (Thread, NULL);
#else
  Application->RemoveThread(this);
  on_end();
#endif
  CControl::Destroy ();
}

void 
CThread::Kill (void)
{
#ifdef HAVE_LIBPTHREAD
   pthread_cancel (Thread);
   pthread_join (Thread, NULL);
#else
   Application->RemoveThread(this);
   on_end();
#endif
}

bool 
CThread::TestDestroy (void)
{
return tdestroy;
}

lxStringList 
CThread::GetContext (void)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
  CObject::GetContext ();
  //Context.AddLine (xml_out (lxT("EvThreadRun"), lxT("Event"), btoa (GetEv (true))));
  //Context.AddLine (xml_out (lxT("EvThreadEnd"), lxT("Event"), btoa (GetEv ())));
  return Context;
}

void 
CThread::SetContext (lxStringList context)
{
  lxString name, type, value;

//  CControl::SetContext (context);
  CObject::SetContext (context);
  /*
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      xml_in (Context.GetLine (i), name, type, value);
      if (name.compare (wxT("EvThreadRun")) == 0)
	SetEv (atob (value),true);
      if (name.compare (wxT("EvThreadEnd")) == 0)
	SetEv (atob (value),false);

    };
  */  
}

#ifdef HAVE_LIBPTHREAD
void *
cthread1 (void *arg)
{
  pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS,NULL);  
  CThread *th = (CThread *) arg;

  th->on_run();
  
  th->on_end();

  return NULL;
};
#endif

int  
CThread::Run (void)
{
  if ((FOwner) && (EvThreadRun))
     { 
       if(!runstate)
       {
       //create the thread
         runstate=1;
#ifdef HAVE_LIBPTHREAD
	 tdestroy=0;
	 pthread_create (&Thread, NULL, cthread1, (void *) this);
#else
	 tdestroy=1;
	 Application->AddThread(this);
#endif
	 return 0;  
       }
       else
        return 1;
    }

   return 1;
}

bool 
CThread::GetRunState (void)
{
 return runstate;	
}

void 
CThread::SetRunState (bool rs)
{
  runstate=rs;	
}

void 
CThread::SetName (const lxString name)
{
  Name = name;
}

void
CThread::on_run (void)
{
  runstate=true;
  if ((FOwner) && (EvThreadRun))
    (FOwner->*EvThreadRun) (this);
}

void
CThread::on_end (void)
{
  runstate=false;
  if ((FOwner) && (EvThreadEnd))
    (FOwner->*EvThreadEnd) (this);
}
