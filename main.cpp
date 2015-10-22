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




// GLOBAL vars
int window_width = 800;
int window_height = 600;


// check key inputs
int keys[65536];

//bool pausegame = false;
bool killmovement = false;

// Gordon's timer & x11/opengl code
#include "gordoncode.cpp"

// game textures/sprite draw commands
#include "textures.cpp"


// NOT used for fps counter!
// now used for sprite timer
int frames = 0;
timespec start;

int check_keys(XEvent *e, Game * game);
void check_mouse(XEvent *e, Game *game);
//void physics(Game * game);
void render(Game * game);
void makeParticle(int x, int y);

using std::cout;
using std::endl;


// TODO fix gravity when window resizes
#define GRAVITY 1
#define MAX_VELOCITY 10
#define INITIAL_VELOCITY 5



// random function
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PARTICLES 10000
Particle par[MAX_PARTICLES];
int numParticles = 0;
bool bubbler = false;
bool setbackground = false;

#include "nicholasM.cpp"


int main()
{
    initXWindows();
    init_opengl();
    loadTextures(); 

    cout << "start game" << endl;

    Game game;

    game.setGravity(GRAVITY);

    // set players position
    game.setPos(window_width/2, window_height/2);

    game.setResolution(window_width, window_height);


    srand(time(NULL));

    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    clock_gettime(CLOCK_REALTIME, &start);

    while(game.run)
    {
	// check input
	XEvent e;
	while(XPending(dpy))
	{
	    XNextEvent(dpy, &e);
	    check_keys(&e, &game);
	    check_resize(&e);
	    //check_mouse(&e, &game);
	    // if window resets, then the game should handle this event
	    game.setResolution(window_width, window_height);
	}
	//if(!pausegame)
	//{
	// asteroids timer
	// get lastest time
	clock_gettime(CLOCK_REALTIME, &timeCurrent);
	timeSpan = timeDiff(&timeStart, &timeCurrent);
	// start time is now current time;
	timeCopy(&timeStart, &timeCurrent);

	physicsCountdown += timeSpan;
	//}	// check for collisions, move player
	while(physicsCountdown >= physicsRate) {
	    physics(&game);

	    physicsCountdown -= physicsRate;
	}

	// used for sprite timing
	if(frames > 2)
	{
	    clock_gettime(CLOCK_REALTIME, &start);
	    frames = 0;
	}
	frames++;

	render(&game);
	glXSwapBuffers(dpy, win);

    }	

    cleanupXWindows();
    cleanup_fonts();

    cout << "end game" << endl;
    return 0;
}


// TODO!
// -----------------------
// Kind of working! 
// Needs to check if key pressed is later released, Example user presses LEFT then lets go,
// the code then should kill movement on the x axis
int check_keys(XEvent *e, Game * game)
{
    int key = XLookupKeysym(&e->xkey, 0);


    //killmovement = true;
    if(e->type == KeyRelease) 
    {
	keys[key] = 0;
	killmovement = true;
    }

    if(e->type == KeyPress)
    {
	keys[key] = 1;

	if(key == XK_p)
	{
	    if(pausegame)
	    {
		pausegame = false;
	    }
	    else
		pausegame = true;
	}
	
	if(!pausegame)
	{
	    if(key != XK_Left || key != XK_Right || key != XK_space)
		killmovement = false;

	    if(key == XK_b)
	    {
		if(bubbler)
		    bubbler = false;
		else
		    bubbler = true;
	    }	
	}
 	//the following keys should run even if the game is paused, until these features are removed to create a smoother game
	if(key == XK_Escape)	//so that the user can leave the game from the pause screen
	{
	    game->run = false;
	}

	if(key == XK_w)	//so that the user can change the background while it is paused
	{
	    if(setbackground)
		setbackground = false;
	    else 
		setbackground = true;
	}

    }	
    return 0;
}



void makeParticle(int x, int y) {
    if (numParticles >= MAX_PARTICLES)
	return;
    //std::cout << "makeParticle() " << x << " " << y << std::endl;
    //position of particle
    Particle *p = &par[numParticles];

    p->s.center.x = x;
    p->s.center.y = y;

    p->velocity.x =  1.0 + rnd() * 0.1;

    p->velocity.y = rnd() * 1.0 - 0.5;

    numParticles++;
}


// check for button clicks?
void check_mouse(XEvent *e, Game *game)
{

    static int savex = 0;
    static int savey = 0;
    //static int n = 0;

    if (e->type == ButtonRelease) {
	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    //Left button was pressed

	    return;
	}
	if (e->xbutton.button==3) {
	    //Right button was pressed
	    //std::cout << "right mouse b down" << std::endl;
	    return;
	}
    }
    //Did the mouse move?
    if (savex != e->xbutton.x || savey != e->xbutton.y) 
    {


    }
}


void render(Game * game)
{


    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);


    // makes sure not to draw past window edges!
    game->checkRightScreenHit();
    game->checkLeftScreenHit();


    float w, h;
    int x,y;

    // texture
    if(setbackground)
    {
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex2i(0,0);
	glTexCoord2f(0,0);
	glVertex2i(0, window_height);
	glTexCoord2f(1,0);
	glVertex2i(window_width, window_height);
	glTexCoord2f(1,1);
	glVertex2i(window_width, 0);
	glEnd();


	drawSpike();

	drawSkeleton(game);

    }


    // TEXT
    Rect r;
    r.bot = window_height - 20;
    r.left = 10;
    r.center = 0;
    //ggprint8b(&r, 16, 0x00FFFF00, "fps: %i",  static_cast<int>(frames/timeDiff(&start, &timeCurrent)));
    ggprint8b(&r, 16, 0x00FFFF00, "PhysicsRate: %i", static_cast<int>(1/physicsRate));
    ggprint8b(&r, 16, 0x00FFFF00, "water particles: %i", numParticles);
    ggprint8b(&r, 16, 0x00FFFF00, "Hit sides: %i", game->checkLeftScreenHit() || game->checkRightScreenHit());



    if(!setbackground)
    {

	//draw guy/rectangle

	glColor3ub(222,10,90);
	glPushMatrix();
	glTranslatef(game->player.position.x, game->player.position.y, 0);
	w = game->player.width;
	h = game->player.height;
	glBegin(GL_QUADS);
	glVertex2i(-w,-h);
	glVertex2i(-w, h);
	glVertex2i( w, h);
	glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//draw guy's (x,y) center coordinates
	glColor3ub(0,0,255);
	glPushMatrix();
	x = game->player.position.x;
	y = game->player.position.y;
	w = 2;
	h = 2;
	glBegin(GL_QUADS);
	glVertex2i(x-w, y-h);
	glVertex2i(x-w, y+h);
	glVertex2i(x+w, y+h);
	glVertex2i(x+w, y-h);
	glEnd();
	glPopMatrix();
    }
    // draw particles 
    int randColorWater = 0;
    for(int i = 0; i < numParticles; ++i){

	if(randColorWater == 100)
	    randColorWater = 0;
	//glColor3ub(150 ,160 ,255);// BLUE water
	glColor3ub(0+randColorWater ,0+randColorWater ,255);// looks best 
	//glColor3ub(150 + randColorWater, 160 + randColorWater,255); // too light colored
	Vec *c = &par[i].s.center;
	w = 2;
	h = 2;
	glBegin(GL_QUADS);
	glVertex2i(c->x-w, c->y-h);
	glVertex2i(c->x-w, c->y+h);
	glVertex2i(c->x+w, c->y+h);
	glVertex2i(c->x+w, c->y-h);
	glEnd();
	glPopMatrix();	 
	randColorWater+= 10; 
    }





}
