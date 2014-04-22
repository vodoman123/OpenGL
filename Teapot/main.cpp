#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <ctime>
#include <cmath>
#include "glm.h"
#include "imageloader.h"
#include <fstream>

 using namespace std;

//setting global variables
GLfloat speed;
GLMmodel *objmodel_ptr;
float fRotateAngle = 0.f;
#define PI 3.14159265358979323846
GLuint texid[1];
GLuint _textureId; //The id of the texture

static GLfloat lightposition[] = {-10.0, 10.0, 10.0, 0.0};
static GLfloat lightambient[] = {0.3, 0.3, 0.3, 1.0};
static GLfloat lightspecular[] = {0.8, 0.6, 0.8, 1.0};
static GLfloat lightdiffuse[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat materialdiffuse[] = {1.0, 0.7, 1.0, 0.0};


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                 //as unsigned numbers
				 image->pixels);//The actual pixel data
	return textureId; //Returns the id of the texture
}

// Load a PPM Image that does not contain comments

unsigned char * loadImage(char *ifile,  int &w, int &h)
{
	char dummy1=0, dummy2=0; int maxc;
	unsigned char * img;
	ifstream fileinput(ifile, ios::binary);
    
	fileinput.get(dummy1);
	fileinput.get(dummy2);
    
    fileinput >> w >> h;
    fileinput >> maxc;
    fileinput.get(dummy1);
	
    img=new  unsigned char[3*w*h];
	fileinput.read((char *)img, 3*w*h);
	fileinput.close();
	return img;
}

int SetupTexturewithMipmap(char* filename, GLuint id)
{
    int size;
    GLubyte* data;
    
    data=loadImage(filename,size,size);
    cout<<"Texture size:"<<size<<endl;
    glBindTexture(GL_TEXTURE_2D, id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, size, size,  GL_RGB, GL_UNSIGNED_BYTE, data);
    
    return 1;
}

void init()//usual stuff
{
    
    GLuint textures[2];
    glGenTextures(2, textures);
    
    int w, h;
    
    Image* image = loadBMP("/Users/harsha_bande/Dropbox/CS418/CS418_MP3/stonewall.bmp");
    textures[0] = loadTexture(image);
    delete image;
    
    glClearColor(0.5, 0.5, 1.0, 0.0); //background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(54.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightspecular);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    
    // Environment mapping
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    // Environment mapping
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    SetupTexturewithMipmap("/Users/harsha_bande/Dropbox/CS418/CS418_MP3/lightball.ppm", texid[0]);

    //loading the obj file
    if (!objmodel_ptr)
    {
        objmodel_ptr = glmReadOBJ("/Users/harsha_bande/Dropbox/CS418/CS418_MP3/teapot.obj"); //loading the obj have to change the path
        if (!objmodel_ptr)
            exit(0);
        
        glmUnitize(objmodel_ptr);
        glmFacetNormals(objmodel_ptr);
        glmVertexNormals(objmodel_ptr, 90.0);
    }
    
}

void display()//usual stuff
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    //transformation
    GLfloat model[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, model);
    glLoadIdentity();
    gluLookAt(2.5,1.0,1.0,0.0,0.0,0.0,0.0,100.0,0.0);//first teapot
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialdiffuse);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glutSolidTeapot(1.0);
    //glmDraw(objmodel_ptr, GLM_SMOOTH);//draws the object
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_S);
    glPopMatrix();
	gluLookAt(3,0.5,2.0,0.0,0.0,0.0,0.0,100.0,0.0);//second teapot
    glRotatef(fRotateAngle,0.f,1.f,0.f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialdiffuse);
    glBindTexture(GL_TEXTURE_2D, texid[0]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    
    glmDraw(objmodel_ptr, GLM_SMOOTH);//draws the object
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_S);
    glPopMatrix();
    
    fRotateAngle += 1.f;
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (float)w / h, 0.01, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

//keyboard controls
void keyboard(unsigned char key, int x, int y) {
    if (key == 27)
        exit(0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}