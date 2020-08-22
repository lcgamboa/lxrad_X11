#include <X11/XKBlib.h>
#include<sys/stat.h>
#include<errno.h>
#include<typeinfo>

#include"layout2.h"
#include"layout2_d.cc"
#include"layout1.h"

CWindow2 Window2;

//implemetation
int xo = -1, yo = -1, xp, yp;


CWindow2::~CWindow2 (void)
{
  Window2.DestroyChilds ();
};


#include"controlsdef.h"
#include"layout1.h"
#include"layout3.h"
#include"layout4.h"
#include"layout6.h"
#include"layout7.h"
#include"layout8.h"
#include"layout9.h"

extern Bool predicate(Display* display,XEvent *event,XPointer arg);

lxString
space2underline(lxString str)
{
  lxString out=str;	
  
  for(uint i=0; i<str.size();i++)
    if(str[i] == ' ') out[i]='_';
  
  return out;
};


void 
CWindow2::Draw(void)
{
  	
  if(!Visible)	
  {
    SetCanExecuteEvent(true);	  
    Visible=1;	
    CWindow::Draw();
    Visible=0;	
  }
  else
    CWindow::Draw();
};

bool
CWindow2::WEvents (XEvent WEvent)
{
  //XEvent NWEvent;		//Next Window Event      
  switch (WEvent.type)
    {
    case ClientMessage:
      if ((Atom) WEvent.xclient.data.l[0] == *AWMTakeFocus)
	{
	  //code
	};
      if ((Atom) WEvent.xclient.data.l[0] == *AWMDeleteWindow)
	WDestroy ();
      return 1;
      break;
    case DestroyNotify:
      on_destroy ();
      return 1;
      break;
    case CirculateNotify:
      eprint("CirculateNotify\n");
      return 1;
      break;
    case ConfigureNotify:
      X = WEvent.xconfigure.x;
      Y = WEvent.xconfigure.y;
      Width = WEvent.xconfigure.width;
      Height = WEvent.xconfigure.height;
      CreatePixmap(true);
      Border = WEvent.xconfigure.border_width;
      return 1;
      break;
    case GravityNotify:
      eprint("GravitNotify\n");
      return 1;
      break;
    case MapNotify:
      Visible = true;
      eprint("MapNotify\n");
      on_show ();
      return 1;
      break;
    case UnmapNotify:
      Visible = false;
      on_hide ();
      return 1;
      break;
    case EnterNotify:
      on_enter ();
      return 1;
      break;
    case LeaveNotify:
      on_leave ();
      return 1;
      break;
    case FocusIn:
      if (IC)
	XSetICFocus (IC);
      return 1;
      break;
    case FocusOut:
      if (IC)
	XUnsetICFocus (IC);
      return 1;
      break;
    case MappingNotify:
      eprint("MappingNotify\n");
      XRefreshKeyboardMapping (&WEvent.xmapping);
      return 1;
      break;
    default:
      //CControl::Event (WEvent);
      {
	CControl *control = 0;
	CWindow *iswin = 0;
	KeySym key;
	char text[10];
	Status status;
	if (Win == NULL)
	  return 1;
	//mouse position
	//if ((WEvent.type == MotionNotify) && (Win != NULL))
	if ((WEvent.type == ButtonPress) && (Win != NULL))
	  {
	    Win->SetXMouse (WEvent.xmotion.x);
	    Win->SetYMouse (WEvent.xmotion.y);
	  };
	//verify Owner of event  
	for (int j = 0; j <= ChildCount; j++)
	  {
	    if (Child[j]->OwnerEvent (Win->GetXMouse (), Win->GetYMouse ())
		&& (Child[j]->GetVisible ()))
	      control = Child[j];
	  };

	if (control == NULL)
	  {
	    control = this;
	  }
	else
	  {
	    if (!control->GetCanExecuteEvent ())
	      {
		control = control->GetOwner ();
	      };
	  };
        
	
	//verify control focus 
	if (control != LastControl)
	  {
	    control->pointer_in ();
	    if (LastControl != NULL)
	      LastControl->pointer_out ();
	  };
	LastControl = control;
	
	if(control)
	if (control->GetWin() != NULL)
	  {
	    if ((WEvent.type == ButtonPress)
		&& (control != control->GetWin()->GetControlOnFocus ())
		&& (WEvent.xbutton.button == 1))
	      {
		control->SetFocus (true);
	      }
	    else 
	      control = Win->GetControlOnFocus ();
	  }
	
	if (control == NULL)
	  return 1;

	iswin = dynamic_cast < CWindow * >(control);

//activate event in control
	switch (WEvent.type)
	  {
	  case MotionNotify:
	    if (iswin)
	      {
		control->mouse_move (WEvent);
	      }
	    else
	      {
		GMouseMove (control, WEvent.xmotion.x, WEvent.xmotion.y);
	      };
	    return 1;
	    break;
	  case KeyPress:
	    XXLookupString (NULL, &WEvent.xkey, text, 10, &key, &status);
	    if (key == XK_Tab)
	      {
		Win->CirculateFocus (true);
		return 1;
	      };
	    if (key == XK_Delete)
	      {
		DestroyChild (Window2.GetControlOnFocus ());
		ListPropierties (&Window2);
		Save = true;
		return 1;
	      };
	    /*
	       if (Win->GetControlOnFocus ())
	       Win->GetControlOnFocus ()->key_press (WEvent);
	       else
	       control->key_press (WEvent);
	     */
	    GKeyboardPress (control, WEvent.xkey.keycode, WEvent.xkey.x,
			    WEvent.xkey.y, WEvent.xkey.state);
	    return 1;
	    break;
	  case KeyRelease:
	    /*
	       if (Win->GetControlOnFocus ())
	       Win->GetControlOnFocus ()->key_release (WEvent);
	       else
	       control->key_release (WEvent);
	     */
	    return 1;
	    break;
	  case ButtonPress:
	    if (iswin)
	      {
		control->button_press (WEvent);
	      }
	    else
	      {
		GMouseButtonPress (control, WEvent.xbutton.button,
				   WEvent.xbutton.x, WEvent.xbutton.y,
				   WEvent.xbutton.state);
	      };
	    return 1;
	    break;
	  case ButtonRelease:
	    if (iswin)
	      {
		control->button_release (WEvent);
	      }
	    else
	      {
		GMouseButtonRelease (control, WEvent.xbutton.button,
				     WEvent.xbutton.x, WEvent.xbutton.y,
				     WEvent.xbutton.state);
	      };
	    return 1;
	    break;
	  };

      };

      return 1;
    case Expose:
      XRectangle rec;
      Region Reg=XCreateRegion();
      
      rec.x =WEvent.xexpose.x;
      rec.y =WEvent.xexpose.y;
      rec.width =WEvent.xexpose.width;
      rec.height =WEvent.xexpose.height;
      XUnionRectWithRegion(&rec,Reg,Reg);

      while(XCheckIfEvent(ADisplay, &WEvent, predicate,(char*)&WWindow))
      {
      rec.x =WEvent.xexpose.x;
      rec.y =WEvent.xexpose.y;
      rec.width =WEvent.xexpose.width;
      rec.height =WEvent.xexpose.height;
      XUnionRectWithRegion(&rec,Reg,Reg);
      };
      
      //if(WEvent.xexpose.count == 0)
        //Update(); 
        //Update (WEvent.xexpose.x, WEvent.xexpose.y, WEvent.xexpose.width,WEvent.xexpose.height);
        Update (Reg);
      return 1;
      break;
    };
};


