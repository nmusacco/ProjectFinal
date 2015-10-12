// CS335 fall 2015
// ===========================
// Final Project: Group 2
// 
// student: Pedro Gonzalez
// 
// ===========================

// game class 
// -----------------
// hold player and platform coordinates,
// manages movement and velocities of 
// game objects, checks for collisions
// prompts start menu, death/restart screen

#ifndef GAME_H
#define GAME_H


#define PLAYER_WIDTH 5
#define PLAYER_HEIGHT 20
#define GRAVITY 1

struct Vec
{
	float x;
	float y;
};

struct Guy
{
	Vec position;
	Vec velocity;
	int width;
	int height;
};


class Game
{

	//private:

	public:	
		Guy player;	
		bool if_hit; // collision then true, 
		bool if_jump; // if true allow jumping
		bool run; // runs main loop
		int window_height;
		int window_width;
	
		// platform object?	

		Game()
		{
			player.position.x = 30;
			player.position.y = 20;
			player.width = PLAYER_WIDTH;
			player.height = PLAYER_HEIGHT;
			
			if_jump = false;
			if_hit = false;
			run = true;
		}
		
		void setResolution(int x, int y)
		{
			window_width = x;
			window_height = y;
		}
		
		void setPos(float x = 0, float y = 0)
		{
			player.position.x = x;
			player.position.y = y;
		}
		
		void setPosX(float x)
		{
			player.position.x = x;
		}
		
		void setPosY(float y)
		{
			player.position.y = y;
		}
		
		void accel(float x, float y)
		{
			player.velocity.x = x;
			player.velocity.y = y;
		}
		
		void accelX(float x)
		{
			player.velocity.x = x;
		}
		
		void accelY(float y)
		{
			player.velocity.y = y;
		}
		
		void move()
		{
			player.position.x += player.velocity.x;
			player.position.y += player.velocity.y;
		}
		
		// collision bottom and left/right of screen
		void checkscreenedge()
		{
			// bottom of screen, allow double jump
			if(player.position.y - player.height <= 0)
			{
				setPosY(player.height);
				if_jump = true;
			}
			
			// left side of screen
			if(player.position.x > window_width)
			{
				setPosX(player.width);
				accelX(0);
			}	
			
			// right side of screen
			if(player.position.x <= 0)
			{
				setPosX(0);
				accelX(0);
			}
		}

};


#endif