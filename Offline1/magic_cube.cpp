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

class Sphere{
public:

    double factor;
    double curRad;
    double incRad;
    double maxRad;

    Sphere(){
        factor = 1;
        maxRad = 1/sqrt(3);
        curRad = 0;
        incRad = maxRad/10;
    }

    void drawSpherePart()
    {
        int parts = 50;
        for(int i=0; i<parts; i++){
            double x1 = 1 - 2.0*i/parts;
            double x2 = 1 - 2.0*(i+1)/parts;
            for(int j=0; j<parts; j++){
                double y1 = 1 - 2.0*j/parts;
                double y2 = 1 - 2.0*(j+1)/parts;
                Vector v1(x1, y1, 1);
                Vector v2(x2, y1, 1);
                Vector v3(x2, y2, 1);
                Vector v4(x1, y2, 1);
                v1.normalize();
                v2.normalize();
                v3.normalize();
                v4.normalize();
                v1 = v1 * curRad;
                v2 = v2 * curRad;
                v3 = v3 * curRad;
                v4 = v4 * curRad;
                drawQuad(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z, v4.x, v4.y, v4.z);
            }
        }
    }

    void colorSphere(int i)
    {
        if(i%3 == 0){
            glColor3f(1, 0, 0);
        }
        else if(i%3 == 1){
            glColor3f(0, 1, 0);
        }
        else{
            glColor3f(0, 0, 1);
        }
    }

    void drawSphere()
    {
        int num = 0;
        for(int i=0; i<4; i++){
            glPushMatrix();
            glRotatef(90*i, 1, 0, 0);
            glTranslatef(0, 0, factor);
            colorSphere(num);
            num = (num+1)%2;
            drawSpherePart();
            glPopMatrix();
        }
        for(int i=-1; i<2; i+=2){
            glPushMatrix();
            glRotatef(90*(-i), 0, 1, 0);
            glTranslatef(0, 0, factor);
            colorSphere(2);
            drawSpherePart();
            glPopMatrix();
        }
    }
}sphere;

class Cylinder{
public:

    double factor;
    int sectorCount;
    double cylAngle;
    double maxRad;
    double curRad;
    double incRad;

    Cylinder(){
        factor = 1;
        sectorCount = 20;
        cylAngle = 70.5287794;
        maxRad = 1/sqrt(3);
        curRad = 0;
        incRad = maxRad/10;
    }

    void drawCylinder()
    {
        double height = sqrt(2)*factor;

        for(int i=0; i<sectorCount; i++){
            double angle1 = cylAngle/2 - i*cylAngle/sectorCount;
            double x1 = curRad*cos(rad(angle1));
            double y1 = curRad*sin(rad(angle1));
            double z1 = height/2;

            double angle2 = cylAngle/2 - (i+1)*cylAngle/sectorCount;
            double x2 = curRad*cos(rad(angle2));
            double y2 = curRad*sin(rad(angle2));
            double z2 = height/2;

            drawQuad(x1, y1, -z1, x2, y2, -z2, x2, y2, z2, x1, y1, z1);
        }
    }

    void drawCylinders()
    {
        glColor3f(1, 1, 0);
        double r = factor/sqrt(2);
        for(int i=0; i<4; i++){
            glPushMatrix();
            glRotatef(90*i, 0, 0, 1);
            glRotatef(45, 0, 1, 0);
            glTranslatef(r, 0, 0);
            drawCylinder();
            glPopMatrix();
        }
        for(int i=0; i<4; i++){
            glPushMatrix();
            glRotatef(90*i, 0, 0, 1);
            glRotatef(-45, 0, 1, 0);
            glTranslatef(r, 0, 0);
            drawCylinder();
            glPopMatrix();
        }
        for(int i=0; i<4; i++){
            glPushMatrix();
            glRotatef(90*i, 0, 0, 1);
            glRotatef(90, 1, 0, 0);
            glRotatef(45, 0, 1, 0);
            glTranslatef(r, 0, 0);
            drawCylinder();
            glPopMatrix();
        }
    }
}cylinder;

class Octahedron{
public:

    double factor;

    Octahedron(){
        factor = 1;
    }

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
                double shift = (1-factor)/3;
                glTranslatef(shift, shift, shift);
                glScalef(factor, factor, factor);
                drawTriangle(1, 0, 0, 0, 1, 0, 0, 0, 1);
                glPopMatrix();
                color = !color;
            }
            glPopMatrix();
        }
    }
}octahedron;

void display()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    look.lookAt();

    glPushMatrix();
    glRotatef(rotAngle, 0, 0, 1);
    octahedron.drawOctahedron();
    cylinder.drawCylinders();
    sphere.drawSphere();
    glPopMatrix();

    glFlush();
}

void factorIncrease()
{
    if(octahedron.factor < 1){
        octahedron.factor += 0.1;
        cylinder.factor += 0.1;
        sphere.factor += 0.1;
        cylinder.curRad -= cylinder.incRad;
        sphere.curRad -= sphere.incRad;
    }
}

void factorDecrease()
{
    if(octahedron.factor >= 0.1){
        octahedron.factor -= 0.1;
        cylinder.factor -= 0.1;
        sphere.factor -= 0.1;
        cylinder.curRad += cylinder.incRad;
        sphere.curRad += sphere.incRad;
    }
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
        case '.':
            factorIncrease();
            break;
        case ',':
            factorDecrease();
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