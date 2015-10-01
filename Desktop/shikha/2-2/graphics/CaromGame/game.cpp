#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
using namespace std;
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
int yes=0;
long long int fav_col,level=10;
//Class Box 
class Box {
	float len,colorr,colorg,colorb;
	public:
	 void set_values(float,float,float,float); 	
	 void draw_box();
};
void Box::set_values(float box_len,float r,float g,float b)
{
	len = box_len;
	colorr = r;
	colorg = g;
	colorb = b;
}
void Box::draw_box()   {
    
    glBegin(GL_QUADS);
    glColor3f(colorr, colorg, colorb);
    glVertex2f(-len / 2, -len / 2);
    glVertex2f(len / 2, -len / 2);
    glVertex2f(len / 2, len / 2);
    glVertex2f(-len / 2, len / 2);
    glEnd();
    
}

class Pocket  {
	public:
          float  x,y;
	  void set_values(float,float);
	  void draw_it();
};
void Pocket::set_values(float poc_x,float poc_y)
{
	x = poc_x;
	y = poc_y;
}	
void Pocket::draw_it()   {
    glTranslatef(x, y, -8.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    float rad = 0.15f;
    for(int i=0 ; i<360 ; i++) {
       glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    }
    glEnd();	
}

class Design_circle : public Pocket   {
      public:
	void draw_it();	
};
void Design_circle::draw_it()   {
    glTranslatef(x, y, -8.0f);
    glBegin(GL_TRIANGLE_FAN);
    float rad = 0.1f;
    for(int i=0 ; i<360 ; i++) {
       glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    }
    glEnd();	
}
class Coin      {
	public:
	float vel_x,vel_y,pos_x,pos_y,prev_vel_x,prev_vel_y,vel;
	int col,active,done;
	void draw_coin()
    		{
			glTranslatef(pos_x, pos_y, -8.0f);
			if(col==0)
				glColor3f(0.0, 0.0, 0.0);
			else if(col==1)
				glColor3f(1.0, 1.0, 1.0);
			else
				glColor3f(0.6,0.247,0.423);
		        glBegin(GL_TRIANGLE_FAN);
 			float rad = 0.1f;
                        for(int i=0 ; i<360 ; i++) {
                       		glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    			}
    			glEnd();	
		}
	void set_values(float x,float y,int color)
		{
			active=0;
			pos_x=x;
			pos_y=y;
			vel_x=0.0f;
			vel_y=0.0f;
			col=color;
			done=0;
		}

};
class Striker    {
	public:
		int mass,active,shoot_status;
		float i,vel_x,vel_y,pos_x,pos_y,rad,temp_vel_x,temp_vel_y;
		void reset()
		{
			vel_x=0.0f;
			vel_y=0.0f;
			pos_x=0.0f;
			pos_y=-1.42f;
			rad=1.0f;
			i=90.0f;
			active=0;
			shoot_status=0;	
		}
		void set_pos(float x)
		{
			if(pos_x+x<1.45f && pos_x+x>-1.45f)
				pos_x+=x;
		}
		void change_speed(int dir)
		{
			draw_dir();
			update_speed();
			if(dir==0)
				rad+=0.1f;
			else if(dir==1 && rad>0.0f)
				rad-=0.1f;
				
		}
		void update_speed()
		{
			
			//printf("imhere");
			temp_vel_x=(rad/300.0f)*cos(DEG2RAD(i));
			temp_vel_y=(rad/300.0f)*sin(DEG2RAD(i));
			
		}
		void draw_dir()
		{
    			//printf("%f\n",i);
			glPushMatrix();
			glTranslatef(pos_x, pos_y, 0.0f);
			glLineWidth(1.5); 
    			glColor3f(1.0, 0.0, 0.0);
    			glBegin(GL_LINES);
    			glVertex3f(0.0f, 0.0f, -8.0f);
    			glVertex3f(rad*cos(DEG2RAD(i)), rad*sin(DEG2RAD(i)),-8.0f);
			glPopMatrix();
    			glEnd();
			glutSwapBuffers();
		}
		void change_dir(int dir)
		{
			draw_dir();
			if(dir==0 && i<180.0f)
				i=i+2.0f;
			else if(dir==1 && i>0.0f)
				i=i-2.0f;
			update_speed();
		}
		void draw_striker()
    		{
			glTranslatef(pos_x, pos_y, -8.0f);
		        glBegin(GL_TRIANGLE_FAN);
 			float rad = 0.13f;
                        for(int j=0 ; j<360 ; j++) {
                       		glVertex2f(rad * cos(DEG2RAD(j)), rad * sin(DEG2RAD(j)));
    			}
    			glEnd();	
		}
		void shoot()
		{
			if(shoot_status==0)
			{
				update_speed();
				vel_x=temp_vel_x;
				vel_y=temp_vel_y;
				shoot_status=1;	
			}
		}

};
time_t curtime,prevtime;
long long int tym_elapsed;
struct timeval tv;
// Function Declarations
void drawScene();
void drawBox(float len);
void initRendering();
void handleResize(int w, int h);
void drawText(char*string); 
void keyboard(unsigned char key, int x, int y);
void Draw_line(float startx,float starty,float endx, float endy);
void inputKey(int key, int x, int y);
void update(int value);
void movstriker(int x,int y);
void striker_shoot(int b, int s, int x, int y);
// Global Variables
Box outer,inner;
Pocket pock[4];
Design_circle design[4];
Striker striker;
Coin coin[9];
char scoreh[100];
int score=30;
long long int currenttime,starttime,ans;
int main(int argc, char **argv) {

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    int windowWidth = w * 2 / 3;
    int windowHeight = h * 2 / 3;
    //printf("%d %d\n",windowWidth,windowHeight);	
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

    glutCreateWindow("Carrom game");
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    outer.set_values(4.5f,0.54509803921f, 0.27058823529f, 0.07450980392f);
    inner.set_values(4.0f,0.96078431372f, 0.87058823529f , 0.70196078431f);
    pock[0].set_values(1.85f,1.85f);
    pock[1].set_values(-1.85f,1.85f);
    pock[2].set_values(-1.85f,-1.85f);
    pock[3].set_values(1.85f,-1.85f);
    design[0].set_values(1.45f,1.45f);		
    design[1].set_values(-1.45f,1.45f);	
    design[2].set_values(-1.45f,-1.45f);	
    design[3].set_values(1.45f,-1.45f);
    coin[0].set_values(0.0f,0.0f,2);
    coin[1].set_values(0.0f,0.4f,1);
    coin[2].set_values(0.4f,0.0f,1);
    coin[3].set_values(0.0f,-0.4f,1);
    coin[4].set_values(-0.4f,0.0f,1);
    coin[5].set_values(0.253f,0.253f,0);
    coin[6].set_values(-0.253f,0.253f,0);
    coin[7].set_values(0.253f,-0.253f,0);	 		
    coin[8].set_values(-0.253f,-0.253f,0);	
    striker.reset();
    gettimeofday(&tv,NULL);
    starttime=tv.tv_sec;
    ans=0;	 		
    initRendering();

    // Register callbacks
    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(inputKey);
    glutMotionFunc(movstriker);
    glutMouseFunc(striker_shoot); 		
    glutReshapeFunc(handleResize);
    glutTimerFunc(20, update, 0);		
    
    glutMainLoop();
    return 0;
}
int right_button,left_button;
float coord_x,coord_y,angle;
//shoot striker using mouse
void striker_shoot(int b, int s, int x, int y)
{
	if( s == GLUT_UP)
	{
		if(b == GLUT_LEFT_BUTTON)
		{
			striker.active=1;
			striker.shoot();
			printf("%d %d\n",x,y);
		}
	}	
	else if(s==GLUT_DOWN)
	{
		if(b==GLUT_RIGHT_BUTTON)
		{
			right_button=1;
			left_button=0;		
		}
		else
		{
			right_button=0;
			left_button=1;
			/*if(y<370)
			{
				coord_x=(float)((x-455)*2)/(float)156;	
				coord_y=(float)((y-256)*2)/(float)157;
				coord_x-=striker.pos_x;	
				coord_y-=striker.pos_y;
				angle=atan(coord_y/coord_x)*180/PI;
				if(angle<0)
					angle=180.0f-(-1*angle);
				striker.i=angle;
			}
			*/
		
		}	
	}
}
//move striker using right mouse click 
void movstriker(int x,int y)
{
	if(right_button)
	{
		if(striker.pos_x >= -1.45f && striker.pos_x <= 1.45f)
		{
			if(x<=343)
				striker.pos_x=-1.45f;
			else if(x>=567)
				striker.pos_x=1.45f;
			else if(x>455)
			{	
				striker.pos_x=(float)((x-455)*2)/(float)155;
			}	
			else if(x<455)
			{
				striker.pos_x=(float)((x-455)*2)/(float)155;
			}
				
		}	
	}
}
//subtract score after time elapses
void timer()
{
	gettimeofday(&tv,NULL);
	currenttime=tv.tv_sec;
	if(((currenttime-starttime)/level)==ans)
	{
		score-=1;
		ans++;
	}
}
//Writes characeters on screen
void drawBoard(char* text,float x, float y , float z)
{
	char *c;
	glPushMatrix();
	glTranslatef(x,y ,z);
	glLineWidth(2.0);
	glScalef(0.45/152.38, 0.45/152.38, 1/152.38);
	for (c=text; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN , *c);
	}
	glPopMatrix();
}
void update(int value)
{
	timer();
	float vel_striker,prev_vel_x,prev_vel_y;
	gettimeofday(&tv,NULL);
	prevtime=curtime;
	curtime=tv.tv_usec;
	tym_elapsed=(curtime-prevtime)/1000;
	//friction effect on striker
	if(striker.active==1)
	{
		vel_striker=sqrt(pow(striker.vel_x,2) + pow(striker.vel_y,2));
		prev_vel_x=striker.vel_x;
		prev_vel_y=striker.vel_y;
		striker.vel_x=(striker.vel_x/vel_striker)*(vel_striker-0.0001f);
		striker.vel_y=(striker.vel_y/vel_striker)*(vel_striker-0.0001f);
		if(prev_vel_x*striker.vel_x<0 || prev_vel_y*striker.vel_y<0)
		{
			striker.vel_x=0;
			striker.vel_y=0;
			striker.active=0;
		}
			
	}
	//motion of coins
	for(int i=0;i<9;i++)
	{
		//friction effect on coins
		if(coin[i].active==1)
		{
			coin[i].vel=sqrt(pow(coin[i].vel_x,2) + pow(coin[i].vel_y,2));
			coin[i].prev_vel_x=coin[i].vel_x;
			coin[i].prev_vel_y=coin[i].vel_y;
			coin[i].vel_x=(coin[i].vel_x/coin[i].vel)*(coin[i].vel-0.00005f);
			coin[i].vel_y=(coin[i].vel_y/coin[i].vel)*(coin[i].vel-0.00005f);
			if(coin[i].prev_vel_x*coin[i].vel_x<0 || coin[i].prev_vel_y*coin[i].vel_y<0)
			{
				coin[i].vel_x=0;
				coin[i].vel_y=0;
				coin[i].active=0;
			}
			
		}
		if(tym_elapsed>=0)
		{
		   //printf("%f",striker.vel_x);
		   coin[i].pos_x+=(tym_elapsed*coin[i].vel_x);
		   coin[i].pos_y+=(tym_elapsed*coin[i].vel_y);
		}
		//effect on coins when board boundaries comes in contact
		if(coin[i].pos_y>=1.9f || coin[i].pos_y<=-1.9f)
		{
			if(coin[i].vel_y>=0 && coin[i].pos_y>=1.9f)
				coin[i].vel_y*=-1;
			else if(coin[i].vel_y<=0 && coin[i].pos_y<=-1.9f)
				coin[i].vel_y*=-1;	
		}
		if(coin[i].pos_x>=1.9f || coin[i].pos_x<=-1.9f)
		{
			if(coin[i].vel_x>=0 && coin[i].pos_x>=1.9f)
				coin[i].vel_x*=-1;
			else if(coin[i].vel_x<=0 && coin[i].pos_x<=-1.9f)
				coin[i].vel_x*=-1;
		}
	}	
	
	//update position of striker with time
        if(tym_elapsed>=0)
		{
		   //printf("%f",striker.vel_x);
		   striker.pos_x+=(tym_elapsed*striker.vel_x);
		   striker.pos_y+=(tym_elapsed*striker.vel_y);
		}
	//effect on striker when board boundaries comes in contact
	if(striker.pos_y>=1.87f || striker.pos_y<=-1.87f)
		{
			if(striker.vel_y>=0 && striker.pos_y>=1.87f)
				striker.vel_y*=-1;
			else if(striker.vel_y<=0 && striker.pos_y<=-1.87f)
				striker.vel_y*=-1;	
		}
	if(striker.pos_x>=1.87f || striker.pos_x<=-1.87f)
		{
			if(striker.vel_x>=0 && striker.pos_x>=1.87f)
				striker.vel_x*=-1;
			else if(striker.vel_x<=0 && striker.pos_x<=-1.87f)
				striker.vel_x*=-1;
		}
	float mod_dir,sx_unit,sy_unit,length_s1,length_s2,s1x,s1y,ds1x,ds1y,s2x,s2y,ds2x,ds2y;
	//effect of coin to coin collision
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(i!=j && +(pow(coin[j].pos_x- coin[i].pos_x,2) + pow(coin[j].pos_y-coin[i].pos_y,2)<=pow(0.23f,2)) && (((coin[i].pos_x-coin[j].pos_x)*(coin[i].vel_x - coin[j].vel_x)+((coin[i].pos_y-coin[j].pos_y)*(coin[i].vel_y-coin[j].vel_y))))<0)
			{
					
				mod_dir=sqrt(pow(coin[i].pos_x-coin[j].pos_x,2)+pow(coin[i].pos_y-coin[j].pos_y,2));
				sx_unit=((coin[j].pos_x-coin[i].pos_x)/mod_dir); 
				sy_unit=((coin[j].pos_y-coin[i].pos_y)/mod_dir);
				length_s1=sx_unit*coin[i].vel_x + sy_unit*coin[i].vel_y;
				s1x=sx_unit*length_s1;
				s1y=sy_unit*length_s1;
				ds1x=coin[i].vel_x-s1x;
				ds1y=coin[i].vel_y-s1y;
				length_s2=(coin[j].vel_x*-1*sx_unit)+(coin[j].vel_y*-1*sy_unit);
				s2x=sx_unit*-1*length_s2;
				s2y=sy_unit*-1*length_s2;
				ds2x=coin[j].vel_x-s2x;
				ds2y=coin[j].vel_x-s2y;
				coin[i].vel_x= s2x+ds1x;
				coin[i].vel_y=s2y+ds1y;
				coin[j].vel_x=s1x+ds2x;
				coin[j].vel_y=s1y+ds2y;
				coin[i].active=1;
				coin[j].active=1;
			}
		}
	}
	//effect of striker-coin collision
	for(int i=0;i<9;i++)
	{
		
		
			if((pow(striker.pos_x- coin[i].pos_x,2) + pow(striker.pos_y-coin[i].pos_y,2)<=pow(0.23f,2)) && (((coin[i].pos_x-striker.pos_x)*(coin[i].vel_x - striker.vel_x)+((coin[i].pos_y-striker.pos_y)*(coin[i].vel_y-striker.vel_y))))<0)
			{
					
				mod_dir=sqrt(pow(coin[i].pos_x-striker.pos_x,2)+pow(coin[i].pos_y-striker.pos_y,2));
				sx_unit=((striker.pos_x-coin[i].pos_x)/mod_dir); 
				sy_unit=((striker.pos_y-coin[i].pos_y)/mod_dir);
				length_s1=sx_unit*coin[i].vel_x + sy_unit*coin[i].vel_y;
				s1x=sx_unit*length_s1;
				s1y=sy_unit*length_s1;
				ds1x=coin[i].vel_x-s1x;
				ds1y=coin[i].vel_y-s1y;
				length_s2=(striker.vel_x*-1*sx_unit)+(striker.vel_y*-1*sy_unit);
				s2x=sx_unit*-1*length_s2;
				s2y=sy_unit*-1*length_s2;
				ds2x=striker.vel_x-s2x;
				ds2y=striker.vel_x-s2y;
				coin[i].vel_x= (s1x*-1)/3 + (4*s2x)/3 + ds1x;
				coin[i].vel_y=(s1y*-1)/3 + (4*s2y)/3 + ds1y;
				striker.vel_x=(s2x*2)/3+s1x/3+ds2x;
				striker.vel_y=s1y/3+(s2y*2)/3+ds2y;
				coin[i].active=1;
				striker.active=1;
			}
		
	}
	int flag=0;
	for(int i=0;i<9;i++)
	{
		if(coin[i].done==0)
			flag=1;
	}
	if(flag==0)
	{
		sprintf(scoreh,"Game Over. Press esc to exit"); 
		drawBoard(scoreh,-2.40f, 0.0f, -8.0f);
	}
	//coins fell in pocket
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(coin[i].done==0 && coin[j].done==0 && (pow(coin[j].pos_x- pock[i].x,2) + pow(coin[j].pos_y-pock[i].y,2))<=pow(0.13f,2))
 	   		{
    				coin[j].pos_x=pock[i].x;
    				coin[j].pos_y=pock[i].y;
				coin[j].vel_x=0.0f;
				coin[j].vel_y=0.0f;
				coin[j].active=0;
				if(coin[j].col==fav_col)
				{
					score+=10;
					coin[j].pos_x=3.0f;
					coin[j].pos_y=0.0f;
					coin[j].done=1;
				}
				else if(coin[j].col==2)
				{
					score+=50;
					coin[j].pos_x=3.0f;
					coin[j].pos_y=1.0f;
					coin[j].done=1;
				}
				else
				{
					coin[j].pos_x=-3.0f;
					coin[j].pos_y=0.0f;
					score-=5;
					coin[j].done=1;
				}	
    			}
		}
		//striker fell in pocket
		if((pow(striker.pos_x- pock[i].x,2) + pow(striker.pos_y-pock[i].y,2))<=pow(0.15f,2))
		{
    				striker.pos_x=pock[i].x;
    				striker.pos_y=pock[i].y;
				striker.vel_x=0.0f;
				striker.vel_y=0.0f;
				if(striker.active==1)
					score-=5;
				striker.active=0;	
    		}
	}
	
	glutTimerFunc(20, update, 0);	
}
void inputKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : 
			striker.set_pos(-0.1f);
			break;
		case GLUT_KEY_RIGHT : 
			striker.set_pos(0.1f);
			break;
		case GLUT_KEY_UP :
			striker.change_speed(0);
			break;
		case GLUT_KEY_DOWN :
			striker.change_speed(1);
			break;
	}
}

