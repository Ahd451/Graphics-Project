
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <sstream>
#include<time.h>
#include "include/irrKlang.h"  //of the irrklang library of sounds

using namespace irrklang;
using namespace std;


#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")

//Stars Struct of space
struct Stars{
	float xpos, ypos;
	char show = true;
};
//char showStars = true;
int const starsSize = 100;
Stars stars[starsSize];
int count_life = 0;


//struct of every row in the balls
struct BallsRow{
	float xposc1 = -2, xposc2 = -5, xposc3 = -8, yposc = 80, radius = 1; //variables of balls function 
	int colorCounter, colorCounter1, colorCounter2;
	int passed = 0; //that means this raw didn't passed the screen yet
	int hitted = 0;  //if the wrong ball hitted then decrease its life
};

int const ballsSize = 150;
BallsRow ballsRow[ballsSize];

//build the struct of the 3mod of light
struct Light{
	//define variables for the right 3mods light row
	double rightXpos1 = -1, rightXpos2 = 0, rightYpos1 = 80, rightYpos2 = 85, rightRadius = 1,
	rightWidth = rightXpos2 - rightXpos1, rightHeight = rightYpos2 - rightYpos1,
	rightEllipseRadiusV = .2, rightEllipseRadiusH = 2,
	moveRightEllipseV = 0, moveRightEllipseH = 0,
	lightRightH = 0;//use to move the ellipse little bit with the 3mod//used to limit the scaling of 3mod

	//define variables for the left 3mod light row
	double leftXpos1 = -9, leftXpos2 = -8, leftYpos1 = 80, leftYpos2 = 85, leftRadius = 1,
		leftWidth = leftXpos2 + leftXpos1, leftHeight = leftYpos2 - leftYpos1,
		leftEllipseRadiusV = .2, leftEllipseRadiusH = 2,
		moveLeftEllipseV = 0, moveLeftEllipseH = 0,
		lightLeftH = 0; //used to limit the scaling of 3mod

};

int const lightSize = 100;
Light light[lightSize];

//draw the route on the road
struct Route{
	float xpos1 = -11, xpos2 = -9, xpos3 = 0, xpos4 = 3.5, ypos1 = 75, ypos2 = 80;
};
const int routeSize = 300;
Route route[routeSize];

struct SideRoute{
	float xpos1 = 100, xpos2 = 102, ypos1 = 70, ypos2 = 75, ypos3 = 75, ypos4 = 70, moveYposToTop = .01,
		moveSmallBallDown = 0; //used to move the little small balls down to stay on the route all time of displaying
};
int const sideRouteSize = 300;
SideRoute sideRoute[sideRouteSize];

//struct of the walls
struct Walls{
	float xpos1 = -9, xpos2 = 0, ypos1 = 80, ypos2 = 85;  //wall varialbles
	int colorCounter;
	char passed = false;  //check if the box passed the screen or not
};

int const wallSize = 40;
int wallCounter = 0;
Walls walls[wallSize];

void InitGraphics(int argc, char *argv[]);
void DrawEllipsefan(float minRad, float majRad, float xc, float yc);
void wall(float, float, float, float, float[3][3], int);
void balls(float xposc1, float xposc2, float xposc3, float yposc, float radius, float wall_color[3][3], int colorCounter, int colorCounter1, int colorCounter2);
void draw_stars();
void draw_lines();
int which_ball_hitted(int index);  //check which ball is hitted from main ball
void drawFilledCircle2(GLfloat x, GLfloat y, GLfloat radius);
void draw_score2(GLfloat x, GLfloat y, GLfloat radius);
void draw_score3(GLfloat x, GLfloat y, GLfloat radius);
void colors_life(GLfloat x, GLfloat y, GLfloat radius);
void main_ball();
void light_3mod();
bool life_ball_hitted();
void moon();
void gameOver();
void draw_arraws();
/**
Sets the logical coordinate system we will use to specify
our drawings.
*/
void SetTransformations();




/**
Handles the paint event. This event is triggered whenever
our displayed graphics are lost or out-of-date.
ALL rendering code should be written here.
*/
void OnDisplay();

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius);
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius);
/**
Handles the key press. This event is whenever
a normal ASCII character is being pressed.
*/
void OnSpecialKeyPress(int key, int x, int y);
void OnKeyPress(unsigned char key, int x, int y);
void Print_score(int n);    // -- print score --
void DrawEllipse(float minRad, float majRad, float xc, float yc, float r, float g, float b);







float fXPos, fYPos, fRot;
float fX1, fY1;

irrklang::ISoundEngine * bgSound = irrklang::createIrrKlangDevice();  //global pointer of irrklang sound library
irrklang::ISoundEngine * ballSound = irrklang::createIrrKlangDevice();  //global pointer of irrklang sound library


int main(int argc, char* argv[])
{

	//call the background song
	//sndPlaySound(TEXT("songs/bgSong.wav"), SND_ASYNC);
	bgSound->setSoundVolume(.15);
	//bgSound->play2D("songs/bgSong.wav", true); // play some mp3 file, looped
	bgSound->play2D("songs/16bit.wav", true); // play some mp3 file, looped

	


	srand(time(NULL));

	//Get Values of Stars
	for (int i = 0; i < starsSize; i++){
		stars[i].xpos = (rand() % 200) - 100;
		stars[i].ypos = (rand() % 200) - 100;
		//cout << stars[i].xpos << " - " << stars[i].ypos << endl;
	}


	//********** Start initiate the positions of balls and walls (boxes) ************
	int distance = 0;
	//set distances betwwen the rows of the balls
	for (int i = 0; i < ballsSize; i++){
		ballsRow[i].yposc += distance;
		distance += 90;

		ballsRow[i].colorCounter = i % 3;  //initiate color for each 

		//if the first ball get color number zero from  the array of the colors 
		if (ballsRow[i].colorCounter == 0){
			ballsRow[i].colorCounter1 = 1;
			ballsRow[i].colorCounter2 = 2;
		}
		//if the first ball get color number 1 from  the array of the colors 
		else if (ballsRow[i].colorCounter == 1){
			ballsRow[i].colorCounter1 = 0;
			ballsRow[i].colorCounter2 = 2;
		}
		//if the first ball get color number 2 from  the array of the colors 
		else{
			ballsRow[i].colorCounter1 = 0;
			ballsRow[i].colorCounter2 = 1;
		}


		//set initial positions for the walls
		if (i % 7 == 0){
			walls[wallCounter].ypos1 += distance + 50;
			walls[wallCounter].ypos2 += distance + 50;
			wallCounter++;

			walls[wallCounter].colorCounter = wallCounter % 3;


		}

	}
	//********** End initiate the positions of balls and walls (boxes) ************


	//********** Start initiate the positions of routes ************
	//get initial positions for the route
	int routeDistance = 0, routeWidth = 10, routeHeight = 5;
	for (int i = 0; i < routeSize; i++){
		route[i].ypos1 += routeDistance;
		route[i].ypos2 += routeDistance;

		routeDistance += 50;
	}
	//********** End initiate the positions of routes ************

	//********** Start initiate the Sided Route *****************
	int sideRouteDistance = 0;
	for (int i = 0; i < sideRouteSize; i++){
		sideRoute[i].xpos1 += sideRouteDistance;
		sideRoute[i].xpos2 += sideRouteDistance;

		sideRouteDistance += 15;
	}

	//********** Start initiate the Sided Route *****************

	//********** Start initiate the positions of the 3mod light ************
	int lightDistance = 0;
	for (int i = 0; i < lightSize; i++){
		//initiate the positions of right 3mods
		light[i].rightYpos1 += lightDistance;
		light[i].rightYpos2 += lightDistance;

		//initiate the positions of left 3mods
		light[i].leftYpos1 += lightDistance +20;
		light[i].leftYpos2 += lightDistance+20;

		lightDistance += 100;
	}

	//********** End initiate the positions of the 3mod light ************


	InitGraphics(argc, argv);
	return 0;
}

