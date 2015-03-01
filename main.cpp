#include "gl/glut.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string>
#include "particle.h"
#include "imageloader.h"
#include "game.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

//particle system
particles ps(0,0,-15);
bool pause, wind, rain;
float gravity;
float speed;
float windX, windZ;

//texture
GLuint textureID1,textureID2,textureID3;

//GAME SYSTEM
Game* game1;
float boundX, boundY, boundZ;
bool difficulty;

//KEYBOARD PRESS
bool keypress[4]={0};

//TEXT




float randomClr (void) {
	return float(rand()%100)/100;
}

void drawParticles() { //draw the particles
	for (vIt it=ps.parts.begin(); it<ps.parts.end(); it++) { //iterate through the particle vector
		glPushMatrix();
		glTranslatef(it->pos[0],it->pos[1],it->pos[2]);
		//cout << it->pos[0] << ", " << it->pos[1] << ", " << it->pos[2] << ", age:" << it->age << "\n";
		glScalef(it->size,it->size,it->size);
		// setMaterial(it->clr_mat);
		glColor3f(it->clr[0],it->clr[1],it->clr[2]);
		// glRotatef(it->ang[0],1,0,0); //rotations not needed in this implementation
		// glRotatef(it->ang[1],0,1,0);
		// glRotatef(it->ang[2],0,0,1);
		if(it->pos[2]<0) glutSolidCube(1); //each particle is drawn as a unit cube
		glPopMatrix();
	}
}

void reset() {//reset function for 'r' feature. erases each particle from vector and memory
	for (vIt it=ps.parts.begin(); it<ps.parts.end(); it++) {
		ps.parts.erase(it--);
	}
}

void particleUpdate() { //particle update
	if (!pause) { //if pause is not activated
		//generate random directions, angles and sizes
		float dir[] = {float((rand() % 100)-50)/100, float((rand() % 100)-50)/100, 0.5f};
		float ang[] = {rand() % 360, rand() % 360, rand() % 360};
		float sz = float((rand() % 10))/400;
		float clr[3];
		if (rain) {
			clr[0]=0.1;
			clr[1]=0.9;
			clr[2]=1.0;
		}
		else for (int i=0;i<3;i++) clr[i]=randomClr();
		//add to particle vector
		ps.add(ps.origin, dir, ang, clr, speed, sz);
	}
	//update all current particles
	if (wind) ps.update(gravity, windX, windZ);
	else ps.update(gravity,0, 0);
}

//texture
GLuint loadTexture(Image* image) { //load texture
	GLuint textureID;
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image->width,image->height,0,GL_RGB,GL_UNSIGNED_BYTE,image->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	return textureID;
}

void drawBackground() { //draw the game border background
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);  
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,textureID2);
	// glColor3f(1,0,0);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f,0.0f);
		glVertex2f(0,0);
		glTexCoord2f(1.0f,0.0f);
		glVertex2f(1000,0);
		glTexCoord2f(1.0f,1.0f);
		glVertex2f(1000,600);
		glTexCoord2f(0.0f,1.0f);
		glVertex2f(0,600);
	glEnd();
	glPopMatrix();
}

/* drawStrokeText -- takes a string (text) and an x,y position and draws _stroke_ font 
 *  text at that position. For extra practice, see if you can write a corresponding
 *  "drawBitmapText" function that works the same way, but uses bitmap fonts instead
 */  
void drawStrokeText(char* text, int x, int y)
{
	char* c;  //character iterator (ptr) 

	/* change to 2D draw mode */
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);  
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();

	/* position and scale the text - scale factor could become a separate
	 *  argument to the function, i.e., to render different sizes of text
	 */
	glTranslatef(x, y, 0);
	glScalef(0.3, 0.3, 0.3);
	
	for(c = text; *c != '\0'; c++) //stop when we hit null character 
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c); //print one char 
	}

	/* no need to explicitly switch back to 3D view settings, as this is
	 *  done at the start of the display cycle (and drawStrokeText is called
	 *  right at the end of display
	 */

}


