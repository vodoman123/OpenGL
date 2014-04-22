//
//  main.cpp
//  MP1_CS418
//
//  Created by harsha on 2/4/14.
//  Copyright (c) 2014 harsha. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>
#include <ctime>
#include <cmath>
#include <stdlib.h>

int nFPS = 33;
clock_t startClock=0,curClock;
long long int prevF=0,curF=0;
int dipMode=1;

bool init()
{
    return true;
}

void draw()//drawing the actual triangle
{
    glColor3f(1.0, .5, 0.0);

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(-.05,.15);
        glVertex2f(-.15,.15);
        glVertex2f(-.15,.25);
        glVertex2f(.15,.25);
        glVertex2f(.05,.15);
        glVertex2f(.05,-.15);
        glVertex2f(-.05,-.15);
    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(-.05,-.15);
        glVertex2f(-.15,-.15);
        glVertex2f(-.15,-.25);
        glVertex2f(.15,-.25);
        glVertex2f(.15,-.15);
    glEnd();
    
    glBegin(GL_TRIANGLES);
        glVertex2f(.05, .15);
        glVertex2f(.15, .15);
        glVertex2f(.15, .25);
    glEnd();
}

void display()
{
    if(dipMode==1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
    
    glClearColor(0.0, 0.0, 1.0, 1.0);

    glClear(GL_COLOR_BUFFER_BIT);
    
    // reset OpenGL transformation matrix
	glMatrixMode(GL_MODELVIEW);
    
	glLoadIdentity(); // reset transformation matrix to identity
    
	glScalef(sin(curClock),cos(curClock),0);
    
    draw();
    
    glFlush();
    
    glutSwapBuffers();
    
    curClock=clock();
	float elapsed=(curClock-startClock)/(float)CLOCKS_PER_SEC;
		float fps=(float)(curF-prevF)/elapsed;
		printf("fps:%f\n",fps);
	
}

void keyboard(unsigned char key, int x, int y)
{
	//Checks to see if the esc key was pressed
	if (key == 27)
	{
		// esc hit, so quit
		exit(0);
	}
    
    if( key == 'h'){
		dipMode = 1-dipMode;
	}

}

void timer(int v)
{
	glutPostRedisplay(); // trigger display function by sending redraw into message queue
	glutTimerFunc(1000/nFPS,timer,v); // restart timer again
}

int main(int argc, char * argv[])
{
    //Initialize GLUT
    glutInit(&argc, argv);
    
    //Set the Window Size and Position
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(500, 500);
    
    //Creates the Window and names it MP1
    glutCreateWindow("MP1");
    
    //displays the Window
    glutDisplayFunc(display);
    
    // called when received keyboard interaction
    glutKeyboardFunc(keyboard);
    
    glutTimerFunc(100,timer,nFPS); // a periodic timer. Usually used for updating animation
	
	startClock=clock();
    
    //Event Processing Loop
    glutMainLoop();
    
    return 0;
}

