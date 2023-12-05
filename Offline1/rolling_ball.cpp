#include <bits/stdc++.h>

#ifdef __linux__
 #include <GL/glut.h>
#elif WIN32
 #include <windows.h>
 #include <GL/glut.h>
#endif

using namespace std;

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

};

class Look
{
    public:
        double eyeX, eyeY, eyeZ;
        double centerX, centerY, centerZ;
        double upX, upY, upZ;
        double del;

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

}look;

void display()
{
    // cout << "Displaying " << counter++ << endl;
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    look.lookAt();
    
    glBegin(GL_QUADS);
    {
        glColor3f(1, 0, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
    }glEnd();

    glFlush();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);
}

void keyboardListener(unsigned char key, int x, int y)
{
   
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