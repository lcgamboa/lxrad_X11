
#include"../include/lxrad.h"

CControl *
newcontrolbycname (lxString controltype)
{
  CControl *ncontrol = NULL;

  if (controltype.compare (lxT("CButton")) == 0)
    {
      ncontrol = new CButton;
    };
  if (controltype.compare (lxT("CToolButton")) == 0)
    {
      ncontrol = new CToolButton;
    };
  if (controltype.compare (lxT("CXpmButton")) == 0)
    {
      ncontrol = new CXpmButton;
    };
  if (controltype.compare (lxT("CEdit")) == 0)
    {
      ncontrol = new CEdit;
    };
  if (controltype.compare (lxT("CLabel")) == 0)
    {
      ncontrol = new CLabel;
    };
  if (controltype.compare (lxT("CCheckBox")) == 0)
    {
      ncontrol = new CCheckBox;
    };
  if (controltype.compare (lxT("CDraw")) == 0)
    {
      ncontrol = new CDraw;
    };
  if (controltype.compare (lxT("CImage")) == 0)
    {
      ncontrol = new CImage;
    };
  if (controltype.compare (lxT("CList")) == 0)
    {
      ncontrol = new CList;
    };
  if (controltype.compare (lxT("CFileList")) == 0)
    {
      ncontrol = new CFileList;
    };
  if (controltype.compare (lxT("CMenu")) == 0)
    {
      ncontrol = new CMenu;
    };
  if (controltype.compare (lxT("CPMenu")) == 0)
    {
      ncontrol = new CPMenu;
    };
  if (controltype.compare (lxT("CScroll")) == 0)
    {
      ncontrol = new CScroll;
    };
  if (controltype.compare (lxT("CCombo")) == 0)
    {
      ncontrol = new CCombo;
    };
  if (controltype.compare (lxT("CText")) == 0)
    {
      ncontrol = new CText;
    };
  if (controltype.compare (lxT("CFileDialog")) == 0)
    {
      ncontrol = new CFileDialog;
    }; 
  if (controltype.compare (lxT("CDirDialog")) == 0)
    {
      ncontrol = new CDirDialog;
    };
  if (controltype.compare (lxT("CItemMenu")) == 0)
    {
      ncontrol = new CItemMenu;
    };
  if (controltype.compare (lxT("CTimer")) == 0)
    {
      ncontrol = new CTimer;
    };
 /* 
  if (controltype.compare (lxT("CPanel")) == 0)
    {
      ncontrol = new CPanel;
    };
 */   
  if (controltype.compare (lxT("CGauge")) == 0)
    {
      ncontrol = new CGauge;
    };
/*  
  if (controltype.compare (lxT("CGrid")) == 0)
    {
      ncontrol = new CGrid;
    };
*/    
  if (controltype.compare (lxT("CHtml")) == 0)
    {
      ncontrol = new CHtml;
    };
  if (controltype.compare (lxT("CStatusbar")) == 0)
    {
      ncontrol = new CStatusbar;
    };
/*  
  if (controltype.compare (lxT("CSpin")) == 0)
    {
      ncontrol = new CSpin;
    };
*/    
  if (controltype.compare (lxT("CSpind")) == 0)
    {
      ncontrol = new CSpind;
    };
  if (controltype.compare (lxT("CToggleButton")) == 0)
    {
      ncontrol = new CToggleButton;
    };
#ifndef __WXX11__ 	    
  if (controltype.compare (lxT("CColorDialog")) == 0)
    {
      ncontrol = new CColorDialog;
    };
#endif
  if (controltype.compare (lxT("CThread")) == 0)
    {
      ncontrol = new CThread;
    };

  if(ncontrol == NULL)
  {  
    eprint (lxT("Unknown Controltype \"") + controltype + lxT("\"!\n"));
    exit (-1);
  }

  return ncontrol;
};
