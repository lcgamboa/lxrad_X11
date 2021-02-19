/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001-2020  Luis Claudio Gamboa Lopes

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
#include"../include/cpaint.h"
#include"../include/cwindow.h"
#include"../include/capplication.h"

// CPaint________________________________________________________________


CPaint::CPaint (void)
{
  SetClass ("CPaint");
  RX=0;
  RY=0;
  Disp=NULL;
  DrawIn=0;
  DrawOut=0;
  DoCalcRXY=true;
  Scalex=1.0;
  Scaley=1.0;
  Agc=0;
  orientation=0;
}
  
void 
CPaint::SetDoCalcRXY(bool docalcrxy)
{
  DoCalcRXY=docalcrxy;
};

void 
CPaint::SetDrawIn(Drawable drawin)
{
  DrawIn=drawin;
};

void 
CPaint::SetDrawOut(Drawable drawout)
{
  DrawOut=drawout;
};

CPaint::~CPaint (void)
{
};

void
CPaint::Create (CControl * control)
{
  Win = control->GetWin ();
  Owner = control;
  DrawIn = Win->GetPixmap();
  DrawOut = Win->GetWWindow();
  Disp = Win->GetADisplay();
  Agc = XCreateGC (Disp, Win->GetWWindow (), 0, NULL);
  XSetGraphicsExposures(Disp, Agc, false);
  Pen.Create (control, &Agc);
};
  
void 
CPaint::Create (CControl * control, lxBitmap *bitmap)
{
  Win = NULL;
  Owner = NULL;
  DrawIn = *bitmap;
  DrawOut = *bitmap;
  Disp = Application->GetADisplay();
  Agc = XCreateGC (Disp, DrawIn, 0, NULL);
  XSetGraphicsExposures(Disp, Agc, false);
  Pen.Create (NULL, &Agc);
  Width = bitmap->GetWidth();
  Height = bitmap->GetHeight();	  
}

void
CPaint::Destroy (void)
{
  if (Agc != 0)
    XFreeGC (Disp, Agc);
};

void
CPaint::SetFont (CControl * control)
{
  XGCValues *Gcv;
  Gcv = new XGCValues;
  XFontStruct *PFont = control->GetFont ();
  Gcv->font = PFont->fid;
  XChangeGC (Disp, Agc, GCFont, Gcv);
  delete Gcv;
};

void
CPaint::InitDraw (CControl * control)
{
  if(DoCalcRXY)
  {	
    control->CalcRXY();
    RX=control->GetRX();
    RY=control->GetRY();
  }
  else
  {
  RX=0;
  RY=0;
  };
  SetFont(control);
};

void
CPaint::DrawControl (CControl * control)
{
  if ((control->GetVisible ())&&
      (DrawIn !=0 )&& 
      (DrawIn != DrawOut))
    {
      if(Owner != Win)
      {
      RX=control->GetRX();
      RY=control->GetRY();
      Pen.SetPen (control->GetPen ());
      XCopyArea (Disp, DrawIn, DrawOut,
		 Agc, RX, RY, control->GetWidth (), control->GetHeight (),RX, RY);
      Pen.SetPen (GXcopy);
      }
      else
      {
      RX=control->GetRX()-1;
      RY=control->GetRY()-1;
      Pen.SetPen (control->GetPen ());
      XCopyArea (Disp, DrawIn, DrawOut,
		 Agc, RX, RY, control->GetWidth ()+2, control->GetHeight ()+2,RX, RY);
      Pen.SetPen (GXcopy);
      }
    };
};




void
CPaint::Point (int x, int y)
{
  Rotate(&x,&y);
  XDrawPoint (Disp, DrawIn, Agc, RX+x, RY+y);
}

void
CPaint::FillPolygon (XPoint * points, int npoints)
{
  for(int c=0;c<npoints;c++)
  {
    points[c].x+=RX;	  
    points[c].y+=RY;	  
  }	
  XFillPolygon (Disp, DrawIn, Agc, points,
		npoints, Nonconvex, CoordModeOrigin);
}

void
CPaint::Line (int x1, int y1, int x2, int y2)
{
  Rotate(&x1,&y1);
  Rotate(&x2,&y2);
  XDrawLine (Disp, DrawIn, Agc, RX+x1, RY+y1, RX+x2, RY+y2);
}

void
CPaint::Lines (XPoint * points, int npoints)
{
  for(int c=0;c<npoints;c++)
  {
    points[c].x+=RX;	  
    points[c].y+=RY;	  
  }	
  XDrawLines (Disp, DrawIn, Agc, points,
	      npoints, CoordModeOrigin);
}


void
CPaint::Rectangle (int x, int y, int w, int h)
{
    int x2,y2; 
    int temp;
    x2=x+w;
    y2=y+h;
    Rotate(&x,&y);
    Rotate(&x2,&y2);
    if(x > x2)
    {
      temp=x;
      x=x2;
      x2=temp;
    }
    if(y > y2)
    {
      temp=y;
      y=y2;
      y2=temp;
    }
    XFillRectangle (Disp, DrawIn, Agc, (RX+x)*Scalex, (RY+y)*Scaley, (x2-x)*Scalex, (y2-y)*Scaley);
}

