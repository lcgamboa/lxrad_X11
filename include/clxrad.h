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

#ifndef CLXRAD
#define CLXRAD


#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#define NO_STL


#ifndef NO_STL
//c++ headers
using namespace std;
#include<iostream>
#include<fstream>
#include<string>
#include<list>
#endif

//c headers
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

//X11 headers
#include<X11/Xlib.h>
#include<X11/Xatom.h>
#include<X11/Xutil.h>
#include<X11/keysym.h>

#include"lxutils.h"

#define Initialize int pmain(void); CApplication application; CApplication *Application=&application;\
	int main(int argc,char **argv){\
	application.Aargc=argc;application.Aargv=argv; pmain();}\
        int pmain(void)


#define AllEventMask KeyPressMask | KeyReleaseMask |KeymapStateMask | ButtonPressMask | ButtonReleaseMask |\
                     ButtonMotionMask| EnterWindowMask | LeaveWindowMask | ExposureMask | \
		     StructureNotifyMask | FocusChangeMask | PointerMotionMask

#define Parent 99999
#define EVMAX 100
#define DOUBLECLICKTIME 400

#define EVMOUSEMOVE              (void(CControl::*)(CControl*,const uint,const uint,const uint,const uint))
#define EVMOUSEBUTTONPRESS       (void(CControl::*)(CControl*,const uint,const uint,const uint,const uint))
#define EVMOUSEBUTTONRELEASE     (void(CControl::*)(CControl*,const uint,const uint,const uint,const uint))
#define EVMOUSEBUTTONCLICK       (void(CControl::*)(CControl*,const uint,const uint,const uint,const uint))
#define EVMOUSEBUTTONDOUBLECLICK (void(CControl::*)(CControl*,const uint,const uint,const uint,const uint))
#define EVKEYBOARDPRESS          (void(CControl::*)(CControl*,const uint,const uint,const uint))
#define EVKEYBOARDRELEASE        (void(CControl::*)(CControl*,const uint,const uint,const uint))
#define EVPOINTERIN              (void(CControl::*)(CControl*))
#define EVPOINTEROUT             (void(CControl::*)(CControl*))
#define EVONDRAW                 (void(CControl::*)(CControl*))
#define EVONFOCUSIN              (void(CControl::*)(CControl*))
#define EVONFOCUSOUT             (void(CControl::*)(CControl*))

#define EVONCHECKBOX            (void(CControl::*)(CControl*))
#define EVMENUACTIVE            (void(CControl::*)(CControl*))
#define EVONTIME                (void(CControl::*)(CControl*))
#define EVONTOGGLEBUTTON        (void(CControl::*)(CControl*))
#define EVTHREADRUN     	(void(CControl::*)(CControl*))
#define EVTHREADEND	        (void(CControl::*)(CControl*))
#define EVONCHANGESPINDOUBLE    (void(CControl::*)(CControl*))
#define EVONCLOSE        	(void(CControl::*)(const int))

typedef unsigned int uint;

#define CA_LEFT   1
#define CA_RIGHT  2
#define CA_CENTER 3
#define CA_FILL   4
typedef int CAlign;

class CApplication;
extern CApplication *Application;

#endif