void
CWindow2::Reset (void)
{
  resetcontrolscount ();
};

lxString
CWindow2::getalign (CAlign align)
{
  lxString a;
  switch (align)
    {
    case CA_RIGHT:
      a = "ca_right";
      break;
    case CA_CENTER:
      a = "ca_center";
      break;
    case CA_LEFT:
      a = "ca_left";
      break;
    };
  return a;
};

bool
CWindow2::testline(lxString line)
{
int pos;
for(uint a =0 ; a < linesbak.GetLinesCount();a++)
{
  pos = line.compare(linesbak.GetLine(a));  
  if(pos ==0)return true;
};

return false;
};

void
CWindow2::MakeList (bool prompt)
{
  FILE * fbak;
  lxString lbak;
  FILE * file;
  lxString filename, filec = "", filec2 = "";
  CControl *controle;
  lxString ctname;
  CStringList List;
  lxString line, arg, ctype;
     
  
     if(prompt)
	 {
        if(!Dialog("Do you want to save changes?"))
	    {
	       return;
	    };
	 };

  
      lxString dirname = PDir;
      lxString basename = PName;

      //dirname += "/" + basename;
      //PName = basename;

      if ((WN > PNW) && (WN == PNW + 1))
	{
	  PNW++;
	}
      else
	{
	  if (WN > PNW)
	    {
	      Message ("Window Number Invalid!");
	      return;
	    };
	};

      MakeProject (dirname + "/" + basename);

//makefile        
      filename = dirname + "/Makefile";
      file=fopen(filename.c_str (),"w");
      fprintf(file,"CC = gcc\nFLAGS = %s\nOBJS = p%s.o",POptions.c_str(),PName.c_str());
      for (int c = 1; c <= PNW; c++)
	fprintf(file," %s%i.o",PName.c_str(),c);
      fprintf(file,"\n\nall: $(OBJS)");
      fprintf(file,"\n\t$(CC) $(FLAGS) $(OBJS) -o%s -llxrad_X11\n\n",PName.c_str());
      fprintf(file,"%%.o: %%.cc\n\t$(CC) -c $(FLAGS) $< \n\n");
      fprintf(file,"run: all\n\t./%s\n\n",PName.c_str());
      fprintf(file,"install:\n\nclean:\n\trm -f %s *.o core",PName.c_str());
      fclose (file);

//project        

//      filename = dirname + "/p" + PName + ".cc";
      filename = dirname + "/p" + basename + ".cc";
      file=fopen (filename.c_str (),"w");
      fprintf(file,"#include<lxrad.h>\n");
      for (int c = 1; c <= PNW; c++)
	fprintf(file,"#include\"%s%i.h\"\n",PName.c_str(),c);
      fprintf(file,"//Program____________________________________________________________\n");
      fprintf(file,"Initialize\n{\n  Application.Start (argc, argv);\n");
      for (int c = 1; c <= PNW; c++)
	fprintf(file,"  Application.ACreateWindow (&Window%i);\n",c);
      fprintf(file,"  Application.Load ();\n};\n");
      fclose (file);

//window.h
      filename = dirname + "/" + basename + itoa (WN) + ".h";
      int hfexist = rename (filename.c_str (), (filename + ".bak").c_str ());

      if (hfexist != 0)
	{
	  //new file              
	  file=fopen (filename.c_str (),"w");
	  fprintf(file, "#ifndef CWINDOW%i \n#define CWINDOW%i\n\n#include<lxrad.h>\n\nclass CWindow%i:public CWindow\n{\n  public:\n",WN,WN,WN);
	  List = GetContext ();
	  for (uint a = 0; a < List.GetLinesCount (); a++)
	    {
	      line = List.GetLine (a);
	      ctype = eqparse (line, arg);
	      if (ctype.compare ("event") == 0)
		{
		  if (atob (arg))
		    {
		      filec += "  void " + WriteEvents ("", line) + ";\n";
		    };
		};
	    };
	  fprintf(file,"  /*#Controls*/\n");
	  for (int count = 0; count <= ChildCount; count++)
	    {
	      controle = Child[count];
	      ctname = controle->GetName ();
	      fprintf(file,"  %s %s;\n",controle->GetClass ().c_str(),ctname.c_str());

	      List = controle->GetContext ();
	      for (uint a = 0; a < List.GetLinesCount (); a++)
		{
		  line = List.GetLine (a);
		  ctype = eqparse (line, arg);
		  if (ctype.compare ("event") == 0)
		    {
		      if (atob (arg))
			{
			  filec +=
			    "  void " + WriteEvents (ctname, line) + ";\n";
			};
		    };
		};
	    };

	  fprintf(file,"  /*#Events*/\n");
	  fprintf(file,"%s",filec.c_str());
	  fprintf(file,"  /*#Others*/\n");
	  fprintf(file,"  CWindow%i(void);\n};\n extern CWindow%i Window%i ;\n",WN,WN,WN);
	  fprintf(file,"\n#endif /*#CWINDOW%i*/\n\n",WN);
	  fclose (file);
	}
      else
	{
	  //update file
	  linesbak.Clear();
	  linesbak.LoadFromFile(filename + ".bak");
	  fbak=fopen ((filename + ".bak").c_str (),"r");
	  file=fopen (filename.c_str (),"w");


	  while (fgetline (fbak, lbak))
	    {
		  int pos = lbak.compare ("  /*#Controls*/");
		  int pos1 = lbak.compare ("#endif /*#CWINDOW"+itoa(WN)+"*/");
		  if (pos1 == 0)break;
		  if (pos == 0)
		{
		  int pos2 = 1;
		  while (fgetline (fbak, lbak) && pos2)
		    {
		      pos2 = lbak.compare ("  /*#Others*/");
		    };

	  filec="";
	  List = GetContext ();
	  for (uint a = 0; a < List.GetLinesCount (); a++)
	    {
	      line = List.GetLine (a);
	      ctype = eqparse (line, arg);
	      if (ctype.compare ("event") == 0)
		{
		  if (atob (arg))
		    {
		      filec += "  void " + WriteEvents ("", line) + ";\n";
		    };
		};
		};
		  fprintf(file,"  /*#Controls*/\n");
		  for (int count = 0; count <= ChildCount; count++)
		    {
		      controle = Child[count];
		      ctname = controle->GetName ();
		      fprintf(file,"  %s %s;\n", controle->GetClass ().c_str(), ctname.c_str());
		      List = controle->GetContext ();
		      for (uint a = 0; a < List.GetLinesCount (); a++)
			{
			  line = List.GetLine (a);
			  ctype = eqparse (line, arg);
			  if (ctype.compare ("event") == 0)
			    {
			      if (atob (arg))
				{
				  filec +=
				    "  void " + WriteEvents (ctname,
							     line) + ";\n";
				};
			    };
			};
		    };

		  fprintf(file,"  /*#Events*/\n");
		  fprintf(file,"%s",filec.c_str());
		  fprintf(file,"  /*#Others*/\n");
		};
	      fprintf(file,"%s\n",lbak.c_str());
	    };
	      fprintf(file,"#endif /*#CWINDOW%i*/\n\n",WN);

	  fclose (file);
	  fclose (fbak);
	  remove((filename + ".bak").c_str ());
	};

//window.cc      
      filename = dirname + "/" + basename + itoa (WN) + ".cc";
      int fexist = rename (filename.c_str (), (filename + ".bak").c_str ());

      if (fexist != 0)
	{
	  //new file              
	  file=fopen (filename.c_str (),"w");
	 // fprintf(file, "#ifndef CWINDOW%i \n#define CWINDOW%i\n\n#include<lxrad.h>\n\nclass CWindow%i:public CWindow\n{\n  public:\n",WN,WN,WN);
	  List = GetContext ();
	  for (uint a = 0; a < List.GetLinesCount (); a++)
	    {
	      line = List.GetLine (a);
	      ctype = eqparse (line, arg);
	      if (ctype.compare ("event") == 0)
		{
		  if (atob (arg))
		    {
		      filec2 +=
			"void\nCWindow" + itoa (WN) + "::" + WriteEvents ("",
									  line)
			+ "\n{\n  //code here:)\n  mprint(\"_" + line +
			"\\n\");\n};\n\n";
		    };
		};
	    };
	  //fprintf(file,"  /*#Controls*/\n");
	  for (int count = 0; count <= ChildCount; count++)
	    {
	      controle = Child[count];
	      ctname = controle->GetName ();
	      //fprintf(file,"  %s %s;\n",controle->GetClass ().c_str(),ctname.c_str());

	      List = controle->GetContext ();
	      for (uint a = 0; a < List.GetLinesCount (); a++)
		{
		  line = List.GetLine (a);
		  ctype = eqparse (line, arg);
		  if (ctype.compare ("event") == 0)
		    {
		      if (atob (arg))
			{
			  filec2 +=
			    "void\nCWindow" + itoa (WN) + "::" +
			    WriteEvents (ctname,
					 line) +
			    "\n{\n  //code here:)\n  mprint(\"" + ctname +
			    "_" + line + "\\n\");\n};\n\n";
			};
		    };
		};
	    };

	  fprintf(file,"\n#include\"%s%i.h\"\n#include\"%s%i_d.cc\"\n\nCWindow%i Window%i;\n",PName.c_str(),WN,PName.c_str(),WN,WN,WN);
	  fprintf(file,"\n//Implementation\n");
	  fprintf(file,"\n%s",filec2.c_str());
	  fprintf(file,"\n\n");
	  fclose (file);
	}
      else
	{
	  //update file
	  linesbak.Clear();
	  linesbak.LoadFromFile(filename + ".bak");
	  fbak=fopen ((filename + ".bak").c_str (),"r");
	  file=fopen (filename.c_str (),"w");


	  while (fgetline (fbak, lbak))
	    {
		  int pos = lbak.compare ("//Implementation");
		  if (pos == 0)
		  {
	  filec2="";
	  List = GetContext ();
	  for (uint a = 0; a < List.GetLinesCount (); a++)
	    {
	      line = List.GetLine (a);
	      ctype = eqparse (line, arg);
	      if (ctype.compare ("event") == 0)
		{
		  if (atob (arg))
		    {
			if(!testline("CWindow" + itoa (WN) + "::" + WriteEvents ("",line)))
			{
		      filec2 +=
			"void\nCWindow" + itoa (WN) + "::" + WriteEvents ("",
									  line)
			+ "\n{\n  //code here:)\n  mprint(\"_" + line +
			"\\n\");\n};\n\n";
		    };
		};
	    };
		};
		  for (int count = 0; count <= ChildCount; count++)
		    {
		      controle = Child[count];
		      ctname = controle->GetName ();
		      List = controle->GetContext ();
		      for (uint a = 0; a < List.GetLinesCount (); a++)
			{
			  line = List.GetLine (a);
			  ctype = eqparse (line, arg);
			  if (ctype.compare ("event") == 0)
			    {
			      if (atob (arg))
				{
			   if(!testline("CWindow" + itoa (WN) + "::" + WriteEvents (ctname,line)))
			      {
				  filec2 +=
				    "void\nCWindow" + itoa (WN) + "::" +
				    WriteEvents (ctname,
						 line) +
				    "\n{\n  //code here:)\n  mprint(\"" +
				    ctname + "_" + line + "\\n\");\n};\n\n";
				  };
				};
			    };
			};
		    };
		};
	      fprintf(file,"%s\n",lbak.c_str());
	    };
	      fprintf(file,"%s",filec2.c_str());

	  fclose (file);
	  fclose (fbak);
	  remove((filename + ".bak").c_str ());
	};


//window description
      filename = dirname + "/" + basename + itoa (WN) + "_d.cc";
      file=fopen (filename.c_str (),"w");
      fprintf(file, "CWindow%i::CWindow%i(void)\n{\n",WN,WN);
      fprintf(file,"  SetFOwner(this);\n");
      List = GetContext ();
      for (uint a = 0; a < List.GetLinesCount (); a++)
	{
	  line = List.GetLine (a);
	  ctype = eqparse (line, arg);
	  if (ctype.compare ("event") != 0)
	    {
	      if ((ctype.compare ("String") == 0)||(ctype.compare ("StringList") == 0)||(ctype.compare ("File")==0))
	      {
		fprintf(file,"  Set%s(\"%s\");\n",line.c_str(),arg.c_str());
	      }
		else
		{
		      if((ctype.compare("SubMenu") != 0)&&(ctype.compare("PoupMenu") != 0))	  
		        fprintf(file,"  Set%s(%s);\n", line.c_str(),arg.c_str());
		      else
			if(arg.compare("NULL") != 0)      
		          fprintf(file,"  Set%s(&%s);\n", line.c_str(),arg.c_str());
		        else
		          fprintf(file,"  Set%s(NULL);\n", line.c_str());
		};
	    }
	  else
	    {
	      if (atob (arg))
		fprintf(file,"  %s", WriteControlEvents ("", line).c_str());
	    };
	};


      for (int count = 0; count <= ChildCount; count++)
	{
	  controle = Child[count];
	  ctname = controle->GetName ();
	  fprintf(file,"  //%s\n",ctname.c_str());
	  fprintf(file,"  %s.SetFOwner(this);\n",ctname.c_str());
	  List = controle->GetContext ();
	  for (uint a = 0; a < List.GetLinesCount (); a++)
	    {
	      line = List.GetLine (a);
	      ctype = eqparse (line, arg);
	      if (ctype.compare ("event") != 0)
		{
	      if ((ctype.compare ("String") == 0)||(ctype.compare ("StringList") == 0)||(ctype.compare ("File")==0)||(ctype.compare ("MenuItens") == 0))
	      {
		    if (arg.size () > 0)
		      fprintf(file,"  %s.Set%s(\"%s\");\n",ctname.c_str(),line.c_str(),arg.c_str());
		    else
		      fprintf(file,"  %s.Set%s(\"\");\n", ctname.c_str(),line.c_str());
	      }
		    else
		    {
		      if((ctype.compare("SubMenu") != 0)&&(ctype.compare("PoupMenu") != 0))	  
		        fprintf(file,"  %s.Set%s(%s);\n", ctname.c_str(),line.c_str(),arg.c_str());
		      else
			if(arg.compare("NULL") != 0)      
		          fprintf(file,"  %s.Set%s(&%s);\n", ctname.c_str(),line.c_str(),arg.c_str());
		        else
		          fprintf(file,"  %s.Set%s(NULL);\n", ctname.c_str(),line.c_str());
		    }
		}
	      else
		{
		  if (atob (arg))
		    fprintf(file,"  %s.%s", ctname.c_str(), WriteControlEvents (ctname,line).c_str());
		};
	    };
	  if(ctname.find("filedialog") < 0)
	  {
	    int i=ctname.find("_");	  
	    if(i < 0)	  
	      fprintf(file,"  CreateChild(& %s);\n",ctname.c_str());
	    else
	      fprintf(file,"  %s.CreateChild(& %s);\n",ctname.substr(0,i).c_str(),ctname.c_str());
	  };
	};
      fprintf(file,"};\n");
      fclose (file);
};

