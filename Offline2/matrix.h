#include <iostream>
#include <cmath>

#ifndef MATRIX_H
#define MATRIX_H

class Pointh;
class Matrixh;
class Vector;

Matrixh IDENTITY;
Vector X_AXIS(1, 0, 0), Y_AXIS(0, 1, 0), Z_AXIS(0, 0, 1);

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

        Vector(Pointh p1, Pointh p2)
        {
            this->x = p2.x - p1.x;
            this->y = p2.y - p1.y;
            this->z = p2.z - p1.z;
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

class Pointh
{
    public:
        double x, y, z, w;

        Pointh()
        {
            x = 0;
            y = 0;
            z = 0;
            w = 1;
        }

        Pointh(double x, double y, double z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            w = 1;
        }

        void normalize()
        {
            x = x / w;
            y = y / w;
            z = z / w;
            w = 1;
        }

        friend Pointh operator*(Matrixh m, Pointh p)
        {
            Pointh res;
            res.x = m.ar[0][0] * p.x + m.ar[0][1] * p.y + m.ar[0][2] * p.z + m.ar[0][3] * p.w;
            res.y = m.ar[1][0] * p.x + m.ar[1][1] * p.y + m.ar[1][2] * p.z + m.ar[1][3] * p.w;
            res.z = m.ar[2][0] * p.x + m.ar[2][1] * p.y + m.ar[2][2] * p.z + m.ar[2][3] * p.w;
            res.w = m.ar[3][0] * p.x + m.ar[3][1] * p.y + m.ar[3][2] * p.z + m.ar[3][3] * p.w;
            res.normalize();
            return res;
        }

};

class Matrixh
{
    public:
        double ar[4][4];

        Matrixh()
        {
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    if(i == j){
                        ar[i][j] = 1;
                    }
                    else{
                        ar[i][j] = 0;
                    }
                }
            }
        }

        Matrixh(double ar[4][4])
        {
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    this->ar[i][j] = ar[i][j];
                }
            }
        }

        Matrixh(Matrixh &m)
        {
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    ar[i][j] = m.ar[i][j];
                }
            }
        }

        void print()
        {
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    std::cout << ar[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }

        Matrixh transpose()
        {
            Matrixh res;
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    res.ar[i][j] = ar[j][i];
                }
            }
            return res;
        }

        Matrixh& operator=(Matrixh &m)
        {
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    ar[i][j] = m.ar[i][j];
                }
            }
            return *this;
        }

        Matrixh operator*(Matrixh &m)
        {
            Matrixh res;
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    res.ar[i][j] = 0;
                    for(int k = 0; k < 4; k++){
                        res.ar[i][j] += ar[i][k] * m.ar[k][j];
                    }
                }
            }
            return res;
        }    

};

#endif