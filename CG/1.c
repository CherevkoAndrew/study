#include <stdlib.h>
#include <stdio.h>

/* підключаємо бібліотеку GLUT */

#include <GL/glut.h>

const int CubeSize = 200;

int size = 100;
int xStart = size;
int yStart = size;

float sqr = 1/1.414;

GLint Width = size*9, Height = size*10;

float triangles[][2] = {{3,2},{3,3},{4,3},
                  {3,2},{4,3},{5,2},
                  {5,2},{3,4},{5,4},
                  {5,4},{3,4},{3,6},
                  {3,4.-sqr},{3-sqr,4.},{3,4.+sqr}
                };
float squares[][2] = {{2,6},{2,7},{3,7},{3,6},
                      {3,7},{3+sqr,7+sqr},{3+3*sqr,7+sqr},{3+2*sqr,7}
                     };

int tlen = 5*3;
int slen = 2*4;
float transp = 0.05;

/* ця функція керує всім виведенням на екран */

void Display(void){
  int left = -1, right = 100, top = -1, bottom = 100;
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3ub(255,255,0);

  for(int i = 0; i<tlen; i+=3){
    glBegin(GL_POLYGON);
      glVertex2fv(triangles[i]);
      glVertex2fv(triangles[i+1]);
      glVertex2fv(triangles[i+2]);
    glEnd();
  }

  for(int i = 0; i<slen; i+=4){
    glBegin(GL_POLYGON);
      glVertex2fv(squares[i]);
      glVertex2fv(squares[i+1]);
      glVertex2fv(squares[i+2]);
      glVertex2fv(squares[i+3]);
    glEnd();
  }

  glColor3ub(255,0,0);

  for(int i = 0; i<tlen; i+=3){
    glBegin(GL_LINE_LOOP);
      glVertex2fv(triangles[i]);
      glVertex2fv(triangles[i+1]);
      glVertex2fv(triangles[i+2]);
    glEnd();
  }

  for(int i = 0; i<slen; i+=4){
    glBegin(GL_LINE_LOOP);
      glVertex2fv(squares[i]);
      glVertex2fv(squares[i+1]);
      glVertex2fv(squares[i+2]);
      glVertex2fv(squares[i+3]);
    glEnd();
  }

  glFinish();
}

/* Функція викликається при зміні розмірів вікна */

void Reshape(GLint w, GLint h){
  Width = w;
  Height = h;
  /* установлюємо розміри області відображення */
  glViewport(0, 0, w, h);

  /* ортографічна проекція */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w/100, 0, h/100, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/* Функція обробляє повідомлення від клавіатури */

void Keyboard( unsigned char key, int x, int y ){

  #define ESCAPE '\033'
  #define PLUS 43
  #define MINUS 45

  printf("ododod");
  if( key == ESCAPE )
    exit(0);

  if(key == PLUS){
    for (int i = 0; i < tlen; i++) {
      triangles[i][0] += transp;
    }

    for (int i = 0; i < slen; i++) {
      squares[i][0] += transp;
    }
  }

  if(key == MINUS){
    for (int i = 0; i < tlen; i++) {
      triangles[i][0] -= transp;
    }

    for (int i = 0; i < slen; i++) {
      squares[i][0] -= transp;
    }
  }
  glutPostRedisplay();
}

/* Головний цикл додатка */

main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(Width, Height);
  glutCreateWindow("Red square example");
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  glutMainLoop();
}
