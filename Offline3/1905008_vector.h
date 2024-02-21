#include <iostream>
#include <cmath>

#ifndef VECTOR_H
#define VECTOR_H
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
            if(len == 0){
                return;
            }
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

        double distance(Vector v)
        {
            return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
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

class Matrix
{
    public:
        double m[3][3];

        Matrix()
        {
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    m[i][j] = 0;
                }
            }
        }

        Matrix(double m[3][3])
        {
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    this->m[i][j] = m[i][j];
                }
            }
        }

        double det()
        {
            return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
        }

};
#endif