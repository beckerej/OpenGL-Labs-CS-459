/********************************************************
Uses glutTimerFunc() to smoothly start and stop animation
*********************************************************/

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h> // for Sleep() 
#include <math.h> // for sin() and cos()

static GLfloat angle1 = 0.0, angle2 = 0.0, angle3 = 0.0;

static GLfloat position[] = {10.0, 10.0, 10.0, 1.0};
static GLdouble cpos[]    = {0.0, 10.0, 200.0};
static GLfloat none[]     = {0.0, 0.0, 0.0, 1.0};
static GLfloat white[]    = {1.0, 1.0, 1.0, 1.0};
static GLfloat red[]     = {1.0, 0.0, 0.0, 1.0}; 
static GLfloat green[]    = {0.0, 1.0, 0.0, 1.0};
static GLfloat orange[]      = {1.0, 0.5, 0.0, 1.0};
static GLfloat blue[]      = {0.0, 0.0, 1.0, 1.0};
static const GLfloat PI = 3.1415;
void writemessage(){
	printf("\n\
		   3D Animation Transformation\n\
		   left mouse bottom --------- start animation\n\
		   right mouse button -------- stop animation\n\
		   w ------------------------- wireframe/fill toggle\n\
		   ESC ----------------------- exit\n\
		   \n");
}

void reshape(int w, int h){
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(8.0, (GLfloat) w / (GLfloat) h, 1.0, 250.0);
}
void drawOrbit(float radius){
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<360;i++)
	{
		float degInRad = i * (PI/180);
		glVertex3f(cos(degInRad)*radius,0,sin(degInRad)*radius);
	}
	glEnd();
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cpos[0], cpos[1], cpos[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, orange);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, orange);

	glPushMatrix();

	glRotatef(45,1,1,0);
	glRotatef(angle3,1,0,0);//rotate the center cube about the x axis by "angle3"
	glutSolidCube(2);      // draw the cube at the center
	 // rotate back the center cube about the x axis
	glRotatef(-angle3,1,0,0);
	glRotatef(-45,1,1,0);


	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, none);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
	glRotatef(90,1,0,0);
	drawOrbit(12);
	glRotatef(-90,1,0,0);

	glRotatef(angle1,0,0,1);// find direction of the red rotating cube using the "angle1"
	glTranslatef(12.0, 0.0, 0.0);         // go to position of the cube 
	glRotatef(-angle1,0,0,1);// restore initial orientation of cube

	// memorize state just after reaching position of the buce
	glPushMatrix();

	// rotate the cube about both x and y axis by 45 degrees
	glRotatef(45,1,1,0);
	// rotate the cube about its z axis by "angle2"	
	glRotatef(angle2,0,0,1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red); // Set the color to be red for the rotating cube
	glutSolidCube(3);          // draw the cube

	 // return to state just after reaching  the cube
	glPopMatrix();

	glPopMatrix();

	//rotate by 30 degrees about z axis - sets the green orbits orientation
	glRotatef(30,0,0,1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, green);//  set the color to green
	drawOrbit(6);// draw the orbit with radius 6

	glRotatef(angle2,0,1,0);// rotate about y axis by "angle2" - sets the direction to put the green cube on the orbit
	glTranslatef(6.0,0.0,0.0);// translate along x axis by 6 - puts the green cube on the orbit
	glutSolidCube(1);

	glFlush();
	glutSwapBuffers();
}

void animate(void){
	angle1 = angle1 + 0.2;
	if (angle1 > 360.0) angle1 = angle1 - 360.0;
	angle2 = angle2 + 1.0;
	if (angle2 > 360.0) angle2 = angle2 - 360.0;
	angle3 = angle3 + 4.0;
	if (angle3 > 360.0) angle3 = angle3 - 360.0;  
	Sleep(10);
	glutPostRedisplay();  
}


void mouse(int button, int state, int x, int y){
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(animate); 
		break;
	case GLUT_MIDDLE_BUTTON:
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y){
	static int polygonmode[2];

	switch (key) {
	case 'w':
		glGetIntegerv(GL_POLYGON_MODE,polygonmode);
		if (polygonmode[0]==GL_FILL) 
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 800); 
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3D Transformations");

	glClearColor(0.5, 0.5, 0.5, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	writemessage();
	glutMainLoop();
	return 0;
}
