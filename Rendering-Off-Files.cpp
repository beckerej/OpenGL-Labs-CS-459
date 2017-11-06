////////////////////////////////////////////////////////
// Rendering .off Files
////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h> 
#include <string.h>
#include <cstdlib>//for random numbers

#define PI 3.14159265 

static float alpha = 0.0;
static float beta = PI/6.0;
static GLdouble cpos[3];
static GLint angleView=30.0;
static GLint flag = 0;//used for color change

static GLfloat lpos[]  = {-1.0, 2.0, 1.0, 1.0};
static GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
static GLfloat red[]   = {1.0, 0.0, 0.0, 1.0};
static GLfloat blue[]  = {0.0, 0.0, 1.0, 1.0};

static GLint numVertices,numPolygons,numEdges;
static GLfloat **vdata;   //array for vertex data
static GLuint **pdata;    //array for polygon data (vertex indices)
static GLuint *psize;     //array for polygon size
static GLfloat **normals; //array for polygon normals
static GLfloat **cdata;//array for polygon color

#define min(x,y) ((x)<(y) ? (x) : (y))
#define max(x,y) ((x)>(y) ? (x) : (y))


void writemessage()
{
	printf("\n\
		   Rendering .off files\n\
		   Reads an OFF file and renders the object\n\
		   c ------------------------- change off files\n\
		   x, X, y, Y, z, Z ---------- change light source position\n\
		   w-------------------------- toggle between wire and solid modes\n\
		   t-------------------------- change color\n\
		   ESC ----------------------- exit\n\
		   \n");
}


void readOFF(){
	int n,j;
	int a,b;
	float x,y,z;
	float resize;
	char line[256];

	FILE* fin = NULL;
	char filename[FILENAME_MAX];

	while (fin == NULL) {
		printf("\n\nEnter an .off file name including extension (or press Enter to abort): ");
		int result=scanf("%99[^\n]%*c",filename);
		if(result!=1) exit(0);
		printf("Opening %s...\n",filename);
		fin =  fopen(filename,"rb");
	};

	/* OFF format */
	while (fgets(line,256,fin) != NULL) {
		if (line[0]=='O' && line[1]=='F' && line[2]=='F')
			break;
	}

	fscanf(fin,"%d %d %d\n",&numVertices,&numPolygons,&numEdges);
	printf("Number of vertices  = %d\n",numVertices);
	printf("Number of polygons = %d\n",numPolygons);
	printf("Number of edges = %d\n",numEdges);
	printf("Loading vertices and polygons for sample_polygon.off... ");

	vdata=new GLfloat*[numVertices];
	for(int i=0;i<numVertices;i++)
		vdata[i]=new GLfloat[3];

	pdata=new GLuint*[numPolygons]; //array for storing polygon data (vertex indices)
	psize=new GLuint[numPolygons];  //array for storing polygon size

	resize = 0.0001;
	for (n = 0; n < numVertices; n++) { //read vertex data
		fscanf(fin,"%f %f %f\n",&x,&y,&z);
		vdata[n][0] = x;
		resize = max(resize,fabs(x));
		vdata[n][1] = y;
		resize = max(resize,fabs(y));
		vdata[n][2] = z;
		resize = max(resize,fabs(z));
	}

	for (n = 0; n < numVertices; n++) { //adjust vertex data
		vdata[n][0] = vdata[n][0]/resize;
		vdata[n][1] = vdata[n][1]/resize;
		vdata[n][2] = vdata[n][2]/resize;
	}

	for (n = 0; n < numPolygons; n++) {
		fscanf(fin,"%d",&a);
		psize[n] = a;  //store n-th polygon size in psize[n]
		pdata[n]=new GLuint[a];		
		for (j = 0; j < a; j++) { //read and save vertices of n-th polygon
			fscanf(fin,"%d",&b);
			pdata[n][j] = b;
		}
	}
	fclose(fin);
	printf("done.\n");

}

