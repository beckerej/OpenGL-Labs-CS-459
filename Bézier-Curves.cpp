/************************************************************
* This program creates Bézier curves, given a number of pts.
************************************************************/

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

/*  Set initial size of the display window.  */
GLsizei winWidth = 600, winHeight = 600;  

/*number of control points*/
GLint nBezCurvePts;

/*  Set size of world-coordinate clipping window.  */
GLfloat xwcMin = -50.0, xwcMax = 50.0;
GLfloat ywcMin = -50.0, ywcMax = 50.0;

class Point {
	public:
		GLfloat x, y, z;
};

void init (void)
{
	/*  Set color of display window to white.  */
	glClearColor (1.0, 1.0, 1.0, 0.0);
}

void plotPoint (Point bezCurvePt)
{
	glBegin (GL_POINTS);
	glVertex2f (bezCurvePt.x, bezCurvePt.y);
	glEnd ( );
}

/*  Compute binomial coefficients C for given value of n.  */
void binomialCoeffs (GLint n, GLint * C)
{
	GLint k, j;
	for (k = 0;  k <= n;  k++) {
		C [k] = 1;
		for (j = n;  j >= k + 1;  j--)
			C [k] *= j;
		for (j = n - k;  j >= 2;  j--)
			C [k] /= j;
	}
	
}

// x,y, and z coordinates of the points that make the curve (pixels) are computed
Point computeBezPt (GLfloat u, GLint nCtrlPts,
	Point * ctrlPts, GLint * C)
{
	GLint k, n = nCtrlPts - 1;
	GLfloat bezBlendFcn;
	Point bezPt;
	bezPt.x = bezPt.y = bezPt.z = 0.0;

	for(int k = 0; k < nCtrlPts; k++){
		bezPt.x += ctrlPts[k].x * C[k] * pow(u,k) * pow(1-u,n-k);
		bezPt.y += ctrlPts[k].y * C[k] * pow(u,k) * pow(1-u,n-k);
		bezPt.z += ctrlPts[k].z * C[k] * pow(u,k) * pow(1-u,n-k);
	}

	return bezPt;
}

void bezier (Point * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
	Point bezCurvePt;
	GLfloat u;
	GLint *C, k;

	//  Allocate space for binomial coefficients
	C = new GLint [nCtrlPts];

	binomialCoeffs (nCtrlPts - 1, C);
	// finds the x,y,z coordinates for a "Bezier curve point" per u value
	for (k = 0;  k <=nBezCurvePts;  k++) {
		u= (double) k/nBezCurvePts;		
		bezCurvePt = computeBezPt (u, nCtrlPts, ctrlPts, C);
		plotPoint (bezCurvePt);
	}
	delete [ ] C;
}

void displayFcn (void)
{
	// Number of control points and number of curve positions to be plotted along the Bezier curve
	GLint nCtrlPts = 4;

	Point ctrlPts [4] = { {-40.0, -40.0, 0.0}, {-10.0, 40.0, 0.0},
	{10.0, -40.0, 0.0}, {40.0, 40.0, 0.0} };

	glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.
	glPointSize (4);
	glColor3f (1.0, 0.0, 0.0);      //  Set point color to red.

	bezier (ctrlPts, nCtrlPts, nBezCurvePts);
	
	//Plotting the control points
	glPointSize (6);
	glColor3f (0.0, 0.0, 1.0);
	for (int i = 0 ; i<4 ; i++)
		plotPoint(ctrlPts[i]);
	glFlush ( );
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
	//  Maintain an aspect ratio of 1.0.
	glViewport (0, 0, newHeight, newHeight);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ( );
	gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);
	glClear (GL_COLOR_BUFFER_BIT);
}

void main (int argc, char** argv)
{
	//Get the number of pixels (Bezier curve point) from user
	printf("\nHow many Bezier curve point: ");
	scanf("%d",&nBezCurvePts);

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition (50, 50);
	glutInitWindowSize (winWidth, winHeight);
	glutCreateWindow ("Bezier Curve");

	init ( );
	glutDisplayFunc (displayFcn);
	glutReshapeFunc (winReshapeFcn);

	glutMainLoop ( );
}
