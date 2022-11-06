/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001  Luis Claudio Gamboa Lopes

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

#include<stdio.h>

#include"../include/cwindow.h"
#include"../include/capplication.h"
#include"../include/newcontrolbycname.h"


void
XFreeTextProperty (XTextProperty & textp)
{
  if (textp.nitems > 0)
    XFree(textp.value);
  textp.value = NULL;
  textp.encoding = 0;
  textp.format = 0;
  textp.nitems = 0;
}

//CWindow _______________________________________________________________

CWindow::CWindow (void)
{
  WWindow = 0;
  ADisplay = NULL;
  AScreen = NULL;
  SetClass ("CWindow");
  Win = NULL;
  PixmapBuffer=true;
  HasMenu=False;
  //  PixmapBuffer=false;
  XMouse = 0;
  YMouse = 0;
  IC = 0;
  SetX (0);
  SetY (0);
  SetWidth (300);
  SetHeight (200);
  PWidth=0;
  PHeight=0;
  SetBorder (1);
  Visible = 1;
  Title = "LXRAD Window";
  WParent=NULL;
  WPixmap = 0;
  WSizeHints.flags = PPosition | PSize | PMinSize | PMaxSize;
  WSizeHints.x = GetX ();
  WSizeHints.y = GetY ();
  WSizeHints.width = GetWidth ();
  WSizeHints.height = GetHeight ();
  WSizeHints.min_width = 10;
  WSizeHints.min_height = 10;
  WWMHints.initial_state = NormalState;
  WWMHints.input = True;
  WWMHints.flags = StateHint | InputHint;
  WClassHints.res_class = (char *)"LXRAD";
  WAttributes.event_mask = AllEventMask;
  WAttributes.save_under = false;
  WAttributes.override_redirect = false;	//With WM 
  WAttributes.colormap = CopyFromParent;

  WAttributes.backing_store = NotUseful;  //WhenMapped,NotUseful,Always

  ControlOnFocus = NULL;
  LastControl = NULL;
//events
  EvOnCreate = NULL;
  EvOnDestroy = NULL;
  EvOnShow = NULL;
  EvOnHide = NULL;
  EvOnEnter = NULL;
  EvOnLeave = NULL;

  char *title;
  title = new char[Title.size () + 1];
  strcpy (title, Title.c_str ());
  if (XStringListToTextProperty (&title, 1, &WTextProperty) == 0)
    eprint( "erro\n");
  delete[]title;
}

CWindow::~CWindow (void)
{
  XFreeTextProperty (WTextProperty);
}

