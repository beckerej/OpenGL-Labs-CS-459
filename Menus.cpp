/*******************************************************
This program is modified from hello.c by Sumanta Guha to
show a simple implementation of a pop-up menu (with sub-
menu) using the GLUT library. Click on the right mouse 
button.
********************************************************/

#include <stdlib.h>
#include <GL/glut.h>
#include <iostream> 

static GLfloat square_color[3] = {1.0, 0.0, 0.0};

void display(void){
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3fv (square_color);
	glLineWidth(5);
	glPointSize(5);
	glBegin(GL_POLYGON);
	glVertex3f (0.25, 0.25, 0.0);
	glVertex3f (0.75, 0.25, 0.0);
	glVertex3f (0.75, 0.75, 0.0);
	glVertex3f (0.25, 0.75, 0.0);
	glEnd();

	glFlush ();
}

void init (void) {
	glClearColor (0.8, 0.8, 0.8, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void top_menu(int id){
	if (id==1) exit(0);
}

void color_menu(int id){
	if (id==2) {
		// Set the correct color here
		square_color[0] = 1.0;
		square_color[1] = 0.0;
		square_color[2] = 0.0;
	};
	if (id==3){
		// Set the correct color here
		square_color[0] = 0.0;
		square_color[1] = 1.0;
		square_color[2] = 0.0;
	};
	if (id==4){
		// Set the correct color here
		square_color[0] = 0.0;
		square_color[1] = 0.0;
		square_color[2] = 1.0;
	};
	glutPostRedisplay();
}
void mode_menu(int id){//Write your code for "polygon mode sub_menu" here

	if (id==6){// point
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	};
	if (id==7){// line
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	};
	if (id==8){// fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	};
	glutPostRedisplay();
}

int main(int argc, char** argv){  
	int sub_menu;
	int sub_menu2;

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (500,500); 
	glutInitWindowPosition (100,100);
	glutCreateWindow("menu");
	init();
	glutDisplayFunc(display); 

	/* sub-menu is created first */
	sub_menu = glutCreateMenu(color_menu);
	printf("Sub_Menu: %d\n",sub_menu);
	glutAddMenuEntry("Red", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Blue",4);

	sub_menu2 = glutCreateMenu(mode_menu);
	printf("Sub_Menu2: %d\n",sub_menu2);
	glutAddMenuEntry("Point", 6);
	glutAddMenuEntry("Line", 7);
	glutAddMenuEntry("Fill", 8);

	/* then the top menu is created */
	glutCreateMenu(top_menu);
	glutAddMenuEntry("Quit",1);
	glutAddSubMenu("Change Color", sub_menu);
	glutAddSubMenu("Change Polygon Mode", sub_menu2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);//Menu is assigned to the right mouse button
	glutMainLoop();
	return 0;  
}

