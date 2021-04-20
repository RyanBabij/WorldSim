/* WorldSim: Driver_GLHooks.hpp
  #include "Driver_GLHooks.hpp"

OpenGL hooks. Catches OpenGL events and translates them for use with internal engine.
If DirectX support is implemented, then we would do the same thing with DX events.

*/
//void glutMouseWheelFunc(void*);

void shutDown(int signal = 0); // Tidy up the game and shutdown.



static void GL_mouseMove(const int mouseX, int mouseY);
static inline void GL_mouseClick (const int clickType, const int state, int mouseX, int mouseY);
void GL_display();
void GL_idle();
static void GL_reshape(const int WIDTH, const int HEIGHT);
static void GL_keyboardUpEvent(const unsigned char key, const int x, const int y);
static void GL_specialFunc(const int key, const int x, const int y);
static void GL_mouseWheel (const int /* wheel */, const int /* direction */, const int /* x */, const int /* y */);
static void GL_specialUpFunc(const int key, const int x, const int y);
static void GL_keyboardEvent(const unsigned char key, const int x, const int y);

void GL_init(int nArgs, char ** arg)
{
  
	/* Set the window position to that defined by the player. This must be put before glutInit because otherwise it will override user preferences provided by the -geometry command line arg. */
	glutInitWindowPosition(0,0);
	/* Set the window size to that defined by the player. This must be put before glutInit because otherwise it will override user preferences provided by the -geometry command line arg. */
	glutInitWindowSize(RESOLUTIONX,RESOLUTIONY);
	/* Glut needs the pointer to nArgs because it extracts args intended for GLUT. glutInit should be run before doing anything with the args. GLUT will initialist itself here. If GLUT cannot set up, the program could be terminated with an error message.
	Useful arg: -geometry 100x200+10+10 will set window 10 pixel in from the top left corner, and make the window 100px wide and 200px high. */
	glutInit(&nArgs, arg);
	
	/* Set display mode to single or double buffering, and RGBA colour mode. */
	if(DOUBLE_BUFFERING==true)
	{ glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); }
	else
	{ glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); }


	/* Set the window title. */
	/* NOTE: For some reason this needs to be after initDisplayMode or we get crashes with certain compilation options. Might be unrelated though. */
	
	std::string windowTitle = G_WINDOW_TITLE+" "+DataTools::toString(VERSION)+" "+DataTools::toString(__DATE__)+" "+DataTools::toString(__TIME__)+" "+DataTools::toString(COMPILE_COUNT);

	glutCreateWindow(windowTitle.c_str());
  
    // Load an icon
    // Stolen from: https://stackoverflow.com/questions/12748103/how-to-change-freeglut-main-window-icon-in-c