void drawText(char* score) 
{ 
 char *c; 
 glPushMatrix(); 
 glTranslatef(-5.55555f,2.0f ,-8.0f); 
 glScalef(0.75/152.38, 0.75/152.38, 1/152.38);
 for (c=score; *c != '\0'; c++) 
 { 
  glutStrokeCharacter(GLUT_STROKE_ROMAN , *c); 
 } 
 glPopMatrix(); 
}
void start(void){

	sprintf(scoreh,"Choose color:"); 
	drawBoard(scoreh,-3.40f, 1.0f, -8.0f);
	sprintf(scoreh,"Press b for black"); 
	drawBoard(scoreh,-3.20f, 0.0f, -8.0f);
	sprintf(scoreh,"Press w for white"); 
	drawBoard(scoreh,-3.20f, -0.50f, -8.0f);
}
// Function to draw objects on the screen
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(yes==0)
	{
		start();	
	}
	else if(yes==2)
	{
		sprintf(scoreh,"Choose difficulty:"); 
		drawBoard(scoreh,-3.40f, 1.0f, -8.0f);
		sprintf(scoreh,"e for Easy (5 sec=-1 point)"); 
		drawBoard(scoreh,-3.20f, 0.0f, -8.0f);
		sprintf(scoreh,"m for Medium (3 sec=-1 point)"); 
		drawBoard(scoreh,-3.20f, -0.50f, -8.0f);
		sprintf(scoreh,"d for Difficult (1 sec=-1 point)"); 
		drawBoard(scoreh,-3.20f, -1.0f, -8.0f);	
	}
	else
	{	 	
		glColor3f(1.0f,1.0f,0.0f);
		//printf("%d\n",fav_col);
		if(fav_col==1)
		{
			sprintf(scoreh,"White: +10, Black: -5"); 
			drawBoard(scoreh,-2.0f,2.5f,-8.0f);
		}	
		else
		{
			sprintf(scoreh,"Black: +10, White: -5"); 
			drawBoard(scoreh, -2.0f,2.5f,-8.0f);
		}
		//drawText("rashi");
		sprintf(scoreh,"Score:%d",score); 
		glColor3f(1.0f,0.0f,0.0f);
		drawText(scoreh);	//pock.draw_pocket();
		//Draw outer Box
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -8.0f);
		outer.draw_box();
		glPopMatrix();
		//Draw inner box
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -8.0f);
		inner.draw_box();
		glPopMatrix();
		for(int i=0;i<4;i++)
		{
			glPushMatrix();
			pock[i].draw_it();
			glPopMatrix();
		}	
		glColor3f(0.0f, 1.0f, 0.0f);
		for(int i=0;i<4;i++)
		{
			glPushMatrix();
			design[i].draw_it();
			glPopMatrix();
		}	
		Draw_line(1.45f,1.55f,-1.45f,1.55f);
		Draw_line(1.55f,1.45f,1.55f,-1.45f);
		Draw_line(-1.55f,1.45f,-1.55f,-1.45f);
		Draw_line(1.45f,-1.55f,-1.45f,-1.55f);
		Draw_line(1.45f,1.35f,-1.45f,1.35f);
		Draw_line(1.35f,1.45f,1.35f,-1.45f);
		Draw_line(-1.35f,1.45f,-1.35f,-1.45f);
		Draw_line(1.45f,-1.35f,-1.45f,-1.35f);
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -8.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
		float rad = 0.5f;
		for(int i=0 ; i<360 ; i++) {
			glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
		}
		glEnd();
		glPopMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glColor3f(0.513f, 0.011f, 0.0f);
		glPushMatrix();
		striker.draw_striker();
		glPopMatrix();
		for(int i=0;i<9;i++)
		{
			glPushMatrix();
			coin[i].draw_coin();
			glPopMatrix();
		}
	}			

	glutSwapBuffers();
}

// Initializing some openGL 3D rendering options
void initRendering() {

    glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
    glEnable(GL_COLOR_MATERIAL);    // Enable coloring
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
      case 'a':
	striker.change_dir(0);
	break;
      case 'c':
	striker.change_dir(1);
        break;				
      case 32:
	//striker.curr_time=0;
	striker.active=1;
	striker.shoot();
	break;
      case 13:
	striker.reset();
	break;	
      case 'b':
	fav_col=0;
	yes=2;
	//score=30;
	break;
      case 'w':
	fav_col=1;
	yes=2;
	//score=30;
	break;
      case 'e':
	level=5;
	yes=3;
	score=30;
	ans=1;
	break;	
      case 'm':
	level=3;
	yes=3;
	score=30;
	ans=1;
	break;
      case 'd':
	level=2;
	yes=3;
	score=30;
	ans=1;
	break;  		 		 			 		 	
   }
}
void Draw_line(float startx,float starty,float endx, float endy)
{
    glLineWidth(1.5); 
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(startx, starty, -8.0f);
    glVertex3f(endx, endy,-8.0f);
    glEnd();
}


