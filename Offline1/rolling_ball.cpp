#include <bits/stdc++.h>

#ifdef __linux__
 #include <GL/glut.h>
#elif WIN32
 #include <windows.h>
 #include <GL/glut.h>
#endif

#define rad(x) ((x) * 3.1416 / 180)

using namespace std;

double wallDim = 1.5, wallHeight = 0.25, wallSide, wallNum = 4, wallAngle;
vector<pair<double, pair<double, double>>> walls;

class Vector
{
    public:
        double x, y, z;

        Vector()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        Vector(double x, double y, double z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Vector(double x1, double y1, double z1, double x2, double y2, double z2)
        {
            this->x = x2 - x1;
            this->y = y2 - y1;
            this->z = z2 - z1;
        }

        double value()
        {
            return sqrt(x * x + y * y + z * z);
        }

        Vector cross(Vector v)
        {
            Vector res;
            res.x = y * v.z - z * v.y;
            res.y = z * v.x - x * v.z;
            res.z = x * v.y - y * v.x;
            return res;
        }

        double dot(Vector v)
        {
            return x * v.x + y * v.y + z * v.z;
        }

        void normalize()
        {
            double len = value();
            x /= len;
            y /= len;
            z /= len;
        }

        Vector operator+(Vector v)
        {
            Vector res;
            res.x = x + v.x;
            res.y = y + v.y;
            res.z = z + v.z;
            return res;
        }

        Vector operator-(Vector v)
        {
            Vector res;
            res.x = x - v.x;
            res.y = y - v.y;
            res.z = z - v.z;
            return res;
        }

        friend Vector operator*(double a, Vector v)
        {
            Vector res;
            res.x = a * v.x;
            res.y = a * v.y;
            res.z = a * v.z;
            return res;
        }

        friend Vector operator*(Vector v, double a)
        {
            return a * v;
        }

};

class Look
{
    public:
        double eyeX, eyeY, eyeZ;
        double centerX, centerY, centerZ;
        double upX, upY, upZ;
        double del, theta;

        Look()
        {
            eyeX = 0;
            eyeY = 0;
            eyeZ = 3;
            centerX = 0;
            centerY = 0;
            centerZ = 0;
            upX = 0;
            upY = 1;
            upZ = 0;
            del = 0.1;
            theta = 0.05; // radians
        }

        void lookAt()
        {
            gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
        }

