
#include"layout8.h"
#include"layout8_d.cc"

CWindow8 Window8;

#include"layout2.h"

//Implementation

void  
CWindow8::MenuItensMouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  
  Window8.menu = dynamic_cast<CMenu*>(Window2.GetControlOnFocus ());
  Window8.pmenu = dynamic_cast<CPMenu*>(Window2.GetControlOnFocus ());
  
  if(Window2.GetControlOnFocus () != NULL)
  {	  
  int f ;
  lxString litens;
  
  if( Window8.menu)
     litens=Window8.menu->GetMenuItems();
  else
     litens=Window8.pmenu->GetMenuItems();
 
  printf("%s\n",litens.c_str());
  
  Window8.list1.DeleteItems ();
  
  f = litens.find (",");
  while (f > 0)
    {
      Window8.list1.AddItem (litens.substr (0, f));
      litens = litens.substr (f + 1, litens.size ());
      f = litens.find (",");
    };
    
    Window8.ShowExclusive(); 
  };
};



void
CWindow8::button1_MouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  lxString item;
  
  if(Input("Item Text",item))
  {
  list1.AddItem(item);
  
  CItemMenu * im;
  im = new CItemMenu;
  im->SetVisible(false);
  if(menu)
    im->SetName(space2underline(menu->GetName())+"_"+item);
  else
    im->SetName(space2underline(pmenu->GetName())+"_"+item);
  im->SetText(item);

  Window2.CreateChild(im);
  };
};

void
CWindow8::button2_MouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  if(menu)	
    Window2.DestroyChild(Window2.GetChildByName(space2underline(menu->GetName())+"_"+list1.GetSelectedItem()));
  else
    Window2.DestroyChild(Window2.GetChildByName(space2underline(pmenu->GetName())+"_"+list1.GetSelectedItem()));
  list1.DeleteItem(list1.GetSelectedItemN());
};

void
CWindow8::button3_MouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  int ii;
  lxString item;
  
  ii=list1.GetSelectedItemN();
  if(ii > 0)
  {
  item=list1.GetItem(ii-1);
  list1.SetItem(ii-1,list1.GetSelectedItem());
  list1.SetItem(ii,item);
  list1.SetSelectedItemN(ii-1);
  list1.Draw();
  };
};

void
CWindow8::button4_MouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  int ii;
  lxString item;
  
  ii=list1.GetSelectedItemN();
  if(ii < (list1.GetItemsCount()-1))
  {
  item=list1.GetItem(ii+1);
  list1.SetItem(ii+1,list1.GetSelectedItem());
  list1.SetItem(ii,item);
  list1.SetSelectedItemN(ii+1);
  list1.Draw();
  };
};

void
CWindow8::button5_MouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  
  lxString itens = "";
  for (int c = 0; c < list1.GetItemsCount (); c++)
    {
      itens += list1.GetItem (c) + ",";
    };
  
  if(menu)
    menu->SetMenuItems(itens);
  else
    pmenu->SetMenuItems(itens);
  HideExclusive(); 

  Window2.ListPropierties(Window2.GetControlOnFocus ());
};

void
CWindow8::button6_MouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state)
{
  HideExclusive(); 
};

