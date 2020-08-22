
#include"layout3.h"
#include"layout3_d.cc"

CWindow3 Window3;

//implemetation
#include"layout2.h"

void
CWindow3::editfocusout (CControl * control)
{
  CStringList List;
  CEdit *ed;
  CLabel *st;
  if (Window2.GetControlOnFocus () == NULL)
    return;
  Window2.GetControlOnFocus ()->Erase ();
  /*
     for (int c = 0; c < ((Window3.GetChildCount () + 1) / 2); c++)
     {
     st = (ClxString *) (Window3.GetChildByName ("clxString" + itoa (c + 1)));
     ed = (CEdit *) (Window3.GetChildByName ("cedit" + itoa (c + 1)));
     List.AddLine (st->GetText () + "=" + ed->GetText ());
     };
   */
  for (int c = 0; c <= Window3.GetChildCount (); c++)
    {
      st =dynamic_cast <CLabel*>(Window3.GetChildByName ("clabel" + itoa (c + 1)));
      ed =dynamic_cast <CEdit*>(Window3.GetChildByName ("cedit" + itoa (c + 1)));
      if((st)&&(ed)&&(!ed->GetReadOnly()))
	    List.AddLine (st->GetText () + "=" + ed->GetText ());
    };

  dynamic_cast<CObject*>(Window2.GetControlOnFocus ())->SetContext (List);
  Window2.GetControlOnFocus ()->Draw ();
};

void 
CWindow3::FileMouseButtonRelease (CControl * control, uint button, uint x, uint y,uint state)
{
  CToolButton *tb;		
  CEdit *ed;
  tb =dynamic_cast <CToolButton*>(control);
  if(tb)
  {
  if(dialog1.Run())
  {		  
     ed =dynamic_cast <CEdit*>(GetChildByName ("cedit" + itoa (tb->GetTag())));
	 if(ed)
	 {	 
		ed->SetText(dialog1.GetDir()+"/"+dialog1.GetFileName());	 
        editfocusout (control);
	 };
  };
  
  };
};


