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

double wallDim = 1.5, wallHeight = 0.25, wallSide, wallNum = 4, wallAngle;
vector<pair<double, pair<double, double>>> walls;


class Ball
{
    public:
        int sector;
        int stack;
        double radius;
        double posx, posy, posz;
        vector<Vector> vertices;
        Vector velocity, actualUp;
        bool showUp;
        int showUpCounter;

        Ball()
        {
            sector = 8;
            stack = 20;
            radius = .125;
            posx = 0;
            posy = 0;
            posz = radius;
            velocity = Vector(.05, .05, 0);
            actualUp = Vector(0, 0, 1);
            showUp = true;
            showUpCounter = 0;
        }

        void initializeVertices()
        {
            for(int i=0; i<=stack; i++){
                double phi = (pi/2) - pi * ((i*1.0) / stack);
                double xy = radius * cos(phi);
                double z = radius * sin(phi);

                for(int j=0; j<=sector; j++){
                    double theta = 2 * pi * ((j*1.0) / sector);
                    double x = xy * cos(theta);
                    double y = xy * sin(theta);

                    Vector point = Vector(x, y, z);
                    vertices.push_back(point);
                }
            }
        }

        void draw()
        {
            for(int i=0; i<stack; i++){
                int k1 = i * (sector + 1);
                int k2 = k1 + sector + 1;
                double phi = (pi/2) - pi * ((i*1.0) / stack);
                bool col = (phi > 0) ? true : false;

                for(int j=0; j<sector; j++, k1++, k2++){
                    col = !col;

                    if(i != 0){
                        if(col){
                            glColor3f(1, 0, 0);
                        }
                        else{
                            glColor3f(0, 1, 0);
                        }
                        drawTriangle(vertices[k1].x, vertices[k1].y, vertices[k1].z, vertices[k2].x, vertices[k2].y, vertices[k2].z, vertices[k1+1].x, vertices[k1+1].y, vertices[k1+1].z);
                    }
                    
                    if(i != stack-1){
                        if(col){
                            glColor3f(1, 0, 0);
                        }
                        else{
                            glColor3f(0, 1, 0);
                        }
                        drawTriangle(vertices[k1+1].x, vertices[k1+1].y, vertices[k1+1].z, vertices[k2].x, vertices[k2].y, vertices[k2].z, vertices[k2+1].x, vertices[k2+1].y, vertices[k2+1].z);
                    }
                }
            }
        }

        void drawArrowBody()
        {
            for(int i=0; i<4; i++){
                glPushMatrix();
                glRotatef(90*i, 1, 0, 0);
                glTranslatef(0, 0, 0.01);
                drawQuad(0, -0.01, 0, 0, 0.01, 0, 2*radius, 0.01, 0, 2*radius, -0.01, 0);
                glPopMatrix();
            }
        }

        void drawArrowHead()
        {
            for(int i=0; i<4; i++){
                glPushMatrix();
                glRotatef(90*i, 1, 0, 0);
                drawTriangle(0, 0.015, 0.015, 0, -0.015, 0.015, .05, 0, 0);
                glPopMatrix();
            }
        }

        void drawDirection()
        {
            double val = velocity.value();
            double angle = acos(velocity.x / val) * 180 / pi;
            if(velocity.y < 0){
                angle = 360 - angle;
            }
            glPushMatrix();
            glRotatef(angle, 0, 0, 1);
            drawArrowBody();
            glPushMatrix();
            glTranslatef(2*radius, 0, 0);
            drawArrowHead();
            glPopMatrix();
            glPopMatrix();
        }

        void drawRealUp()
        {
            glPushMatrix();
            glRotatef(-90, 0, 1, 0);
            drawArrowBody();
            glPushMatrix();
            glTranslatef(2*radius, 0, 0);
            drawArrowHead();
            glPopMatrix();
            glPopMatrix();
        }

        void drawBall()
        {
            glPushMatrix();
            glTranslatef(posx, posy, posz);
            draw();
            glColor3f(0, 0, 1);
            drawDirection();
            if(showUp){
                glColor3f(0, 1, 1);
                drawRealUp();    
            }
            showUpCounter++;
            if(showUpCounter == 10){
                showUp = !showUp;
                showUpCounter = 0;
            }
            glPopMatrix();
        }

