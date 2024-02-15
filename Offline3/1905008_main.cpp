#include <bits/stdc++.h>

#ifdef __linux__
 #include <GL/glut.h>
#elif WIN32
 #include <windows.h>
 #include <GL/glut.h>
#endif

#include "1905008_vector.h"
#include "1905008_camera.h"
#include "1905008_classes.h"

#define rad(x) ((x) * 3.1416 / 180)
#define pi 3.1416

using namespace std;

void display()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    look.lookAt();

    glFlush();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 500);
}

void keyboardListener(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '1':
            look.rotateLeft();
            break;
        case '2':
            look.rotateRight();
            break;
        case '3':
            look.lookUp();
            break;
        case '4':
            look.lookDown();
            break;
        case '5':
            look.tiltCounterClock();
            break;
        case '6':
            look.tiltClock();
            break;
        case 'w':
            look.moveUpWithoutChange();
            break;
        case 's':
            look.moveDownWithoutChange();
            break;
        default:
            break;
    }
   
}

void specialKeyListener(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            look.moveForward();
            break;
        case GLUT_KEY_DOWN:
            look.moveBackward();
            break;
        case GLUT_KEY_RIGHT:
            look.moveRight();
            break;
        case GLUT_KEY_LEFT:
            look.moveLeft();
            break;
        case GLUT_KEY_PAGE_UP:
            look.moveUp();
            break;
        case GLUT_KEY_PAGE_DOWN:
            look.moveDown();
            break;
        default:
            break;
    }
}

void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(50, Timer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Ray Tracing");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutTimerFunc(50, Timer, 0);

    glutMainLoop();
    return 0;
}