/**
Creates the main window, registers event handlers, and
initializes OpenGL stuff.
*/
void InitGraphics(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	//Create an 800x600 window with its top-left corner at pixel (100, 100)
	glutInitWindowPosition(200, 10); //pass (-1, -1) for Window-Manager defaults
	glutInitWindowSize(600, 700);
	glutCreateWindow("Color Road");
	//OnDisplay will handle the paint event
	glutDisplayFunc(OnDisplay);
	// here is the setting of the idle function
	glutIdleFunc(OnDisplay);
	// here is the setting of the key function
	glutSpecialFunc(OnSpecialKeyPress);
	glutKeyboardFunc(OnKeyPress);

	SetTransformations();

	glutMainLoop();
}

/**
Sets the logical coordinate system we will use to specify
our drawings.
*/
void SetTransformations() {
	//set up the logical coordinate system of the window: [-100, 100] x [-100, 100]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-100, 100, -100, 100);
	glMatrixMode(GL_MODELVIEW);
}

//float wall_color[3][3] = {
//	{ 0.14, 0.34, 0.54 },
//	{ 0.95, 0.83, 0.01 },
//	{ 0.76, 0.16, 0.18 }
//};
//
float wall_color[3][3] = {
	{ 1, 0, 0 },
	{ 1, 1, 0 },
	{ 0, 0, 1 }
};

int row = 0;


//float dx_scale = .1, dy_scale = .1;//the scale of walls 

int randRow; // get the random color for the balls