void
CWindow2::MakeProject (lxString basename)
{
  lxString filename, ctname;
  FILE* file;
  CStringList List;
  CControl *controle;


  filename = basename + ".prj_lxrad";
  file=fopen (filename.c_str (),"w");
  fprintf(file,"Version=%s;String\n",Version.c_str());
  fprintf(file,"PName=%s;String\n",PName.c_str());
  fprintf(file,"POptions=%s;String\n",POptions.c_str());
  fprintf(file,"PNW=%i;int\n",PNW);
  fclose (file);

  filename = basename + "_" + itoa (WN) + ".lxrad";
  file=fopen (filename.c_str (),"w");
  fprintf(file,"LXRAD Context\n\n");
  fprintf(file,"#Window%i\n",WN);
  List = GetContext ();
  for (uint a = 0; a < List.GetLinesCount (); a++)
    {
      fprintf(file,"%s\n",List.GetLine (a).c_str());
    };

  for (int count = 0; count <= ChildCount; count++)
    {
      controle = Child[count];
      ctname = controle->GetName ();
      fprintf(file,"#%%%s\n", controle->GetClass().c_str());

      List = controle->GetContext ();
      for (uint a = 0; a < List.GetLinesCount (); a++)
	{
	  fprintf(file,"%s\n",List.GetLine (a).c_str());
	};
    };
  fprintf(file,"\n\n");
  fclose (file);
};