#ifdef WILDCAT_WINDOWS
   HWND hwnd = FindWindow(NULL, windowTitle.c_str()); //probably you can get the window handler in different way..
   HANDLE icon = LoadImage(NULL, "Textures/icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE | LR_COLOR);
   SendMessage(hwnd, (UINT)WM_SETICON, ICON_BIG, (LPARAM)icon);
   // MAXIMUS THE WINDOW
   if (MAXIMISE_WINDOW)
   {
      ShowWindow(hwnd, SW_SHOWMAXIMIZED);
   }

   // CHECK FREE RAM
   // The game uses a lot of RAM, so it's useful to know how much we have to work with.
   // Stolen from https://msdn.microsoft.com/en-us/library/windows/desktop/aa366589%28v=vs.85%29.aspx

   MEMORYSTATUSEX statex;
   statex.dwLength = sizeof (statex);

   GlobalMemoryStatusEx (&statex);
   std::cout<<"There is "<<statex.ullTotalPhys/1024/1024<<" MB of RAM.\n";
   std::cout<<"There is "<<statex.ullAvailPhys/1024/1024<<" MB of free RAM.\n";

   std::cout<<"There is "<<statex.ullTotalPageFile/1024/1024<<" MB of paging file.\n";
   std::cout<<"There is "<<statex.ullAvailPageFile/1024/1024<<" MB of free paging file.\n";

   std::cout<<"There is "<<statex.ullTotalVirtual/1024/1024<<" MB of virtual memory.\n";
   std::cout<<"There is "<<statex.ullAvailVirtual/1024/1024<<" MB of free virtual memory.\n";

   // Local maps currently need about 2MB of RAM each but this is likely to significantly increase
   // in some cases we might want to make use of virtual memory as well as RAM
   MAP_CACHE_SIZE = statex.ullAvailPhys/1024/1024/5;
   if ( MAP_CACHE_SIZE < 12 )
   {
      MAP_CACHE_SIZE = 12;
      std::cout<<"Warning, free RAM may be too low.\n";
   }
   std::cout<<"Setting map cache to "<<MAP_CACHE_SIZE<<" maps.\n";
#endif
  
  // Automatically set the ncores to the OS reported amount of cores (includes logical and physical)
#ifdef AUTO_SET_CORES
   N_CORES = std::thread::hardware_concurrency();
	if (N_CORES>MAX_CORES)
	{ N_CORES=MAX_CORES; }
#endif
  
	/* Set perspective? */
	/* Set window and call reshape. */
	//glutSetWindow(glutGetWindow());
	//glutReshapeWindow(RESOLUTIONX,RESOLUTIONY);
	//glViewport(0, 0, RESOLUTIONX, RESOLUTIONY);
	
	/* Initialise GLEW. OpenGL Extension Wrangler Library. Provides extended functions like mipmap generation. Not entirely stable. Must be called after OpenGL is intialised. */
	/* BUG: Glew seems to crash my netbook when generating mipmaps even though it claims to load properly. My guess is that it expects ^2 textures. */
	//GLenum err = glewInit();
	//if (GLEW_OK != err)
	//{ fprintf(stderr, "GLEW didn't load. Error: %s\n", glewGetErrorString(err));
	//} fprintf(stdout, "GLEW version: %s\n", glewGetString(GLEW_VERSION));
	
	/* Performance tweaks. Hints can be GL_FASTEST, GL_NICEST, or GL_DONT_CARE. */
	glHint(GL_FOG_HINT, GL_FASTEST);
		glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_FASTEST); /* GL version 2.0 or greater only. */ /* GLEW */
		glHint(GL_GENERATE_MIPMAP_HINT, GL_FASTEST); /* GL version 1.4 or greater only. */ /* GLEW */
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST); /* GL version 1.3 or greater only. */ /* GLEW */

	/* Define OpenGL features. */
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable (GL_BLEND); /* Allows transparency */
	glEnable(GL_TEXTURE_2D);
	
	glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); /* Transparency function */
	glShadeModel(GL_FLAT); /* GL_FLAT or GL_SMOOTH Stops shit from blurring. Alternative is GL_SMOOTH. */
	
	/* I don't think this actually does anything. */
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_AUTO_NORMAL);
	//glDisable(GL_CLIP_PLANE); /* GLEW */
	glDisable(GL_COLOR_LOGIC_OP);
	glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_COLOR_SUM); /* GLEW */
		glDisable(GL_COLOR_TABLE); /* GLEW */
	glDisable(GL_DITHER);
	glDisable(GL_FOG);
		glDisable(GL_HISTOGRAM); /* GLEW */
	glDisable(GL_LIGHTING);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POINT_SMOOTH);
		glDisable(GL_POINT_SPRITE); /* GLEW */
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_3D); /* GLEW */
	
	
	Renderer::setTextureMode();
	

	/* OPENGL VERSION? */
	std::cout<<"OpenGL version: "<<glGetString(GL_VERSION)<<".\n";
	
	/* Function pointers for OpenGL to call. */
	glutDisplayFunc(GL_display);
	glutReshapeFunc(GL_reshape);
	glutMouseFunc(GL_mouseClick);
    glutIdleFunc(GL_idle);
    glutPassiveMotionFunc(GL_mouseMove);
	glutMotionFunc(GL_mouseMove);
	glutKeyboardFunc(GL_keyboardEvent);
	glutKeyboardUpFunc(GL_keyboardUpEvent);
	glutSpecialFunc(GL_specialFunc);
	glutSpecialUpFunc(GL_specialUpFunc);
	glutMouseWheelFunc(GL_mouseWheel);
	
	/* Set screen background to violet. */
	//glClearColor(127,0,255,255);
	/* Set current colour to white. Alpha 255 is fully opaque. Alpha 0 is fully transparent. */
	glColor4ub(255, 255, 255, 255);

  RENDER_NEXT_FRAME=true;

}