//the positions of the main ball
float xposmBall = 0, yposmBall = -50, radiusmBall = 6;
float speed = 1.2; //used to move the balls and walls in equal speed
int mBallColor[3] = { 1, 1, 0 }; //the color of main ball
int score = 0;  //stores the score of the game
int NumOfPassedBalls = 0;  //counts each 10 raws of balls and high there speed
int totalNumOfPassedBalls = 0;  //counts the number of all balls that are passed away from the screen
float strechXpos = .15, strechYpos = .12, strechRadius = .054; //used to strech the distance between balls and the walls
float lifeXpos = -4.5, lifeYpos = 80, lifeRadius = 1;  //used in life ball which gives another life
bool appeared = false;  //used to check if the life ball appeared to the user or not
bool bgSoundChanged = false;  //change the bgSound
void OnDisplay()
{
	glLoadIdentity();

	//set the background color to white
	glClearColor(0, 0, 0, 0);
	//fill the whole color buffer with the clear color
	glClear(GL_COLOR_BUFFER_BIT);

	//draw the moon
	moon();

	//call draw stars function
	draw_stars();

	//call draw lines function
	draw_lines();

	//******* Start drawing the life balls **************
	if (count_life > 2){

		//draw the life ball
		if (lifeYpos <= 80 && appeared == false){
			colors_life(lifeXpos, lifeYpos, lifeRadius);

			//strech the radius of the colored ball
			if (lifeRadius <= 6)
				lifeRadius += .08;

		}

		lifeYpos -= .8;
			
		
		if (life_ball_hitted() == 1){
			count_life--;
			ballSound->play2D("songs/lifeBall.wav"); // play some mp3 file, looped
		}

	}

	if (lifeYpos < 80 && count_life <= 2){
		appeared = true; //that means the life ball appeared and will not appear again
		//cout << "Entered here\n";

		lifeYpos = -120;
		/*lifeYpos = 80;
		lifeRadius = 1;
		lifeXpos = -4.5;*/
	}

	//if the user lossed the game then gameOver will appear
	if (count_life >3)
		gameOver();

	//******* End drawing the life balls **************

	//********* Start draw the balls ***********
	//int distance = 0;
	//int randRow = 0;
	for (int i = 0; i < ballsSize; i++){

		//draw each raw of balls
		if (ballsRow[i].yposc <= 80){
			balls(ballsRow[i].xposc1, ballsRow[i].xposc2, ballsRow[i].xposc3,
				ballsRow[i].yposc, ballsRow[i].radius, wall_color,
				ballsRow[i].colorCounter, ballsRow[i].colorCounter1, ballsRow[i].colorCounter2);

		}

		//if (randRow == 2)  //back the colors array to 0 again to select the first index
		//	randRow = 0;

		ballsRow[i].yposc -= speed;

		if (ballsRow[i].yposc <= 80 && ballsRow[i].yposc >= 75){

			ballsRow[i].radius = 1;
			ballsRow[i].xposc1 = -2;
			ballsRow[i].xposc3 = -8;
		}


		//scale the balls
		if (ballsRow[i].radius <= 6){
			ballsRow[i].radius += strechRadius;
		}
		if (ballsRow[i].xposc1 <= 15 || ballsRow[i].xposc3 >= -15){
			ballsRow[i].xposc1 += strechXpos;
			ballsRow[i].xposc3 -= strechXpos;
		}

		//call the function to check which ball hitted 
		int check = which_ball_hitted(i);
		if (check == 1){
			score++;
		}
		else if (check == 0 && ballsRow[i].hitted == 0){
			ballsRow[i].hitted = 1;
			count_life++;
			ballSound->setSoundVolume(.3);
			ballSound->play2D("songs/wrong4.wav"); // play some mp3 file, looped
		}



		//print score
		glColor3f(0, 0, 0);
		if (count_life == 0)
		{
			drawFilledCircle2(70, 45, 20);
		}
		else if (count_life == 1)
		{
			draw_score2(70, 45, 20);
			}
		else if (count_life == 2)
		{
			draw_score3(70, 45, 20);

		}

		/*glColor3f(0.752941176470588, 0.752941176470588, 0.752941176470588);
		drawFilledCircle(70, 60, 10);*/

		Print_score(score);

		light_3mod();


		//================  Start the first Level of Hard  ===================
		//***** Start check each 10 balls passed then *****

		//if the speed didn't exeet the limited speed
		//if (speed <= .07){
		//if the raw of ball passed the screen
		if (ballsRow[i].passed == 0)
		if (ballsRow[i].yposc < -100)
			ballsRow[i].passed = 1;

		//inc the number of passed balls
		if (ballsRow[i].passed == 1){
			NumOfPassedBalls++;
			totalNumOfPassedBalls++; //inc the total num of passed balls away from the screen
			ballsRow[i].passed = 2;  //means i will not calculate again

			cout << totalNumOfPassedBalls << endl;
		}

		//if number of passed balls reached 10 then increase the speed
		if (NumOfPassedBalls == 5){

		//	cout << "----------- speed = " << speed << endl;
			speed += .02;
		//	cout << "----------- speed = " << speed << endl;
			NumOfPassedBalls = 0;


			cout << NumOfPassedBalls << "  " << speed << endl;
			//maximize the strech of shapes
			strechRadius += .0003;
			strechXpos += .003;
			strechYpos += .0004;
		}
		//}

		//because after a while the game is slowed so we will speed the game more and more
		//if (totalNumOfPassedBalls >= 30){
		//	speed += .007;
		//}

		//***** End check each 10 balls passed then *****
		//================  End the first Level of Hard  ===================

		//================  Start the Second Level of Hard  ===================

		//......... here will reduce the distance every 10 passed balls ............

		//================  End the Second Level of Hard  ===================

	}

	//********* End draw the balls ***********

	//********* Start draw the walls ***********
	for (int i = 0; i < wallCounter; i++){
		if (walls[i].ypos1 <= 80){
			wall(walls[i].xpos1, walls[i].xpos2, walls[i].ypos1, walls[i].ypos2, wall_color, walls[i].colorCounter);
		}

		walls[i].ypos1 -= speed;
		walls[i].ypos2 -= speed;

		// Start make scaling for the boxes
		if (walls[i].ypos1 <= 80){
			if (walls[i].xpos1 > -36)
				walls[i].xpos1 -= strechXpos + .06;
			if (walls[i].xpos2 < 33)
				walls[i].xpos2 += strechXpos + .06;
			if (walls[i].ypos2 - walls[i].ypos1 <= 13)
				walls[i].ypos2 += strechYpos;
		}
		// End make scaling for the boxes

		//change the color of the main ball if the box hitted it
		if (walls[i].ypos2 - 12 <= yposmBall && walls[i].passed == false){
			mBallColor[0] = wall_color[walls[i].colorCounter][0];
			mBallColor[1] = wall_color[walls[i].colorCounter][1];
			mBallColor[2] = wall_color[walls[i].colorCounter][2];
			//give a sound for the box
			ballSound->play2D("songs/sound1.wav"); // play some mp3 file, looped

			walls[i].passed = true;

		}


	}

	//change the bgsound after a while
	if (totalNumOfPassedBalls == 20 &&bgSoundChanged == false){
		//bgSound->drop();
		bgSound->stopAllSounds();
		bgSound->play2D("songs/bgSong2.wav", true);
		bgSoundChanged = true;
	}


	//draw the main ball
	main_ball();

	//draw the arraws
	draw_arraws();

	glutSwapBuffers();

}


//wall function
void wall(float xpos1, float xpos2, float ypos1, float ypos2, float wall_color[3][3], int row){
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1); // 
	glVertex2f(xpos1, ypos1);
	glVertex2f(xpos1, ypos2);

	glVertex2f(xpos2, ypos2);
	glVertex2f(xpos2, ypos1);


	glEnd();
	////box 1
	//
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(xpos2 - 8, ypos1 + 2);
	glVertex2f(xpos2 - 8, ypos2 - 1);
	glVertex2f(xpos2 - 1, ypos2 - 1);
	glVertex2f(xpos2 - 1, ypos1 + 2);

	glEnd();


	glBegin(GL_TRIANGLES);
	glColor3f(wall_color[row][0], wall_color[row][1], wall_color[row][2]);

	glVertex2f(xpos2 - 8, ypos1 + 2);
	glVertex2f(xpos2 - 5, ypos2 - 2);
	glVertex2f(xpos2 - 2, ypos1 + 2);
	glEnd();




	// box 2 
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(xpos1 + 13, ypos1 + 2);
	glVertex2f(xpos1 + 13, ypos2 - 1);

	glVertex2f(xpos2 - 10, ypos2 - 1);
	glVertex2f(xpos2 - 10, ypos1 + 2);
	glEnd();




	glBegin(GL_POLYGON);
	glColor3f(wall_color[row][0], wall_color[row][1], wall_color[row][2]);
	glVertex2f(xpos1 + 14, ypos1 + 3);
	glVertex2f(xpos1 + 14, ypos2 - 2);
	glVertex2f(xpos2 - 11, ypos2 - 2);
	glVertex2f(xpos2 - 11, ypos1 + 3);
	glEnd();


	//box 3
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(xpos1 + 2, ypos1 + 2);
	glVertex2f(xpos1 + 2, ypos2 - 1);
	glVertex2f(xpos1 + 11, ypos2 - 1);
	glVertex2f(xpos1 + 11, ypos1 + 2);

	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(wall_color[row][0], wall_color[row][1], wall_color[row][2]);
	glVertex2f(xpos1 + 3, ypos1 + 2);
	glVertex2f(xpos1 + 6, ypos2 - 2);
	glVertex2f(xpos1 + 10, ypos1 + 2);
	glEnd();
}