bool
CWindow2::LoadProject (lxString dirname, lxString filename)
{
/*
  lxString dirname ="./"+filename.substr(0,filename.find(".lxrad"));
  cout<<"file= "<<filename<<endl;
  cout<<"dir = "<<dirname<<endl;
*/
  lxString basename, name;
  FILE* file1,*file2;
  lxString line, arg;
  CStringList list;
  int pos;
/*
  int pos = dirname.rfind ("/");
  if (pos >= 0)
    {
      basename = dirname + dirname.substr (pos, dirname.size () - 1);
    }
  else
    {
      basename = dirname + "/" + dirname;
    };
 */

  pos= filename.find (".prj_lxrad");

  if(pos <= 0)
  {
     Message ("Invalid File!");
     return false;
  };
  
  basename = dirname + "/" + filename.substr (0,pos);
  name = basename + ".prj_lxrad";


  file1=fopen (name.c_str (),"r");
  if (file1)
    {
      while (fgetline (file1, line))
	{
	  eqparse (line, arg);
	  if (line.compare ("PName") == 0)
	    PName = arg;
	  if (line.compare ("POptions") == 0)
	    POptions = arg;
	  if (line.compare ("PNW") == 0)
	    PNW = atoi (arg);
	  //  if (line.compare ("Version") == 0)
	};
      fclose (file1);
    }
  else
    {
      Message ("File not Found!");
      return false;
    };

  if ((WN > PNW) && (WN == PNW + 1))
    {
      PNW++;
    }
  else
    {
      if (WN > PNW)
	{
	  Message ("Window Number Invalid!");
	  return false;
	};
    };

  basename += "_" + itoa (WN) + ".lxrad";
  file2=fopen (basename.c_str (),"r");
  
  if (file2)
    {
  while (fgetline (file2, line))
    {    
      pos = line.find ("#");
      if (pos >= 0)
	{
	  list.Clear ();
	  do
	    {
	      fgetline (file2, line);
	      pos = line.find ("#%");
	      if (pos < 0)
		list.AddLine (line);
	    }
	  while ((pos < 0) && (line.size () > 0));
	  {
	  SetContext (list);
	  };
	  while (line.size () > 0)
	    {
	      lxString controlclass;
	      list.Clear ();
	      controlclass = line.substr (3, line.size ());
	      CControl *ncontrol = newcontrol (controlclass, this);
	      ncontrol->SetCanFocus (true);
	      ncontrol->SetFOwner (this);
              if(ncontrol->GetClass().compare("CItemMenu") == 0)
              {
	         ncontrol->SetVisible (false, false);
              };
	      ncontrol->EvMouseButtonPress =
		EVMOUSEBUTTONPRESS & CWindow2::GMouseButtonPress;
	      ncontrol->EvMouseButtonRelease =
		EVMOUSEBUTTONRELEASE & CWindow2::GMouseButtonRelease;
	      ncontrol->EvMouseMove = EVMOUSEMOVE & CWindow2::GMouseMove;
	      ncontrol->EvKeyboardPress =
		EVKEYBOARDPRESS & CWindow2::GKeyboardPress;
	      do
		{
		  fgetline (file2, line);
		  pos = line.find ("#%");
		  if (pos < 0)
		    list.AddLine (line);
		}
	      while ((pos < 0) && (line.size () > 0));
	      SCreateChild (ncontrol);
	      ncontrol->SetContext (list);
	    }
	};
    };
    fclose(file2);
    }
  else
    {
      Message ("File not Found!");
      return false;
    };
   
   ListPropierties(this);
   return true;
};


