// Cubica

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0

GLUnurbsObj *theNurb;

GLfloat ctlpoints[21][21][3] = {{{0}}};

GLfloat knotsSurf[25] = {
	0.0,0.0,0.0,0.0,1.0,
	2.0,3.0,4.0,5.0,6.0,
	7.0,8.0,9.10,11.0,12.0,
	13.0,14.0,15.0,16.0,17.0,
	18.0,19.0,19.0,19.0,19.0,
};

typedef struct{
	GLfloat x;
	GLfloat y;
}Vector2D;

// Valores de entrada para la ola 1
float L1	= 8.0f;				//Distancia de la ola 1
float A1	= 0.4f;				//Altura de la ola 1
float S1	= 2.0f;				//Velocidad de la ola 1
Vector2D D1	= {0.0,-1.0};		//Vector que determina la dirección de la ola 1

//Variables para la ola 1
float w1;
float signo1;
float escalar1;
Vector2D normal1;

// Valores de entrada para la ola 2
float L2    = 4.0f;				//Distancia de la ola 2
float A2    = 0.3f;				//Altura de la ola 2
float S2	= 1.0f;				//Velocidad de la ola 2
Vector2D D2 = {1.0,1.0};		//Vector que determina la dirección de la ola 2

//Variables para la ola 2
float w2;
float signo2;
float escalar2;
Vector2D normal2;

float t			 = 0.0f;
float pi		 = 3.141592f;
bool  pausar	 = true;
bool activarOla1 = true;
bool activarOla2 = true;

void ejesCoordenada() {
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++) {
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);
					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else {
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);
					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);
				}
			}
		}
	glEnd();
	glLineWidth(1.0);
}

void changeViewport(int w, int h) {
	float aspectratio;
	if (h==0){
		h=1;	
	}
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 200.0);
	glMatrixMode (GL_MODELVIEW);
}

void init_surface() {
	int x = 10;
	int y = 0;
	int z = 10;

	for (int f = 0; f < 21; f++) {
		for (int c = 0; c < 21; c++) {
			ctlpoints[f][c][0] = x - c;
			ctlpoints[f][c][1] = y;
			ctlpoints[f][c][2] = z; 
		}
		z -= 1;
	}		
}

void init(){
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);

   init_surface();

   theNurb = gluNewNurbsRenderer();
   gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 15.0);
   gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
}

void Keyboard(unsigned char key, int x, int y){
  switch (key){
	case '1':
		activarOla1 = true;
		activarOla2 = false;
		break;
	case '2':
		activarOla1 = false;
		activarOla2 = true;
		break;
	case 97:	//97 en ASCII es: a.
		if (activarOla1) {
			L1 -= 0.1;
		} else if (activarOla2) {
			L2 -= 0.1;
		} else {
			break;
		}
	case 122:	//122 en ASCII es: z.
		if (activarOla1) {
			L1 += 0.1;
		} else if (activarOla2) {
			L2 += 0.1;
		} else {
			break;
		}
	case 115:	//115 en ASCII es: s.
		if (activarOla1) {
			A1 -= 0.1;
		} else if (activarOla2) {
			A2 -= 0.1;
		} else {
			break;
		}
	case 120:	//120 en ASCII es: x.
		if (activarOla1) {
			A1 += 0.1;
		} else if (activarOla2) {
			A2 += 0.1;
		} else {
			break;
		}
	case 100:	//100 en ASCII es: d.
		if (activarOla1) {
			S1 -= 0.1;
		} else if (activarOla2) {
			S2 -= 0.1;
		} else {
			break;
		}
	case 99:	//99 en ASCII es: c.
		if (activarOla1) {
			S1 += 0.1;
		} else if (activarOla2) {
			S2 += 0.1;
		} else {
			break;
		}
	case 102:	//102 en ASCII es: f.
		if (activarOla1) {
			D1.x -= 0.1;
		} else if (activarOla2) {
			D2.x -= 0.1;
		} else {
			break;
		}
	case 118:	//118 en ASCII es: v.
		if (activarOla1) {
			D1.x += 0.1;
		} else if (activarOla2) {
			D2.x += 0.1;
		} else {
			break;
		}
	case 103:	//103 en ASCII es: g.
		if (activarOla1) {
			D1.y -= 0.1;
		} else if (activarOla2) {
			D2.y -= 0.1;
		} else {
			break;
		}
	case 98:	//98 en ASCII es: b.
		if (activarOla1) {
			D1.y += 0.1;
		} else if (activarOla2) {
			D2.y += 0.1;
		} else {
			break;
		}
	case 114: //114 en ASCII es: r.
		pausar = false;  
		break;
	case 112: //112 en ASCII es: p.
		pausar = true;
		break;
	case 27:  //27 en ASCII es: ESC.        
		exit (0);
		break;	
  }
  glutPostRedisplay();
}