void balls(float xposc1, float xposc2, float xposc3, float yposc, float radius, float wall_color[3][3], int colorCounter, int colorCounter1, int colorCounter2){


	//draw the first ball from left
	glColor3f(wall_color[colorCounter][0], wall_color[colorCounter][1], wall_color[colorCounter][2]);
	drawFilledCircle(xposc1, yposc, radius);

	//draw the second ball inside
	glColor3f(wall_color[colorCounter1][0], wall_color[colorCounter1][1], wall_color[colorCounter1][2]);
	drawFilledCircle(xposc2, yposc, radius);

	//draw the third ball
	glColor3f(wall_color[colorCounter2][0], wall_color[colorCounter2][1], wall_color[colorCounter2][2]);
	drawFilledCircle(xposc3, yposc, radius);


}


void draw_stars(){

	
	//*** Start draw the stars ***
	glLoadIdentity();
	glColor3f(1, 1, 1);

	for (int i = 0; i < starsSize; i++){

		//change the size of stars
		if (i <= starsSize / 2)
			glPointSize(1.5);
		else
			glPointSize(1);

		glBegin(GL_POINTS);
		glVertex2d(stars[i].xpos, stars[i].ypos);
		glEnd();

		stars[i].ypos -= .2;

		if (stars[i].ypos < -100)
			stars[i].ypos = 105;
	}
	
	//*** End draw the stars ***

	//*** Start draw the stars ***
	glLoadIdentity();
	glColor3f(0.643137254901961, 0.0823529411764706, 0.556862745098039);

	for (int i = 0; i < starsSize; i += 3){

		//change the size of stars
		if (i <= starsSize / 2)
			glPointSize(1.5);
		else
			glPointSize(1);

		glBegin(GL_POINTS);
		glVertex2d(stars[i].xpos, stars[i].ypos);
		glEnd();

		stars[i].ypos -= .01;

		if (stars[i].ypos < -100)
			stars[i].ypos = 105;

	}
	
	//*** End draw the stars ***


	//*** Start draw the stars ***
	glColor3f(0.643137254901961, 0.0823529411764706, 0.556862745098039);
	glLoadIdentity();

	for (int i = 0; i < starsSize; i += 5){

		//change the size of stars
		if (i <= starsSize / 2)
			glPointSize(2);
		else
			glPointSize(1);

		glBegin(GL_POINTS);
		glVertex2d(stars[i].xpos, stars[i].ypos);
		glEnd();

		stars[i].ypos -= .01;

		if (stars[i].ypos < -100)
			stars[i].ypos = 105;
	}
	
	//*** End draw the stars ***

	//*** Start draw the stars ***
	glColor3f(1, 1, 1);
	glLoadIdentity();

	for (int i = 0; i < starsSize; i += 4){
		glBegin(GL_POINTS);
		glVertex2d(stars[i].xpos, stars[i].ypos);
		stars[i].ypos -= .01;

		if (stars[i].ypos < -100)
			stars[i].ypos = 105;
	}
	glEnd();
	//*** End draw the stars ***
}


float strechRouteTop = .5, strechRouteBottom = .54, strechRouteYpos = 0;  //strech the route
int pass = 0; //usedin if statement of strechRoutes
float strechSideRouteTop = .001, strechSideRouteBottom = .008;

