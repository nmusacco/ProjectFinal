/*Nadia Salem*/

void drawPlatform(Game *game, int amt)
{
    glColor3ub(255,255,255);
	
    float w, h;
    for(int j = 0; j <amt; j++)
    {
        glPushMatrix();
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
// 					tied with player's width and height"
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
        game->platform[i].pos.x= rand() % window_width;
        //game->platform[i].pos.y=(window_height-(i+1)*window_height/amt);
		
	// h_w - h_w/5 * (i + 1) + player height * 2
	game->platform[i].pos.y = window_height - 1.0/6.0 *window_height* (i + 1) + game->platform[i].height*2;
    }
}
