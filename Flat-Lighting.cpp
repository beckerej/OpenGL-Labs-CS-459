/**************************************************************
*  Simple test of lights on 2 planes for comparison
*  between a small number and large amount of polygons
***************************************************************/

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static GLfloat lpos[] = {2.7, 1.0, 2.2, 1.0};
static GLfloat white[]   = {1.0, 1.0, 1.0, 1.0};
static GLfloat black[]   = {0.0, 0.0, 0.0, 1.0};
static GLfloat red[]   = {1.0, 0.0, 0.0, 1.0};
static GLfloat blue[]   = {0.0, 0.0, 1.0, 1.0};
static GLfloat green[]   = {0.0, 1.0, 0.0, 1.0};


void writemessage()
{
	printf("\n\
		   Flat Lighting\n\
		   x, X, y, Y, z, Z ---------- change light source position\n\
		   w-------------------------- toggle between wire and solid modes\n\
		   ESC ----------------------- exit\n\
		   \n");
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

void drawPlane(float xlen, float ylen, int n, int xoffset, int yoffset){
	
	//Declare xb,yb,xt,yt to hold the x and y coordinates of the bottom left and top right corners of each cell
	GLfloat** xb = new GLfloat*[n];
	for(int i = 0; i < n; i++) xb[i] = new GLfloat[n];

	GLfloat** yb = new GLfloat*[n];
	for(int i = 0; i < n; i++) yb[i] = new GLfloat[n];

	GLfloat** xt = new GLfloat*[n];
	for(int i = 0; i < n; i++) xt[i] = new GLfloat[n];

	GLfloat** yt = new GLfloat*[n];
	for(int i = 0; i < n; i++) yt[i] = new GLfloat[n];

	
	//Compute the width and height of each cell and declare "xStep" and "yStep" variables to hold width and height, respectively.
	GLfloat xStep = xlen / n;
	GLfloat yStep = ylen / n;

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			xb[i][j] = xoffset + xStep * i;
			yb[i][j] = yoffset + yStep * j;
			//glNormal3f(xb[i][j],yb[i][j],2.0);
			xt[i][j] = xoffset + xStep * (i+1);
			yt[i][j] = yoffset + yStep * (j+1);
			//glNormal3f(xt[i][j],yt[i][j],2.0);
		}
	}
		
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red); // The color isn't changing?
			glBegin(GL_POLYGON);
				glVertex3f(xb[i][j],yb[i][j],2);
				glVertex3f(xb[i][j],yt[i][j],2);
				glVertex3f(xt[i][j],yt[i][j],2);
				glVertex3f(xt[i][j],yb[i][j],2);
			glEnd();
		}
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glMaterialfv(GL_FRONT, GL_EMISSION, white); 
	glPushMatrix();
	glTranslatef(lpos[0],lpos[1],lpos[2]);
	glutSolidSphere(0.1, 10, 8);
	glPopMatrix();


	//Draw your objects here
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	drawPlane(2,2,4,-1,0);
	drawPlane(2,2,30,2,0);

	glFlush();
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	static int polygonmode[2];
	switch (key) {
	case 27:
		exit(0);
		break; 
	case 'x':                
		lpos[0] = lpos[0] + 0.2;
		glutPostRedisplay();
		break;
	case 'X':                 
		lpos[0] = lpos[0] - 0.2;
		glutPostRedisplay();
		break;
	case 'y':                
		lpos[1] = lpos[1] + 0.2;
		glutPostRedisplay();
		break;
	case 'Y':                 
		lpos[1] = lpos[1] - 0.2;
		glutPostRedisplay();
		break;
	case 'z':                
		lpos[2] = lpos[2] + 0.2;
		glutPostRedisplay();
		break;
	case 'Z':                 
		lpos[2] = lpos[2] - 0.2;
		glutPostRedisplay();
		break;
	case 'w':
		glGetIntegerv(GL_POLYGON_MODE,polygonmode);
		if (polygonmode[0]==GL_FILL) 
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	writemessage(); 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); 
	glutInitWindowSize(1200, 800); 
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();       
	gluLookAt(3.0,5.0,10.0, 0.0,1.0,0.0, 0.0,1.0,0.0);

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}