// Important special keys:
// Arrows

static void GL_specialUpFunc(const int key, const int x, const int y)
{
 //std::cout<<"s Key: "<<(int)key<<".\n";
  playerKeypressTimer.start();
	globalKeyboard.specialKeyUp(key);
  
  if ( globalKeyboard.isPressed(Keyboard::LEFT_CTRL) == false && globalKeyboard.isPressed(Keyboard::RIGHT_CTRL) == false)
  {
    globalMouse.ctrlPressed=false;
  }
  RENDER_NEXT_FRAME=true;
}
static void GL_specialFunc(const int key, const int x, const int y)
{
  playerKeypressTimer.start();
	globalKeyboard.specialKeyDown(key);
  
  if ( globalKeyboard.isPressed(Keyboard::LEFT_CTRL) || globalKeyboard.isPressed(Keyboard::RIGHT_CTRL))
  {
    globalMouse.ctrlPressed=true;
  }
  
  if (activeMenu == MENU_ADVENTUREMODE )
  {
    menuAdventureMode.keyboardEvent(&globalKeyboard);
  }
  else if (activeMenu == MENU_WORLDSIMULATOR )
  {
    menuWorldSimulator.keyboardEvent(&globalKeyboard);
  }
  
	globalGuiManager.keyboardEvent(&globalKeyboard);
  RENDER_NEXT_FRAME=true;
}

static void GL_reshape(const int WIDTH, const int HEIGHT)
{
	/* Called whenever window size changes. */
	/* Update global window coordinates. */
	RESOLUTIONX=WIDTH;
	RESOLUTIONY=HEIGHT;

	/* Update projection matrix */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* Viewport should match window dimensions. */
	glViewport(0, 0,RESOLUTIONX, RESOLUTIONY);
	gluOrtho2D(0.0, RESOLUTIONX, 0.0, RESOLUTIONY);

	/* Go back to default matrix mode. */
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	
	/* Update GUI coords. */
	menuTitle.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
  menuTitle.eventResize();
	menuOptions.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
  menuOptions.eventResize();
	menuLoadGame.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
  menuLoadGame.eventResize();
	menuWorldGenerator.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
  menuWorldGenerator.eventResize();
	menuWorldSimulator.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
  menuWorldSimulator.eventResize();
	menuAdventureMode.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
  menuAdventureMode.eventResize();
  RENDER_NEXT_FRAME=true;
}


unsigned char lastKey=0;
bool waitingForKeyPress=true;


static void GL_keyboardUpEvent(const unsigned char key, const int x, const int y)
{
   playerKeypressTimer.start();

   if ( key==Keyboard::LEFT_CTRL || key==Keyboard::RIGHT_CTRL)
   {
      globalMouse.ctrlPressed=false;
   }

   globalKeyboard.keyUp(key);
   waitingForKeyPress=true;
   RENDER_NEXT_FRAME=true;
   //mainRender.keyUp(key,x,y); waitingForKeyPress=true;
}

