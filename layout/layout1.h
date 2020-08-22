#ifndef CWINDOW1
#define CWINDOW1

#include"../include/lxrad.h"

class CWindow1:public CWindow
{
public:
  CMenu menu1;
  CItemMenu menuitem1;
  CItemMenu menuitem2;
  CItemMenu menuitem3;
  CItemMenu menuitem4;
  CItemMenu menuitem5;
  CPMenu pmenu1;
  CItemMenu pmenuitem1_1;
  CItemMenu pmenuitem1_2;
  CItemMenu pmenuitem1_3;
  CItemMenu pmenuitem1_4;
  CItemMenu pmenuitem1_5;
  CPMenu pmenu2;
  CItemMenu pmenuitem2_1;
  CItemMenu pmenuitem2_2;
  CItemMenu pmenuitem2_3;
  CPMenu pmenu3;
  CItemMenu pmenuitem3_1;
  CPMenu pmenu4;
  CItemMenu pmenuitem4_1;
  CItemMenu pmenuitem4_2;
  CPMenu pmenu5;
  CItemMenu pmenuitem5_1;
  CItemMenu pmenuitem5_2;
  CItemMenu pmenuitem5_3;
  CItemMenu pmenuitem5_4;
  CEdit edit1;
  CLabel string1;

  void edit1focusout (CControl * control);
  
  void _OnCreate(CControl * control);

  void PreferencesMouseButtonPress (CControl * control, uint button, uint x, uint y,
			     uint state);
  void NewMouseButtonPress (CControl * control, uint button, uint x, uint y,
			     uint state);
  void NewWindowMouseButtonPress (CControl * control, uint button, uint x, uint y,
			     uint state);
  void ClearMouseButtonPress (CControl * control, uint button, uint x, uint y,
			      uint state);
  void SaveMouseButtonPress (CControl * control, uint button, uint x, uint y,
			     uint state);
  void MouseButtonClick (CControl * control, uint button, uint x, uint y,
			 uint state);
  void ExitMouseButtonPress (CControl * control, uint button, uint x, uint y,
			     uint state);
  void DeleteMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state);
  void OptionsMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state);
  void WindowsMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state);
  void ContentsMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state);
  void ReferenceMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state);
  void RunMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state);
  void BuildMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state);
  void SourceMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state);
  void DebugMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state);
    CWindow1 (void);
};

extern CWindow1 Window1;

extern int WN;
extern lxString Version;
extern int PNW;
extern lxString PName;
extern lxString PDir;
extern lxString POptions;
extern bool Save;
extern bool Move;
extern lxString operation;
extern lxString Browser;
extern lxString Editor;
extern lxString Debuger;
extern lxString Home;

#endif