void drawWalls(float x, float y, float z) { //draw the walls of the stage
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,textureID1);
	glColor3f(0.1f,0.9f,1.0f);

	glBegin(GL_POLYGON); //right side
		// glColor3f(1,0,0);
		glNormal3f(-1,0,0.5);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(x,-y,z);
		glTexCoord2f(-z/5,0.0f);
		glVertex3f(x,-y,0);
		glTexCoord2f(-z/5,y/2);
		glVertex3f(x,y,0);
		glTexCoord2f(0.0f,y/2);
		glVertex3f(x,y,z);
	glEnd();

	glBegin(GL_POLYGON); //left side
		// glColor3f(1,0,0);
		glNormal3f(1,0,0.5);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-x,-y,z);
		glTexCoord2f(-z/5,0.0f);
		glVertex3f(-x,-y,0);
		glTexCoord2f(-z/5,y/2);
		glVertex3f(-x,y,0);
		glTexCoord2f(0.0f,y/2);
		glVertex3f(-x,y,z);
	glEnd();

	glBegin(GL_POLYGON); //bottom
		// glColor3f(1,0,0);
		glNormal3f(0,1,0.5);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-x,-y,0);
		glTexCoord2f(x/1,0.0f);
		glVertex3f(x,-y,0);
		glTexCoord2f(x/1,-z/5);
		glVertex3f(x,-y,z);
		glTexCoord2f(0.0f,-z/5);
		glVertex3f(-x,-y,z);
	glEnd();

	glBegin(GL_POLYGON); //top
		// glColor3f(1,0,0);
		glNormal3f(0,-1,0.5);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-x,y,0);
		glTexCoord2f(x/1,0.0f);
		glVertex3f(x,y,0);
		glTexCoord2f(x/1,-z/5);
		glVertex3f(x,y,z);
		glTexCoord2f(0.0f,-z/5);
		glVertex3f(-x,y,z);
	glEnd();

	glBegin(GL_POLYGON); //back
		// glColor3f(1,0,0);
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-x,-y,z);
		glTexCoord2f(x/2,0.0f);
		glVertex3f(x,-y,z);
		glTexCoord2f(x/2,y/2);
		glVertex3f(x,y,z);
		glTexCoord2f(0.0f,y/2);
		glVertex3f(-x,y,z);
	glEnd();


	glPopMatrix();
}

void fog(void) { //fog
	float fogcolor[] = {0.2f,0.2f,0.3f,1.0f};
	glFogfv(GL_FOG_COLOR,fogcolor);
	glFogi(GL_FOG_MODE,GL_LINEAR);
	glFogf(GL_FOG_START,5);
	glFogf(GL_FOG_END,30);
}

void drawGame(Game* game) { //draw the moving parts of the game
	glPushMatrix();
	//DRAW PADDLE
	float px=game->_player->get_positionX();
	float py=game->_player->get_positionY();
	float pz=game->_player->get_positionZ();
	float psize=game->_player->get_size();
	// std::cout<<px << ", " << py << ", " << pz << ", " << psize << "\n";
	// std::cout<< px << ", " << px+psize << "\n";
	glColor4f(0.5f,0.5f,0.5f,0.5f);
	glBegin(GL_POLYGON);
		glNormal3f(0,0,1);
		glVertex3f(px,py-psize,pz);
		glVertex3f(px+psize,py-psize,pz);
		glVertex3f(px+psize,py,pz);
		glVertex3f(px,py,pz);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//DRAW BALL
	glBindTexture(GL_TEXTURE_2D,textureID3);
	float bx = game->_gameball->get_positionX();
	float by = game->_gameball->get_positionY();
	float bz = game->_gameball->get_positionZ();
	float bsize = game->_gameball->get_size();
	// std::cout<< bx << ", " << by << ", " << bz << ", " << bsize << "\n" ;
	glColor3f(1.0f,1.0f,0.0f);
	glBegin(GL_POLYGON);
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(bx,by-bsize,bz);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(bx+bsize,by-bsize,bz);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(bx+bsize,by,bz);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(bx,by,bz);
	glEnd();
	glPopMatrix();
}

void display(void) {

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);
	drawBackground(); //game border
	// glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION); //go back to 3d
	glLoadIdentity();
	gluPerspective(45,SCREEN_WIDTH/SCREEN_HEIGHT,1,100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//set camera with our pre defined variable values
	gluLookAt(0,0,6,0,0,0,0,1,0);

	// setLight(genericLight);

	//rotate scene (essentially rotating camera) using rotate variables
	// glRotatef(rotateX,1,0,0);
	// glRotatef(rotateY,0,1,0);
	
	drawWalls(2,2,-30);

	if (game1->canDraw()) drawGame(game1); //draw game objects if they exist

	drawParticles(); //call draw particle function


	// glDisable(GL_LIGHTING); //"draw" TEXT
	glPushMatrix();
	glColor3f(1,1,1);
	if(game1->gameOver() && game1->_playerWin)drawStrokeText("YOU WIN",SCREEN_WIDTH/2-100,SCREEN_HEIGHT/2);//message for winner
	else if(game1->gameOver() && !game1->_playerWin)drawStrokeText("YOU LOSE",SCREEN_WIDTH/2-100,SCREEN_HEIGHT/2);//message for loser
	glPopMatrix();
	// glEnable(GL_LIGHTING);

	glutSwapBuffers();
}