void
CWindow::WCreate (CWindow* window)
{
  ADisplay = Application->GetADisplay ();
  AScreen = Application->GetAScreen ();
  ADepth = Application->GetADepth ();
  ADefaultRootWindow = Application->GetADefaultRootWindow ();
  ABlackColor = Application->GetABlackColor ();
  AWhiteColor = Application->GetAWhiteColor ();
  AWMProtocols = Application->GetAWMProtocols ();
  AWMDeleteWindow = Application->GetAWMDeleteWindow ();
  AWMTakeFocus = Application->GetAWMTakeFocus ();

  WSizeHints.max_width = AScreen->width;
  WSizeHints.max_height = AScreen->height;
  
  //Window parent 
  Window parent; 
  if(window != NULL)
  {
    parent=window->GetWWindow();
    WParent=window;
  }
  else
    parent=*ADefaultRootWindow;
  
  //window colors
  Color.pixel = *AWhiteColor;
  BorderColor = *ABlackColor;
  WAttributes.background_pixel = ColorByName ("light gray").pixel;

  WWindow = XCreateWindow (ADisplay,	//display
			   parent,	//parent
			   X,	//x
			   Y,	//y
			   Width,	//width
			   Height,	//height
			   GetBorder (),	//border_width
			   CopyFromParent,	//depth
			   InputOutput,	//class
			   CopyFromParent,	//visual
			   (CWBackPixel | CWEventMask | CWSaveUnder | CWOverrideRedirect |CWColormap |CWBackingStore),	//value_mask
			   &WAttributes);	//attributes
  
  
  uint FEvent = 0;
  if (Application->GetIM () != NULL)
    {
      IC = XCreateIC (Application->GetIM (), XNInputStyle,
		      XIMPreeditNothing | XIMStatusNothing, XNClientWindow,
		      WWindow, NULL);
      if (IC == NULL)
	{
	  eprint( "Error : XCreateIC() !\n");
	  exit (0);
	}
      else
	XGetICValues (IC, XNFilterEvents, &FEvent, NULL);
    }

  WAttributes.event_mask = AllEventMask | FEvent;

  char *title;
  title = new char[Title.size () + 1];
  strcpy (title, Title.c_str ());
  WClassHints.res_name = title;

  XSetWMProperties (ADisplay,	//display
		    WWindow,	//window
		    &WTextProperty,	//window_name
		    &WTextProperty,	//icon-name
		    Application->Aargv,	//valor dos argumentos
		    Application->Aargc,	//numero de argumentos
		    &WSizeHints,	//normal_hints
		    &WWMHints,	//wm_hints
		    &WClassHints);	//class_hints

  Win = this;
  CreatePixmap(); 
  WPaint.Create (this);
  Paint = &WPaint;
  
  Create (this);
  Draw();
  
  on_create ();
  
  if(Visible) Show ();
  
  XMoveWindow (ADisplay, WWindow, X, Y);
  
  Atom AWMWindowProtocol[2];
  AWMWindowProtocol[0] = *AWMDeleteWindow;
  AWMWindowProtocol[1] = *AWMTakeFocus;
  XSetWMProtocols (ADisplay, WWindow, AWMWindowProtocol, 2);
  //XFlush (ADisplay);
  
  delete[]WClassHints.res_name;
}

void
CWindow::Destroy (void)
{
  CControl::Destroy ();
  DestroyPixmap();
}


void
CWindow::Draw(void)
{
  if(Paint == NULL)return;
  Paint->InitDraw(this);
  Paint->Pen.SetColor(Color);
  Paint->Rectangle ( 0, 0, Width, Height);
  CControl::Draw ();
}

void
CWindow::SetPixmapBuffer(bool pbuffer)
{
  PixmapBuffer=pbuffer;
}

bool
CWindow::GetPixmapBuffer(void)
{
  return PixmapBuffer;
}

Pixmap 
CWindow::GetPixmap(void)
{
  return WPixmap;
}

void
CWindow::DestroyPixmap (void)
{
  if ((WPixmap != 0)&&(WPixmap != WWindow))
    {
      XFreePixmap (ADisplay , WPixmap);
      WPixmap=0;
    }
}

void
CWindow::CreatePixmap(bool draw)
{
  if(PixmapBuffer)
  {
  if((Width > PWidth) || ( Height > PHeight))
  {
  DestroyPixmap();	
  WPixmap=XCreatePixmap(ADisplay , WWindow ,Width , Height ,*ADepth);
  WPaint.SetDrawIn(WPixmap);
  if(Width > PWidth)PWidth=Width;
  if(Height > PHeight)PHeight=Height;
  if(draw) Draw();
  }
  }
  else
   WPixmap=WWindow;
}



void
CWindow::DestroyChild (CControl * control)
{
  if (control == ControlOnFocus)
    ControlOnFocus = NULL;
  CControl::DestroyChild (control);
}

void
CWindow::DestroyChilds (void)
{
  ControlOnFocus = NULL;
  CControl::DestroyChilds ();
}


void
CWindow::WDestroy (void)
{
  Hide ();
  on_destroy ();
  if ((CanDestroy) || (this == Application->GetAWindow ((uint) 0)))
    {
      Destroy ();
      WPaint.Destroy ();
      if(WParent != NULL)
      {
        XDestroyWindow (ADisplay, GetWWindow ());
      }
      if (IC)
	XDestroyIC (IC);
      Win = NULL;
      Application->ADestroyWindow (this);
      WWindow = 0;
    }
}

