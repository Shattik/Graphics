#include <iostream>
#include <stack>
#include <cmath>
#include "matrix.h"

#ifndef TRANSFORM_H
#define TRANSFORM_H

#define PI 3.1416
#define RAD(x) ((x) * PI / 180)

class Model;

class Model {
    public:
        std::stack<Matrixh> transformations;
        Matrixh current;

        Model()
        {
            current = IDENTITY;
        }

        void pushModel()
        {
            transformations.push(current);
        }

        void popModel()
        {
            current = transformations.top();
            transformations.pop();
        }

        void translate(double x, double y, double z)
        {
            Matrixh m;
            m.ar[0][3] = x;
            m.ar[1][3] = y;
            m.ar[2][3] = z;
            current = current * m;
        }

        void scale(double x, double y, double z)
        {
            Matrixh m;
            m.ar[0][0] = x;
            m.ar[1][1] = y;
            m.ar[2][2] = z;
            current = current * m;
        }

        void rotate(double angle, double x, double y, double z)
        {
            Vector a(x, y, z);
            a.normalize();
            double r = RAD(angle);
            double c = cos(r);
            double s = sin(r);

            Vector rx = c * X_AXIS + (1 - c) * (a.dot(X_AXIS)) * a + s * a.cross(X_AXIS);
            Vector ry = c * Y_AXIS + (1 - c) * (a.dot(Y_AXIS)) * a + s * a.cross(Y_AXIS);
            Vector rz = c * Z_AXIS + (1 - c) * (a.dot(Z_AXIS)) * a + s * a.cross(Z_AXIS);

            Matrixh m;
            m.ar[0][0] = rx.x;
            m.ar[1][0] = rx.y;
            m.ar[2][0] = rx.z;
            m.ar[0][1] = ry.x;
            m.ar[1][1] = ry.y;
            m.ar[2][1] = ry.z;
            m.ar[0][2] = rz.x;
            m.ar[1][2] = rz.y;
            m.ar[2][2] = rz.z;
            current = current * m;
        }

        void printCurrent(std::ostream &out)
        {
            current.print(out);
        }

};

#endif