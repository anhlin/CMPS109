#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

struct {
   int width {384};
   int height {256};
} window;

const GLubyte R[] {255, 0, 0};
const GLubyte O[] {255, 165, 0};
const GLubyte Y[] {255, 255, 0};
const GLubyte G[] {0, 255, 0};
const GLubyte B[] {0, 0, 255}; 
const GLubyte I[] {75, 0, 130};
const GLubyte V[] {238, 130, 238}; 

const vector<const GLubyte*> colors {R,O,Y,G,B,I,V};
constexpr GLfloat aspect_ratio = 2.0 / 3.0;

void draw_rainbow() {
   for (size_t i = 0; i < 7; ++i) {
      glBegin (GL_POLYGON);
      glColor3ubv (colors[i]);
      glVertex2f (window.width * i / 7.0, 0);
      glVertex2f (window.width * (i + 1) / 7.0, 0);
      glVertex2f (window.width * (i + 1) / 7.0, window.height);
      glVertex2f (window.width * i / 7.0, window.height);
      glEnd();
   }
}

void display() {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
   draw_rainbow();
   glutSwapBuffers();
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
   glutReshapeFunc (reshape);
   glutMainLoop();
   return 0;
}

