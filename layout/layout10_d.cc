CSplash::CSplash (void)
{
  SetName ("splash1");
  SetX (250);
  SetY (200);
  SetWidth (302);
  SetHeight (202);
//  SetTitle ("Splash");
  SetCanDestroy ("true");
  SetOverrideRedirect (true);
  SetVisible(true);
//draw1 
  draw1.SetName ("draw1");
  draw1.SetX (1);
  draw1.SetY (1);
  draw1.SetWidth (300);
  draw1.SetHeight (200);
  draw1.SetPixmapFileName (lxString(_SHARE)+"/splash.xpm");
  CreateChild (&draw1);
};
