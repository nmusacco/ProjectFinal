// ----------------------------------------------
// 
// -----------------------------------------
// collisions!

#include <cmath>

bool Game::checkBottomScreen()
{
	// bottom of screen, allow double jump
	if(player.position.y - player.height <= 0) // WORKS!!!
	{
		setPosY(player.height);
		setAccel(velX(),0);
		if_jump = true;
		return true;
	}
	return false;
}

bool Game::checkLeftScreenHit()
{
	// left side of screen
	if(player.position.x - player.width <= 0)
	{
		setPosX(player.width);
		setAccel(0,velY());
		return true;
	}
	return false;
}

bool Game::checkRightScreenHit()
{
	// right side of screen
	if(player.position.x + player.width >= window_width)
	{
		setPosX(window_width - player.width);
		setAccel(0,velY());
		return true;
	}	
	return false;
}

// checks PLATFORM collisions
bool Game::checkCollision()
{
	for(int i =0; i <5; i++)
	{
		Platform *p = &platform[i];
		// check y axis
		if(posY() - player.height >= p->pos.y  && posY() - player.height <= p->pos.y + p->height)
			// checks x axis
			if(posX() <= (p->pos.x + p->width) && posX() >= (p->pos.x - p->width))
				// makes him land first before setting it as a collision
				if(velY() <= 0)
				{ 
					setPosY(player.height + (p->pos.y + p->height) - 0.04 * player.height);
					setAccel(velX(),0);
					if_jump = true;  
					return true;
				}
	}
	return false;
}

// needs work
bool Game::checkMissileHit()
{
	int x = posX();
	int y = posY();
	int mx = missiles.position.x;
	int my = missiles.position.y;
	
	float distance = sqrt(pow(x-mx,2) + pow(y-my,2));
	if(distance <= missiles.height/3 || distance <= player.width)
		return true;
	return false;
}

// more collisions!!!