static void GL_keyboardEvent(const unsigned char key, const int x, const int y)
{
  //std::cout<<"Key: "<<(int)key<<".\n";
  playerKeypressTimer.start();
  globalKeyboard.keyDown(key);
  
  if(key==Keyboard::TILDE) // SHIFT+TILDE
  {
    HOTKEYS_ENABLED = !HOTKEYS_ENABLED;
    
    if ( HOTKEYS_ENABLED )
    {
      std::cout<<"Debug hotkeys: ENABLED.\n";
    }
    else
    {
      std::cout<<"Debug hotkeys: DISABLED.\n";
    }
    
  }

	
  
   if ( activeMenu == MENU_TITLE )
   {
      menuTitle.keyboardEvent(&globalKeyboard);
   }
   else if (activeMenu == MENU_OPTIONS )
   {
      menuOptions.keyboardEvent(&globalKeyboard);
   }
   else if (activeMenu == MENU_LOADGAME )
   {
      menuLoadGame.keyboardEvent(&globalKeyboard);
   }
   else if (activeMenu == MENU_WORLDGENERATOR )
   {
      menuWorldGenerator.keyboardEvent(&globalKeyboard);
   }
   else if (activeMenu == MENU_WORLDSIMULATOR )
   {
      menuWorldSimulator.keyboardEvent(&globalKeyboard);
   }
   else if (activeMenu == MENU_ADVENTUREMODE )
   {
      menuAdventureMode.keyboardEvent(&globalKeyboard);
   }

   if ( key==Keyboard::LEFT_CTRL || key==Keyboard::RIGHT_CTRL)
   {
      globalMouse.ctrlPressed=true;
   }
  
  // NOTE:
  // It seems globalGUIManager is not active for anything except preventing accidental
  // Hotkey input. In future all gui controls should pass through globalGUIManager.
  // It should be possible to add guiManagers to globalGUIManager.
		
	//if ( keyboardInterfaceManager.keyboardEventAll(&globalKeyboard) )
	if ( globalGuiManager.keyboardEvent(&globalKeyboard) )
	//if ( globalGuiManager.stealKeyboard() )
	{
		//std::cout<<"GL steal\n";
	}
	else if ( HOTKEYS_ENABLED == true && globalGuiManager.stealKeyboard() == false  )
	{
		//std::cout<<"Key:"<<key<<".\n";
		// if(key==Keyboard::EQUALS)
		// {
			// std::cout<<"LOGIC up.\n";
			// U_LOGIC_PER_SECOND/=2;
		// }
		// if(key==Keyboard::MINUS)
		// {
			// std::cout<<"LOGIC down.\n";
			// U_LOGIC_PER_SECOND*=2;
		//}
		// if(key==Keyboard::COMMA)
		// {
			// std::cout<<"Logic step.\n";
			// FORCE_LOGIC_FRAME=true;
		// }
		// if(key==Keyboard::PERIOD)
		// {
			// std::cout<<"Logic paused.\n";
			// PAUSE_LOGIC=!PAUSE_LOGIC;
		// }
		
		if(key==Keyboard::CTRL_C) /* CTRL_C is value 3 and works like normal keypress. */
		{
      // If we exit with CTRL+C, don't erase save data.
			CLEAN_SAVES_ON_EXIT=false;
			QUIT_FLAG=true;
		}
    
		// /* Change framerate to once per second. */
		// if(key==Keyboard::R || key==Keyboard::r)
		// {
			
			// SLOW_FRAMERATE_ACTIVE = !SLOW_FRAMERATE_ACTIVE;
			// //UFRAMERATE = 1000000/2;
		
			// /* Previous setting was to disable rendering completely,
				// but I decided to change it to render once per second. */
		
			// //DONT_RENDER = !DONT_RENDER;
			// //glClear(GL_COLOR_BUFFER_BIT);
		// }
		// if(key==Keyboard::S || key==Keyboard::s)
		// {
			// LIMIT_LOGIC = !LIMIT_LOGIC;
			// if(LIMIT_LOGIC==true)
			// { std::cout<<"Logic limited.\n"; }
			// else
			// { std::cout<<"Logic unlimited.\n"; }
		// }
		
	}
  RENDER_NEXT_FRAME=true;
}

long int logicLateness=0;
long int frameLateness=0;

