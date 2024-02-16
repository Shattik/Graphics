#include "1905008_vector.h"

#ifdef __linux__
 #include <GL/glut.h>
#elif WIN32
 #include <windows.h>
 #include <GL/glut.h>
#endif

#ifndef CAMERA_H
#define CAMERA_H
class Look
{
    public:
        double eyeX, eyeY, eyeZ;
        double centerX, centerY, centerZ;
        double upX, upY, upZ;
        double del, theta;

        Look()
        {
            eyeX = -60;
            eyeY = -60;
            eyeZ = 60;
            centerX = 0;
            centerY = 0;
            centerZ = 0;
            upX = 0.5;
            upY = 0.5;
            upZ = 1;
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
            centerX += (v.x/len) * del;
            centerY += (v.y/len) * del;
            centerZ += (v.z/len) * del;
        }

        void moveBackward()
        {
            Vector v(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
            double len = v.value();
            eyeX -= (v.x/len) * del;
            eyeY -= (v.y/len) * del;
            eyeZ -= (v.z/len) * del;
            centerX -= (v.x/len) * del;
            centerY -= (v.y/len) * del;
            centerZ -= (v.z/len) * del;
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
            Vector projPerp = Vector(-l.y, l.x, 0);
            projPerp.normalize();
            double cosine = l.dot(newL);
            double sine = sqrt(1 - cosine * cosine);
            Vector uPerp = projPerp.cross(u);
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
            Vector projPerp = Vector(-l.y, l.x, 0);
            projPerp.normalize();
            double cosine = l.dot(newL);
            double sine = sqrt(1 - cosine * cosine);
            Vector uPerp = projPerp.cross(u);
            Vector newU = u * cosine - uPerp * sine;
            upX = newU.x;
            upY = newU.y;
            upZ = newU.z;
        }

}look;
#endif