void
CWindow::Show (void)
{
  if(Win != NULL)
    {
      SetVisible (true);
      Draw();
      XMapWindow (ADisplay, GetWWindow ());
      XFlush(ADisplay);  
      Update ();
    }
}

void
CWindow::ShowExclusive (void)
{
  Show ();
  CanExitExclusive = true;
  while (CanExitExclusive)
    Application->ProcessEvents (this);
}

void
CWindow::Hide (void)
{
  if (Win != NULL)
    {
      XUnmapWindow (ADisplay, GetWWindow ());
      SetVisible (false);
    }
}

void
CWindow::HideExclusive (void)
{
  CanExitExclusive = false;
  Hide ();
}

void
CWindow::Update (void)
{
  /*	
  for (int i = 0; i <= ChildCount; i++)
    {
      if (Child[i]->GetVisible ())
	{
	  if(PixmapBuffer)	
	    Child[i]->Update ();
	  else
	    Child[i]->Draw ();
	}
    }
  */
  CControl::Update();
}

void
CWindow::Update (Region Reg)
{
  if (ChildCount == -1)
    return;
  else
    {
      
      XSetRegion (GetADisplay (), WPaint.Agc, Reg);
      
      for (int i = 0; i <= ChildCount; i++)
	{
	  if (Child[i]->GetVisible ())
	    {
	      XRectangle rec=Child[i]->GetRectangle();	    
	      if (XRectInRegion (Reg,rec.x,rec.y,rec.width,rec.height) != RectangleOut)
		{
		  if(PixmapBuffer)	
		    Child[i]->Update ();
		  else
		    Child[i]->Draw ();
		}
	    }
	}

      XSetClipMask (GetADisplay (), WPaint.Agc, None);
      XDestroyRegion (Reg);
    }
}

void
CWindow::SetOverrideRedirect (bool redirect)
{
  WAttributes.override_redirect = redirect;
}

void
CWindow::SetSaveUnder (bool saveunder)
{
  WAttributes.save_under = saveunder;
}


Bool predicate (Display *display,XEvent *event,XPointer arg)
{
  if((event->type == Expose)&&(event->xexpose.window == *((uint*)arg)))
    return 1;
  else
    return 0;
}