void GL_idle()
{
	if(QUIT_FLAG==true)
	{
		shutDown();
	}

	pollRateTimer.update();
	if(pollRateTimer.totalUSeconds>UPOLLSPERSECOND)
	{
		pollRateTimer.start();
		//world.keyboardEvent();
		//world.mouseClick();
		//world.aiTurn();
		pollRateTimer.init();
	}
	
	animationTimer.update();
	if(animationTimer.totalUSeconds>UANIMATIONSPERSECOND)
	{
		animationTimer.start();

    ++CURRENT_ANIMATION_FRAME == 100? CURRENT_ANIMATION_FRAME=0 : 0;

    
		animationTimer.init();
	}
  
	// if(LIMIT_FRAMERATE==true && BUSY_WAIT==true)
	// {
	
		// frameRateTimer.update();
		// if(frameRateTimer.totalUSeconds<UFRAMERATE && frameRateTimer.totalUSeconds+BUSY_WAIT_USEC>=UFRAMERATE)
		// {
			// //std::cout<<"bw.\n";
			// while(frameRateTimer.totalUSeconds<UFRAMERATE)
			// {
				// frameRateTimer.update();
			// }
		// }
	// }
	
	if (SLOW_FRAMERATE_ACTIVE)
	{
		frameRateTimer.update();
		if(frameRateTimer.totalUSeconds+frameLateness>=SLOW_FRAMERATE)
		{
				/* Adjust for late frames by a maximum of half of the time between frames. */
			//frameLateness = frameRateTimer.totalUSeconds-SLOW_FRAMERATE;
			if ( frameLateness > SLOW_FRAMERATE/2 )
			{
				//frameLateness = SLOW_FRAMERATE/2;
			}
			glutPostRedisplay();
      //GL_display();
			//frameRateTimer.init();
			//frameRateTimer.start();
		}
	}
	else if(LIMIT_FRAMERATE==true)
	{
		frameRateTimer.update();
		if(frameRateTimer.totalUSeconds+frameLateness>=UFRAMERATE)
		{
				/* Adjust for late frames by a maximum of half of the time between frames. */
			//frameLateness = frameRateTimer.totalUSeconds-UFRAMERATE;
			if ( frameLateness > UFRAMERATE/2 )
			{
				//frameLateness = UFRAMERATE/2;
			}
			glutPostRedisplay();
      //GL_display();
			//frameRateTimer.init();
			//frameRateTimer.start();
		}
	}
	else
	{ glutPostRedisplay(); }
	
	/* Logic goes last to prevent hanging the rendering. */
	/* This is still a problem though. We need to make sure logic is only done when rendering doesn't need to be done soon. Preferably logic should be checked immediately after a render. */
	
	if(PAUSE_LOGIC==true)
	{
		logicLateness=0;
		logicRateTimer.start();
		logicRateTimer.init();
	}
	
	if(FORCE_LOGIC_FRAME==true)
	{
		FORCE_LOGIC_FRAME=false;
		logicRateTimer.update();
		logicLateness=0;
		
		logicTickManager.tickAll();
		logicRateTimer.start();
		logicRateTimer.init();
		
	}
	else if(LIMIT_LOGIC==true && PAUSE_LOGIC==false)
	{
		logicRateTimer.update();

		if(logicRateTimer.totalUSeconds+logicLateness>=U_LOGIC_PER_SECOND)
		{
			logicLateness = logicRateTimer.totalUSeconds+logicLateness - U_LOGIC_PER_SECOND;

			logicTickManager.tickAll();
			logicRateTimer.start();
			logicRateTimer.init();
		}

	}
	else if (PAUSE_LOGIC==false)
	{ logicTickManager.tickAll();
	}
	
		/* Use up some free time between frames to do calculations */
	idleManager.tickAll();
	
	/* relinquish CPU. Alpha freeglut does this automatically (?) */
  playerKeypressTimer.update();
	if(RELINQUISH_CPU==true && LIMIT_LOGIC==true && world.ticksBacklog<=0 && playerKeypressTimer.fullSeconds > 0.5 && NO_BACKLOG)
	{
#ifdef WILDCAT_WINDOWS
		MsgWaitForMultipleObjects( 0, NULL, FALSE, RELINQUISH_CPU_TIMEOUT, QS_ALLINPUT ); /* parameter 4 is milliseconds ie 1000 = 1 second. */
#endif
	}
	
	
}

double aFrameTime [OUTPUT_FRAMERATE_SAMPLE_SIZE];
int iFrameTime=0;

double rollingAverage=0;