void
CWindow2::Window2MouseButtonClick (CControl * control, uint button, uint x,
				   uint y, uint state)
{
  if (operation.compare ("editar") == 0)
    {
      if (button == 1)
	{
	  Move = false;
	}
    }
  else
    {
      CControl *ncontrol = newcontrol (operation, this);
      ncontrol->SetVisible (false, false);
      ncontrol->SetX (x);
      ncontrol->SetY (y);
      ncontrol->SetVisible (true, false);
      
      if(ncontrol->GetClass().compare("CFileDialog") != 0)
      {
        ncontrol->SetCanExecuteEvent (true);
        ncontrol->SetCanFocus (true);
      }
      else
      {      
        ncontrol->SetCanExecuteEvent (false);
        ncontrol->SetCanFocus (false);
        ncontrol->SetVisible (false, false);
      };
      
      if(ncontrol->GetClass().compare("CItemMenu") == 0)
      {
	 ncontrol->SetVisible (false, false);
      }
      ncontrol->SetFOwner (this);
      ncontrol->EvMouseButtonPress =
	EVMOUSEBUTTONPRESS & CWindow2::GMouseButtonPress;
      ncontrol->EvMouseButtonRelease =
	EVMOUSEBUTTONRELEASE & CWindow2::GMouseButtonRelease;
      ncontrol->EvMouseMove = EVMOUSEMOVE & CWindow2::GMouseMove;
      ncontrol->EvKeyboardPress = EVKEYBOARDPRESS & CWindow2::GKeyboardPress;
      SCreateChild (ncontrol);
      ncontrol->Draw ();
      operation = "editar";
    };
};

