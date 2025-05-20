#include<windows.h>
#include <GL/glut.h>
#include<bits/stdc++.h>
#include <stdlib.h>
#define rad (3.1416/180)
#define EN_SIZE 10

#include "objL/model.h"

using namespace std;

float zoom=4;
float tX=0,tY=0,tZ=-8,rX=0,rY=0,rZ=4;
float tZ1=-20,tZ2=-40,tZ3=-60,tZ4=-80,tZ5=-100,tZ6=-120;
float rotX=0,rotY=0,rotZ=0;
float cosX=0,cosY=1,cosZ=0;
float angle=0;
float cenX=0,cenY=0,cenZ=0,roll=0;
float radius=0;
float theta=0,slope=0;
float speed = 0.3;
float angleBackFrac = 0.2;
float r[] = {0.1,0.4,0.0,0.9,0.2,0.5,0.0,0.7,0.5,0.0};
float g[] = {0.2,0.0,0.4,0.5,0.2,0.0,0.3,0.9,0.0,0.2};
float b[] = {0.4,0.5,0.0,0.7,0.9,0.0,0.1,0.2,0.5,0.0};
int TIME=0;
bool START = false;
float diamondPosX[7] = {1,-2,3,-4,-2,0,2};
float diamondPosY[7] = {2,3,10,6,7,4,1};
bool diamondCollected[7] = {false, false, false, false, false, false, false};

bool rot = false;

Model plane_model;
Model ground_model;
Model house_model;
Model house2_model;
Model tree1_model;
Model tree2_model;
Model tree3_model;
Model tree4_model;
Model rock1_model;
Model rock2_model;
Model diamond_model;

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ground(){
    ground_model.draw();
}

void house(){
    house_model.draw();
}

void house2(){
    house2_model.draw();
}

void tree1(){
    tree1_model.draw();

}
void tree2(){
    tree2_model.draw();

}
void tree3(){
    tree3_model.draw();
}
void tree4(){
    tree4_model.draw();
}

void plane(){
    plane_model.draw();
}

void rock1(){
    rock1_model.draw();
}

void rock2(){
    rock2_model.draw();
}

void diamond(){
    diamond_model.draw();
}

int score = 0;
void environment(int n){
    if (!diamondCollected[n] && 
        diamond_model.check_collision(0, 1, 0, 
                                    tX + diamondPosX[n],
                                    tY + diamondPosY[n], 
                                    tZ, 
                                    3.0f)) 
    {
        diamondCollected[n] = true;
        score += 1;
        cout << "Score: " << score << endl;
    }
    glPushMatrix();
        glTranslated(28,0,0);
        glScaled(EN_SIZE*2,10,EN_SIZE*2);
        ground();
    glPopMatrix();

    glPushMatrix();
        glTranslated(diamondPosX[n],diamondPosY[n],0);
        glScaled(3,3,3);
        diamond();
    glPopMatrix(); 

    glPushMatrix();
        glTranslated(-2,0,-5);
        glScaled(2,2,2);
        tree1();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-35,0,0);
        glScaled(3,3,3);
        tree3();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-8,0,3);
        glRotated(90, 0, 1, 0);
        glScaled(3,3,3);
        house();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-14,0,6);
        glScaled(2,2,2);
        tree4();
    glPopMatrix();

    glPushMatrix();
        glTranslated(9,0,-5);
        glRotated(-65,0,1,0);
        glScaled(3,3,3);
        tree2();
    glPopMatrix();

    glPushMatrix();
       glTranslated(-12,0,-5);
        glScaled(2,2,2);
        rock1();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-15,0,-20);
        glScaled(2,2,2);
        rock2();
    glPopMatrix();  

    glPushMatrix();
        glTranslated(9,0,-7);
        glScaled(2,2,2);
        house2();
    glPopMatrix();      
}

