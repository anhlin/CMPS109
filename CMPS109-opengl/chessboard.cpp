//Anthony Lin (anhlin)
//Mihir Malik (mimalik) 
#include <cmath>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

struct{
  int width = 640;
  int height = 480;
} window;

const GLubyte GREY[] {211, 211, 211};
GLubyte BLACK[] =  {0, 0, 0};
GLubyte B[] =  {0, 0, 255};
GLubyte C[] = {0, 255, 255};
GLubyte G[] =  {0, 255, 0};
GLubyte R[] =  {255, 0, 0};
GLubyte W[] = {255, 255, 255};
GLubyte* color1 = W;
GLubyte* color2 = BLACK;
constexpr GLfloat aspect_ratio = 2.0 / 3.0;

void draw_background() {
   glBegin (GL_POLYGON);
   glColor3ubv (GREY);
   glVertex2f (0, 0);
   glVertex2f (window.width, 0);
   glVertex2f (window.width, window.height);
   glVertex2f (0, window.height);
   glEnd();
}

void draw_board(){
  int offsetX,offsetY,edgesize;
  edgesize = 7*(min(window.height,window.width)/64);
  offsetX = (window.width/2)-(4*edgesize);
  offsetY = (window.height/2)-(4*edgesize);
  for(int i = 0; i < 8; ++i){
    for(int j = 0; j < 8; ++j){
      glBegin(GL_POLYGON);
      if(((i + j) % 2) != 0){
        glColor3ubv(color1);
      }else{
        glColor3ubv(color2);
      }
      glVertex2f(offsetX+((i*edgesize)),offsetY+(j*edgesize)-1);
      glVertex2f(offsetX+(i*edgesize),offsetY+((j+1)*edgesize)-1);
      glVertex2f(offsetX+(i+1)*edgesize,offsetY+((j+1)*edgesize)-1);
      glVertex2f(offsetX+(i+1)*edgesize,offsetY+(j*edgesize)-1);
      glEnd();
      glClear(GL_DEPTH_BUFFER_BIT);
    }
  }
}

void blackWhite(){
  color1 = W;
  color2 = BLACK;
}

void blueCyan(){
  color1 = C;
  color2 = B;
}

void greenRed(){
  color1 = G;
  color2 = R;
}

void keyboard (GLubyte key, int, int) {
  constexpr char ESC {27};
  switch (key) {
    case 'k': case 'K':
      blackWhite();
      break;
    case 'b': case 'B':
      blueCyan();
      break;
    case 'g': case 'G':
      greenRed();
      break;
    case 'q': case 'Q': case ESC:
      exit (EXIT_SUCCESS);
  }
  glutPostRedisplay();
}

void adjust_aspect (int width, int height) {
   if (window.width != width) {
      height = width * aspect_ratio;
   }else if (window.height != height) {
      width = height / aspect_ratio;
   }else { 
      return;
   }
   window.width = width;
   window.height = height; 
   glutReshapeWindow (window.width, window.height);

}
void display() {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
   draw_background();
   draw_board();
   glutSwapBuffers();
}

void reshape (int width, int height) {
   adjust_aspect (width, height);
   window.width = width;
   window.height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window.width, 0, window.height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window.width, window.height);
   glutPostRedisplay();
}

int main (int argc, char** argv) {
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window.width, window.height);
   glutInitWindowPosition (256, 256);
   glutCreateWindow (basename (argv[0]));
   glutDisplayFunc (display);
   glutKeyboardFunc (keyboard);
   glutReshapeFunc (reshape);
   glutMainLoop();
   return 0;
}