void
CWindow2::ListPropierties (CControl * control)
{
  lxString line, arg, ctype;
  CStringList cList;
  cList.Clear ();
  cList = control->GetContext ();
  Window3.DestroyChilds ();
  Window4.DestroyChilds ();
 
  Window6.combo1.DeleteItems();
  for(int i=0;i <= GetChildCount();i++)
     Window6.combo1.AddItem(GetChild(i)->GetName());
  Window6.combo1.AddItem(GetName());
  Window6.combo1.SetSelectedItem(control->GetName());	
  
  int b = 0, c = 0;
  for (uint a = 0; a < cList.GetLinesCount (); a++)
    {
      line = cList.GetLine (a);
      ctype = eqparse (line, arg);

      if (ctype.compare ("event") == 0)
	{
	  CCheckBox *control1;
	  control1 = new CCheckBox;
	  control1->SetName ("ccheckbox" + itoa (b + 1));
	  control1->SetX (15);
	  control1->SetY (3 + 22 * b);
	  control1->SetWidth (150);
	  control1->SetCheck (atob (arg));
	  control1->SetFOwner (this);
	  control1->EvMouseButtonPress =
	    EVMOUSEBUTTONPRESS & CWindow4::checkboxclick;
	  control1->SetText (line);
	  Window4.CreateChild (control1);
	  b++;
	}
      else
	{
	  CLabel *control1;
	  control1 = new CLabel;
	  control1->SetText ("");
	  control1->SetName ("clxString" + itoa (c + 1));
	  control1->SetX (0);
	  control1->SetWidth (85);
	  control1->SetY (3 + 22 * c);
	  control1->SetText (line);
	  Window3.CreateChild (control1);

	  CEdit *control2;
	  control2 = new CEdit;
	  control2->SetName ("cedit" + itoa (c + 1));
	  control2->SetX (85);
	  control2->SetY (3 + 22 * c);
	  control2->SetFOwner (&Window3);
	  control2->EvOnFocusOut = EVONFOCUSOUT & CWindow3::editfocusout;
	  control2->SetText (arg);
	  Window3.CreateChild (control2);
	  
	  if(ctype.compare("File") ==0)
	  {
	  CToolButton *control3;
	  control3 = new CToolButton;
	  control3->SetName ("ctoolbutton" + itoa (c + 1));
	  control3->SetX (145);
	  control3->SetY (3 + 22 * c);
          control3->SetWidth(20);
	  control3->SetHeight(20);
	  control3->SetTag(c+1);
	  control3->SetFOwner (&Window3);
	  control3->EvMouseButtonRelease= EVMOUSEBUTTONRELEASE & CWindow3::FileMouseButtonRelease;
	  control3->EvOnFocusOut = EVONFOCUSOUT & CWindow3::editfocusout;
	  Window3.CreateChild (control3);
	  };
	  if(ctype.compare("StringList") ==0)
	  {
	  control2->SetText ("CList");
	  control2->SetReadOnly (true);
	  CToolButton *control3;
	  control3 = new CToolButton;
	  control3->SetName ("ctoolbutton" + itoa (c + 1));
	  control3->SetX (145);
	  control3->SetY (3 + 22 * c);
          control3->SetWidth(20);
	  control3->SetHeight(20);
	  control3->SetTag(c+1);
	  control3->SetFOwner (&Window3);
	  control3->EvMouseButtonRelease= EVMOUSEBUTTONRELEASE & CWindow7::StringListMouseButtonRelease;
	  control3->EvOnFocusOut = EVONFOCUSOUT & CWindow3::editfocusout;
	  Window3.CreateChild (control3);
	  };
	  if(ctype.compare("MenuItens") ==0)
	  {
	  //control2->SetText ("CMenuItens");
	  control2->SetReadOnly (true);
	  CToolButton *control3;
	  control3 = new CToolButton;
	  control3->SetName ("ctoolbutton" + itoa (c + 1));
	  control3->SetX (145);
	  control3->SetY (3 + 22 * c);
          control3->SetWidth(20);
	  control3->SetHeight(20);
	  control3->SetTag(c+1);
	  control3->SetFOwner (&Window3);
	  control3->EvMouseButtonRelease= EVMOUSEBUTTONRELEASE & CWindow8::MenuItensMouseButtonRelease;
	  control3->EvOnFocusOut = EVONFOCUSOUT & CWindow3::editfocusout;
	  Window3.CreateChild (control3);
	  };
	  if(ctype.compare("SubMenu") ==0)
	  {
	  //control2->SetText ("SubMenu");
	  control2->SetReadOnly (true);
	  CToolButton *control3;
	  control3 = new CToolButton;
	  control3->SetName ("ctoolbutton" + itoa (c + 1));
	  control3->SetX (145);
	  control3->SetY (3 + 22 * c);
          control3->SetWidth(20);
	  control3->SetHeight(20);
	  control3->SetTag(c+1);
	  control3->SetFOwner (&Window3);
	  control3->EvMouseButtonRelease= EVMOUSEBUTTONRELEASE & CWindow9::SubMenuMouseButtonRelease;
	  control3->EvOnFocusOut = EVONFOCUSOUT & CWindow3::editfocusout;
	  Window3.CreateChild (control3);
	  };
	  if(ctype.compare("PoupMenu") ==0)
	  {
	  //control2->SetText ("PoupMenu");
	  control2->SetReadOnly (true);
	  CToolButton *control3;
	  control3 = new CToolButton;
	  control3->SetName ("ctoolbutton" + itoa (c + 1));
	  control3->SetX (145);
	  control3->SetY (3 + 22 * c);
          control3->SetWidth(20);
	  control3->SetHeight(20);
	  control3->SetTag(c+1);
	  control3->SetFOwner (&Window3);
	  control3->EvMouseButtonRelease= EVMOUSEBUTTONRELEASE & CWindow9::SubMenuMouseButtonRelease;
	  control3->EvOnFocusOut = EVONFOCUSOUT & CWindow3::editfocusout;
	  Window3.CreateChild (control3);
	  };

	  c++;
	};
    };
  Window3.Draw ();
  Window4.Draw ();
  Window3.Update ();
  Window4.Update ();

};