        void vertexRotation(Vector axis, double angle)
        {
            for(int i=0; i<vertices.size(); i++){
                Vector v = vertices[i];
                vertices[i] = v*cos(angle) + axis.cross(v)*sin(angle) + axis*(axis.dot(v))*(1-cos(angle));
            }
        }

        void moveForward()
        {
            Vector axis = actualUp.cross(velocity);
            axis.normalize();
            double angle = velocity.value() / radius; // radian
            vertexRotation(axis, angle);
            posx += velocity.x;
            posy += velocity.y;
            posz += velocity.z;
        }

        void moveBackward()
        {
            Vector axis = actualUp.cross(velocity);
            axis.normalize();
            double angle = velocity.value() / radius; // radian
            vertexRotation(axis, -angle);
            posx -= velocity.x;
            posy -= velocity.y;
            posz -= velocity.z;
        }

        void rotateDirectionAnticlock()
        {
            actualUp.normalize();
            Vector left = actualUp.cross(velocity);
            velocity = velocity * cos(0.05) + left * sin(0.05);
        }

        void rotateDirectionClock()
        {
            actualUp.normalize();
            Vector left = actualUp.cross(velocity);
            velocity = velocity * cos(0.05) - left * sin(0.05);
        }

}ball;

void drawCheckerBoard()
{
    glColor3f(0, 0, 0);
    for(int i=-100; i<=100; i++){
        for(int j=-200; j<=200; j++){
            if(j%2 == 0){
                glPushMatrix();
                glTranslatef(i+.5, j*.5, 0);
                drawQuad(0, 0, 0, .5, 0, 0, .5, .5, 0, 0, .5, 0);
                glPopMatrix();
            }
            else{
                glPushMatrix();
                glTranslatef(i, j*.5, 0);
                drawQuad(0, 0, 0, .5, 0, 0, .5, .5, 0, 0, .5, 0);
                glPopMatrix();
            }
        }
    }
    glColor3f(1, 1, 1);
    for(int i=-100; i<=100; i++){
        for(int j=-200; j<=200; j++){
            if(j%2 == 0){
                glPushMatrix();
                glTranslatef(i, j*.5, 0);
                drawQuad(.5, .5, 0, 0, .5, 0, 0, 0, 0, .5, 0, 0);
                glPopMatrix();
            }
            else{
                glPushMatrix();
                glTranslatef(i+.5, j*.5, 0);
                drawQuad(.5, .5, 0, 0, .5, 0, 0, 0, 0, .5, 0, 0);
                glPopMatrix();
            }
        }
    }
}

void drawWalls()
{
    glColor3f(1, 0, 0);
    for(int i=0; i<wallNum; i++){
        glPushMatrix();
        glRotatef(wallAngle*i, 0, 0, 1);
        glTranslatef(wallDim, 0, 0);
        drawQuad(0, -wallSide, 0, 0, wallSide, 0, 0, wallSide, wallHeight, 0, -wallSide, wallHeight);
        glPopMatrix();
    }
}

void display()
{
    // cout << "Displaying " << counter++ << endl;
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    look.lookAt();
    
    drawCheckerBoard();
    drawWalls();
    ball.drawBall();

    glFlush();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 500);

    wallAngle = 360 / wallNum;
    wallSide = wallDim * tan(rad(wallAngle/2));
    for(int i=0; i<wallNum; i++){
        double angle = wallAngle * i;
        double m;
        if((angle+90) == 90 || (angle+90) == 270){
            m = INT_MAX;
        }
        else{
            m = tan(rad(angle + 90));
        }
        double x = wallDim * cos(rad(angle));
        double y = wallDim * sin(rad(angle));
        walls.push_back({m, {x, y}});
    }

    ball.initializeVertices();
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
        case 'i':
            ball.moveForward();
            break;
        case 'k':
            ball.moveBackward();
            break;
        case 'j':
            ball.rotateDirectionAnticlock();
            break;
        case 'l':
            ball.rotateDirectionClock();
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
    glutCreateWindow("Rolling Ball");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutTimerFunc(50, Timer, 0);

    glutMainLoop();
    return 0;
}