void reshape(int w, int h) {
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,w/h,1,100);
	glMatrixMode(GL_MODELVIEW);
}

void manageKeys() { //RESPOND TO KEY PRESSES
	if (game1->gameStarted() && (keypress[0] || keypress[1] || keypress[2] || keypress[3])) game1->playerControl(keypress[0],keypress[1],keypress[2],keypress[3]);
}

void keys(unsigned char key, int x, int y) {
	switch (key) {
		case 's': //s key
			game1->startGame();
			break;
		case 32: //space
			game1->launch(float(rand()%10-5)/100,float(rand()%10-5)/100); //launch ball with random X and Y axis directions
			break;
		case 27: //Escape key
			exit(0);
			delete game1;
			break;
	}
	glutPostRedisplay();
}

void specKeys(int key, int x, int y) { //REGISTER KEYPRESSES WITHOUT REACTING
	switch (key) {
		case GLUT_KEY_UP:
			keypress[0]=1;
			break;
		case GLUT_KEY_DOWN:
			keypress[1]=1;
			break;
		case GLUT_KEY_LEFT:
			keypress[2]=1;
			break;
		case GLUT_KEY_RIGHT:
			keypress[3]=1;
			break;
	}
}

void specKeysUp(int key, int x, int y) { //DE-REGISTER KEYPRESSES WITHOUT REACTING
	switch (key) {
		case GLUT_KEY_UP:
			keypress[0]=0;
			break;
		case GLUT_KEY_DOWN:
			keypress[1]=0;
			break;
		case GLUT_KEY_LEFT:
			keypress[2]=0;
			break;
		case GLUT_KEY_RIGHT:
			keypress[3]=0;
			break;
	}
	// std::cout<<"UP"<<keypress[0]<<keypress[1]<<keypress[2]<<keypress[3]<<"\n";
}

void timer(int value) { //timer function

	manageKeys(); //REACT TO KEYPRESSES
	particleUpdate(); //ADVANCE PARTICLES
	game1->update(); //ADVANCE GAME
	glutPostRedisplay(); //UPDATE DISPLAY
	glutTimerFunc(40, timer, 0);

}

void init(void) { //Initialization function
	// glEnable(GL_DEPTH_TEST); //enable Depth
	glEnable(GL_LIGHTING); //enable Lighting
	glEnable(GL_LIGHT0); //enable Light0 which is the light we'll be using
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_BLEND); //enable BLENDING
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glEnable(GL_CULL_FACE); //Enable culling, set to back-face by default
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_FOG); //FOG
	fog();

	// ps = new particles(0,0,-15);
	gravity = 0.0f; speed = 0.3f; rain = 1;

	//LOAD TEXTURES WE NEED
	Image* image = loadBMP("textures/water.bmp"); textureID1 = loadTexture(image);
	delete image;
	image = loadBMP("textures/border.bmp"); textureID2 = loadTexture(image);
	delete image;
	image = loadBMP("textures/rock.bmp"); textureID3 = loadTexture(image);
	delete image;

	//SET GAME BOUNDARIES AND INITIATE NEW GAME
	boundX = 2; boundY = 2; boundZ = -30;
	game1 = new Game(boundX,boundY,boundZ,0.5f,0.05f,0.1f,0.5f,1,0);


	

}

int main(int argc, char** argv) {
	std::cout << "Select your difficulty (1 for difficult, 0 for easy)\n";
	std::cin >>difficulty;
	std::cout << "Press S to start new game, press spacebar to begin. Arrow keys to move. Esc to exit. First to 1 point wins\n";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	glutInitWindowPosition(10,10);
	glutCreateWindow("Window");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutSpecialFunc(specKeys);
	glutSpecialUpFunc(specKeysUp);
	glutTimerFunc(60, timer, 0);

	init();


	glutMainLoop();
	return 0;

}