void 
CWindow2::SCreateChild(CControl * control)
{
  CreateChild(control);

  if(control->GetClass().compare("CPMenu") == 0)
  {
  CControl *ctrl;

  for(int c=0;c<=ChildCount;c++)
  { 
    if(Child[c]->GetClass().compare("CPMenu") != 0)
    {
	  ctrl=Child[c];
	  Child[c]=Child[ChildCount];
	  Child[ChildCount]=ctrl;
	  break;
    };
  };
  };
};

void
CWindow2::GKeyboardPress (CControl * control, uint key, uint x, uint y,
			  uint mask)
{
  KeySym dkey = XkbKeycodeToKeysym (Window2.GetADisplay (), key, 1,0);


  if (mask & ShiftMask)
    switch (dkey)
      {
/*
case XK_Delete:
cout<<"delete\n";	
break;
*/
      case XK_Right:
	control->SetX (control->GetX () + 1);
	break;
      case XK_Left:
	control->SetX (control->GetX () - 1);
	break;
      case XK_Down:
	control->SetY (control->GetY () + 1);
	break;
      case XK_Up:
	control->SetY (control->GetY () - 1);
	break;
      };

  if (mask & ControlMask)
    switch (dkey)
      {
      case XK_Right:
	control->SetWidth (control->GetWidth () + 1);
	break;
      case XK_Left:
	control->SetWidth (control->GetWidth () - 1);
	break;
      case XK_Down:
	control->SetHeight (control->GetHeight () + 1);
	break;
      case XK_Up:
	control->SetHeight (control->GetHeight () - 1);
	break;
      };
};