void animacion(int value) {
	if (!pausar){
		t += 0.001;
		w1 = (2 * pi) / L1;	//Onda 1
		w2 = (2 * pi) / L2;	//Onda 2
		signo1 = S1 * w1;	//Valor de signo extraño 1
		signo2 = S2 * w2;	//Valor de signo extraño 2
		normal1.x = (1 / (sqrt((D1.x)*(D1.x) + (D1.y)*(D1.y)))) * D1.x;	// Normalizar componente x del vector D1
		normal1.y = (1 / (sqrt((D1.x)*(D1.x) + (D1.y)*(D1.y)))) * D1.y; // Normalizar componente y del vector D1
		normal2.x = (1 / (sqrt((D2.x)*(D2.x) + (D2.y)*(D2.y)))) * D2.x; // Normalizar componente x del vector D2
		normal2.y = (1 / (sqrt((D2.x)*(D2.x) + (D2.y)*(D2.y)))) * D2.y; // Normalizar componente y del vector D2
		for (int f = 0; f < 10; f++) {
			for (int c = 0; c < 21; c++) {
				escalar1	= (normal1.x * ctlpoints[f][c][0]) + (normal1.y * ctlpoints[f][c][2]); 
				ctlpoints[f][c][1] = A1 * sin(escalar1 + t * signo1);
			}
		}		
		for (int f = 11; f < 21; f++) {
			for (int c = 0; c < 21; c++) {
				escalar2	= (normal2.x * ctlpoints[f][c][0]) + (normal2.y * ctlpoints[f][c][2]); 
				ctlpoints[f][c][1] = A2 * sin(escalar2 + t * signo2);
			}
		}		
		glutTimerFunc(200,animacion,1);	
	}
	glutPostRedisplay();	
}

void render(){
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
	int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (25.0, 12.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Luz y material	
	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.9, 1.0 };
	GLfloat mat_specular[] = { 0.8, 0.8, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 60.0 };


	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	

	GLfloat light_ambient[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light_position[] = { -10.0, 5.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   

	// Render Grid 
	/*	glDisable(GL_LIGHTING);
		glLineWidth(1.0);
		glPushMatrix();
		glRotatef(90,1.0,0.0,0.0);
		glColor3f( 0.0, 0.7, 0.7 );
		glBegin( GL_LINES );
		zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
		for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ )
		{
		xLocal = DEF_floorGridScale * loopX;
		glVertex3f( xLocal, -zExtent, 0.0 );
		glVertex3f( xLocal, zExtent,  0.0 );
		}
		xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
		for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ )
		{
		zLocal = DEF_floorGridScale * loopZ;
		glVertex3f( -xExtent, zLocal, 0.0 );
		glVertex3f(  xExtent, zLocal, 0.0 );
	}
	glEnd();
	ejesCoordenada();
	glPopMatrix();
	glEnable(GL_LIGHTING);*/
	// Fin Grid

	//Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );	

	glPushMatrix();
		gluBeginSurface(theNurb);
	   
		gluNurbsSurface(theNurb, 
						25,knotsSurf,25,knotsSurf,
						21 * 3, 3, &ctlpoints[0][0][0], 
						4, 4, GL_MAP2_VERTEX_3);

		gluEndSurface(theNurb);
	glPopMatrix();

	/* Muestra los puntos de control */
	
	/*	int i,j;
		glPointSize(5.0);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		for (i = 0; i < 21; i++) {
			for (j = 0; j < 21; j++) {
	            glVertex3f(ctlpoints[i][j][0], 	ctlpoints[i][j][1], ctlpoints[i][j][2]);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
	*/
		
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	//Pausamos la animacion de las olas.
	if (!pausar) {
		glutTimerFunc(200,animacion,1);
	}

	glutSwapBuffers();
}

int main (int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(960,540);
	glutCreateWindow("Nurbs Proyecto - Ola");

	init ();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(Keyboard);
	
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	
	glutMainLoop();
	return 0;
}