/*******************************************************
This program does example line clipping with a square &
a line.
********************************************************/

#include <GL/glut.h>   
#include <stdlib.h>
#include <iostream>

class scrPt {
public:
	GLint x, y;
};
scrPt vert1, vert2, vert3,vert4;
scrPt lineVert1, lineVert2, lineVert3,lineVert4;
int t=0,b=0,l=0,r=0;
const int width=600, height=600;


void init (void)
{
	glClearColor (1.0, 1.0, 1.0, 1.0);  // Set display-window color to white.
	glMatrixMode (GL_PROJECTION);       // Set projection parameters.
	gluOrtho2D (0.0, 200.0, 0.0, 150.0); // clipping 
}

// Exercise checking for the intersections
void topIntersection(){
	if(x_intersection<=vert3.x && x_intersection>=vert4.x && vert4.y<=lineVert4.y){
		t = 1;
	}
	else{
		t = 0;
	}
}
void rightIntersection(){
	int y_intersection = vert3.x - 100;

	if(y_intersection>=vert2.y && y_intersection<=vert3.y && vert2.x<=lineVert4.x){
		r = 1;
	}
	else{
		r = 0;
	}
}

void leftIntersection(){
	int y_intersection = vert1.x - 100;

	if(y_intersection>=vert1.y && y_intersection<=vert4.y && vert2.x<=lineVert4.x){
		l = 1;
	}
	else{
		l = 0;
	}
}
void bottomIntersection(){
	int x_intersection = vert1.y + 100;

	if(x_intersection<=vert2.x && x_intersection>=vert1.x && vert1.y>=lineVert1.y){
		b = 1;
	}
	else{
		b = 0;
	}
}

//finding the intersections
void findBottomInterxection(int v){
	if (v==2){
		lineVert2.x = vert1.y + 100;
		lineVert2.y = vert1.y;
	}
	else{
		lineVert3.x = vert1.y + 100;
		lineVert3.y = vert1.y;
	}
}
void findLeftInterxection(int v){
	if (v==2){
		lineVert2.x = vert1.x;
		lineVert2.y = vert1.x - 100;
	}
	else{
		lineVert3.x = vert1.x;
		lineVert3.y = vert1.x - 100;
	}
}
void findRightInterxection(int v){
	if (v==2){
		lineVert2.x = vert2.x;
		lineVert2.y = vert2.x - 100;
	}
	else{
		lineVert3.x = vert2.x;
		lineVert3.y = vert2.x - 100;
	}
}
void findTopInterxection(int v){
	if (v==2){
		lineVert2.x = vert4.y + 100;
		lineVert2.y = vert4.y;
	}
	else{
		lineVert3.x = vert4.y + 100;
		lineVert3.y = vert4.y;
	}
}
void findIntersection(){
	topIntersection();
	rightIntersection();
	leftIntersection();
	bottomIntersection();

	if (r+l+t+b ==2){
		if (r==1 && l==1){
			findRightInterxection(3);
			findLeftInterxection(2);
		}
		else if(r==1 && t==1){
			findRightInterxection(3);
			findTopInterxection(2);
		}
		else if (r==1 && b==1){
			findRightInterxection(3);
			findBottomInterxection(2);
		}
		else if (l==1 && t==1){
			findLeftInterxection(2);
			findTopInterxection(3);
		}
		else if (l==1 && b==1){
			findLeftInterxection(2);
			findBottomInterxection(3);
		}
		else if (t==1 && b==1){
			findTopInterxection(2);
			findBottomInterxection(3);
		}
	}
	else if (r+l+t+b ==3){
		lineVert2.x=vert1.x;
		lineVert2.y=vert1.y;
		lineVert3.x=vert3.x;
		lineVert3.y=vert3.y;
	}
}

void display (void){
	glLineWidth(3);
	glClear (GL_COLOR_BUFFER_BIT);  
	glColor3f (0, 0, 0);   
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	glBegin (GL_POLYGON);
	glVertex2i (vert1.x, vert1.y);  
	glVertex2i (vert2.x, vert2.y);
	glVertex2i (vert3.x, vert3.y);
	glVertex2i (vert4.x, vert4.y);
	glEnd ( );

	glBegin (GL_LINES);
	glColor3f (0, 0, 1);
	glVertex2i (lineVert1.x, lineVert1.y); 
	glVertex2i (lineVert2.x, lineVert2.y);
	glColor3f (1, 0, 0);
	glVertex2i (lineVert2.x, lineVert2.y);
	glVertex2i (lineVert3.x, lineVert3.y);
	glColor3f (0, 0, 1);
	glVertex2i (lineVert3.x, lineVert3.y);
	glVertex2i (lineVert4.x, lineVert4.y);
	glEnd ( );
	glFlush ( );     // Process all OpenGL routines as quickly as possible.
	glutSwapBuffers();
}

void winReshapeFcn (int newWidth, int newHeight){
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ( );
	gluOrtho2D (0.0, (GLdouble) newWidth, 0.0, (GLdouble) newHeight);
	glClear (GL_COLOR_BUFFER_BIT);  
}

void spec ( int key, int x, int y ){
	switch ( key )
	{
	case GLUT_KEY_UP : 
		printf ("UP arrow pressed\n"); 
		if(vert1.y<=height-10){
			vert1.y += 10;
			vert2.y += 10;
			vert3.y += 10;
			vert4.y += 10;
		}
		break;
	case GLUT_KEY_DOWN : 
		printf ("DOWN arrow pressed\n"); 
		if (vert3.y>=10){
			vert1.y -= 10;
			vert2.y -= 10;
			vert3.y -= 10;
			vert4.y -= 10;
		}
		break;
	case GLUT_KEY_RIGHT : 
		printf ("RIGHT arrow pressed\n");
		if(vert2.x<=lineVert4.x-10){
			vert1.x += 10;
			vert2.x += 10;
			vert3.x += 10;
			vert4.x += 10;
		}
		break;
	case GLUT_KEY_LEFT : 
		printf ("LEFT arrow pressed\n");
		if (vert1.x >=lineVert1.x+10){
			vert1.x -= 10;
			vert2.x -= 10;
			vert3.x -= 10;
			vert4.x -= 10;
		}
		break;
	}

	r=t=l=b=0;
	findIntersection();
	display();
}

void writeMessage(){
	printf("* ++++++++++++++++++++++++++++++++++++++++++ *\n");
	printf("-->\t press arrow keys to translate the square\n");
}

void main (int argc, char** argv){
	writeMessage();
	glutInit (&argc, argv);                         // Initialize GLUT.
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);  // Set display mode.(State machine)
	glutInitWindowPosition (350, 100);   // Set top-left display-window position.
	glutInitWindowSize (width, height);      // Set display-window width and height.
	glutCreateWindow ("Line clipping - Evan Becker"); // Create display window.

	//polygon
	vert1.x = 200;
	vert1.y = 200;  
	vert2.x = 400;
	vert2.y = 200;
	vert3.x = 400;
	vert3.y = 400;
	vert4.x = 200;
	vert4.y = 400;

	//line
	lineVert1.x = 200;
	lineVert1.y = 100;
	lineVert2.x = 300;
	lineVert2.y = 200;
	lineVert3.x = 400;
	lineVert3.y = 300;
	lineVert4.x = 500;
	lineVert4.y = 400;

	init ( );

	// Execute initialization procedure.
	glutDisplayFunc (display);       // Send graphics to display window.
	glutReshapeFunc (winReshapeFcn);
	glutSpecialFunc( spec ) ;
	glutMainLoop ( );  // Display everything and wait.(Event driven language)
}
