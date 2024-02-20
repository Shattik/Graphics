#include "1905008_vector.h"
#include "1905008_drawing.h"
#include <vector>

#ifdef __linux__
 #include <GL/glut.h>
#elif WIN32
 #include <windows.h>
 #include <GL/glut.h>
#endif

#ifndef CLASSES_H
#define CLASSES_H

class Object;
class Sphere;
class Triangle;
class General;
class Floor;
class PointLight;
class SpotLight;
class Ray;

extern std::vector<Object*> objects;
extern std::vector<PointLight*> pointLights;
extern std::vector<SpotLight*> spotLights;

class Object {
    public:
        Vector reference_point;
        double height, width, length;
        double color[3];
        double coEfficients[4];
        int shine;

        Object()
        {
            reference_point = Vector(0, 0, 0);
            height = 0;
            width = 0;
            length = 0;
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
            coEfficients[0] = 0;
            coEfficients[1] = 0;
            coEfficients[2] = 0;
            coEfficients[3] = 0;
            shine = 0;
        }

        virtual void draw() = 0;

        void setColor(double r, double g, double b)
        {
            color[0] = r;
            color[1] = g;
            color[2] = b;
        }

        void setShine(int s)
        {
            shine = s;
        }

        void setCoEfficients(double a, double b, double c, double d)
        {
            coEfficients[0] = a;
            coEfficients[1] = b;
            coEfficients[2] = c;
            coEfficients[3] = d;
        }

        virtual double intersect(Ray *r, double *color, int level)
        {
            return -1.0;
        }
};

class Sphere : public Object
{
    public:
        Sphere(Vector c, double r)
        {
            reference_point = c;
            length = r;
        }

        void draw()
        {
            glPushMatrix();
            glTranslatef(reference_point.x, reference_point.y, reference_point.z);
            glColor3f(color[0], color[1], color[2]);
            int stack = 100;
            int sector = 100;
            double pi = acos(-1);
            std::vector<Vector> vertices;
            for(int i=0; i<=stack; i++){
                double phi = (pi/2) - pi * ((i*1.0) / stack);
                double xy = length * cos(phi);
                double z = length * sin(phi);

                for(int j=0; j<=sector; j++){
                    double theta = 2 * pi * ((j*1.0) / sector);
                    double x = xy * cos(theta);
                    double y = xy * sin(theta);

                    Vector point = Vector(x, y, z);
                    vertices.push_back(point);
                }
            }

            for(int i=0; i<stack; i++){
                int k1 = i * (sector + 1);
                int k2 = k1 + sector + 1;

                for(int j=0; j<sector; j++, k1++, k2++){
                    
                    if(i != 0){
                        drawTriangle(vertices[k1].x, vertices[k1].y, vertices[k1].z, vertices[k2].x, vertices[k2].y, vertices[k2].z, vertices[k1+1].x, vertices[k1+1].y, vertices[k1+1].z);
                    }
                    
                    if(i != stack-1){
                        drawTriangle(vertices[k1+1].x, vertices[k1+1].y, vertices[k1+1].z, vertices[k2].x, vertices[k2].y, vertices[k2].z, vertices[k2+1].x, vertices[k2+1].y, vertices[k2+1].z);
                    }
                }
            }
            glPopMatrix();
        }
};

class Triangle : public Object
{
    public:
        Vector a, b, c;

        Triangle(Vector a, Vector b, Vector c)
        {
            this->a = a;
            this->b = b;
            this->c = c;
        }

        void draw()
        {
            glColor3f(color[0], color[1], color[2]);
            drawTriangle(a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z);
        }
};

class General : public Object
{
    public:
        double a, b, c, d, e, f, g, h, i, j;

        General(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j)
        {
            this->a = a;
            this->b = b;
            this->c = c;
            this->d = d;
            this->e = e;
            this->f = f;
            this->g = g;
            this->h = h;
            this->i = i;
            this->j = j;
        }

        void setDimensions(double a, double b, double c, double l, double w, double h)
        {
            reference_point = Vector(a, b, c);
            length = l;
            width = w;
            height = h;
        }

        void draw()
        {}
};

class Floor : public Object
{
    public:
        double tileWidth;
        double altColor[3];

        Floor(double tileWidth, double floorWidth)
        {
            this->tileWidth = tileWidth;
            reference_point = Vector(-floorWidth/2, -floorWidth/2, 0);
            length = floorWidth;
            width = floorWidth;
        }

        void setAltColor(double r, double g, double b)
        {
            altColor[0] = r;
            altColor[1] = g;
            altColor[2] = b;
        }

        void draw()
        {
            int n = length / tileWidth;
            for(int i=0; i<n; i++){
                for(int j=0; j<n; j++){
                    if((i+j)%2 == 0){
                        glColor3f(color[0], color[1], color[2]);
                    }
                    else{
                        glColor3f(altColor[0], altColor[1], altColor[2]);
                    }
                    glPushMatrix();
                    glTranslatef(reference_point.x + i*tileWidth, reference_point.y + j*tileWidth, reference_point.z);
                    drawQuad(0, 0, 0, tileWidth, 0, 0, tileWidth, tileWidth, 0, 0, tileWidth, 0);
                    glPopMatrix();
                }
            }
        }
};

class PointLight
{
    public:
        Vector position;
        double color[3];

        PointLight()
        {
            position = Vector(0, 0, 0);
            color[0] = 1;
            color[1] = 1;
            color[2] = 1;
        }

        PointLight(Vector p, double r, double g, double b)
        {
            position = p;
            color[0] = r;
            color[1] = g;
            color[2] = b;
        }

        void draw()
        {
            glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glColor3f(color[0], color[1], color[2]);
            glutSolidSphere(0.5, 100, 100);
            glPopMatrix();
        }
};

class SpotLight
{
    public:
        PointLight point_light;
        Vector direction;
        double cut_off;

        SpotLight(PointLight p, Vector d, double c)
        {
            point_light = p;
            direction = d;
            cut_off = c;
        }

        void draw()
        {
            point_light.draw();
        }
};

class Ray 
{
    public:
        Vector start, dir;

        Ray(Vector s, Vector d)
        {
            start = s;
            dir = d;
            dir.normalize();
        }
};
#endif