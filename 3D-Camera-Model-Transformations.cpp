/******************************************************
This example views a model and changes the different 
angles and perspectives of the camera and lightsource.
*******************************************************/

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265
static float alpha = 0.0;
static float beta = PI/6.0;
static GLdouble cpos[3];
static int angle = 45;
static GLfloat lpos[] = {0.0, 3.0, 0.0, 1.0};
static GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
static GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
static GLint w = 1200;
static GLint h = 800;

void writemessage(){
	printf("\n\
		   3D Camera Transformations & Models \n\
		   x, X, y, Y, z, Z ---------- change light source position\n\
		   w-------------------------- toggle between wire and solid modes\n\
		   1,2,3,4 ------------------- change the camera position\n\
		   5,6 ----------------------- zoom in and zoom out\n\
		   0 ------------------------- turn the ligt on\off\
		   ESC ----------------------- exit\n\
		   \n");
}

void reshape(int w, int h){
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective(angle, (GLfloat) w / (GLfloat) h, 1.5, 20.0);
	cpos[0] = 10.0 * cos(beta) * sin(alpha);
	cpos[1] = 10.0 * sin(beta);
	cpos[2] = 10.0 * cos(beta) * cos(alpha);
	gluLookAt(cpos[0],cpos[1],cpos[2], 0.0,1.0,0.0, 0.0,1.0,0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);	
	glPushMatrix();
	glTranslatef(lpos[0],lpos[1],lpos[2]);

	if(glIsEnabled(GL_LIGHT0))
		glMaterialfv(GL_FRONT, GL_EMISSION, white);
	else
		glMaterialfv(GL_FRONT, GL_EMISSION, black);

	glutSolidSphere(0.1, 10, 8);
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glFrontFace(GL_CW);
	glutSolidTeapot(1.);
	glFrontFace(GL_CCW);
	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
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

		case '1':
			beta = PI/2.0;
			glutPostRedisplay();
			break;

		case '2':
			beta = PI/3.0;
			glutPostRedisplay();
			break;

		case '3':
			beta = PI/4.0;
			glutPostRedisplay();
			break;

		case '4':
			beta = PI/5.0;
			glutPostRedisplay();
			break;

		case '5':
			if(angle>10) 
				angle -= 10;
			glutPostRedisplay();
			break;

		case '6':
			if(angle<120) 
				angle += 10;
			glutPostRedisplay();
			break;

		case '0':
			if(glIsEnabled(GL_LIGHT0))
				glDisable(GL_LIGHT0);
			else
				glEnable(GL_LIGHT0);
			glutPostRedisplay();
			break;
		
		default:
			break;
		}
	}

	void specialkey(GLint key, int x, int y){
		switch (key) {
		case GLUT_KEY_RIGHT:                
			alpha = alpha + PI/180;
			if (alpha > 2*PI) alpha = alpha - 2*PI;
				glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:                 
			alpha = alpha - PI/180;
			if (alpha < 0) alpha = alpha + 2*PI;
				glutPostRedisplay();
			break;
		case GLUT_KEY_UP:                
			if (beta < 0.45*PI) beta = beta + PI/180;
				glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:                 
			if (beta > -0.45*PI) beta = beta - PI/180;
				glutPostRedisplay();
			break;

		default:
			break;  
	}
}

int main(int argc, char** argv){
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

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkey);

	glutMainLoop();
	return 0;
}

