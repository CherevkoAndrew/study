
/* Copyright (c) Mark J. Kilgard, 1997. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

/* This program was requested by Patrick Earl; hopefully someone else
   will write the equivalent Direct3D immediate mode program. */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>


GLfloat R = 5.;
double alpha = 0.;
double beta = 0.;

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

void
drawBox(void)
{
  int i;

  for (i = 0; i < 6; i++) {
    glBegin(GL_LINE_LOOP);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }

	glPushMatrix();
        glTranslated(0.0,0,0.);
        glutWireSphere(1,16,16);
    glPopMatrix(); 
	//glBegin(GL_LINE_LOOP);
		for(float x = -1.; x < 1.; x+=0.1)
			for(float y = -1.; y < 1.; y+=0.1 ){
				glBegin(GL_LINE_STRIP);
				glVertex3f(x, y, sin(x) + cos(y));
				glVertex3f(x+0.1, y, sin(x+0.1)+cos(y) );
				glVertex3f(x+0.1, y+0.1, sin(x+0.1)+cos(y+0.1));
				glVertex3f(x, y+0.1, sin(x)+cos(y+0.1));
				glEnd();

		}
	//glEnd();
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  glutSwapBuffers();
}

void
init(GLint w, GLint h)
{
  /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(5., 5.0, 0.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */
  //glTranslatef(0., 0., -1.0);
  //glScalef(.5,.5,.5);
 // glRotatef(60, 1.0, 0.0, 0.0);
 // glRotatef(-20, 0.0, 0.0, 1.0);
}

void Keyboard( unsigned char key, int x, int y){
  
  #define up 119
  #define down 115
  #define left 97
  #define right 100
  if(key == left)
 	alpha += 0.01;
  if(key == right)
	alpha -= 0.01; 
  if(key == up)
    beta += 0.01;
  if(key == down)
	beta -= 0.01;
 
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(R*sin(alpha*M_PI)* cos(beta*M_PI),R*sin(beta*M_PI),R*cos(alpha*M_PI)* cos(beta*M_PI),0.,0.,0.,0.,cos(beta*M_PI)/abs(cos(beta*M_PI)),0.);
  
  printf("%f\n", beta);
  glutPostRedisplay();
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("red 3D lighted cube");
  glutDisplayFunc(display);
  glutReshapeFunc(init);
  //init(0,0);
  glutKeyboardFunc(Keyboard);
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
