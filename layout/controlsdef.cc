#include"../include/lxrad.h"
#include"controlsdef.h"
#include"layout1.h"

uint tbuttonc = 1;
uint ttoolbuttonc = 1;
uint txpmbuttonc = 1;
uint teditc = 1;
uint tlxStringc = 1;
uint tcheckboxc = 1;
uint tdrawc = 1;
uint tlistc = 1;
uint tfilelistc = 1;
uint tmenuc = 1;
uint tpmenuc = 1;
uint tscrollc = 1;
uint tcomboc = 1;
uint tfiledialogc =1;
uint titemmenuc =1;
uint ttimerc =1;

void
resetcontrolscount (void)
{
  tbuttonc = 1;
  ttoolbuttonc = 1;
  txpmbuttonc = 1;
  teditc = 1;
  tlxStringc = 1;
  tcheckboxc = 1;
  tdrawc = 1;
  tlistc = 1;
  tfilelistc = 1;
  tmenuc = 1;
  tpmenuc = 1;
  tscrollc = 1;
  tcomboc = 1;
  tfiledialogc =1;
  titemmenuc = 1;
  ttimerc =1;
};

CControl *
newcontrol (lxString controltype, CControl * owner)
{
  CControl *ncontrol;
  if (controltype.compare ("Button") == 0)
    {
      ncontrol = new CButton;
      ncontrol->Create (owner);
      ncontrol->SetName ("button" + itoa (tbuttonc++));
      return ncontrol;
    };
  if (controltype.compare ("ToolButton") == 0)
    {
      ncontrol = new CToolButton;
      ncontrol->Create (owner);
      ncontrol->SetName ("toolbutton" + itoa (ttoolbuttonc++));
      return ncontrol;
    };
  if (controltype.compare ("XpmButton") == 0)
    {
      ncontrol = new CXpmButton;
      ncontrol->Create (owner);
      ncontrol->SetName ("xpmbutton" + itoa (txpmbuttonc++));
      return ncontrol;
    };
  if (controltype.compare ("Edit") == 0)
    {
      ncontrol = new CEdit;
      ncontrol->Create (owner);
      ncontrol->SetName ("edit" + itoa (teditc++));
      return ncontrol;
    };
  if (controltype.compare ("Label") == 0)
    {
      ncontrol = new CLabel;
      ncontrol->Create (owner);
      ncontrol->SetName ("label" + itoa (tlxStringc++));
      ncontrol->SetCanExecuteEvent (true);
	  return ncontrol;
    };
  if (controltype.compare ("CheckBox") == 0)
    {
      ncontrol = new CCheckBox;
      ncontrol->Create (owner);
      ncontrol->SetName ("checkbox" + itoa (tcheckboxc++));
      return ncontrol;
    };
  if (controltype.compare ("Draw") == 0)
    {
      ncontrol = new CDraw;
      ncontrol->Create (owner);
      ncontrol->SetName ("draw" + itoa (tdrawc++));
      return ncontrol;
    };
  if (controltype.compare ("List") == 0)
    {
      ncontrol = new CList;
      ncontrol->Create (owner);
      ncontrol->SetName ("list" + itoa (tlistc++));
      return ncontrol;
    };
  if (controltype.compare ("FileList") == 0)
    {
      ncontrol = new CFileList;
      ncontrol->Create (owner);
      ncontrol->SetName ("filelist" + itoa (tfilelistc++));
      return ncontrol;
    };
  if (controltype.compare ("Menu") == 0)
    {
      ncontrol = new CMenu;
      ncontrol->Create (owner);
      ncontrol->SetName ("menu" + itoa (tmenuc++));
      return ncontrol;
    };
  if (controltype.compare ("PMenu") == 0)
    {
      ncontrol = new CPMenu;
      ncontrol->Create (owner);
      ncontrol->SetName ("pmenu" + itoa (tpmenuc++));
      return ncontrol;
    };
  if (controltype.compare ("Scroll") == 0)
    {
      ncontrol = new CScroll;
      ncontrol->Create (owner);
      ncontrol->SetName ("scroll" + itoa (tscrollc++));
      return ncontrol;
    };
  if (controltype.compare ("Combo") == 0)
    {
      ncontrol = new CCombo;
      ncontrol->Create (owner);
      ncontrol->SetName ("combo" + itoa (tcomboc++));
      return ncontrol;
    };
  if (controltype.compare ("FileDialog") == 0)
    {
      ncontrol = new CFileDialog;
      ncontrol->Create (owner);
      ncontrol->SetName ("filedialog" + itoa (tfiledialogc++));
      return ncontrol;
    };
  if (controltype.compare ("ItemMenu") == 0)
    {
      ncontrol = new CItemMenu;
      ncontrol->Create (owner);
      ncontrol->SetName ("itemmenu" + itoa (titemmenuc++));
      return ncontrol;
    };
  if (controltype.compare ("Timer") == 0)
    {
      ncontrol = new CTimer;
      ncontrol->Create (owner);
      ncontrol->SetName ("timer" + itoa (ttimerc++));
      return ncontrol;
    };
  
  eprint("unknown control!\n");
  return NULL;
};

void
getbuttons (CControl * owner)
{
  CToolButton *button;
//button1  
  button = new CToolButton;
  button->SetName ("Button");
  button->SetX (0);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE) + "/button.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button2  
  button = new CToolButton;
  button->SetName ("String");
  button->SetX (26);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/lxString.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button3 
  button = new CToolButton;
  button->SetName ("Edit");
  button->SetX (52);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/edit.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button4 
  button = new CToolButton;
  button->SetName ("CheckBox");
  button->SetX (78);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/checkbox.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button5 
  button = new CToolButton;
  button->SetName ("Draw");
  button->SetX (104);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/draw.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button6 
  button = new CToolButton;
  button->SetName ("ToolButton");
  button->SetX (130);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/toolbutton.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button7 
  button = new CToolButton;
  button->SetName ("XpmButton");
  button->SetX (156);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/xpmbutton.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button8 
  button = new CToolButton;
  button->SetName ("List");
  button->SetX (182);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/list.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button9 
  button = new CToolButton;
  button->SetName ("FileList");
  button->SetX (208);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/filelist.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button10 
  button = new CToolButton;
  button->SetName ("Menu");
  button->SetX (234);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/menu.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button11 
  button = new CToolButton;
  button->SetName ("PMenu");
  button->SetX (260);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/pmenu.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button12 
  button = new CToolButton;
  button->SetName ("Scroll");
  button->SetX (286);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/scroll.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button13 
  button = new CToolButton;
  button->SetName ("Combo");
  button->SetX (312);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/combo.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button14 
  button = new CToolButton;
  button->SetName ("FileDialog");
  button->SetX (338);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/filedialog.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
//button15 
  button = new CToolButton;
  button->SetName ("Timer");
  button->SetX (364);
  button->SetY (26);
  button->SetHint(button->GetName());
  button->SetPixmapFileName ( lxString(_SHARE)+"/timer.xpm");
  button->SetFOwner (&Window1);
  button->EvMouseButtonClick = EVMOUSEBUTTONCLICK & CWindow1::MouseButtonClick;
  owner->CreateChild (button);
};