void
CPaint::Frame (int x, int y, int w, int h, uint wb)
{
  for (uint c = 0; c < wb; c++)
    XDrawRectangle (Disp, DrawIn, Agc, (RX+x + c)*Scalex,
		    (RY+y + c)*Scaley, (w - (c * 2))*Scalex, (h - (c * 2))*Scaley);
};

void
CPaint::LowerFrame (int x, int y, int w, int h, uint wb)
{
  XColor OldPen;
  for (uint c = 0; c < wb; c++)
    {
      OldPen = Pen.GetFgColor ();
      Pen.SetFgColor (ColorByName ("gray35"));
      Line ( x + c, y + c, x + c, h - 2 - c);
      Line ( x + c, y + c, w - c - 1, y + c);
      Pen.SetFgColor (ColorByName ("gray59"));
      Line ( x + 1 + c, y + 1 + c, x + 1 + c, h - 2 - c);
      Line ( x + 1 + c, y + 1 + c, w - 2 - c, y + 1 + c);
      Pen.SetFgColor (ColorByName ("gray96"));
      Line ( x + c, h - c - 1, w - c - 1, h - 1 - c);
      Line ( w - 1 - c, y + 1 + c, w - 1 - c, h - c - 1);
      Pen.SetFgColor (ColorByName ("gray82"));
      Line ( x + 1 + c, h - 2 - c, w - 2 - c, h - 2 - c);
      Line ( w - 2 - c, y + 2 + c, w - 2 - c, h - 2 - c);
      Pen.SetFgColor (OldPen);
    }
}


void
CPaint::RaiserFrame (int x, int y, int w, int h, uint wb)
{
  XColor OldPen;
  for (uint c = 0; c < wb; c++)
    {
      OldPen = Pen.GetFgColor ();
      Pen.SetFgColor (ColorByName ("gray96"));
      Line ( x + c, y + c, x + c, h - 2 - c);
      Line ( x + c, y + c, w - c - 1, y + c);
      Pen.SetFgColor (ColorByName ("gray86"));
      Line ( x + 1 + c, y + 1 + c, x + 1 + c, h - 3 - c);
      Line ( x + 1 + c, y + 1 + c, w - 2 - c, y + 1 + c);
      Pen.SetFgColor (ColorByName ("gray35"));
      Line ( x + c, h - 1 - c, w - c - 1, h - 1 - c);
      Line ( w - 1 - c, y + 1 + c, w - 1 - c, h - c - 1);
      Pen.SetFgColor (ColorByName ("gray59"));
      Line ( x + 1 + c, h - 2 - c, w - 2 - c, h - 2 - c);
      Line ( w - 2 - c, y + 2 + c, w - 2 - c, h - 2 - c);
      Pen.SetFgColor (OldPen);
    }
}

void
CPaint::Text (lxString text,  int x1, int y1)
{
  //FIXME font size 13	
  XDrawString (Disp, DrawIn, Agc, RX+x1, RY+y1+ 13 ,
	       text.c_str (), text.size ());
}
  
void 
CPaint::TextOnRect (lxString text,lxRect rect,CAlign align)
{
  //FIXME font size 13	
  XDrawString (Disp, DrawIn, Agc, RX+rect.x, RY+rect.y+ 13 ,
	       text.c_str (), text.size ());
}


void
CPaint::ImgText ( int x1, int y1, lxString text)
{
  XDrawImageString (Disp, DrawIn, Agc, RX+x1, RY+y1,
	       text.c_str (), text.size ());
}
  
void 
CPaint::PutPixmap (int x,int y, int w, int h,Pixmap pixmap)
{
   XCopyArea (Disp, pixmap, DrawIn ,Agc, 0, 0, w, h, RX+x, RY+y);
}
  
void 
CPaint::SetLineWidth(int w)
{
  Pen.SetWidth (w);
}

void 
CPaint::Init(void)
{
  Scalex=1.0;
  Scaley=1.0;  
  orientation=0;
}
  

void 
CPaint::Init(float sx, float sy, int _orientation)
{
  Scalex=sx;
  Scaley=sy;  
  
  orientation = _orientation;
}

void 
CPaint::End(void)
{
  if(Owner)  
    Owner->Draw();	
}

void 
CPaint::SetFgColor(unsigned char r,unsigned char g, unsigned char b)
{
   Pen.SetFgColor (ColorByRGB(r,g,b));
}

void 
CPaint::SetFgColor(lxString cname)
{
   Pen.SetBgColor (ColorByName(cname));
}


void 
CPaint::SetBgColor(unsigned char r,unsigned char g, unsigned char b)
{
   Pen.SetBgColor (ColorByRGB(r,g,b));
}

	
void 
CPaint::SetBgColor(lxString cname)
{
   Pen.SetBgColor (ColorByName(cname));
}

