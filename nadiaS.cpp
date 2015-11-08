/*Nadia Salem*/
Ppmimage *platform = NULL;
GLuint platformTexture;


void loadTexture()
{
    //load platform texture image from ppm structure
    platform = ppm6GetImage("./images/brick.ppm");
    glGenTextures(1, &platformTexture);
    glBindTexture(GL_TEXTURE_2D, platformTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, platform->width, platform->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, platform->data);
}

void drawPlatform(Game *game, int amt)
{
	float w, h;
    	
    	if(setbackground)
		for(int j = 0; j <amt; j++)
		{
			
		    glPushMatrix();
			glColor3ub(255,255,255);
		    glTranslatef(game->platform[j].pos.x,game->platform[j].pos.y,0);
		    w = game->platform[j].width;
		    h = game->platform[j].height;
		    glBindTexture(GL_TEXTURE_2D, platformTexture);
		    glBegin(GL_QUADS);
		    glTexCoord2f(0,1); // bottom left
		    glVertex2i(-w,-h);
		    glTexCoord2f(0,0); // top left
		    glVertex2i(-w,h);
		    glTexCoord2f(1,0); // top right
		    glVertex2i(w,h);
		    glTexCoord2f(1,1); // bottom right
		    glVertex2i(w,-h);
		    glEnd();
		    glPopMatrix();
		}
	else
	    for(int j = 0; j < amt; j++)
	    {
		
	    	glPushMatrix();
		glColor3ub(255,255,255);
		glTranslatef(game->platform[j].pos.x,game->platform[j].pos.y, 0);
		w=game->platform[j].width;
		h=game->platform[j].height;
		glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w,h);
		glVertex2i(w,h);
		glVertex2i(w,-h);
		glEnd();
		glPopMatrix();
	    }
}

// HEY!!! (10/28/2015)
// =====================================================================
// Pedro Gonzalez: "changed some stuff so the heights and widths are 
// 			tied with player's width and height"
// =====================================================================
//Nadia Salem: I fixed Drawplatforms so it won't happen

void makePlatform(int amt, Game *game)
{   
	int width = game->player.width * 15;
	int height = game->player.height * 0.35;
	srand(time(NULL));
	for(int i = 0; i<amt; i++)
	{
	    game->platform[i].width =  width; 
	    game->platform[i].height= height;
	    game->platform[i].pos.x= rand() % window_width-rand()%game->platform[i].width;
	    //game->platform[i].pos.y=(window_height-(i+1)*window_height/amt);

	    // h_w - h_w/5 * (i + 1) + player height * 2
	    game->platform[i].pos.y = window_height - 1.0/5.0 *window_height* (i + 1) + game->platform[i].height*2;
	}
}
