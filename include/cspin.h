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

/**
 * \file cspin.h
 * \author Luis Claudio Gamboa Lopes
 * \date 07-15-2020
 */

#ifndef CSPIN
#define CSPIN

#include"ccontrol.h"
#include"cwindow.h"
#include"cedit.h"
#include"ctoolbutton.h"

/** \brief SPIN Control.
 *
 * Generic Spin Control Class.
 */

class CSpin:public CControl
{
protected:
  CEdit edit1;
  CToolButton tbutton1;
  CToolButton tbutton2;
  int Value;				
  int Min;
  int Max;
  int Inc;
  int CEvent (int event);
public:
    CSpin (void);
   ~CSpin (void);
  void Draw (void);
  void SetWidth(unsigned width);
  void SetHeight(unsigned height );
  int  Create (CControl * control);
  lxStringList GetContext (void);
  void SetContext (lxStringList context);
  //void Event (wxEvent & event);
  //propiedades
  void SetValue(int v);
  int GetValue (void);
  void SetMin(int v);
  int GetMin (void);
  void SetMax(int v);
  int GetMax (void);
  void SetInc(int v);
  int GetInc (void);
  //eventos
  void EditKeyPress (CControl * control, const uint key, const uint hk, uint mask);
  void TButtonPress1 (CControl * control, uint button, uint x, uint y,uint state);
  void TButtonPress2 (CControl * control, uint button, uint x, uint y,uint state);
  void (CControl::*EvOnChangeSpin) (CControl * control);
};

#endif
