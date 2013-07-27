#include "def.h"
#include "inc.h"
#include "pp.h"
#include <mmsystem.h>

static const int TIMER_MILLISECONDS = 1000 / TICKS_PER_SECOND;
static BOOL bDown = FALSE;
GLuint textureId;
GLubyte *textureImage;

GLuint bTex;
GLuint rTTex;
GLuint rBTex;
GLuint backTex;

Game game;

//Звук
ALuint buffer;
ALuint source;
ALuint source2;
ALuint source3;
float ROT = 0;
int main(int argc, char** argv) 
{
  	glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  	glutInitWindowPosition(100,100);
  	glutInitWindowSize(GAME_WIDTH,GAME_HEIGHT);
  	glutCreateWindow("pp_demo");
  	glutDisplayFunc(display);
  	glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse_event);
    glutMotionFunc(mouse_move);
  	glutTimerFunc(TIMER_MILLISECONDS, timer, 0);
    glutSetCursor(GLUT_CURSOR_INFO);
    //glutFullScreen();
    //звук
    alutInit(&argc, argv);
    buffer = alutCreateBufferFromFile("wav\\ball_rocket_left.wav");
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
   
    buffer = alutCreateBufferFromFile("wav\\ball_rocket_right.wav");
    alGenSources(1, &source2);
    alSourcei(source2, AL_BUFFER, buffer);

    buffer = alutCreateBufferFromFile("wav\\ball_wall.wav");
    alGenSources(1, &source3);
    alSourcei(source3, AL_BUFFER, buffer);
     //
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    game.init(GAME_WIDTH, GAME_HEIGHT);
    game.start();

   	glutMainLoop();
   	return 0;
};


void display() 
{
  //glClear(GL_COLOR_BUFFER_BIT);
  //glColor3f(1.0, 1.0, 1.0);
 // glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
  glClear( GL_COLOR_BUFFER_BIT );
  glClearColor(0.5, 0.5, 0.5, 0.f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0, GAME_WIDTH, GAME_HEIGHT, 0, 0, 1);
  glMatrixMode (GL_MODELVIEW);
  glEnable(GL_TEXTURE_2D);

  game.update();
  //printf("%s\n", glGetError());
  glutSwapBuffers();
};

void timer(int value) 
{
    glutTimerFunc(TIMER_MILLISECONDS, timer, 0);
    game.tick();
    glutPostRedisplay();
};

void mouse_move(int x, int y) 
{
  ROT+=1;
  if (bDown)
  {
      game.move_rocket(x,y);
      glutPostRedisplay();
  }
};

void mouse_event(int button, int state, int x, int y) 
{

  if (button== GLUT_LEFT_BUTTON) {
    bDown = (state == GLUT_DOWN) ? TRUE : FALSE;
    if (bDown) {
      game.use_rocket(x,y);
      glutSetCursor(GLUT_CURSOR_NONE);
    } else {
      glutSetCursor(GLUT_CURSOR_INFO);
    }
  }
};

void keyboard (unsigned char key, int x, int y)
{

  switch (key) {
    case 27:
      exit(0);
      break;
  
    default:
      break;
   }
};