        void moveForward()
        {
            Vector v(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            double len = v.value();
            eyeX += (v.x/len) * del;
            eyeY += (v.y/len) * del;
            eyeZ += (v.z/len) * del;
        }

        void moveBackward()
        {
            Vector v(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            double len = v.value();
            eyeX -= (v.x/len) * del;
            eyeY -= (v.y/len) * del;
            eyeZ -= (v.z/len) * del;
        }

        void moveRight()
        {
            Vector v(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            Vector u(upX, upY, upZ);
            Vector w = v.cross(u);
            double len = w.value();
            eyeX += (w.x/len) * del;
            eyeY += (w.y/len) * del;
            eyeZ += (w.z/len) * del;
            centerX += (w.x/len) * del;
            centerY += (w.y/len) * del;
            centerZ += (w.z/len) * del;
        }

        void moveLeft()
        {
            Vector v(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            Vector u(upX, upY, upZ);
            Vector w = v.cross(u);
            double len = w.value();
            eyeX -= (w.x/len) * del;
            eyeY -= (w.y/len) * del;
            eyeZ -= (w.z/len) * del;
            centerX -= (w.x/len) * del;
            centerY -= (w.y/len) * del;
            centerZ -= (w.z/len) * del;
        }

        void moveUp()
        {
            Vector u(upX, upY, upZ);
            double len = u.value();
            eyeX += (u.x/len) * del;
            eyeY += (u.y/len) * del;
            eyeZ += (u.z/len) * del;
            centerX += (u.x/len) * del;
            centerY += (u.y/len) * del;
            centerZ += (u.z/len) * del;
        }

        void moveDown()
        {
            Vector u(upX, upY, upZ);
            double len = u.value();
            eyeX -= (u.x/len) * del;
            eyeY -= (u.y/len) * del;
            eyeZ -= (u.z/len) * del;
            centerX -= (u.x/len) * del;
            centerY -= (u.y/len) * del;
            centerZ -= (u.z/len) * del;
        }

        void rotateLeft()
        {
            Vector l(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            Vector u(upX, upY, upZ);
            u.normalize();
            Vector r = u.cross(l);
            Vector v = l * cos(theta) + r * sin(theta);
            centerX = eyeX + v.x;
            centerY = eyeY + v.y;
            centerZ = eyeZ + v.z;
        }

        void rotateRight()
        {
            Vector l(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            Vector u(upX, upY, upZ);
            u.normalize();
            Vector r = u.cross(l);
            Vector v = l * cos(theta) - r * sin(theta);
            centerX = eyeX + v.x;
            centerY = eyeY + v.y;
            centerZ = eyeZ + v.z;
        }

        void lookUp()
        {
            Vector l(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            Vector u(upX, upY, upZ);
            u.normalize();
            double val = l.value();
            Vector lPerp = u * val;
            Vector uPerp = l * (1/val);
            Vector newL = l * cos(theta) + lPerp * sin(theta);
            Vector newU = u * cos(theta) - uPerp * sin(theta);
            centerX = eyeX + newL.x;
            centerY = eyeY + newL.y;
            centerZ = eyeZ + newL.z;
            upX = newU.x;
            upY = newU.y;
            upZ = newU.z;
        }

        void lookDown()
        {
            Vector l(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            Vector u(upX, upY, upZ);
            u.normalize();
            double val = l.value();
            Vector lPerp = u * val;
            Vector uPerp = l * (1/val);
            Vector newL = l * cos(theta) - lPerp * sin(theta);
            Vector newU = u * cos(theta) + uPerp * sin(theta);
            centerX = eyeX + newL.x;
            centerY = eyeY + newL.y;
            centerZ = eyeZ + newL.z;
            upX = newU.x;
            upY = newU.y;
            upZ = newU.z;
        }

        void tiltCounterClock()
        {
            Vector l(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            Vector u(upX, upY, upZ);
            l.normalize();
            Vector r = l.cross(u);
            Vector v = u * cos(theta) + r * sin(theta);
            upX = v.x;
            upY = v.y;
            upZ = v.z;
        }

        void tiltClock()
        {
            Vector l(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            Vector u(upX, upY, upZ);
            l.normalize();
            Vector r = l.cross(u);
            Vector v = u * cos(theta) - r * sin(theta);
            upX = v.x;
            upY = v.y;
            upZ = v.z;
        }

        void moveUpWithoutChange()
        {
            Vector l(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            eyeZ += del;
            Vector newL(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            Vector u(upX, upY, upZ);
            l.normalize();
            newL.normalize();
            double cosine = l.dot(newL);
            double sine = sqrt(1 - cosine * cosine);
            Vector uPerp = l * u.value();
            Vector newU = u * cosine + uPerp * sine;
            upX = newU.x;
            upY = newU.y;
            upZ = newU.z;
        }

        void moveDownWithoutChange()
        {
            Vector l(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            eyeZ -= del;
            Vector newL(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            Vector u(upX, upY, upZ);
            l.normalize();
            newL.normalize();
            double cosine = l.dot(newL);
            double sine = sqrt(1 - cosine * cosine);
            Vector uPerp = l * u.value();
            Vector newU = u * cosine - uPerp * sine;
            upX = newU.x;
            upY = newU.y;
            upZ = newU.z;
        }

}look;

void drawQuad(double x1, double y1, double z1, double x2, double y2, double z2, double x3,
              double y3, double z3, double x4, double y4, double z4)
{
    glBegin(GL_QUADS);
    {
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
        glVertex3f(x4, y4, z4);
    }glEnd();
}

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

    glFlush();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

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
    glutCreateWindow("Rolling Ball");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutTimerFunc(50, Timer, 0);

    glutMainLoop();
    return 0;
}