void 
CPaint::Rectangle (bool filled, int x, int y, int w, int h)
{
  if(filled)
  { 
    lxColor old = Pen.GetFgColor ();
    Pen.SetFgColor (Pen.GetBgColor ());
    Rectangle ( x, y,  w, h);
    Pen.SetFgColor (old);
  }
  
  Frame ( x, y, w, h,1);
}

//FIXME
void 
CPaint::RotatedText (lxString str, int x, int y, int _angle)
{
     Rotate(&x,&y);
     switch(orientation)
     {
        case 1:
          _angle+= -90;
	  break;
        case 2:
          _angle+= 180;
	  break;
        case 3:
          _angle+= 90;
	  break;
        default:
	  break;
     }

  //FIXME font size 13
  char sstr[2];
  int dx=13*cos(_angle*3.1416/180.0);
  int dy=-13*sin(_angle*3.1416/180.0);
  
  int size=str.size();

  sstr[1]=0
	  ;
  for(int i=0; i< size; i++)
  {
     sstr[0]=((const char *)str.c_str())[size-i-1];
     XDrawString (Disp, DrawIn, Agc, RX+x+dx*i, RY+y+ 13+dy*(i+1) ,sstr, 1);
  }
}

void 
CPaint::PutBitmap (lxBitmap* bitmap,int x,int y)
{
  Window root;
  int rx,ry;
  unsigned int rw,rh,rb,rd;
  
  XGetGeometry(Disp, *bitmap,&root,&rx,&ry,&rw,&rh,&rb,&rd);
 
  if(DrawOut)
  {	  
     Rotate(&x,&y);
     switch(orientation)
     {
       case 1:
         x-=rw;
         break;	 
       case 2:
         x-=rw;
         y-=rh;
         break;	 
       case 3:	       
         y-=rh;
	 break;
     }     
#ifdef HAVE_LIBIMLIB2
	Imlib_Image img;
	img = imlib_create_image(rw, rh);

        imlib_context_set_image(img);
        imlib_context_set_drawable(*bitmap);
        imlib_copy_drawable_to_image(0, 0, 0, rw, rh, 0, 0, 1);


        imlib_context_set_drawable(DrawIn);
        //imlib_context_set_mask(CMask);
        imlib_render_image_on_drawable_at_size((RX+x)*Scalex,(RY+y)*Scaley,rw*Scalex,rh*Scaley);
        imlib_free_image();
#else     
  XCopyArea (Disp, *bitmap, DrawIn,  Agc, 0, 0, rw, rh ,(RX+x), (RY+y));
#endif
  }
}

void 
CPaint::SetBitmap(lxBitmap * bitmap,double xs, double ys)
{
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
}

void 
CPaint::SetFont (lxFont font)
{
#ifdef _DEBUG	
  printf ("Incomplete: %s -> %s :%i\n", __func__,__FILE__, __LINE__);
#endif
}

void 
CPaint::ChangeScale(float sx, float sy)
{
  Scalex=sx;
  Scaley=sy;	  
}

void 
CPaint::Circle (bool filled, int x, int y, int radius)
{
  int off=radius;	
  Rotate(&x,&y);

  if(filled)
  { 
    lxColor old = Pen.GetFgColor ();
    Pen.SetFgColor (Pen.GetBgColor ());
    XFillArc (Disp, DrawIn, Agc, (RX+x-off)*Scalex, (RY+y-off)*Scaley, 2*radius*Scalex, 2*radius*Scaley,0,360*64);
    Pen.SetFgColor (old);
  }
  
  XDrawArc (Disp, DrawIn, Agc, (RX+x-off)*Scalex, (RY+y-off)*Scaley, 2*radius*Scalex, 2*radius*Scaley,0,360*64);
}
  

void 
CPaint::Polygon(bool filled, lxPoint * points, int npoints)
{
  points[0].x+=RX;
  points[0].y+=RY;
  
  if(filled)
  { 
    lxColor old = Pen.GetFgColor ();
    Pen.SetFgColor (Pen.GetBgColor ());
    XFillPolygon(Disp, DrawIn, Agc, points, npoints,  Complex , CoordModeOrigin);
    Pen.SetFgColor (old);
  }
  
  XDrawLines (Disp, DrawIn, Agc, points,
	      npoints, CoordModeOrigin);
}

void 
CPaint::SetColor(unsigned char r,unsigned char g, unsigned char b)
{
  Pen.SetColor (ColorByRGB(r,g,b));
}

void 
CPaint::SetColor(lxColor c)
{
  Pen.SetColor (c);
}

void CPaint::SetFgColor(lxColor c)
{
  Pen.SetFgColor (c);
}

void CPaint::SetBgColor(lxColor c)
{
  Pen.SetBgColor (c);
}

void 
CPaint::Rotate(int *x, int *y)
{
  int ox=*x;
  int oy=*y;

  switch(orientation)
  {
    case 1:
    *x= Width -oy;
    *y= ox;
    break;
    case 2:
    *x= Width -ox;
    *y= Height -oy;
    break;
    case 3:
    *x= oy;
    *y= Height - ox;
    break;
    default:
    break;
  } 

}