void draw_lines(){

	//draw the main road
	glBegin(GL_POLYGON);
	glColor3f(0.04, 0.13, 0.24);
	//glColor3f(0, 0.53, 0);
	glVertex2f(150, -150);
	glVertex2f(80, -80);  //
	glVertex2f(-80, -80);
	glVertex2f(-120, -150);

	glVertex2f(80, -80);
	glVertex2f(0, 80);
	glVertex2f(-9, 80);
	glVertex2f(-80, -80);
	glEnd();

	//draw the far road
	glBegin(GL_POLYGON);
	glVertex2f(-9, 80);
	glVertex2f(100, 75);
	glVertex2f(100, 70);
	glVertex2f(0, 80);
	glEnd();


	//draw the side Route on the sided road
	
	for (int i = 0; i < sideRouteSize; i++){
		if (sideRoute[i].xpos1 <= 100 && sideRoute[i].xpos1 >= 0){

			//draw the side route of side road
			glColor3f(0.1, 0.23, 0.31);
			glBegin(GL_POLYGON);
			glVertex2f(sideRoute[i].xpos1, sideRoute[i].ypos1 + sideRoute[i].moveYposToTop);
			glVertex2f(sideRoute[i].xpos1, sideRoute[i].ypos2 + sideRoute[i].moveYposToTop);
			glVertex2f(sideRoute[i].xpos2, sideRoute[i].ypos3 + sideRoute[i].moveYposToTop);
			glVertex2f(sideRoute[i].xpos2, sideRoute[i].ypos4 + sideRoute[i].moveYposToTop);
			glEnd();

			//draw the balls on the road
			glColor3f(1, 0, 0);
			drawFilledCircle(sideRoute[i].xpos1 + 1, sideRoute[i].ypos1 + sideRoute[i].moveYposToTop + 1 , .5);
			glColor3f(1, 1, 0);
			drawFilledCircle(sideRoute[i].xpos1 + 1, sideRoute[i].ypos1 + sideRoute[i].moveYposToTop + 2 + sideRoute[i].moveSmallBallDown, .5);
			glColor3f(0,0, 1);
			drawFilledCircle(sideRoute[i].xpos1 + 1, sideRoute[i].ypos1 + sideRoute[i].moveYposToTop + 2.6 + sideRoute[i].moveSmallBallDown*.0008, .5);

			//draw the 3mod on the left side
			glColor3f(0.18, 0.56, 0.55);
			glBegin(GL_POLYGON);
			glVertex2f(sideRoute[i].xpos1+2, sideRoute[i].ypos2 + sideRoute[i].moveYposToTop);
			glVertex2f(sideRoute[i].xpos1+2, sideRoute[i].ypos2 + sideRoute[i].moveYposToTop + 3.5);
			glVertex2f(sideRoute[i].xpos1 + 3.2, sideRoute[i].ypos2 + sideRoute[i].moveYposToTop + 3.5);
			glVertex2f(sideRoute[i].xpos1 + 3.2, sideRoute[i].ypos2 + sideRoute[i].moveYposToTop);
			glEnd();

			//draw the circle light of the left most 3mods on the route
			glColor3f(1, 1, 1);
			drawFilledCircle(sideRoute[i].xpos1 + 2.6, sideRoute[i].ypos2 + sideRoute[i].moveYposToTop + 3.5, .8);

			//draw the 3mod on the right side
			glColor3f(0.18, 0.56, 0.55);
			glBegin(GL_POLYGON);
			glVertex2f(sideRoute[i].xpos1 +8, sideRoute[i].ypos1 + sideRoute[i].moveYposToTop);
			glVertex2f(sideRoute[i].xpos1 +8, sideRoute[i].ypos1 + sideRoute[i].moveYposToTop + 3.5);
			glVertex2f(sideRoute[i].xpos1 +9, sideRoute[i].ypos1 + sideRoute[i].moveYposToTop + 3.5);
			glVertex2f(sideRoute[i].xpos1 +9, sideRoute[i].ypos1 + sideRoute[i].moveYposToTop);
			glEnd();


			//draw the circle light of the right most 3mods on the route
			glColor3f(1, 1, 1);
			drawFilledCircle(sideRoute[i].xpos1 + 8.7, sideRoute[i].ypos1 + sideRoute[i].moveYposToTop + 3.5, .8);


			sideRoute[i].moveSmallBallDown -= .001; //used to move the ball down to stay on the road
			sideRoute[i].moveYposToTop += .01;  //move the ypos of the road to top little to stay on the road

			//strech the sideRoute
			sideRoute[i].ypos1 += strechSideRouteBottom;
			sideRoute[i].ypos2 -= strechSideRouteTop;
			sideRoute[i].ypos3 -= strechSideRouteTop;
			sideRoute[i].ypos4 += strechSideRouteBottom;
		}

		sideRoute[i].xpos1 -= .2;
		sideRoute[i].xpos2 -= .2;



	}
	




	//draw the route on the road
	glColor3f(0.1, 0.23, 0.31);

	for (int i = 0; i < routeSize; i++){
		if (route[i].ypos2 <= 80){
			glBegin(GL_POLYGON);
			glVertex2f(route[i].xpos1, route[i].ypos1);
			glVertex2f(route[i].xpos2, route[i].ypos2);
			glVertex2f(route[i].xpos3, route[i].ypos2);
			glVertex2f(route[i].xpos4, route[i].ypos1);
			glEnd();

			//strech this route
			route[i].ypos2 += .07;

			route[i].xpos1 -= strechRouteBottom;
			route[i].xpos2 -= strechRouteTop;
			route[i].xpos3 += strechRouteTop + .25;
			route[i].xpos4 += strechRouteBottom + .25;


			//strech the road more when the number of balls pass the screen
			//if (totalNumOfPassedBalls == 8) pass = 1;
			if (totalNumOfPassedBalls >= 8 && totalNumOfPassedBalls <= 18){
				strechRouteBottom += .00000002;
				strechRouteTop += .000000013;
			}
			else if (totalNumOfPassedBalls > 18 && totalNumOfPassedBalls <= 35){
				strechRouteBottom += .000000051;
				strechRouteTop += .000000042;
			}
			else if (totalNumOfPassedBalls > 35 && totalNumOfPassedBalls <= 80){
				strechRouteBottom += .00000007;
				strechRouteTop += .00000006;
			}
			else if (totalNumOfPassedBalls > 80){
				strechRouteBottom += .00000006;
				strechRouteTop += .00000005;
			}

			

		}
		
		//move the routes down
		route[i].ypos1 -= speed;
		route[i].ypos2 -= speed;

		//if the route exceeded the screen then repeat it to the begining of the road again
	/*	if (route[i].ypos2 < -100){
			route[i].xpos1 = -11;
			route[i].xpos2 = -9;
			route[i].xpos3 = 0;
			route[i].xpos4 = 3.5;

			route[i].ypos1 = 75;
			route[i].ypos2 = 80;
		}*/


	}

}

int which_ball_hitted(int index){
	if ((ballsRow[index].yposc - ballsRow[index].radius * 2 <= yposmBall) && (ballsRow[index].yposc + ballsRow[index].radius * 2 >= yposmBall)){

		//if hitted ball 1
		if (((ballsRow[index].xposc1 < xposmBall) && (ballsRow[index].xposc1 + ballsRow[index].radius * 2 > xposmBall)) ||
			((ballsRow[index].xposc1 < xposmBall + radiusmBall * 2) &&
			(ballsRow[index].xposc1 + ballsRow[index].radius * 2 > xposmBall + radiusmBall * 2))) {

			//if the two balls are the same color then hide the ball from the raw
			if (mBallColor[0] == wall_color[ballsRow[index].colorCounter][0] &&
				mBallColor[1] == wall_color[ballsRow[index].colorCounter][1] &&
				mBallColor[2] == wall_color[ballsRow[index].colorCounter][2]){

				ballsRow[index].xposc1 = 120;

				//get out good sound means hitted the true ball
				if (ballsRow[index].yposc - ballsRow[index].radius * 2 - yposmBall <= 0){
					//cout << "hitted ball 1 \n";
					//call the sound of good hit
					ballSound->play2D("songs/coin1.wav"); // play some mp3 file, looped
					//ballSound->removeSoundSource("songs/ball.wav");

				}


				return 1;
			}
			//if two balls are not the same color then stop the game 
			else{
				return 0;
			}





		}

		//if hitted ball 2
		if (((ballsRow[index].xposc2 < xposmBall) && (ballsRow[index].xposc2 + ballsRow[index].radius * 2 > xposmBall)) ||
			((ballsRow[index].xposc2 < xposmBall + radiusmBall * 2) &&
			(ballsRow[index].xposc2 + ballsRow[index].radius * 2 > xposmBall + radiusmBall * 2))){

			//cout << "ball 2 \n";

			//if the two balls are the same color then hide the ball from the raw
			if (mBallColor[0] == wall_color[ballsRow[index].colorCounter1][0] &&
				mBallColor[1] == wall_color[ballsRow[index].colorCounter1][1] &&
				mBallColor[2] == wall_color[ballsRow[index].colorCounter1][2]){

				ballsRow[index].xposc2 = 120;

				//get out good sound means hitted the true ball
				if (ballsRow[index].yposc - ballsRow[index].radius * 2 - yposmBall <= 0){
					//cout << "hitted ball 2 \n";
					//call the sound of good hit
					ballSound->play2D("songs/coin1.wav"); // play some mp3 file, looped
					//ballSound->removeSoundSource("songs/ball.wav");
				}

				return 1;
			}
			//if two balls are not the same color then stop the game 
			else{
				return 0;
			}

		}

		//if hitted ball 3
		if (((ballsRow[index].xposc3 < xposmBall) && (ballsRow[index].xposc3 + ballsRow[index].radius * 2 > xposmBall)) ||
			((ballsRow[index].xposc3 < xposmBall + radiusmBall * 2) &&
			(ballsRow[index].xposc3 + ballsRow[index].radius * 2 > xposmBall + radiusmBall * 2))){


			//if the two balls are the same color then hide the ball from the raw
			if (mBallColor[0] == wall_color[ballsRow[index].colorCounter2][0] &&
				mBallColor[1] == wall_color[ballsRow[index].colorCounter2][1] &&
				mBallColor[2] == wall_color[ballsRow[index].colorCounter2][2]){

				//get out good sound means hitted the true ball
				if (ballsRow[index].yposc - ballsRow[index].radius * 2 - yposmBall <= 0){
					//cout << "hitted ball 3 \n";
					//call the sound of good hit
					ballSound->play2D("songs/sand.wav"); // play some mp3 file, looped
					//ballSound->removeSoundSource("songs/ball.wav");
				}

				ballsRow[index].xposc3 = 120;
				return 1;
			}
			//if two balls are not the same color then stop the game 
			else{
				return 0;
			}

		}
	}

	return 2;
}