bool
CWindow::WEvents (XEvent WEvent)
{
  int ret=0;

  switch (WEvent.type)
    {
    case ClientMessage:
      if ((Atom) WEvent.xclient.data.l[0] == *AWMTakeFocus)
	{
	  //code
	}
      if (((Atom) WEvent.xclient.data.l[0] == *AWMDeleteWindow)
	  && (!CanExitExclusive))
	WDestroy ();
      ret=1;
      break;
    case DestroyNotify:
      on_destroy ();
      ret=1;
      break;
    case CirculateNotify:
      eprint( "CirculateNotify\n");
      ret=1;
      break;
    case ConfigureNotify:
      //look in the end of function for the last
      ret=1;
      break;
//    case GravityNotify:
//      ret=1;
//      break;
    case MapNotify:
      on_show ();
      ret=1;
      break;
//  case ReparentNotify:                
//  ret= 1;break;
    case UnmapNotify:
      on_hide ();
      ret=1;
      break;
//  ret=1 1;break;
//  case MotionNotify:          
//  ret=1 1;break;
//  case KeyPress:              
//  ret= 1;break;
//  case KeyRelease:            
//  ret= 1;break;
//  case ButtonPress:           
//  ret= 1;break;
//  case ButtonRelease:
//  ret= 1;break;
    case EnterNotify:
      on_enter ();
      ret= 1;
      break;
    case LeaveNotify:
      on_leave ();
      ret=1;
      break;
//  case ColormapNotify:                
//  ret=1 1;break;
//  case GraphicsExpose:                
//  ret=1 1;break;
//  case NoExpose:              
//  ret=1 1;break;
    case FocusIn:
      if (IC)
	XSetICFocus (IC);
      ret= 1;
      break;
    case FocusOut:
      if (IC)
	XUnsetICFocus (IC);
      ret= 1;
      break;
//  case KeymapNotify:          
//  ret=1 1;break;
//  case PropertyNotify:                
//  ret=1 1;break;
//  case ResizeRequest:         
//  ret= 1;
//  break;
    case MappingNotify:
      XRefreshKeyboardMapping (&WEvent.xmapping);
      ret= 1;
      break;
//  case SelectionClear:                
//  ret= 1;break;
//  case SelectionNotify:               
//  ret= 1;break;
//  case SelectionRequest:      
//  ret= 1;break;
//  case VisibilityNotify:      
//      eprint("visibilityNotify\n");
//                      ret= 1;
//                      break;
//  case LASTEvent:             
//  ret= 1;break;
    default:
      CControl::Event (WEvent);
      ret= 1;
    case Expose:
      XRectangle rec;
      Region Reg=XCreateRegion();
      
      rec.x =WEvent.xexpose.x;
      rec.y =WEvent.xexpose.y;
      rec.width =WEvent.xexpose.width;
      rec.height =WEvent.xexpose.height;
      XUnionRectWithRegion(&rec,Reg,Reg);

      while(XCheckIfEvent(ADisplay, &WEvent, predicate,(char*)&WWindow))
      {
      rec.x =WEvent.xexpose.x;
      rec.y =WEvent.xexpose.y;
      rec.width =WEvent.xexpose.width;
      rec.height =WEvent.xexpose.height;
      XUnionRectWithRegion(&rec,Reg,Reg);
      }
      Update (Reg);
      ret= 1;
      break;
    }


  if ((LEvent.type != WEvent.type)&&(LEvent.type == ConfigureNotify))
  {
      Display *disp=Application->GetADisplay();

      if(disp)
      {
        int se=0;
	if(Width != (uint)LEvent.xconfigure.width)se=1;
        if(Height != (uint)LEvent.xconfigure.height)se=1;
        XLockDisplay(disp);
        X = LEvent.xconfigure.x;
        Y = LEvent.xconfigure.y;
        Width=LEvent.xconfigure.width;
        Height=LEvent.xconfigure.height;
        Border = LEvent.xconfigure.border_width;
	if(se)
	{
          CreatePixmap(true);
          on_show ();
	  Draw();
	}
        XUnlockDisplay(disp);
      }
  }


  LEvent=WEvent;

  return ret;
}


lxStringList CWindow::GetContext (void)
{
  CControl::GetContext ();
  Context.AddLine ("Title=" + GetTitle () + ";String");
  //events 
  Context.AddLine ("OnCreate=" + btoa (GetEv (false)) + ";event");
  Context.AddLine ("OnDestroy=" + btoa (GetEv (false)) + ";event");
  Context.AddLine ("OnShow=" + btoa (GetEv (false)) + ";event");
  Context.AddLine ("OnHide=" + btoa (GetEv (false)) + ";event");
  Context.AddLine ("OnEnter=" + btoa (GetEv (false)) + ";event");
  Context.AddLine ("OnLeave=" + btoa (GetEv (false)) + ";event");
  return Context;
}

void
CWindow::SetContext (lxStringList context)
{
  Erase ();
  CControl::SetContext (context);
  for (uint i = 0; i < context.GetLinesCount (); i++)
    {
      lxString line = Context.GetLine (i);
      lxString arg;
      eqparse (line, arg);
      if (line.compare ("Title") == 0)
	SetTitle (arg);
      if (line.compare ("OnCreate") == 0)
	SetEv (atob (arg), false);
      if (line.compare ("OnDestroy") == 0)
	SetEv (atob (arg), false);
      if (line.compare ("OnShow") == 0)
	SetEv (atob (arg), false);
      if (line.compare ("OnHide") == 0)
	SetEv (atob (arg), false);
      if (line.compare ("OnEnter") == 0)
	SetEv (atob (arg), false);
      if (line.compare ("OnLeave") == 0)
	SetEv (atob (arg), false);
    }
  Draw ();
}