void GL_display()
{
	// lock for entire scope
	//std::lock_guard<std::mutex> guard(render_mutex);
  
  if ( DONT_RENDER )
  { return; }

  float frameTime = 0;
  
  if (OUTPUT_FRAMERATE)
  {
    frameRateTimer.update();
    frameTime = frameRateTimer.fullSeconds;
  }
  
  if (SLOW_FRAMERATE_ACTIVE)
	{
		frameRateTimer.update();
    
		if(frameRateTimer.totalUSeconds+frameLateness>=SLOW_FRAMERATE)
		{
				/* Adjust for late frames by a maximum of 1/4 of the time between frames. */
			frameLateness = frameRateTimer.totalUSeconds-SLOW_FRAMERATE;
			if ( frameLateness > SLOW_FRAMERATE/4 )
			{
				frameLateness = SLOW_FRAMERATE/4;
			}
			frameRateTimer.init();
			frameRateTimer.start();
		}
	}
  else if(LIMIT_FRAMERATE)
	{
		frameRateTimer.update();
		if(frameRateTimer.totalUSeconds+frameLateness>=UFRAMERATE)
		{
				/* Adjust for late frames by a maximum of 1/4 of the time between frames. */
			frameLateness = frameRateTimer.totalUSeconds-UFRAMERATE;
			if ( frameLateness > UFRAMERATE/3 )
			{
				frameLateness = UFRAMERATE/3;
			}
      //frameLateness = 0;
      
			frameRateTimer.init();
			frameRateTimer.start();
		}
    else
    {
      return;
    }
	}


  


  if( OUTPUT_FRAMERATE )
  {
  
    /* Timer must be done like this because OpenGL threads shit which can hang the app but won't show up if the timer is checked directly after OpenGL calls. Maybe the framerate smoother could learn from this. */
    //debugTimer.update();
    //std::cout<<"Seconds per frame rolling average: "<<debugTimer.fullSeconds<<".\n";
    
    aFrameTime[iFrameTime] = frameTime;
    ++iFrameTime;
    if(iFrameTime==OUTPUT_FRAMERATE_SAMPLE_SIZE)
    {
      float totalFrameTime = 0;
      for (int i=0;i<OUTPUT_FRAMERATE_SAMPLE_SIZE;++i)
      {
        totalFrameTime+=aFrameTime[i];
      }
      totalFrameTime/=OUTPUT_FRAMERATE_SAMPLE_SIZE;
      std::cout<<"SPF: "<<totalFrameTime<<". ";
      float frameRate = 1/totalFrameTime;
      std::cout<<"FPS: "<<frameRate<<".\n";
    
      iFrameTime=0;
    }
    //debugTimer.start();
    
  }

   if ( LAZY_RENDERING==false || RENDER_NEXT_FRAME )
   {
      RENDER_NEXT_FRAME=false;

      glClear(GL_COLOR_BUFFER_BIT);

      // NEW SYSTEM FOR MENU MANAGEMENT.
      // No more complicated hierarchies or menu managers. Just one global variable.
      // Submenus are still contained within their parent menu.

      if ( activeMenu == MENU_TITLE )
      {
         menuTitle.render();
      }
      else if (activeMenu == MENU_OPTIONS )
      {
         menuOptions.render();
      }
      else if (activeMenu == MENU_LOADGAME )
      {
         menuLoadGame.render();
      }
      else if (activeMenu == MENU_WORLDGENERATOR )
      {
         menuWorldGenerator.render();
      }
      else if (activeMenu == MENU_WORLDSIMULATOR )
      {
         menuWorldSimulator.render();
      }
      else if (activeMenu == MENU_ADVENTUREMODE )
      {
         menuAdventureMode.render();
      }

      // Render everything that wants to render.
      displayInterfaceManager.renderAll();

      if(DOUBLE_BUFFERING==true)
      { glutSwapBuffers(); }
      else
      { glFlush(); }
      }
   }


static void GL_mouseWheel (const int wheel, const int direction, const int _x, const int _y)
{
  
	/* Get window size. */
	GLint windowDimensions[4];
	glGetIntegerv( GL_VIEWPORT, windowDimensions );
	/* Change mouse coordinates to match poly coordinates. (Bottom left origin).*/
	//_y=windowDimensions[3]-_y;
	/* Update globalMouse pointer coordinates. */
	globalMouse.move(_x,windowDimensions[3]-_y);

	if(direction==1)
	{ globalMouse.isWheelUp=true; globalMouse.isWheelDown=false; }
	else if (direction==-1)
	{ globalMouse.isWheelDown=true; globalMouse.isWheelUp=false; }

	/* Send mouse event to all MouseInterface objects. */
	//mouseInterfaceManager.mouseEventAll(&globalMouse);
  
  if ( activeMenu == MENU_TITLE )
  {
    menuTitle.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_OPTIONS )
  {
    menuOptions.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_LOADGAME )
  {
    menuLoadGame.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_WORLDGENERATOR )
  {
    menuWorldGenerator.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_WORLDSIMULATOR )
  {
    menuWorldSimulator.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_ADVENTUREMODE )
  {
    menuAdventureMode.mouseEvent(&globalMouse);
  }
  RENDER_NEXT_FRAME=true;
}