float strech3modRadius = 0;//strech3modXpos = 0, strech3modYpos = 0;
//float moveRightEllipseV = 0, moveRightEllipseH = 0, moveLeftEllipseV = 0, moveLeftEllipseH = 0;; //use to move the ellipse little bit with the 3mod

void light_3mod()
{


	for (int i = 0; i < lightSize; i++){

		//********** Start moving the right 3mods ****************
		if (light[i].rightYpos1 <= 80 && light[i].rightYpos1 >-100){

			glBegin(GL_POLYGON);
			glColor3f(0.18, 0.56, 0.55);
			glVertex2d(light[i].rightXpos1, light[i].rightYpos1);
			glVertex2d(light[i].rightXpos1, light[i].rightYpos2);
			glVertex2d(light[i].rightXpos2, light[i].rightYpos2);
			glVertex2d(light[i].rightXpos2, light[i].rightYpos1);
			glEnd();

			glColor3f(1, 1, 1);
			drawFilledCircle(light[i].rightXpos2 - light[i].lightRightH, light[i].rightYpos2, light[i].rightRadius);



			/*glColor3f(0.04, 0.13, 0.24);
			DrawEllipse(light[i].rightEllipseRadiusH, light[i].rightEllipseRadiusV, 
				light[i].rightXpos1 + light[i].moveRightEllipseH,
				light[i].rightYpos1 + light[i].rightHeight - light[i].moveRightEllipseV, 1, 1, 0);
			*/

			//****** start scalling the 3mod light when appear on the road ***********
			//special for 3mod
			//limits the width
			if (light[i].rightWidth <= 5){

				light[i].rightWidth += .0005;
				light[i].rightXpos1 -= .0005;  //strech the 3mod light xpos

			}

			//limits the height
			if (light[i].rightHeight <= 25){
			//	strech3modYpos += .0000001;
				light[i].rightHeight += .002;
				light[i].rightYpos2 += .002;  //strech ypos of 3mod
			}
			//special for the circle of the lamp
			//limits the radius
			if (light[i].rightRadius <= 6){

				light[i].rightRadius += .00032; //strech the diameter of 3mod
			}
			//limits the movement of ligth lamp xpos
			if (light[i].lightRightH <= 2){
				light[i].lightRightH += .00036;
			}

			////specialparts for the ellips shape under the lamp
			////limis the horizontal radius of Ellipse
			//if (light[i].rightEllipseRadiusH <= 5){
			//	light[i].rightEllipseRadiusH += .0003;
			//}

			////limis the vertical radius of Ellipse
			//if (light[i].rightEllipseRadiusV <= 1){
			//	light[i].rightEllipseRadiusV += .0002;
			//}

			////limits the movement of vertical Ellipse
			//if (light[i].moveRightEllipseV <= 6){
			//	light[i].moveRightEllipseV += .00049;
			//}

			////limits the movement of horizontal
			//if (light[i].moveRightEllipseH <= 2.5){
			//	light[i].moveRightEllipseH += .0005;
			//}
			
			//****** End scalling the 3mod light when appear on the road ***********


			//move the 3mod parallel with the road
			light[i].rightXpos1 += .003;
			light[i].rightXpos2 += .003;

		}

		light[i].rightYpos1 -= .005;
		light[i].rightYpos2 -= .005;
		//******************************************************** End moving the right 3mods ****************


		//******************************************************** Start moving the left 3mods ****************
		if (light[i].leftYpos1 <= 80 && light[i].leftYpos1 >-100){

			glBegin(GL_POLYGON);
			glColor3f(0.18, 0.56, 0.55);
			glVertex2d(light[i].leftXpos1, light[i].leftYpos1);
			glVertex2d(light[i].leftXpos1, light[i].leftYpos2);
			glVertex2d(light[i].leftXpos2, light[i].leftYpos2);
			glVertex2d(light[i].leftXpos2, light[i].leftYpos1);
			glEnd();

			glColor3f(1, 1, 1);
			drawFilledCircle(light[i].leftXpos2 - light[i].lightLeftH, light[i].leftYpos2, light[i].leftRadius);

			//glColor3f(0.04, 0.13, 0.24);
			/*DrawEllipse(light[i].leftEllipseRadiusH, light[i].leftEllipseRadiusV,
				light[i].leftXpos1 + light[i].moveLeftEllipseH,
				light[i].leftYpos1 + light[i].leftHeight - light[i].moveLeftEllipseV, 1, 1, 0);
*/
			//****** start scalling the 3mod light when appear on the road ***********
			//special for the 3mod
			if (light[i].leftWidth <= 5){

				light[i].leftWidth += .003;
				light[i].leftXpos1 -= .0005;  //strech the 3mod light xpos
			}

			if (light[i].leftHeight <= 25){
				//	strech3modYpos += .0000001;
				light[i].leftHeight += .002;
				light[i].leftYpos2 += .002;  //strech ypos of 3mod
			}
			//special for the circle lamp
			if (light[i].leftRadius <= 6){
				light[i].leftRadius += .00032; //strech the diameter of 3mod
			}
			if (light[i].lightLeftH <= 2){
				light[i].lightRightH += .0005;
			}
			//limits the movement of ligth lamp xpos
			if (light[i].lightLeftH <= 2){
				light[i].lightLeftH += .0005;
			}

			////specialparts for the ellips shape under the lamp
			////limis the horizontal radius of Ellipse
			//if (light[i].leftEllipseRadiusH <= 5){
			//	light[i].leftEllipseRadiusH += .0003;
			//}

			////limis the vertical radius of Ellipse
			//if (light[i].leftEllipseRadiusV <= 1){
			//	light[i].leftEllipseRadiusV += .0002;
			//}

			////limits the movement of vertical Ellipse
			//if (light[i].moveLeftEllipseV <= 6){
			//	light[i].moveLeftEllipseV += .00049;
			//}

			////limits the movement of horizontal
			//if (light[i].moveLeftEllipseH <= 2.5){
			//	light[i].moveLeftEllipseH += .0005;
			//}
			//
			//****** End scalling the 3mod light when appear on the road ***********


			//move the 3mod parallel with the road
			light[i].leftXpos1 -= .0018;
			light[i].leftXpos2 -= .0018;

		}

		light[i].leftYpos1 -= .005;
		light[i].leftYpos2 -= .005;
		//********** End moving the left 3mods ****************
		


	}



}



