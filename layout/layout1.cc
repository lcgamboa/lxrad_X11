
#include"layout1.h"
#include"controlsdef.h"
#include"layout1_d.cc"

int WN = 1;
lxString Version = "0.4";
int PNW = 1;
lxString PName = "untitled";
lxString PDir = "~/";
lxString POptions = "-Wall -O5";

//Implemetation

#include<stdlib.h>
#include<time.h>
#include<stdio.h>



CWindow1 Window1;


bool Save = false;
bool Move;
lxString operation = "editar";
lxString Browser="konqueror";
lxString Editor="xterm -e vim";
lxString Debuger="ddd";
lxString Home="";

#include"layout2.h"
#include"layout5.h"


void
CWindow1::_OnCreate(CControl * control)
{
  FILE *fin,*fout;
  Home=getenv("HOME");
  Home+="/.lxrad";
  fin=fopen(Home.c_str(),"r");
  if(fin)
  {
    char buffer[200];	
    buffer[199]='\0';
    //fscanf(fin,"%s\n",buffer);	  
    fgets(buffer,200,fin);	  
    buffer[strlen(buffer)-1]='\0';
    Browser=buffer;
    //fscanf(fin,"%s\n",buffer);	  
    fgets(buffer,200,fin);	  
    buffer[strlen(buffer)-1]='\0';
    Editor=buffer;
    //fscanf(fin,"%s\n",buffer);
    fgets(buffer,200,fin);	  
    buffer[strlen(buffer)-1]='\0';
    Debuger=buffer;
    fclose(fin);
  }
  else
  {
  fout=fopen(Home.c_str(),"w");
  if(fout)
  {
     fprintf(fout,"%s\n",Browser.c_str());	  
     fprintf(fout,"%s\n",Editor.c_str());	  
     fprintf(fout,"%s\n",Debuger.c_str());
     fclose(fout);
  };
  };
};

void
CWindow1::PreferencesMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state)
{
  lxString temp;
  FILE* fout;
  
  temp=Browser;
  if(Input("Browser:",temp))Browser=temp;
  temp=Editor;
  if(Input("Editor:",temp))Editor=temp;
  temp=Debuger;
  if(Input("Debuger:",temp))Debuger=temp;

  fout=fopen(Home.c_str(),"w");
  if(fout)
  {
     fprintf(fout,"%s\n",Browser.c_str());	  
     fprintf(fout,"%s\n",Editor.c_str());	  
     fprintf(fout,"%s\n",Debuger.c_str());
     fclose(fout);
  }
  else
  {
     Message("Error on saving preferences!");
  };	  
   
};

void
CWindow1::NewMouseButtonPress (CControl * control, uint button, uint x,
			       uint y, uint state)
{
  if (Save)
  {
    Save = false;
    Window2.MakeList (true);
  };
  
  Window5.ShowExclusive();		
};

void
CWindow1::NewWindowMouseButtonPress (CControl * control, uint button, uint x,
				     uint y, uint state)
{
  if (Save)
    {
      Save = false;
      Window2.MakeList (true);
    };
  PNW++;
  WN = PNW;
  Window1.pmenu3.DestroyChilds ();
  for (int n = 1; n <= PNW; n++)
    {
      CItemMenu *im;
      im = new CItemMenu;
      im->SetText ("Window" + itoa (n));
      im->SetName ("Window" + itoa (n));
      im->SetTag (n);
      im->SetFOwner (this);
      im->EvMouseButtonPress =
	EVMOUSEBUTTONPRESS & CWindow1::WindowsMouseButtonPress;
      Window1.pmenu3.CreateChild (im);
    };
  operation = "editar";
  Window2.ListPropierties (&Window2);
  Window2.DestroyChilds ();
  Window2.Reset ();
  Window2.SetName ("window2");
  Window2.SetX (240);
  Window2.SetY (40);
  Window2.SetWidth (400);
  Window2.SetHeight (400);
  Window2.SetTitle (PName);
  Save = true;
};

void
CWindow1::ClearMouseButtonPress (CControl * control, uint button, uint x,
				 uint y, uint state)
{

  operation = "editar";
  Window2.ListPropierties (&Window2);
  Window2.DestroyChilds ();
  Window2.Reset ();
  Save = true;
};

void
CWindow1::SaveMouseButtonPress (CControl * control, uint button, uint x,
				uint y, uint state)
{
  Window2.MakeList (false);
  Save = false;
};

void
CWindow1::ExitMouseButtonPress (CControl * control, uint button, uint x,
				uint y, uint state)
{
  if (Save)
  {
     Save = false;
     Window2.MakeList (true);
  };
  Window1.WDestroy ();
};

void
CWindow1::DeleteMouseButtonPress (CControl * control, uint button, uint x,
				  uint y, uint state)
{
  Window2.DestroyChild (Window2.GetControlOnFocus ());
  Window2.ListPropierties (&Window2);
  Save = true;
};

void
CWindow1::OptionsMouseButtonPress (CControl * control, uint button, uint x,
				   uint y, uint state)
{
  Input ("Options:", POptions);
  Save = true;
};

void
CWindow1::WindowsMouseButtonPress (CControl * control, uint button, uint x,
				   uint y, uint state)
{
  CItemMenu *im;
  im = dynamic_cast < CItemMenu * >(control);
  if (Save)
    {
      Save = false;
      Window2.MakeList (true);
    };
  WN = im->GetTag ();
  //Window2.ListPropierties (&Window2);
  Window2.DestroyChilds ();
  Window2.Reset ();
  Window2.LoadProject (PDir,PName+".prj_lxrad");
  Save = true;
};

void
CWindow1::MouseButtonClick (CControl * control, uint button, uint x, uint y,
			    uint state)
{
  Window1.SetTitle ("LXRAD Layout - " + ((CButton *) control)->GetName ());
  operation = ((CButton *) control)->GetName ();
  Save = true;
};

void
CWindow1::ContentsMouseButtonPress (CControl * control, uint button, uint x, uint y,
			    uint state)
{
  system((Browser+" "+lxString(_SHARE)+"/../docs/index.html&").c_str());
};

void
CWindow1::ReferenceMouseButtonPress (CControl * control, uint button, uint x, uint y,
			    uint state)
{
  system((Browser+" "+lxString(_SHARE)+"/../docs/reference.html&").c_str());
};

void
CWindow1::RunMouseButtonPress (CControl * control, uint button, uint x, uint y,
			    uint state)
{
  chdir(PDir.c_str());
  //Window2.MakeList(false);
  system("make run");		
};

void
CWindow1::BuildMouseButtonPress (CControl * control, uint button, uint x, uint y,
			    uint state)
{
  chdir(PDir.c_str());
  Window2.MakeList(false);
  system("make");		
};

void
CWindow1::SourceMouseButtonPress (CControl * control, uint button, uint x, uint y,
			    uint state)
{	
  chdir(PDir.c_str());
  Window2.MakeList(false);
  system((Editor+" "+PName+itoa(WN)+".cc").c_str());
};

void
CWindow1::DebugMouseButtonPress (CControl * control, uint button, uint x, uint y,
			    uint state)
{	
  lxString temp;
  chdir(PDir.c_str());
  temp = POptions;
  POptions="-Wall -ggdb3";
  Window2.MakeList(false);
  system("make");
  POptions=temp;
  system((Debuger+" "+PName).c_str());
  system("make clean");
};


