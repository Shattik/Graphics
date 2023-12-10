#include <bits/stdc++.h>

#ifdef __linux__
 #include <GL/glut.h>
#elif WIN32
 #include <windows.h>
 #include <GL/glut.h>
#endif

#include "basic_drawing.h"
#include "physics_vector.h"
#include "camera.h"

#define rad(x) ((x) * 3.1416 / 180)
#define pi 3.1416

using namespace std;

int rotAngle = 0, rotInc = 10;

void drawOctahedron()
{
    bool color = true;
    for(int i=0; i<2; i++){
        glPushMatrix();
        glRotatef(180*i, 1, 0, 0);
        for(int j=0; j<4; j++){
            if(color){
                glColor3f(1, 0, 1);
            }
            else{
                glColor3f(0, 1, 1);
            }
            glPushMatrix();
            glRotatef(90*j, 0, 0, 1);
            drawTriangle(1, 0, 0, 0, 1, 0, 0, 0, 1);
            glPopMatrix();
            color = !color;
        }
        glPopMatrix();
    }
}


void display()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    look.lookAt();

    glPushMatrix();
    glRotatef(rotAngle, 0, 0, 1);
    drawOctahedron();
    glPopMatrix();

    glFlush();
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
        case 'a':
            rotAngle = (rotAngle - rotInc) % 360;
            break;
        case 'd':
            rotAngle = (rotAngle + rotInc) % 360;
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

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 500);
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
    glutCreateWindow("Magic Cube");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutTimerFunc(50, Timer, 0);

    glutMainLoop();
    return 0;
}