static inline void GL_mouseClick (const int clickType, const int state, int mouseX, int mouseY)
{
  RENDER_NEXT_FRAME=true;
	/* Remove negative mouse values. */
	if(mouseX<0) { mouseX=0; }
	if(mouseY<0) { mouseY=0; }
	/* Get window size. */
	GLint windowDimensions[4];
	glGetIntegerv( GL_VIEWPORT, windowDimensions );
	/* Change mouse coordinates to match poly coordinates. (Bottom left origin).*/
	mouseY=windowDimensions[3]-mouseY;

	/* Mouse click types. */
	if(state==GLUT_DOWN)
	{
		if(clickType==GLUT_LEFT_BUTTON) { globalMouse.isLeftClick=true; globalMouse.isLeftDown=true; }
		else if(clickType==GLUT_MIDDLE_BUTTON) { globalMouse.isMiddleClick=true; }
		else if(clickType==GLUT_RIGHT_BUTTON) { globalMouse.isRightClick=true; }
	}
	else if(state==GLUT_UP)
	{
		if(clickType==GLUT_LEFT_BUTTON) { globalMouse.isLeftClick=false;  globalMouse.isLeftDown=false; if (EMULATE_2_BUTTON_MIDDLE_CLICK) {globalMouse.isMiddleClick=false;} }
		else if(clickType==GLUT_MIDDLE_BUTTON) { globalMouse.isMiddleClick=false; }
		else if(clickType==GLUT_RIGHT_BUTTON) { globalMouse.isRightClick=false; if (EMULATE_2_BUTTON_MIDDLE_CLICK) {globalMouse.isMiddleClick=false;} }
	}
	
	if ( EMULATE_2_BUTTON_MIDDLE_CLICK && globalMouse.isLeftClick && globalMouse.isRightClick )
	{
		globalMouse.isMiddleClick=true;
		globalMouse.isLeftClick=false;
		globalMouse.isRightClick=false;
	}
	/* Update globalMouse pointer coords. */
	globalMouse.move(mouseX,mouseY);
	
	/* Send mouse event to all MouseInterface objects. */
	//mouseInterfaceManager.mouseEventAll(&globalMouse);
  
  if ( activeMenu == MENU_TITLE )
  {
    menuTitle.mouseEvent(&globalMouse);
  }
  else if ( activeMenu == MENU_OPTIONS )
  {
    menuOptions.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_LOADGAME )
  {
    menuLoadGame.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_WORLDGENERATOR )
  {
    menuWorldGenerator.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_WORLDSIMULATOR )
  {
    menuWorldSimulator.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_ADVENTUREMODE )
  {
    menuAdventureMode.mouseEvent(&globalMouse);
  }
}


/* Called if the mouse moves within the window. */
static void GL_mouseMove(const int mouseX, int mouseY)
{
	/* Get window size. */
	GLint windowDimensions[4];
	glGetIntegerv( GL_VIEWPORT, windowDimensions );
	/* Change mouse coordinates to match poly coordinates. (Bottom left origin).*/
	mouseY=windowDimensions[3]-mouseY;
	/* Update globalMouse pointer coordinates. */
	globalMouse.move(mouseX,mouseY);

	/* Send mouse event to all MouseInterface objects. */
	//mouseInterfaceManager.mouseEventAll(&globalMouse);	
  
  if ( activeMenu == MENU_TITLE )
  {
    menuTitle.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_OPTIONS )
  {
    menuOptions.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_LOADGAME )
  {
    menuLoadGame.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_WORLDGENERATOR )
  {
    menuWorldGenerator.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_WORLDSIMULATOR )
  {
    menuWorldSimulator.mouseEvent(&globalMouse);
  }
  else if (activeMenu == MENU_ADVENTUREMODE )
  {
    menuAdventureMode.mouseEvent(&globalMouse);
  }
  RENDER_NEXT_FRAME=true;
}