void
CWindow::CirculateFocus (bool asc)
{
  if (asc)
    {
      for (int c = 0; c < ChildCount; c++)
	{
	  if (Child[c] == ControlOnFocus)
	    {
	      if (Child[c + 1]->GetCanFocus ())
		{
		  Child[c + 1]->SetFocus (true);
		  return;
		}
	      else
		{
		  SetControlOnFocus (NULL);
		  ControlOnFocus = Child[c + 1];
		  CirculateFocus (asc);
		  return;
		}
	    }
	}
      if (Child[0]->GetCanFocus ())
	{
	  Child[0]->SetFocus (true);
	  return;
	}
      else
	{
	  SetControlOnFocus (NULL);
	  ControlOnFocus = Child[0];
	  CirculateFocus (asc);
	  return;
	}
    }
  else
    {
      for (int c = ChildCount - 1; c >= 0; c++)
	if (Child[c] == ControlOnFocus)
	  {
	    if (Child[c - 1]->GetCanExecuteEvent ())
	      {
		Child[c - 1]->SetFocus (true);
		return;
	      }
	    else
	      ControlOnFocus = Child[c - 1];
	  }
    }
}

void
CWindow::SetFocus (void)
{
  XSetInputFocus (ADisplay, WWindow, RevertToParent, CurrentTime);
}

XIC
CWindow::GetIC (void)
{
  return IC;
}


//propiedades
Window 
CWindow::GetWWindow (void)
{
  return WWindow;
}

Display *
CWindow::GetADisplay (void)
{
  return ADisplay;
}

int *
CWindow::GetADepth (void)
{
  return ADepth;
}

void
CWindow::SetCanDestroy (bool candestroy)
{
  CanDestroy = candestroy;
}

bool
CWindow::GetCanDestroy (void)
{
  return CanDestroy;
}

void
CWindow::SetXMouse (uint x)
{
  XMouse = x;
}

uint
CWindow::GetXMouse (void)
{
  return XMouse;
}

void
CWindow::SetYMouse (uint y)
{
  YMouse = y;
}

uint
CWindow::GetYMouse (void)
{
  return YMouse;
}

void
CWindow::SetTitle (const lxString & title)
{
  Title = title;
  XFreeTextProperty (WTextProperty);
  char *cstr;
  cstr = new char[title.size () + 1];
  strcpy (cstr, title.c_str ());
  if (XStringListToTextProperty (&cstr, 1, &WTextProperty) == 0)
    eprint("erro\n");
  delete[]cstr;
  if (WWindow)
    {
      XStoreName (ADisplay, WWindow, Title.c_str ());
      XSetIconName (ADisplay, WWindow, Title.c_str ());
    }
}

lxString CWindow::GetTitle (void)
{
  return Title;
}

CControl *
CWindow::GetControlOnFocus (void)
{
  if (ControlOnFocus)
    return ControlOnFocus;
  else
    return NULL;
}


void
CWindow::SetControlOnFocus (CControl * controlonfocus)
{
  if ((ControlOnFocus != NULL) && (ControlOnFocus->GetCanFocus ()))
    {
      CControl *last = ControlOnFocus;
      ControlOnFocus = NULL;
      last->focus_out ();
    }
  ControlOnFocus = controlonfocus;
}

CControl *
CWindow::GetLastControl (void)
{
  if (LastControl)
    return LastControl;
  else
    return NULL;
}

void
CWindow::SetLastControl (CControl * lastcontrol)
{
  LastControl = lastcontrol;
}

void
CWindow::SetX (int x)
{
  CControl::SetX (x);
  if (WWindow)
    XMoveWindow (ADisplay, WWindow, x, Y);
}

void
CWindow::SetY (int y)
{
  CControl::SetY (y);
  if (WWindow)
    XMoveWindow (ADisplay, WWindow, X, y);
}

