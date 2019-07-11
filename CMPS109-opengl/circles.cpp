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

struct {
   int width = 640;
   int height = 480;
} window;

template <typename item_t>
struct cycle_iterator {
   const vector<item_t> items;
   size_t curr_item {0};
   cycle_iterator (initializer_list<item_t> list):
               items(vector<item_t> (list)) {}
   const item_t& operator*() { return items[curr_item]; }
   const item_t& operator[] (size_t index) {
      return items[(curr_item + index) % items.size()];
   }
   cycle_iterator& operator++() { 
      curr_item = (curr_item + 1) % items.size();
      return *this;
   }
};

const GLubyte GREY[] = {211, 211, 211};
GLubyte RED[] =  {0xFF, 0x00, 0x00}; 
GLubyte GREEN[] = {0x00, 0xFF, 0x00};
GLubyte BLUE[] = {0x00, 0x00, 0xFF}; 
cycle_iterator<GLubyte*> colors {RED, GREEN, BLUE}; 

const GLfloat delta = 2 * M_PI / 64;
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

void draw_outer_circle(const GLubyte* color){
   glBegin (GL_POLYGON);
   glColor3ubv (color);
   GLfloat xoffset = window.width/2;
   GLfloat yoffset = window.height/2;
   GLfloat radius = window.height * (4.5 / 10.0);
   for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
      GLfloat xpos = radius * cos (theta) + xoffset;
      GLfloat ypos = radius * sin (theta) + yoffset;
      glVertex2f (xpos, ypos);
   }
   glEnd();
   glClear(GL_DEPTH_BUFFER_BIT);
}

void draw_mid_circle(const GLubyte* color){
   glBegin(GL_POLYGON);
   glColor3ubv (color);
   GLfloat xoffset = window.width/2;
   GLfloat yoffset = window.height/2;
   GLfloat radius = window.height * (4.5 / 10.0) * (2.0/3.0); 
   for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
      GLfloat xpos = radius * cos (theta) + xoffset;
      GLfloat ypos = radius * sin (theta) + yoffset;
      glVertex2f (xpos, ypos);
   }
  glEnd();
  glClear(GL_DEPTH_BUFFER_BIT); 
}

void draw_inner_circle(const GLubyte* color){
  glBegin(GL_POLYGON); 
  glColor3ubv(color);
  GLfloat xoffset = window.width/2;
  GLfloat yoffset = window.height/2;
  GLfloat radius = window.height * (4.5 / 10.0)/3.0;
  for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
    GLfloat xpos = radius * cos (theta) + xoffset;
    GLfloat ypos = radius * sin (theta) + yoffset;
    glVertex2f (xpos, ypos);
  }
   glEnd();
}

void display() {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
   draw_background(); 
   draw_outer_circle(colors[2]); 
   draw_mid_circle(colors[1]); 
   draw_inner_circle(colors[0]); 
   glutSwapBuffers();
}

void invert_colors() {
  for (size_t color = 0; color < 3; ++color) {
    for (size_t rgb = 0; rgb < 3; ++rgb) {
      colors[color][rgb] ^= 0xFF; 
    }
  }
}

void keyboard (GLubyte key, int, int) {
  constexpr char ESC {27}; 
  switch (key) {
    case 'c': case 'C': 
      ++colors; 
      break;
    case 's': case 'S':
      invert_colors(); 
      break;
    case 'q': case 'Q': case ESC:
      exit (EXIT_SUCCESS); 
  }
  glutPostRedisplay();
}

void mouse (int button, int state, int, int) {
  if (state == GLUT_DOWN) {
    switch (button) {
      case GLUT_LEFT_BUTTON:
        ++colors; 
        break;
      case GLUT_MIDDLE_BUTTON: 
        ++colors; 
        break; 
      case GLUT_RIGHT_BUTTON: 
        ++colors;
        break; 
    }
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
   glutInitWindowPosition (256, 265);
   glutCreateWindow (basename (argv[0]));
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutKeyboardFunc (keyboard); 
   glutMouseFunc (mouse); 
   glutMainLoop();
   return 0;
}

