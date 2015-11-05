// cs335 Fall 2015
// ==========================
// student: Pedro Gonzalez
// ==========================
// I load textures and animate sprites 
// Used lab2_rainforest texture code as template
// then used online sources for further explaination of code/magic

// sites I used to learn some of these commands/args
// https://open.gl/textures
// OLD!!! from 2007, hardware dependent!!!
// http://www.gamedev.net/page/resources/_/technical/opengl/rendering-efficient-2d-sprites-in-opengl-using-r2429

// explaination of texture sprite coordinates
// http://stackoverflow.com/questions/11457394/texture-sampling-coordinates-to-render-a-sprite

// rotate sprite texture
// https://www.opengl.org/discussion_boards/showthread.php/165578-Rotating-a-Texture-on-a-Quad

// sprite animation helpful
// http://forum.devmaster.net/t/spritesheet-animation-in-opengl/24020

// using this site for help with missile chasing algorithm
// http://code.tutsplus.com/tutorials/hit-the-target-with-a-deadly-homing-missile--active-8933

// using this site https://convertio.co/png-ppm/ for png to ppm conversion 
// Skeleton spritesheet by MoikMellah
// http://opengameart.org/content/mv-platformer-skeleton
// platformer spritesheet by clint bellanger
// http://opengameart.org/content/platformer-animations
// metal slug 2 rocket
// http://www.spriters-resource.com/arcade/ms2/sheet/53203/

#include <math.h> // atan2();  

Ppmimage * spike = NULL;
GLuint spikeTexture; 

Ppmimage * missile = NULL;
GLuint missileTexture;

// skeleton is the player's texture
Ppmimage * skeletonBase = NULL;
GLuint skeletonTexture;

Ppmimage * guts = NULL;
GLuint gutsTexture;

Ppmimage *background = NULL;
GLuint backgroundTexture;

GLuint silhouetteTexture;
GLuint silhouetteTextureSpikes;
GLuint silhouetteTextureMissile;
GLuint silhouetteTextureGuts;

void loadTextures()
{
	// load image from ppm structure
	skeletonBase = ppm6GetImage("./images/platformer_sprites_pixelized_0.ppm");
	background = ppm6GetImage("./images/neb.ppm");
	spike = ppm6GetImage("./images/superspikes.ppm");
	missile = ppm6GetImage("./images/missile.ppm");
	guts = ppm6GetImage("./images/guts.ppm");
	
	// generate opengl texture element
	glGenTextures(1, &skeletonTexture);
	glGenTextures(1, &backgroundTexture);
	glGenTextures(1, &spikeTexture);
	glGenTextures(1, &missileTexture);
	glGenTextures(1, &gutsTexture);
	
	glGenTextures(1, &silhouetteTexture);
	glGenTextures(1, &silhouetteTextureSpikes);
	glGenTextures(1, &silhouetteTextureMissile);
	glGenTextures(1, &silhouetteTextureGuts);
	
	//////////////////// Missile //////////////////////
	glBindTexture(GL_TEXTURE_2D, missileTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, missile->width, missile->height,
	0, GL_RGB, GL_UNSIGNED_BYTE, missile->data);
	
	////////////////////// guts ////////////////////////////
	glBindTexture(GL_TEXTURE_2D, gutsTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, guts->width, guts->height,
				0, GL_RGB, GL_UNSIGNED_BYTE, guts->data);
	
	/////////////////// spikes ////////////////
	glBindTexture(GL_TEXTURE_2D, spikeTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, spike->width, spike->height,
	0, GL_RGB, GL_UNSIGNED_BYTE, spike->data);
	
	
	/////////////////// Skeleton sprite texture ////////////////////////
	
	glBindTexture(GL_TEXTURE_2D, skeletonTexture);
	// bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// write to video memory
	glTexImage2D(GL_TEXTURE_2D, 0, 3, skeletonBase->width, skeletonBase->height,
	0, GL_RGB, GL_UNSIGNED_BYTE, skeletonBase->data);

	
	//////////////////// silhouette for character///////////////////////
	glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *silhouetteData = buildAlphaData(skeletonBase);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, skeletonBase->width, skeletonBase->height, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	delete [] silhouetteData;
	
	//////////////////// silhouette for spikes///////////////////////
	glBindTexture(GL_TEXTURE_2D, silhouetteTextureSpikes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	silhouetteData = buildAlphaData(spike);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spike->width, spike->height, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	delete [] silhouetteData;
	
	//////////////////// silhouette for missile///////////////////////
	glBindTexture(GL_TEXTURE_2D, silhouetteTextureMissile);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	silhouetteData = buildAlphaData(missile);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, missile->width, missile->height, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	delete [] silhouetteData;
	
	//////////////////// silhouette for missile///////////////////////
	glBindTexture(GL_TEXTURE_2D, silhouetteTextureGuts);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	silhouetteData = buildAlphaData(guts);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, guts->width, guts->height, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	delete [] silhouetteData;
	
	
	////////////// GAME BACKGROUND ////////////////////////////
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, background->width, background->height,
	0, GL_RGB, GL_UNSIGNED_BYTE, background->data);
	
	
}

