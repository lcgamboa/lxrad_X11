#ifndef CWINDOW2
#define CWINDOW2

#include"../include/lxrad.h"

class CWindow2:public CWindow
{
public:
  virtual bool WEvents (XEvent WEvent);
  lxString getalign (CAlign align);
  CControl *NewControl (lxString cclass, uint x, uint y);
  lxString WriteEvents (lxString name, lxString event);
  lxString WriteControlEvents (lxString name, lxString event);
  CStringList linesbak;
  void Draw(void);
  void Reset (void);
  void MakeList (bool prompt);
  bool testline(lxString line);
  void MakeProject (lxString basename);
  bool LoadProject (lxString dirname, lxString filename);
  void ListPropierties (CControl * control);
  void SCreateChild(CControl * control);
  void GKeyboardPress (CControl * control, uint key, uint x, uint y,
		       uint mask);
  void Window2MouseButtonClick (CControl * control, uint button, uint x,
				uint y, uint state);
  void GMouseButtonPress (CControl * control, uint button, uint x, uint y,
			  uint state);
  void GMouseButtonRelease (CControl * control, uint button, uint x, uint y,
			    uint state);
  void GMouseMove (CControl * control, uint x, uint y);
  
    CWindow2 (void);
   ~CWindow2 (void);
};

extern CWindow2 Window2;

lxString space2underline(lxString str);

#endif