void calculateNormal(){
	normals=new GLfloat*[numPolygons];
	for(int i=0;i<numPolygons;i++)
		normals[i]=new GLfloat[3];

	for(int i=0;i<numPolygons;i++){

		GLint t1=pdata[i][0],t2=pdata[i][1],t3=pdata[i][2];
		GLfloat v1[3]={vdata[t1][0],vdata[t1][1],vdata[t1][2]};
		GLfloat v2[3]={vdata[t2][0],vdata[t2][1],vdata[t2][2]};
		GLfloat v3[3]={vdata[t3][0],vdata[t3][1],vdata[t3][2]};

		GLfloat n1[3] = {v2[0] - v1[0],v2[1] - v1[1],v2[2] - v1[2]};
		GLfloat n2[3] = {v3[0] - v1[0],v3[1] - v1[1],v3[2] - v1[2]};

		float	normx = (n1[1] * n2[2]) - (n2[1] * n1[2]),
			normy = (n1[2] * n2[0]) - (n2[2] * n1[0]),
			normz = (n1[0] * n2[1]) - (n2[0] * n1[1]);

		float factor = sqrt(pow(normx,2) + pow(normy,2) + pow(normz,2));
		normx /= factor;
		normy /= factor;
		normz /= factor;
		normals[i][0]=normx;
		normals[i][1]=normy;
		normals[i][2]=normz;
	}
}

	// Not necessary, but useful to view polygons
	void setRandomColor(){
		cdata=new GLfloat*[numPolygons];
		for(int i=0;i<numPolygons;i++)
			cdata[i]=new GLfloat[4];
	
		for(int i = 0; i < numPolygons; i++){
			cdata[i][0] = (rand()*1.0)/RAND_MAX;
			cdata[i][1] = (rand()*1.0)/RAND_MAX;
			cdata[i][2] = (rand()*1.0)/RAND_MAX;
			cdata[i][3] = 1.0;
		}
	}

	void reshape(int w, int h){
		glViewport(0, 0, (GLsizei) w, (GLsizei) h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(angleView, (GLfloat) w / (GLfloat) h, 1.5, 20.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void display(void){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		cpos[0] = 5.0 * cos(beta) * sin(alpha);
		cpos[1] = 5.0 * sin(beta);
		cpos[2] = 5.0 * cos(beta) * cos(alpha);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cpos[0],cpos[1],cpos[2], 0.0,0.0,0.0, 0.0,1.0,0.0);

		glLightfv(GL_LIGHT0, GL_POSITION, lpos);	
		glPushMatrix();
		glTranslatef(lpos[0],lpos[1],lpos[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
		glMaterialfv(GL_FRONT, GL_EMISSION, white);
		glutSolidSphere(0.05, 10, 8);
		glPopMatrix();

		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		if(flag==0){
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		}
		glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, blue);

		for(int i = 0; i < numPolygons; i++){
			if(flag==1){
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cdata[i]);
			}
			glBegin(GL_POLYGON);
			glNormal3f(
					normals[i][0],
					normals[i][1],
					normals[i][2]
				);
			for(int j = 0; j < psize[i]; j++){
				glVertex3f(
					vdata[pdata[i][j]][0],
					vdata[pdata[i][j]][1],
					vdata[pdata[i][j]][2]
				);
			}
			glEnd();
		}

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
		case 'c':   // change off file        
			printf("change off file...\n");
			readOFF();
			setRandomColor();
			calculateNormal();
			glutPostRedisplay();
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
		case 't':
			flag= 1-flag;
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}
	void specialkey(GLint key, int x, int y)
	{
		switch (key) {
		case GLUT_KEY_RIGHT:                
			alpha = alpha + PI/180;
			if (alpha > 2*PI) 
				alpha = alpha - 2*PI;
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:                 
			alpha = alpha - PI/180;
			if (alpha < 0) 
				alpha = alpha + 2*PI;
			glutPostRedisplay();
			break;
		case GLUT_KEY_UP:                
			if (beta < 0.45*PI) 
				beta = beta + PI/180;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:                 
			if (beta > -0.45*PI)
				beta = beta - PI/180;
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
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(1200, 800); 
		glutInitWindowPosition(0, 0);
		glutCreateWindow(argv[0]);

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

		glutDisplayFunc(display); 
		glutReshapeFunc(reshape); 
		glutKeyboardFunc(keyboard);
		glutSpecialFunc(specialkey);

		readOFF();
		calculateNormal();
		setRandomColor();
		glutMainLoop();
		return 0;
	}