void drawBackground()
{
	//int width = background->width;
	//int height = background->height;
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex2i(0,0);
	glTexCoord2f(0,0);
	glVertex2i(0, window_height);//height/2);//window_height);
	glTexCoord2f(1,0);
	glVertex2i(window_width, window_height);//width/2, height/2);//window_width, window_height);
	glTexCoord2f(1,1);
	glVertex2i(window_width, 0);//width/2,0);//window_width, 0);
	glEnd();
}

void drawWater()
{
	int randColorWater = 0;
	for(int i = 0; i < numParticles; ++i)
	{
		if(randColorWater == 100)
			randColorWater = 0;
		//glColor3ub(255 ,160 ,255);// reD? water
		glColor3ub(0+randColorWater ,0+randColorWater ,255);// looks best 
		//glColor3ub(150 + randColorWater, 160 + randColorWater,255); // too light colored
		Vec *c = &par[i].s.center;
		float w = 2;
		float h = 2;
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

void drawThrust(float angle, float x, float y, float PI, int height)
{
	float rad = (((-angle)+90.0) / 360.0f) * PI * 2.0;
	//convert angle to a vector
	float xdir = cos(rad);
	float ydir = sin(rad);
	glColor3ub(150 ,160 ,255);
	float xs,ys,xe,ye,r;
	glBegin(GL_LINES);
	for (int i=0; i<16; i++) 
	{
		xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
		ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
		r = rnd()*40.0+  height;
		xe = -xdir * r + rnd() * 18.0 - 9.0;
		ye = -ydir * r + rnd() * 18.0 - 9.0;
		glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
		glVertex2f(x+xs, y+ys);
		glVertex2f(x+xe, y+ye);
	}
	glEnd();
}

void drawTESTspikes(Game * game)
{
	float w = 10 * game->player.width;//spike->width *
	//int w = game->player.width;
	//int h = game->player.height;
	for(int i = 0; i < window_width; i+= w)
	{
		glPushMatrix();
		glTranslatef(i, 0, 0);
		glColor3ub(255,200,1);
	
	
		glBegin(GL_TRIANGLES);
		glVertex2i(0,0); // bottom left
		glVertex2i(w , 0); // bottom right
		glVertex2i(w/2 , w ); // top middle

		glEnd();
		glPopMatrix();
	}
}

void drawTESTmissile(Game * game)
{
	int x, y, w, h;
	x = game->missiles.position.x;
	y = game->missiles.position.y;
	w = game->missiles.width;
	h = game->missiles.height;
	
	float PI = 3.14159265359;
	float angle = atan2(game->posX() - x, game->posY() - y) * 180 / PI;
	
	drawThrust(angle, x, y, PI, h/2);
	
	glColor3ub(0,0,255);
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	glRotatef(angle,0,0.0,-1.0);
	glBegin(GL_TRIANGLES);
	glVertex2i(-w/2,-h/4); // bottom left
	glVertex2i(w/2 , -h/4); // bottom right
	glVertex2i(0 , h/4 ); // top middle

	glEnd();
	glPopMatrix();
}

// makes blood particles
void makeTESTguts(Game * game)
{
	
	int vely = 4;
	if(setbackground)
		numblood = 100;
	else
	{
		numblood = 200;
		vely = 2 ;
	}
	int x = game->posX();
	int y = game->posY();
	
	//Particles &p = par;
	for(int i = 0; i < numblood; ++i)
	{
		blood[i].s.center.x = x;
		blood[i].s.center.y = y;
		blood[i].velocity.x = rnd() * 5 - rnd() * 5;
		
		blood[i].velocity.y = rnd() * game->player.height/vely - rnd() * game->player.height/vely ;;
	}
}

// draws blood particles both in debug and regular texture version of the game
void drawTESTguts(Game * game)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	// draw gut particles particles 
	int randColorWater = 0;
	for(int i = 0; i < numblood; ++i)
	{
		if(randColorWater == 100)
			randColorWater = 0;
		
		if(setbackground)
			glColor3ub(255 - randColorWater,10 ,5);// reD? water
		else
			glColor3ub(222,10,90);
		//glColor3ub(0+randColorWater ,0+randColorWater ,255);// looks best 
		//glColor3ub(150 + randColorWater, 160 + randColorWater,255); // too light colored
		Vec *c = &blood[i].s.center;
		float w = 2; //- rnd() * 1;
		float h = 2; //- rnd() * 1;
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

// draws the debug/retrostyle version of the game character
void drawTESTguy(Game * game)
{
	if(game->guts == true)
	{
		if(numblood == 0)
			makeTESTguts(game);
		else
			drawTESTguts(game);
		return;
	}
	
	float w, h;
	//draw character as rectangle	
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
	int x = game->player.position.x;
	int y = game->player.position.y;
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

void drawMissile(Game * game)
{
	// used to allow non textures objects to maintain their color
	glBindTexture(GL_TEXTURE_2D, 0);

	//cout << "missile checked" << endl;
	float x = game->missiles.position.x;
	float y = game->missiles.position.y;
	
	// 12 missiles on spritesheet 1/12 is 0.083
	float x_i = 1.0/12.0;
	// 3 missiles on spritesheet, 2 big 1 small
	// 1 big missiles has a height of 134px and spritetexture's height is 380 so 134/380 = 0.352...
	float y_i =  0.352632;
	
	float PI = 3.14159265359;
	float angle = atan2(game->posX() - x, game->posY() - y) * 180 / PI;
	//cout << "angle :" << angle << endl;
	int wid = 3*game->player.width;
	int height = 2*game->player.height;
	
	drawThrust(angle, x, y, PI, height);
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(angle,0,0.0,-1.0);
	glBindTexture(GL_TEXTURE_2D, missileTexture);

	glBindTexture(GL_TEXTURE_2D, silhouetteTextureMissile);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	
	int x_frame = game->missiles.nextframe;
	float toplvl = 0;
	
	if(game->missiles.nextframe > 23)
	{
		game->missiles.nextframe = 0;
		toplvl = 0;
		x_frame = 0;
	}
	else if(game->missiles.nextframe > 11)
	{
		//cout << x_frame << endl;
		toplvl = y_i;
		x_frame -= 12; 
	}

	glBegin(GL_QUADS);
	glTexCoord2f( x_frame*x_i, toplvl + y_i); glVertex2i(-wid, -height); // bottom left
	glTexCoord2f( x_frame*x_i,  toplvl); glVertex2i(-wid,height); //top left
	glTexCoord2f(x_i + x_frame*x_i,toplvl); glVertex2i( wid,height); // top right
	glTexCoord2f(x_i + x_frame*x_i, toplvl + y_i); glVertex2i( wid, -height); // bottom right
	glEnd();
	glPopMatrix();
	
	glDisable(GL_ALPHA_TEST);
	
}

void drawSpike(Game * game)
{
	glBindTexture(GL_TEXTURE_2D, spikeTexture);
	float wid = 10 * game->player.width;//spike->width *
	for(int i = 0; i < window_width; i+= wid)
	{
		glPushMatrix();
		glTranslatef(i, 0, 0);
		glBindTexture(GL_TEXTURE_2D, silhouetteTextureSpikes);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);	
	
		glBegin(GL_QUADS);
	
		glTexCoord2f(0,1);
		glVertex2i(0,0);
		glTexCoord2f(0,0);
		glVertex2i(0, wid);
		glTexCoord2f(1,0);
		glVertex2i(wid, wid);
		glTexCoord2f(1,1);
		glVertex2i(wid, 0);
		glEnd();

		glEnd();
		glPopMatrix();
	}
	glDisable(GL_ALPHA_TEST);
}

// Don't touch works
void renderCell(float f, float x_i, float y_i, float lvl, float toplvl, Game * g)
{
	int wid = 10 * g->player.width;//skeletonBase->width;
	
	glPushMatrix();
	glTranslatef(g->player.position.x, g->player.position.y + g->player.width*5, 0);
	glBindTexture(GL_TEXTURE_2D, skeletonTexture);

	if(g->player.left)
		glScalef( -1.0f, 1, 1);
	if(g->player.right)
		glScalef(1.0f, 1, 1 );

	glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	
	if(lvl == 2)
		f -= 8*x_i;
	
	glBegin(GL_QUADS);
	// corner coordinates must follow this order
	glTexCoord2f(f, 0.0 + toplvl); glVertex2i(-wid, wid); // top left
	glTexCoord2f(f,  lvl * y_i); glVertex2i(-wid,-wid); //bottom left
	glTexCoord2f(f + x_i, lvl * y_i); glVertex2i( wid,-wid); // bottom right
	glTexCoord2f(f + x_i, 0.0 + toplvl); glVertex2i( wid, wid); // top right

	glEnd();
	glPopMatrix();
	
	glDisable(GL_ALPHA_TEST);
}

int gutcount = 0;

// handles drawing the sprite animation of blood
void drawGuts(Game * g)
{
	float x_i = 0.125;
	float y_i = 0.111;
	//if(gutcount > 9)
		//gutcount = 0;
	float f;
	if(frames == 2)
	{
		gutcount++;
	}
	if(gutcount > 7)
		return;//gutcount = 0;
	f = gutcount*x_i;
	
	int wid = 10 * g->player.width;
	glPushMatrix();
	glTranslatef(g->player.position.x, g->player.position.y + g->player.width*5, 0);
	glBindTexture(GL_TEXTURE_2D, gutsTexture);

	if(g->player.left)
		glScalef( -1.0f, 1, 1);
	if(g->player.right)
		glScalef(1.0f, 1, 1 );

	glBindTexture(GL_TEXTURE_2D, silhouetteTextureGuts);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	
	glBegin(GL_QUADS);
	// corner coordinates must follow this order
	glTexCoord2f(f, 0.0); glVertex2i(-wid, wid); // top left
	glTexCoord2f(f, y_i); glVertex2i(-wid,-wid); //bottom left
	glTexCoord2f(f + x_i, y_i); glVertex2i( wid,-wid); // bottom right
	glTexCoord2f(f + x_i, 0.0); glVertex2i( wid, wid); // top right

	glEnd();
	glPopMatrix();
	
	glDisable(GL_ALPHA_TEST);
}

float frame = 0;

// frames determines how the sprite is drawn, timer
// frame is the x axis distance between cells
void drawSkeleton(Game * game)
{
	// spritesheet has 8 sprites cells on the x axis 1/8 = 0.125
	// spritesheet has 9 sprites cells on the y axis 1/9 = 0.111
	float x_increment = 0.125;
	float y_increment = 0.111;

	if(game->guts == true)
	{
		if(numblood == 0)
			makeTESTguts(game);
		else
		{
			
			drawGuts(game);
			drawTESTguts(game);
		}
		return;
	}
	
	gutcount = 0;
	if(frame <= 3*x_increment)
		frame = 4*x_increment;
	
	int lvl = 1;
	float toplvl = 0.0f;
	
	if(game->player.velocity.y > 0) // up
	{
		frame = 5*x_increment;
		lvl = 1;
		toplvl = 0.0f;
	}
	if(game->player.velocity.y < 0) // down
	{
		frame = 3*x_increment;
		lvl = 1;
		toplvl = 0.0f;
	}
	if(game->player.velocity.x == 0)
	{
		frame = 0.0; // starting position 
		lvl = 9;
		toplvl =  8 * 0.1111111111;
		if(game->player.velocity.y > 0) // up
		{
			frame = 5*x_increment;
			lvl = 1;
			toplvl = 0.0f;
		}
		if(game->player.velocity.y < 0) // down
		{
			frame = 3*x_increment;
			lvl = 1;
			toplvl = 0.0f;
		}
	}
	
	// lower level of texture
	if(frame > 7 * x_increment)
	{
		lvl = 2;
		toplvl = 1 * 0.111111111;
	}
	
	
	if(frame > 11 * x_increment)
	{
		lvl = 1;
		toplvl = 0.0;
		frame = 4*x_increment;
	}

	renderCell(frame, x_increment, y_increment, lvl, toplvl, game);
}

// all draw functions that get called in render funciton
void drawGame_Textures(Game * game)
{
	drawBackground();
	drawSpike(game);
	drawMissile(game);

	// used to allow non textures objects to maintain their color
	glBindTexture(GL_TEXTURE_2D, 0);

	drawPlatform(game,5);
	drawSkeleton(game);	
}

// debug mode/retro style
void drawGame_TEST(Game * game)
{
	drawTESTspikes(game);
	drawPlatform(game,5);
	drawTESTmissile(game);
	drawTESTguy(game);
}