void
CWindow2::GMouseButtonPress (CControl * control, uint button, uint x, uint y,
			     uint state)
{
  if (button == 1)
    {
      Move = true;
      xp = control->GetX () - x;
      yp = control->GetY () - y;
    };
};

void
CWindow2::GMouseButtonRelease (CControl * control, uint button, uint x,
			       uint y, uint state)
{

  if ((button == 1) && (Move))
    {
      Move = false;
      ListPropierties (control);
      if (control->GetClass ().compare ("CWindow") != 0)
	{
	  if (xo + yo != -2)
	    {
	      Window2.Paint->InitDraw(&Window2);	    
	      Window2.Paint->Frame ( xo + xp, yo + yp,
				    control->GetWidth (),
				    control->GetHeight ());
	      xo = -1;
	      yo = -1;
	    };
	  Window2.Paint->Pen.SetPen (GXcopy);
	  //control->Eraser ();
	  control->SetVisible (false);
	  control->SetX (x + xp);
	  control->SetY (y + yp);
	  control->SetVisible (true);
	  //control->Draw ();
	  //Window2.Update ();
	  Window2.Draw();
	};
    }
};


void
CWindow2::GMouseMove (CControl * control, uint x, uint y)
{
  if (Move)
    {
      Window2.Paint->InitDraw(&Window2);	    
      Window2.Paint->Pen.SetPen (GXinvert);
      if (xo + yo != -2)
	Window2.Paint->Frame ( xp + xo, yp + yo,
				   control->GetWidth (),
				   control->GetHeight ());
      Window2.Paint->Frame ( xp + x, yp + y,
				 control->GetWidth (), control->GetHeight ());
      
      Window2.Update ();
      xo = x;
      yo = y;
    };
};

lxString
CWindow2::WriteEvents (lxString name, lxString event)
{
  int pos = 0;
  lxString out = name + "_" + event;

  pos = event.find ("Mouse");
  if (pos >= 0)
    {
      out += "(CControl * control, uint button, uint x, uint y,uint state)";
    }
  else
    {
      pos = event.find ("Keyboard");
      if (pos >= 0)
	out += "(CControl * control, uint key, uint x, uint y,uint mask)";
      else
	out += "(CControl * control)";
    };
  return out;
};




lxString
CWindow2::WriteControlEvents (lxString name, lxString event)
{
  lxString SWN = itoa (WN);
  lxString
    out =
    event + "=" + uppercase (event) + " & CWindow" + SWN + "::" + name + "_" +
    event + ";\n";
  return out;
};