char textx[20];
void Print_score(int n)    // -- print score --
{

	//glLoadIdentity();
	//printf(text, "X:%.0f Y:%.0f", fXPos, fYPos);  // print text in opengl window 

	string s;
	stringstream ss;
	ss << n;
	ss >> textx;

//	cout << s;
	glColor3f(1, 1, 0);
	glRasterPos2f(68, 40);
	for (int i = 0; textx[i] != NULL; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, textx[i]);
}


bool life_ball_hitted(){

	//||
	//(lifeYpos + lifeRadius * 2 <= yposmBall && lifeYpos + lifeRadius * 2 >= yposmBall + radiusmBall * 2)
	//cout << lifeYpos << "  " << yposmBall << endl;

	//check if the life ball hitted or not
	if ((yposmBall <= lifeYpos && yposmBall >= lifeYpos - lifeRadius * 2))
		if ((lifeXpos <= xposmBall && lifeXpos + lifeRadius * 2 >= xposmBall) ||
			(lifeXpos <= xposmBall + radiusmBall * 2 && lifeXpos + lifeRadius * 2 >= xposmBall + radiusmBall * 2)){
			
			return 1;
		}
	return 0;
}

/**
Handles the key press. This event is whenever
a normal ASCII character is being pressed.
*/
void OnKeyPress(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);

	{
		switch (key)
		{
		case 'a'://		a key  to Show the skier
		case 'A':
		{
					break;
		}
		case 'f'://		f key to Hide the skier
		case 'F':
		{

					break;
		}
		case 'd'://		d key to Stop wrapping
		case 'D':
		{
					break;
		}
		case 's'://		s key to Start wrapping 
		case 'S':
		{

					break;
		}
		case 'g'://		s key to Start wrapping 
		case 'G':
		{
					exit(0);
					break;
		}

		};
	}

}

void OnSpecialKeyPress(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT://		Left function key
		if (xposmBall > -25)
			xposmBall -= 4;

		break;
	case GLUT_KEY_RIGHT://		Right function key
		if (xposmBall < 15)
			xposmBall += 4;
		//cout << xposmBall << endl;
		break;
	case GLUT_KEY_UP://		Up function key

		break;
	case GLUT_KEY_DOWN://		Down function key

		break;
	};
}

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius){

	float PI = 3.14159265;
	int lineAmount = 60; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;

	//glLineWidth(4);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= lineAmount; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / lineAmount)),
			y + (radius* sin(i * twicePi / lineAmount))
			);
	}

	glEnd();
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){

	float PI = 3.14159265;
	int triangleAmount = 60; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius

	GLfloat twicePi = 2.0 * PI;

	glBegin(GL_POLYGON);
	glVertex2f(x, y); // center of circle
	for (int i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),

			y + (radius * sin(i * twicePi / triangleAmount))
			);
	}
	glEnd();
}


void drawFilledCircle2(GLfloat x, GLfloat y, GLfloat radius){

	float PI = 3.14159265;
	int triangleAmount = 60; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius

	GLfloat twicePi = 2.0 * PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (int i = 0; i <= triangleAmount; i++) {
		//glColor3f(1, 1, 0);
		if (i == 0)
		{
			glColor3f(0.501960784313725, 0, 1);
			//glColor3f(0, 0, 1);
		}

		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),

			y + (radius * sin(i * twicePi / triangleAmount))
			);

		if (i == 10)
		{
			glColor3f(0.501960784313725, 0.501960784313725, 0.752941176470588);
			//glColor3f(1, 0, 0);
		}
		if (i == 20)
		{
			glColor3f(0.501960784313725, 0.501960784313725, 1);
			//glColor3f(1, 1, 0);

		}

		if (i == 30)
		{
			glColor3f(0, 0, 0);
		}


	}
	glEnd();
}



void draw_score2(GLfloat x, GLfloat y, GLfloat radius){

	float PI = 3.14159265;
	int triangleAmount = 60; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius

	GLfloat twicePi = 2.0 * PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (int i = 0; i <= triangleAmount; i++) {
		//glColor3f(1, 1, 0);
		/*if (i == 0)
		{
		glColor3f(0.501960784313725, 0, 1);;
		}*/

		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),

			y + (radius * sin(i * twicePi / triangleAmount))
			);

		if (i == 10)
		{
			//glColor3f(1,0,0);
			glColor3f(0.501960784313725, 0.501960784313725, 0.752941176470588);

		}
		if (i == 20)
		{
			//glColor3f(1,1,0);
			glColor3f(0.501960784313725, 0.501960784313725, 1);

		}

		if (i == 30)
		{
			glColor3f(0, 0, 0);
		}


	}
	glEnd();
}

void draw_score3(GLfloat x, GLfloat y, GLfloat radius){

	float PI = 3.14159265;
	int triangleAmount = 60; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius

	GLfloat twicePi = 2.0 * PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (int i = 0; i <= triangleAmount; i++) {
		//glColor3f(1, 1, 0);
		/*if (i == 0)
		{
		glColor3f(0.501960784313725, 0, 1);;
		}*/

		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),

			y + (radius * sin(i * twicePi / triangleAmount))
			);

		/*if (i == 10)
		{
		glColor3f(0.501960784313725, 0.501960784313725, 0.752941176470588);
		}*/
		if (i == 20)
		{
			//glColor3f(1, 1, 0);
			glColor3f(0.501960784313725, 0.501960784313725, 1);

		}

		if (i == 30)
		{
			glColor3f(0, 0, 0);
		}


	}
	glEnd();
}



