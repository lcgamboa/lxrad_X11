/* ########################################################################

   LXRAD -  GUI for X programing

   ########################################################################

   Copyright (c) : 2001  Luis Claudio Gamb�a Lopes

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

#ifndef CPAINT
#define CPAINT

#include"cobject.h"
#include"cpen.h"

class CControl;
class CWindow;

class CPaint:public CObject
{
private:
  Display * Disp;	
  CWindow * Win;
  Drawable DrawIn;
  Drawable DrawOut;
  CControl *Owner;
  int RX,RY;
  bool DoCalcRXY;
  float Scalex;
  float Scaley;
  int orientation;
  void Rotate(int *x, int *y);
  int Width;
  int Height;
public:
    GC Agc;
    CPen Pen;
    CPaint (void);
   ~CPaint (void);
  void SetDoCalcRXY(bool docalcrxy); 
  void SetDrawIn(Drawable drawin);
  void SetDrawOut(Drawable drawout);
  void InitDraw (CControl * control);
  void DrawControl (CControl * control);
  void Create (CControl * control);
  void Create (CControl * control, lxBitmap *bitmap);
  void Destroy (void);
  void SetFont (CControl * control);
  void SetFontSize (const int pointsize);
  void SetFontWeight (const int weight);
  void SetPen (int function);
  void Point (int x, int y);
  void FillPolygon (XPoint * points, int npoints);
  void Line (int x1, int y1, int x2, int y2);
  void Lines (XPoint * points, int npoints);
  void Rectangle (int x, int y, int w, int h);
  void Frame (int x, int y, int w, int h, uint wb = 1);
  void LowerFrame (int x, int y, int w, int h, uint wb = 1);
  void RaiserFrame (int x, int y, int w, int h, uint wb = 1);
  void Text (lxString text, int x, int y);
  void RotatedText (lxString str, int x, int y, int orientation);
  void ImgText (int x, int y, lxString text);
  void PutPixmap (int x,int y, int w, int h,Pixmap pixmap);
  void PutBitmap (lxBitmap* bitmap,int x,int y);
  void SetBitmap(lxBitmap * bitmap,double xs, double ys);
  void SetFont (lxFont font);
  void TextOnRect (lxString text,lxRect rect,CAlign align);
  void SetLineWidth(int w);
  void Init(void);
  void Init(float sx, float sy, int orientation = 0);
  void ChangeScale(float sx, float sy);
  void End(void);
  void SetColor(unsigned char r,unsigned char g, unsigned char b); 
  void SetFgColor(unsigned char r,unsigned char g, unsigned char b); 
  void SetBgColor(unsigned char r,unsigned char g, unsigned char b); 
  void SetFgColor(lxString cname); 
  void SetBgColor(lxString cname); 
  void SetFgColor(lxColor c); 
  void SetBgColor(lxColor c); 
  void SetColor(lxColor c); 
  lxColor GetFgColor(void);
  lxColor GetBgColor(void);
  void Rectangle (bool filled, int x, int y, int w, int h);
  void Circle (bool filled, int x, int y, int radius);
  void Polygon(bool filed, lxPoint * points, int npoints);
  void Arc (bool filled, int x1, int y1, int x2, int y2, int xc, int yc);
  void EllipticArc(bool filled, int x, int y, int width, int height, double start, double end);
};

#endif
