//Nicholas Musacco
//CS 335




bool pausegame = false;


void physics(Game * game)
{

    game->inAir(); 
    game->checkBottomScreen();

    if(!pausegame)
    {
	game->applyGravity();


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

	if(killmovement) // kill movement on x axis only
	    game->player.velocity.x = 0;

	if(game->velX() > MAX_VELOCITY)
	    game->player.velocity.x = MAX_VELOCITY;
	if(game->velX() < -1 * MAX_VELOCITY)
	    game->player.velocity.x = -1 * MAX_VELOCITY;

	if(!pausegame)
	{
	    game->move();
	}

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

    // particles
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
	}
    }
}








