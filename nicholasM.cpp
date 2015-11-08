// CS335 fall 2015
// ===========================
// Final Project: Group 2
// 
// student: Nicholas Musacco
// 
// ===========================

//
//
//Currently contains the physics function along with basic 
//pause features to stop motion when the game is paused
//
//Future addition will include a pause screen that will
//pop up with the Esc key is pressed
//
//The pause screen will have a few buttons that will be
//clicked by the users mouse, the buttons will be resume and quit
//Also when the game is paused the score will be put up

struct button {
    float width, height;
    //	float radius;
    Vec center;
};


bool pausegame = true;

void setMenuBackground()
{
    button startb;
    startb.center.y  =  window_height - 250;
    startb.center.x  = 200;
    startb.width = 50;
    startb.height = 50;

    button exitb;
    exitb.center.y = window_height - 250;
    exitb.center.x = window_width  - 200;
    exitb.width  = 50;
    exitb.height = 50;

    //int width = background->width;
    //int height = background->height;
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, menuTexture);
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



    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3ub(0, 200, 0);
    glPushMatrix();
    glTranslatef(startb.center.x, startb.center.y, 0);

    glBegin(GL_QUADS);
    glVertex2i(-startb.width, -startb.height);
    glVertex2i(-startb.width,  startb.height);
    glVertex2i( startb.width,  startb.height);
    glVertex2i( startb.width, -startb.height);
    glEnd();
    glPopMatrix();

    glColor3ub(200, 0, 0);
    glPushMatrix();
    glTranslatef(exitb.center.x, exitb.center.y, 0);

    glBegin(GL_QUADS);
    glVertex2i(-exitb.width, -exitb.height);
    glVertex2i(-exitb.width,  exitb.height);
    glVertex2i( exitb.width,  exitb.height);
    glVertex2i( exitb.width, -exitb.height);
    glEnd();
    glPopMatrix();


}



void physics(Game * game)
{

    if(STATE == RUN_GAME && !pausegame)
    {
	game->inAir(); 
	game->updatePlatforms();
	game->applyGravity();
	if(game->checkBottomScreen()) // spikes collision?
	    game->guts = true;
	game->missileChasePlayer();
	game->checkCollision();

	if(game->checkMissileHit())
	    game->guts = true;

	if(keys[XK_Left]) // left
	{
	    game->player.right = false;
	    game->player.left = true;
	    if(frames == 1)
		frame += 0.125;
	    //cout << "left" << endl;
	    game->accelX(-1 * INITIAL_VELOCITY);
	}

	if(keys[XK_Right]) // right
	{
	    game->player.left = false;
	    game->player.right = true;
	    if(frames == 1)
		frame +=  0.125;
	    //cout << "right" << endl;
	    game->accelX(INITIAL_VELOCITY);
	}

	if(keys[XK_space] && game->if_jump) // spacebar
	{
	    //cout << "jump" <<endl;

	    game->accelY(2 * INITIAL_VELOCITY);
	}

	if(killmovement && game->inAir()) // kill movement on x axis only
	    game->player.velocity.x = 0;

	if(game->velX() > MAX_VELOCITY)
	    game->player.velocity.x = MAX_VELOCITY;
	if(game->velX() < -1 * MAX_VELOCITY)
	    game->player.velocity.x = -1 * MAX_VELOCITY;

	game->move();
    }
    int x_bubbler = 100;
    int y_bubbler = window_height;

    if(bubbler) // if bubbler is toggled only stream water from the top, no mouse involved
    {
	for(int i = 0; i < window_height * 0.15; i++)
	{
	    x_bubbler += rnd()*10;
	    makeParticle(x_bubbler, y_bubbler);
	}
    }


    if(game->guts == false) // respawn, reset guts animation
	numblood = 0;

    // waterfall settings
    Particle *p = &par[numParticles];
    for(int i = 0; i < numParticles; ++i)
    {
	p = &par[i];
	p->s.center.x += p->velocity.x;
	p->s.center.y += p->velocity.y;
	p->velocity.y -= 0.1; 

	if (p->s.center.y < 0.0 || p->s.center.y > window_height) 
	{
	    //std::cout << "off screen" << std::endl;
	    memcpy(&par[i], &par[numParticles -1], 
		    sizeof(Particle));
	    numParticles--;
	    //if(numParticles == 0)
	    //game->guts = false;
	}
    }

    // blood settings
    Particle *p2 = &blood[numblood];
    for(int i = 0; i < numblood; ++i)
    {
	p2 = &blood[i];
	p2->s.center.x += p2->velocity.x;
	p2->s.center.y += p2->velocity.y;
	p2->velocity.y -= 0.1; 

	if (p2->s.center.y < 0.0 || p2->s.center.y > window_height) 
	{
	    memcpy(&blood[i], &blood[numblood -1], 
		    sizeof(Particle));
	    numblood--;
	}
    }
}


int check_keys(XEvent *e, Game * game)
{
    int key = XLookupKeysym(&e->xkey, 0);


    killmovement = true;
    if(e->type == KeyRelease) 
    {
	keys[key] = 0;
	if(key == XK_space)
	    killmovement = false;
    }

    if(e->type == KeyPress)
    {
	keys[key] = 1;

	if(key == XK_k) // respawn
	{
	    if(game->guts == true) // 
	    {
		game->setPos(window_width/2, window_height);
		game->guts = false;
	    }
	}

	if(key == XK_p)
	{
	    if(!pausegame)
		pausegame = true;
	    else
		pausegame = false;

	}
	if(STATE == RUN_GAME && !pausegame)
	{
	    if(key != XK_Left || key != XK_Right)
		killmovement = false;

	    if(key == XK_b)
	    {
		if(bubbler)
		    bubbler = false;
		else
		    bubbler = true;
	    }

	    if(key == XK_m)
	    {
		game->createMissiles();
	    }
	}
	if(key == XK_w)
	{
	    if(setbackground)
		setbackground = false;
	    else 
		setbackground = true;
	}

	if(key == XK_Escape)
	{
	    game->run = false;
	}
    }	
    return 0;
}





// check for button clicks?
void check_mouse(XEvent *e, Game *game)
{
    int mousex = e->xbutton.x;
    int mousey = e->xbutton.y;
    if (e->type == ButtonRelease) {
	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    //Left button was pressed
	    if(STATE == MAIN_MENU)
	    {
		if((window_height - mousey >= window_height - 300)
			&& window_height - mousey <= window_height - 200)
		{
		    if(mousex >= 150 && mousex <= 250)
		    {
			STATE = RUN_GAME;
			//pausegame = false;	

		    }
		    if(mousex <= window_width - 150
		       && mousex >= window_width - 250)
		    {
			game->run = false;
		    }
		}
	    }
	    return;
	}
	if (e->xbutton.button==3) {
	    //Right button was pressed
	    //std::cout << "right mouse b down" << std::endl;
	    game->run = false;
	    return;
	}
    }
    //Did the mouse move?
    //	if (savex != e->xbutton.x || savey != e->xbutton.y) 
    //	{
    //
    //
    //	}
}

