

#include"layout7.h"
#include"layout7_d.cc"


CWindow7 Window7;

//implemetation

#include"layout2.h"

void  
CWindow7::StringListMouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  Window7.cl = dynamic_cast<CList*>(Window2.GetControlOnFocus ());
  Window7.cc = dynamic_cast<CCombo*>(Window2.GetControlOnFocus ());
  
  if(Window7.cl != NULL)
  {	  
    Window7.list1.SetItems(Window7.cl->GetItems());
    Window7.ShowExclusive(); 
  };
  
  if(Window7.cc != NULL)
  {	  
    Window7.list1.SetItems(Window7.cc->GetItems());
    Window7.ShowExclusive(); 
  };
};

void
CWindow7::button1_MouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  lxString item;
  if(Input("Add String: ",item))
  {
  list1.AddItem(item);
  };
};

void
CWindow7::button2_MouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  list1.DeleteItem(list1.GetSelectedItemN());
};

void
CWindow7::button3_MouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  list1.DeleteItems();
};

void
CWindow7::button4_MouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  HideExclusive(); 
  if(cl)cl->SetItems(list1.GetItems());
  if(cc)cc->SetItems(list1.GetItems());
};