void draw(){
    double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double a = t*90.0;

    TIME = t;

    if(rotX>11)rotX=11;
    if(rotX<-11)rotX=-11;
    if(rotZ>10)rotZ=10;
    if(rotZ<-15)rotZ=-15;

    glPushMatrix();
        glTranslated(0,1,0);
        glRotated(90,0,1,0);
        glRotated(5,0,0,1);
        glRotated(rotX,1,0,0);
        glRotated(rotY,0,1,0);
        glRotated(rotZ,0,0,1);

        glScaled(0.4,0.4,0.4);
        plane();
    glPopMatrix();

    ///Environment
    if(tX>=4.1)tX=4.1;
    if(tX<=-4.1)tX=-4.1;
    if(tY>0.1)tY= 0.1;
    if(tY<-15)tY= -15;

    glPushMatrix();
        glTranslated(tX,tY,tZ);
        environment(2);
    glPopMatrix();

    glPushMatrix();
        glTranslated(tX,tY,tZ2);
        environment(3);
    glPopMatrix();

    glPushMatrix();
        glTranslated(tX,tY,tZ3);
        environment(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(tX,tY,tZ4);
        environment(5);
    glPopMatrix();

    glPushMatrix();
        glTranslated(tX,tY,tZ5);
        environment(4);
    glPopMatrix();

    glPushMatrix();
        glTranslated(tX,tY,tZ6);
        environment(6);
    glPopMatrix();

    tZ+=speed;
    tZ1+=speed;
    tZ2+=speed;
    tZ3+=speed;
    tZ4+=speed;
    tZ5+=speed;
    tZ6+=speed;

    if(tZ>=20)tZ=-110;
    if(tZ1>=20)tZ1=-110;
    if(tZ2>=20)tZ2=-110;
    if(tZ3>=20)tZ3=-110;
    if(tZ4>=20)tZ4=-110;
    if(tZ5>=20)tZ5=-110;
    if(tZ6>=20)tZ6=-110;

    if(rotX>0)rotX-=angleBackFrac;
    if(rotX<0)rotX+=angleBackFrac;
    if(rotY>0)rotY-=angleBackFrac;
    if(rotY<0)rotY+=angleBackFrac;
    if(rotZ>0)rotZ-=angleBackFrac;
    if(rotZ<0)rotZ+=angleBackFrac;

    speed += 0.0002;
    if(speed>=0.7)speed=0.7;
}

void drawStrokeText(string str,int x,int y,int z)
{
	  glPushMatrix();
	  glTranslatef(x, y+8,z);
	  glScalef(0.002f,0.002f,z);

	  for (int i =0; i < str.size(); i++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , str[i]);
	  }
	  glPopMatrix();
}

void drawStrokeText2(string str,int x,int y,int z)
{
	  glPushMatrix();
	  glTranslatef(x, y+8,z);
	  glScalef(0.005f,0.005f,z);

	  for (int i =0; i < str.size(); i++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , str[i]);
	  }
	  glPopMatrix();
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double a = t*90.0;
    double aa=a;

    if(!rot){
        a=0;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(	0.0, 4.5, 10.0,
                0, 4, 0,
                0, 1.0f, 0.0f);

    if(START){

        glPushMatrix();
            glTranslated(0,0,0);
            glScaled(zoom,zoom,zoom);
            glRotated(a,0,1,0);
            draw();
        glPopMatrix();

        drawStrokeText("UP: W",-8,0,0);
        drawStrokeText("DOWN: S",-8,-1,0);
        drawStrokeText("LEFT: A",-8,-2,0);
        drawStrokeText("RIGHT: D",-8,-3,0);
        drawStrokeText("MAIN MENU: M",-8,-4,0);
        drawStrokeText("TIME : ",3,0,0);
        drawStrokeText(std::to_string(TIME),5,0,0);
        drawStrokeText("SCORE : ",-3,0,0);
        drawStrokeText(std::to_string(score),-1,0,0);
        TIME = TIME + 1;
    }
    else{
        glPushMatrix();
            glTranslated(0,3,0);
            glRotated(aa,0,1,0);
            glScaled(1.5,1.5,1.5);
            plane();
        glPopMatrix();

        drawStrokeText("Press G to Start Game",-5,-2,0);
        drawStrokeText2("Open Flight FX",-5,-1,0);
    }

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y){
    float frac = 0.3;
    float rotFrac = 1;
    switch (key){
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'r':
            rot=true;
            break;
        case 't':
            rot=false;
            break;
        case 'z':
            zoom+=0.05;
            break;
        case 'Z':
            zoom-=0.05;
        case 'w':
            tY-=frac;
            rotZ+=rotFrac;
            break;
        case 's':
            tY+=frac;
            rotZ-=rotFrac;
            break;
        case 'a':
            tX+=frac;
            rotX-=rotFrac*3;
            rotY+=rotFrac/2;
            break;
        case 'd':
            tX-=frac;
            rotX+=rotFrac*3;
            rotY-=rotFrac/2;
            break;
//        
        case 'g':
            START=true;
            break;
        case 'm':
            START=false;
            break;
    }
    glutPostRedisplay();
}

static void idle(void){glutPostRedisplay();}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowPosition(0,0);
	glutInitWindowSize(1920,1070);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glutSetOption(GLUT_MULTISAMPLE, 8);

    glutCreateWindow("Open Filght FX");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    plane_model.load("models/plane.obj");
    ground_model.load("models/ground.obj");
    house_model.load("models/house.obj");
    house2_model.load("models/house2.obj");
    tree1_model.load("models/tree1.obj");
    tree2_model.load("models/tree2.obj");
    tree3_model.load("models/tree3.obj");
    tree4_model.load("models/tree4.obj");
    rock1_model.load("models/rock1.obj");
    rock2_model.load("models/rock2.obj");
    diamond_model.load("models/diamond.obj");

    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glDisable(GL_CULL_FACE);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