void colors_life(GLfloat x, GLfloat y, GLfloat radius){

	float PI = 3.14159265;
	int triangleAmount = 60; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius

	GLfloat twicePi = 2.0 * PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (int i = 0; i <= triangleAmount; i++) {
		//glColor3f(1, 1, 0);
		if (i == 0)
		{
			glColor3f(0, 1, 0);
		}

		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),

			y + (radius * sin(i * twicePi / triangleAmount))
			);

		if (i == 10)
		{
			glColor3f(0, 0.250980392156863, 0.501960784313725);
		}
		if (i == 20)
		{
			glColor3f(0, 0.501960784313725, 0.501960784313725);
		}

		if (i == 30)
		{
			glColor3f(0.501960784313725, 0.501960784313725, 1);
		}
		if (i == 40)
		{
			glColor3f(0.501960784313725, 0, 0.250980392156863);
		}
		if (i == 50)
		{
			glColor3f(1, 0, 0.501960784313725);
		}



	}
	glEnd();
}




float shadowRed = 0, shadowGreen = 0, shadowBlue = 0;

void main_ball()
{
	//draw the main ball

	glColor3f(mBallColor[0], mBallColor[1], mBallColor[2]);
	drawFilledCircle(xposmBall, yposmBall, radiusmBall);


	//draw the shadow
	if (mBallColor[0] == 1 && mBallColor[1] == 1 && mBallColor[2] == 0){  //if the main ball is yellow 
		shadowRed = 1;
		shadowGreen = 0.71;
		shadowBlue = 0;
	}
	else if (mBallColor[0] == 1 && mBallColor[1] == 0 && mBallColor[2] == 0){  //if the main ball is red
		shadowRed = 0.98;
		shadowGreen = 0.24;
		shadowBlue = 0.24;
	}
	else{  //if the main ball is blue
		shadowRed = 0.07;
		shadowGreen = 0.49;
		shadowBlue = 0.85;
	}

	double inc = 0.004;
	
	for (float i = 0.1; i<40; i++)
	{
		glColor3f(shadowRed + inc, shadowGreen + inc, shadowBlue + inc); //yellow

		//glColor3f(0.2 + inc, 0.2 + inc, 0.2 + inc);  // shadow
		inc += .017;
		drawFilledCircle(xposmBall, yposmBall - 15 - i, radiusmBall - 1);

	}

}


void DrawEllipse(float minRad, float majRad, float xc, float yc, float r, float g, float b){
	float DEG2RAD = 3.14159265 / 180;
	//float Colors [7][3]={{1,0,0},{1,0.65,0},{1,1,0},{0,1,0},{0,0,1},{0.29,0,0.51},{0.93,0.51,0.93}}; 
	//float Colors[7][3] = { { 1, 0, 0 }, { 1, 0.65, 0 }, { 1, 1, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0.5, 0, 0.5 }, { 1, 0, 1 } };

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(r, g, b);
	glVertex3f(xc, yc, 0);

	for (int i = 0; i < 1000; i++)
	{
		//glColor4fv(Colors[i / 10]);
		glColor3f(r, g, b);
		float degInRad = i*(360 / 70)*DEG2RAD;
		glVertex2f(xc + minRad*cos(degInRad), yc + majRad*sin(degInRad));
	}

	//glColor4fv(Colors[0]);
	glColor3f(r, g, b);
	glVertex2f(xc + minRad*cos(0.0), yc + majRad*sin(0.0));

	glEnd();
}


void DrawEllipsefan(float minRad, float majRad, float xc, float yc)
{
	float PI = 3.14159265;
	int triangleAmount = 50; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius

	GLfloat twicePi = 2.0 * PI;

	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			xc + (minRad* cos(i *  twicePi / triangleAmount)),
			yc + (majRad * sin(i * twicePi / triangleAmount))
			);
	}
	glEnd();
}


void moon()
{

	glColor3f(1, 1, 1);
	DrawEllipsefan(7.5, 9, -70, 70);

	glColor3f(0, 0, 0);
	DrawEllipsefan(8, 8, -68, 72);

}

void gameOver(){
	bgSound->drop();
	cout << "Game Over\n";
	int u;
	cin >> u;
}
void draw_arraws(){


	glColor3f(0.498039215686275, 0.498039215686275, 0.498039215686275);
	drawFilledCircle(-80, -82, 12);



	//glColor3f(0.784313725490196, 0.784313725490196, 0.784313725490196);
	glColor3f(0.898039215686275, 0.898039215686275, 0.898039215686275);
	drawFilledCircle(-80, -82, 10);


	glColor3f(0.498039215686275, 0.498039215686275, 0.498039215686275);
	glBegin(GL_TRIANGLES);

	glVertex2f(-90, -83);
	glVertex2f(-75, -73);
	glVertex2f(-75, -88);

	glEnd();
	//glColor3f(0.784313725490196, 0.784313725490196, 0.784313725490196);
	glColor3f(0.898039215686275, 0.898039215686275, 0.898039215686275);
	glBegin(GL_TRIANGLES);

	glVertex2f(-84, -83);
	glVertex2f(-75, -73);
	glVertex2f(-75, -87);

	glEnd();


	//Arraw Number 2



	glColor3f(0.498039215686275, 0.498039215686275, 0.498039215686275);
	drawFilledCircle(-50, -82, 12);



	//glColor3f(0.784313725490196, 0.784313725490196, 0.784313725490196);
	glColor3f(0.898039215686275, 0.898039215686275, 0.898039215686275);
	drawFilledCircle(-50, -82, 10);


	glColor3f(0.498039215686275, 0.498039215686275, 0.498039215686275);
	glBegin(GL_TRIANGLES);

	glVertex2f(-45, -83);
	glVertex2f(-55, -73);
	glVertex2f(-55, -88);

	glEnd();
	//glColor3f(0.784313725490196, 0.784313725490196, 0.784313725490196);
	glColor3f(0.898039215686275, 0.898039215686275, 0.898039215686275);
	glBegin(GL_TRIANGLES);

	glVertex2f(-50, -83);
	glVertex2f(-55, -73);
	glVertex2f(-55, -87);

	glEnd();

}