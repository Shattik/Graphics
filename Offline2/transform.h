#include <iostream>
#include <iomanip>
#include <stack>
#include <cmath>
#include "matrix.h"

#ifndef TRANSFORM_H
#define TRANSFORM_H

#define PI 3.14159265358979323846
#define RAD(x) ((x) * PI / 180)

class Model;
class Triangle;
class View;
class Projection;

class Model
{
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

class Triangle
{
    public:
        Pointh points[3];

    Triangle(Pointh p1, Pointh p2, Pointh p3)
    {
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
    }

    Triangle()
    {
        points[0] = Pointh();
        points[1] = Pointh();
        points[2] = Pointh();
    }

    void print(std::ostream &out)
    {
        for(int i = 0; i < 3; i++)
        {
            out << std::fixed << std::setprecision(7) << points[i].x << " " << points[i].y << " " << points[i].z << " ";
            out << std::endl;
        }
    }

    void transform(Matrixh m)
    {
        for(int i = 0; i < 3; i++)
        {
            points[i] = m * points[i];
        }
    }

};

class View
{
    public:
        Vector l, r, u;
        Matrixh m;

        View()
        {
            l = Vector(0, 0, -1);
            r = Vector(1, 0, 0);
            u = Vector(0, 1, 0);
            m = buildMatrix(0, 0, 0);
        }

        View(double eyex, double eyey, double eyez, double lookx, double looky, double lookz, double upx, double upy, double upz)
        {
            l = Vector(lookx - eyex, looky - eyey, lookz - eyez);
            l.normalize();
            r = l.cross(Vector(upx, upy, upz));
            r.normalize();
            u = r.cross(l);
            m = buildMatrix(eyex, eyey, eyez);
        }

        View(Pointh eye, Pointh look, Vector up)
        {
            l = Vector(eye, look);
            l.normalize();
            r = l.cross(up);
            r.normalize();
            u = r.cross(l);
            m = buildMatrix(eye.x, eye.y, eye.z);
        }

        Matrixh buildMatrix(double eyex, double eyey, double eyez)
        {
            Matrixh T;
            T.ar[0][3] = -eyex;
            T.ar[1][3] = -eyey;
            T.ar[2][3] = -eyez;

            Matrixh R;
            R.ar[0][0] = r.x;
            R.ar[0][1] = r.y;
            R.ar[0][2] = r.z;
            R.ar[1][0] = u.x;
            R.ar[1][1] = u.y;
            R.ar[1][2] = u.z;
            R.ar[2][0] = -l.x;
            R.ar[2][1] = -l.y;
            R.ar[2][2] = -l.z;

            return R * T;
        }
        
};

class Projection
{
    public:
        double t, r, near, far;
        Matrixh m;

        Projection()
        {
            t = 1;
            r = 1;
            near = 0;
            far = 2;
            m = buildMatrix();
        }

        Projection(double fovy, double aspect, double near, double far)
        {
            fovy = RAD(fovy);
            double fovx = fovy * aspect;
            t = near * tan(fovy / 2);
            r = near * tan(fovx / 2);
            this->near = near;
            this->far = far;
            m = buildMatrix();
        }

        Matrixh buildMatrix()
        {
            Matrixh m;
            m.ar[0][0] = near / r;
            m.ar[1][1] = near / t;
            m.ar[2][2] = -(far + near) / (far - near);
            m.ar[2][3] = -(2 * far * near) / (far - near);
            m.ar[3][2] = -1;
            m.ar[3][3] = 0;
            return m;
        }

};

#endif