void
CWindow::SetWidth (uint width)
{
  CControl::SetWidth (width);
  if (WWindow)
    XResizeWindow (ADisplay, WWindow, width, Height);
}

void
CWindow::SetHeight (uint height)
{
  CControl::SetHeight (height);
  if (WWindow)
    XResizeWindow (ADisplay, WWindow, Width, height);
}

int
CWindow::GetClientWidth(void)
{
 return Width;
}


int
CWindow::GetClientHeight(void)
{
 return Height;
}

//operators

void *
  CWindow::operator
new (size_t sz)
{
  CWindow *m = (CWindow *) calloc (sz, 1);
  if (!m)
    puts ("out of memory");
  m->Dynamic = true;
  m->CanDestroy = true;
  return (void *) m;
}

//eventos

void
CWindow::on_create (void)
{
  if ((FOwner) && (EvOnCreate))
    (FOwner->*EvOnCreate) (this);
}

void
CWindow::on_destroy (void)
{
  if ((FOwner) && (EvOnDestroy))
    (FOwner->*EvOnDestroy) (this);
}

void
CWindow::on_show (void)
{
  if ((FOwner) && (EvOnShow))
    (FOwner->*EvOnShow) (this);
}

void
CWindow::on_hide (void)
{
  if ((FOwner) && (EvOnHide))
    (FOwner->*EvOnHide) (this);
}

void
CWindow::on_enter (void)
{
  if ((FOwner) && (EvOnEnter))
    (FOwner->*EvOnEnter) (this);
}

void
CWindow::on_leave (void)
{
  if ((FOwner) && (EvOnLeave))
    (FOwner->*EvOnLeave) (this);
}


bool 
CWindow::LoadXMLContextAndCreateChilds(lxString filename, CControl* ctrl)
{
  FILE* file2;
  lxStringList list;
  lxString line;

  file2 = fopen (filename.c_str(),"r");
  rewind(file2);
  

  if (file2)
    {

      if(ctrl == NULL)//for owner window
      {
        if(fgetline (file2, line))
        {
          ctrl = this;
          ctrl->SetName(line.substr (1, line.size () - 2));//Get Window name
          ctrl->SetFOwner (this);
          rewind(file2);
        }
      }

      list.Clear ();
      while (fgetline (file2, line))
        {
          if (line.compare (lxT ("<") + ctrl->GetName () + lxT (">")) == 0)
            {
              fgetline (file2, line);
              do
                {
                  list.AddLine (line);
                  fgetline (file2, line);
                }
              while (line.c_str()[0] == ' ');
              ctrl->SetContext (list);

              while (line.compare (lxT ("</") + ctrl->GetName () + lxT (">")) != 0)
                {
                  lxString controlclass, ctype, name, cname;

                  cname = line.substr (1, line.size () - 2);
                  fgetline (file2, line);
                  xml_in (line, name, ctype, controlclass);

                  CControl *ch = newcontrolbycname (controlclass);
                  ch->SetName (cname);
                  ch->SetFOwner (ctrl);
                 
		  /* 
		  if (ch->GetClass ().compare (lxT ("CItemMenu")) == 0)
                    {
                      ch->SetVisible (false, false);
                    }
                  */
                  ctrl->CreateChild (ch);

                  if (ch != NULL)
                    LoadXMLContextAndCreateChilds (filename, ch);
                  else
                    printf ("Child Not Found! %s \n", (char*) name.char_str ());

                  do
                    {
                      fgetline (file2, line);
                    }
                  while ((line.compare (lxT ("</") + cname + lxT (">")) != 0));
                  fgetline (file2, line);
                }

            }

        }

      fclose(file2);
      return 1;
    }
  else
    printf ("File (%s) not found!\n",(char *)filename.char_str());

  return 0;
}

bool 
CWindow::GetCanExitExclusive(void)
{
  return CanExitExclusive;
}
  
CWindow* 
CWindow::GetWWidget (void)
{
  return this; 
}
