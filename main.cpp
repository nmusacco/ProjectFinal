// CS335 fall 2015
// ====================
// Final Project - Group 2
// Game: Doodle Jump
// Student: Pedro Gonzalez
// 
// ---------------------
// used the asteroids game as a template
// for animation/physics


// game class
#include "game.h"
#include<iostream>
// GLOBAL vars
int window_width = 800;
int window_height = 600;


// check key inputs
int keys[65536];
bool killmovement = false;


unsigned long SCORE = 0;


// Gordon's timer & x11/opengl code
#include "gordoncode.cpp"


// NOT used for fps counter!
// now used for sprite timer
int frames = 0;
timespec start;
int fps = 0; // USED FOR FRAMES PER SECOND

timespec timeScore;

int check_keys(XEvent *e, Game * game);
void check_mouse(XEvent *e, Game *game);
void physics(Game * game);
void render(Game * game);
void makeParticle(int x, int y);

using std::cout;
using std::endl;


// TODO fix gravity when window resizes
#define GRAVITY 1
#define MAX_VELOCITY 6
#define INITIAL_VELOCITY 5

int MAIN_MENU = 1;
int RUN_GAME = 2;

int STATE = MAIN_MENU;
bool TOGGLE = true;


// random function
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PARTICLES 10000
Particle par[MAX_PARTICLES];
int numParticles = 0;
bool bubbler = false;
bool setbackground = false;

Particle blood[MAX_PARTICLES];
int numblood = 0;


#include "nadiaS.cpp" // make platform and draw function
#include "pedroG.cpp" // game textures/sprite draw functions
#include "nicholasM.cpp" // input checking, call all game methods in physics()

int main(int argc, char ** argv)
{
    // makes the game run on a full window
    /*
       Display* disp = XOpenDisplay(NULL);
       Screen*  scrn = DefaultScreenOfDisplay(disp);
       window_height = scrn->height;
       window_width = scrn->width;
       */

    initXWindows();
    init_opengl();

    loadTextures(); 
    //assert(true);
    cout << "start game" << endl;

    Game game;
    game.setGravity(GRAVITY);

    // set players position
    game.setPos(window_width/2, window_height);
    game.setResolution(window_width, window_height);
    makePlatform(5,&game);

    srand(time(NULL));

    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    clock_gettime(CLOCK_REALTIME, &start);
//    game.state = RUN_GAME;
    while(game.run)
    {
	
         
	
	while(STATE == MAIN_MENU && game.run)
	{
	   
	    XEvent menu;
	    while(XPending(dpy))
	    {
		XNextEvent(dpy, &menu);
		check_keys(&menu, &game);
		check_mouse(&menu, &game);
		game.setResolution(window_width, window_height);
	    }
	    setMenuBackground();
	    glXSwapBuffers(dpy, win);
	}
		
	
	
	while(STATE == RUN_GAME && game.run)
	{

	    // check input
	    XEvent e;
	    while(XPending(dpy))
	    {
	        if(TOGGLE)
	        {
		   TOGGLE = false;
		   pausegame = false;
	        }

		XNextEvent(dpy, &e);
		check_keys(&e, &game);
		check_resize(&e);
		//check_mouse(&e, &game);
		// if window resets, then the game should handle this event
		game.setResolution(window_width, window_height);
	    }
	    

	    

	    clock_gettime(CLOCK_REALTIME, &timeCurrent);
	    timeSpan = timeDiff(&timeStart, &timeCurrent);
	    timeCopy(&timeStart, &timeCurrent);
            
	    if(!pausegame)
	    {
	
		    SCORE++;
	    }

	    physicsCountdown += timeSpan;
	    // check for collisions, move player
	    while(physicsCountdown >= physicsRate) {
		physics(&game);

		physicsCountdown -= physicsRate;
	    }

	    // used for sprite timing DON'T TOUCH
	    if(frames > 2)
		frames = 0;
	    frames++;

	    // FPS COUNTER/RESET
	    if(fps > 100)
	    {
		clock_gettime(CLOCK_REALTIME, &start);
		fps = 0;
	    }
	    fps++;
	   
	    render(&game);
	    glXSwapBuffers(dpy, win);
	}
    }	

    cleanupXWindows();
    cleanup_fonts();

    cout << "end game" << endl;
    return 0;
}

void makeParticle(int x, int y) 
{
    if (numParticles >= MAX_PARTICLES)
	return;

    Particle *p = &par[numParticles];
    p->s.center.x = x;
    p->s.center.y = y;
    p->velocity.x =  1.0 + rnd() * 0.1;
    p->velocity.y = rnd() * 1.0 - 0.5;
    numParticles++;
}


void render(Game * game)
{

    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // makes sure not to draw past window edges!
    game->checkRightScreenHit();
    game->checkLeftScreenHit();

    // texture
    if(setbackground)
    {
	// draws background, player, spikes, missiles
	drawGame_Textures(game);
    }

    // TEXT
    Rect r;
    r.bot = window_height - 20;
    r.left = 10;
    r.center = 0;
    //ggprint8b(&r, 16, 0x00FFFF00, "fps: %i",  static_cast<int>(fps/timeDiff(&start, &timeCurrent)));
    if(!pausegame)
    {
    	ggprint8b(&r, 16, 0x00FFFF00, "PhysicsRate: %i", static_cast<int>(1/physicsRate));
    	ggprint8b(&r, 16, 0x00FFFF00, "water particles: %i", numParticles);
    	ggprint8b(&r, 16, 0x00FFFF00, "blood particles: %i", numblood);
    	ggprint8b(&r, 16, 0x00FFFF00, "Hit sides: %i", game->checkLeftScreenHit() || game->checkRightScreenHit());
    }
    ggprint8b(&r, 16, 0x00FFFF00, "Score: %i", SCORE);
    
    // debug/retrostyle mode
    if(!setbackground)
    {
	drawGame_TEST(game);
    }

    // waterfall
    drawWater();
}
