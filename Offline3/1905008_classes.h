#include "1905008_vector.h"
#include "1905008_drawing.h"
#include <cfloat>
#include <vector>

#ifdef __linux__
 #include <GL/glut.h>
#elif WIN32
 #include <windows.h>
 #include <GL/glut.h>
#endif

#ifndef CLASSES_H
#define CLASSES_H

#define epsilon 0.00001;

class Ray;
class Object;
class Sphere;
class Triangle;
class General;
class Floor;
class PointLight;
class SpotLight;

extern std::vector<Object*> objects;
extern std::vector<PointLight*> pointLights;
extern std::vector<SpotLight*> spotLights;
extern int recursionLevel;

double getDegree(double radian)
{
    return radian * 180.0 / acos(-1);
}

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
            direction.normalize();
            cut_off = c;
        }

        void draw()
        {
            point_light.draw();
        }
};

class Object {
    public:
        Vector reference_point;
        double height, width, length;
        double color[3];
        double coEfficients[4];
        int shine;
        int id;

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

        virtual Vector getNormal(Vector point)
        {
            return Vector(0, 0, 0);
        }

        double *getColor(Vector point)
        {
            return color;
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

        int isInside(Vector point)
        {
            double dist = point.dot(point);
            if(dist < length*length){
                return 1;
            }
            else if(dist == length*length){
                return 0;
            }
            else{
                return -1;
            }
        }

        Vector getNormal(Vector point)
        {
            Vector normal = point - reference_point;
            normal.normalize();
            return normal;
        }

        double intersect(Ray *r, double *color, int level)
        {
            double tmin;
            Vector origin(r->start - reference_point);
            int res = isInside(origin);
            if(res == 0){
                origin = origin + r->dir * epsilon;
                res = isInside(origin);
            }
            double tp = (-1)*(origin.dot(r->dir));
            if(res == -1 && tp < 0){
                return -1;
            }
            double dsquare = origin.dot(origin) - tp*tp;
            if(dsquare > length*length){
                return -1;
            }
            double tprime = sqrt(length*length - dsquare);
            if(res == -1){
                tmin = tp - tprime;
            }
            else{
                tmin = tp + tprime;
            }

            if(level == 0){
                return tmin;
            }
            Vector intersection(r->start + r->dir*tmin);
            Vector normal = getNormal(intersection);
            if(normal.dot(r->dir) > 0){
                normal = normal * (-1);
            }
            double *surf_color = getColor(intersection);
            color[0] = surf_color[0] * coEfficients[0];
            color[1] = surf_color[1] * coEfficients[0];
            color[2] = surf_color[2] * coEfficients[0];

            for(int i=0; i<pointLights.size(); i++){
                Ray *shadow = new Ray(pointLights[i]->position, intersection - pointLights[i]->position);
                int nearest = -1;
                double tshadow = DBL_MAX;
                for(int j=0; j<objects.size(); j++){
                    double t = objects[j]->intersect(shadow, color, 0);
                    if(t >= 0 && t < tshadow){
                        tshadow = t;
                        nearest = j;
                    }
                }
                if(nearest == id){
                    double lambert = (-1)*normal.dot(shadow->dir);
                    if(lambert > 0){
                        color[0] += lambert * surf_color[0] * coEfficients[1] * pointLights[i]->color[0];
                        color[1] += lambert * surf_color[1] * coEfficients[1] * pointLights[i]->color[1];
                        color[2] += lambert * surf_color[2] * coEfficients[1] * pointLights[i]->color[2];
                    }
                    Vector reflection = shadow->dir - normal * (2 * shadow->dir.dot(normal));
                    reflection.normalize();
                    double phong = (-1)*reflection.dot(r->dir);
                    if(phong > 0){
                        phong = pow(phong, shine);
                        color[0] += phong * coEfficients[2] * pointLights[i]->color[0] * surf_color[0];
                        color[1] += phong * coEfficients[2] * pointLights[i]->color[1] * surf_color[1];
                        color[2] += phong * coEfficients[2] * pointLights[i]->color[2] * surf_color[2];
                    }
                }
                delete shadow;
            }

            for(int i=0; i<spotLights.size(); i++){
                Ray *shadow = new Ray(spotLights[i]->point_light.position, intersection - spotLights[i]->point_light.position);
                int nearest = -1;
                double tshadow = DBL_MAX;
                double dot = shadow->dir.dot(spotLights[i]->direction);
                if(getDegree(acos(dot)) > spotLights[i]->cut_off){
                    continue;
                }
                for(int j=0; j<objects.size(); j++){
                    double t = objects[j]->intersect(shadow, color, 0);
                    if(t >= 0 && t < tshadow){
                        tshadow = t;
                        nearest = j;
                    }
                }
                if(nearest == id){
                    double lambert = (-1)*normal.dot(shadow->dir);
                    if(lambert > 0){
                        color[0] += lambert * surf_color[0] * coEfficients[1] * spotLights[i]->point_light.color[0] * pow(dot, 2);
                        color[1] += lambert * surf_color[1] * coEfficients[1] * spotLights[i]->point_light.color[1] * pow(dot, 2);
                        color[2] += lambert * surf_color[2] * coEfficients[1] * spotLights[i]->point_light.color[2] * pow(dot, 2);
                    }
                    Vector reflection = shadow->dir - normal * (2 * shadow->dir.dot(normal));
                    reflection.normalize();
                    double phong = (-1)*reflection.dot(r->dir);
                    if(phong > 0){
                        phong = pow(phong, shine);
                        color[0] += phong * coEfficients[2] * spotLights[i]->point_light.color[0] * pow(dot, 2) * surf_color[0];
                        color[1] += phong * coEfficients[2] * spotLights[i]->point_light.color[1] * pow(dot, 2) * surf_color[1];
                        color[2] += phong * coEfficients[2] * spotLights[i]->point_light.color[2] * pow(dot, 2) * surf_color[2];
                    }
                }
                delete shadow;
            }
            
            if(level >= recursionLevel){
                return tmin;
            }

            Vector reflection = r->dir - normal * (2 * r->dir.dot(normal));
            reflection.normalize();
            Vector start = intersection + reflection * epsilon;
            Ray *reflected = new Ray(start, reflection);
            double *reflected_color = new double[3];
            reflected_color[0] = 0;
            reflected_color[1] = 0;
            reflected_color[2] = 0;
            double t = DBL_MAX;
            int nearest = -1;
            for(int i=0; i<objects.size(); i++){
                double t1 = objects[i]->intersect(reflected, reflected_color, 0);
                if(t1 >=0 && t1 < t){
                    t = t1;
                    nearest = i;
                }
            }
            if(nearest != -1){
                t = objects[nearest]->intersect(reflected, reflected_color, level+1);
                color[0] += reflected_color[0] * coEfficients[3];
                color[1] += reflected_color[1] * coEfficients[3];
                color[2] += reflected_color[2] * coEfficients[3];
            }
            delete reflected;
            delete[] reflected_color;
            return tmin;
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

        Vector getNormal(Vector point)
        {
            Vector normal = (b - a).cross(c - a);
            normal.normalize();
            return normal;
        }

        double *getColor(Vector point)
        {
            return color;
        }

        double intersect(Ray *r, double *color, int level)
        {
            double tmin;
            double a[3][3] = {{this->a.x - this->b.x, this->a.x - this->c.x, r->dir.x},
                              {this->a.y - this->b.y, this->a.y - this->c.y, r->dir.y},
                              {this->a.z - this->b.z, this->a.z - this->c.z, r->dir.z}};
            double b[3][3] = {{this->a.x - r->start.x, this->a.x - this->c.x, r->dir.x},
                              {this->a.y - r->start.y, this->a.y - this->c.y, r->dir.y},
                              {this->a.z - r->start.z, this->a.z - this->c.z, r->dir.z}};
            double c[3][3] = {{this->a.x - this->b.x, this->a.x - r->start.x, r->dir.x},
                              {this->a.y - this->b.y, this->a.y - r->start.y, r->dir.y},
                              {this->a.z - this->b.z, this->a.z - r->start.z, r->dir.z}};
            double d[3][3] = {{this->a.x - this->b.x, this->a.x - this->c.x, this->a.x - r->start.x},
                              {this->a.y - this->b.y, this->a.y - this->c.y, this->a.y - r->start.y},
                              {this->a.z - this->b.z, this->a.z - this->c.z, this->a.z - r->start.z}};
            Matrix A(a), B(b), C(c), D(d);
            double del = A.det();
            if(del == 0){
                return -1;
            }
            double beta = B.det() / del;
            double gamma = C.det() / del;
            double tao = D.det() / del;
            if(beta < 0 || gamma < 0 || beta + gamma > 1 || tao < 0){
                return -1;
            }
            Vector intersection(r->start + r->dir*tao);
            tmin = r->start.distance(intersection);
            if(level == 0){
                return tmin;
            }
            Vector normal = getNormal(intersection);
            if(normal.dot(r->dir) > 0){
                normal = normal * (-1);
            }
            double *surf_color = getColor(intersection);
            color[0] = surf_color[0] * coEfficients[0];
            color[1] = surf_color[1] * coEfficients[0];
            color[2] = surf_color[2] * coEfficients[0];

            for(int i=0; i<pointLights.size(); i++){
                Ray *shadow = new Ray(pointLights[i]->position, intersection - pointLights[i]->position);
                int nearest = -1;
                double tshadow = DBL_MAX;
                for(int j=0; j<objects.size(); j++){
                    double t = objects[j]->intersect(shadow, color, 0);
                    if(t >= 0 && t < tshadow){
                        tshadow = t;
                        nearest = j;
                    }
                }
                if(nearest == id){
                    double lambert = (-1)*normal.dot(shadow->dir);
                    if(lambert > 0){
                        color[0] += lambert * surf_color[0] * coEfficients[1] * pointLights[i]->color[0];
                        color[1] += lambert * surf_color[1] * coEfficients[1] * pointLights[i]->color[1];
                        color[2] += lambert * surf_color[2] * coEfficients[1] * pointLights[i]->color[2];
                    }
                    Vector reflection = shadow->dir - normal * (2 * shadow->dir.dot(normal));
                    reflection.normalize();
                    double phong = (-1)*reflection.dot(r->dir);
                    if(phong > 0){
                        phong = pow(phong, shine);
                        color[0] += phong * coEfficients[2] * pointLights[i]->color[0] * surf_color[0];
                        color[1] += phong * coEfficients[2] * pointLights[i]->color[1] * surf_color[1];
                        color[2] += phong * coEfficients[2] * pointLights[i]->color[2] * surf_color[2];
                    }
                }
                delete shadow;
            }

            for(int i=0; i<spotLights.size(); i++){
                Ray *shadow = new Ray(spotLights[i]->point_light.position, intersection - spotLights[i]->point_light.position);
                int nearest = -1;
                double tshadow = DBL_MAX;
                double dot = shadow->dir.dot(spotLights[i]->direction);
                if(getDegree(acos(dot)) > spotLights[i]->cut_off){
                    continue;
                }
                for(int j=0; j<objects.size(); j++){
                    double t = objects[j]->intersect(shadow, color, 0);
                    if(t >= 0 && t < tshadow){
                        tshadow = t;
                        nearest = j;
                    }
                }
                if(nearest == id){
                    double lambert = (-1)*normal.dot(shadow->dir);
                    if(lambert > 0){
                        color[0] += lambert * surf_color[0] * coEfficients[1] * spotLights[i]->point_light.color[0] * pow(dot, 2);
                        color[1] += lambert * surf_color[1] * coEfficients[1] * spotLights[i]->point_light.color[1] * pow(dot, 2);
                        color[2] += lambert * surf_color[2] * coEfficients[1] * spotLights[i]->point_light.color[2] * pow(dot, 2);
                    }
                    Vector reflection = shadow->dir - normal * (2 * shadow->dir.dot(normal));
                    reflection.normalize();
                    double phong = (-1)*reflection.dot(r->dir);
                    if(phong > 0){
                        phong = pow(phong, shine);
                        color[0] += phong * coEfficients[2] * spotLights[i]->point_light.color[0] * pow(dot, 2) * surf_color[0];
                        color[1] += phong * coEfficients[2] * spotLights[i]->point_light.color[1] * pow(dot, 2) * surf_color[1];
                        color[2] += phong * coEfficients[2] * spotLights[i]->point_light.color[2] * pow(dot, 2) * surf_color[2];
                    }
                }
                delete shadow;
            }
            
            if(level >= recursionLevel){
                return tmin;
            }

            Vector reflection = r->dir - normal * (2 * r->dir.dot(normal));
            reflection.normalize();
            Vector start = intersection + reflection * epsilon;
            Ray *reflected = new Ray(start, reflection);
            double *reflected_color = new double[3];
            reflected_color[0] = 0;
            reflected_color[1] = 0;
            reflected_color[2] = 0;
            double t = DBL_MAX;
            int nearest = -1;
            for(int i=0; i<objects.size(); i++){
                double t1 = objects[i]->intersect(reflected, reflected_color, 0);
                if(t1 >= 0 && t1 < t){
                    t = t1;
                    nearest = i;
                }
            }
            if(nearest != -1){
                t = objects[nearest]->intersect(reflected, reflected_color, level+1);
                color[0] += reflected_color[0] * coEfficients[3];
                color[1] += reflected_color[1] * coEfficients[3];
                color[2] += reflected_color[2] * coEfficients[3];
            }
            delete reflected;
            delete[] reflected_color;
            return tmin;
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

        Vector getNormal(Vector point)
        {
            double x = 2*a*point.x + d*point.y + e*point.z + g;
            double y = 2*b*point.y + d*point.x + f*point.z + h;
            double z = 2*c*point.z + e*point.x + f*point.y + i;
            Vector normal(x, y, z);
            normal.normalize();
            return normal;
        }

        double *getColor(Vector point)
        {
            return color;
        }

        double intersect(Ray *r, double *color, int level)
        {
            double tmin;
            Vector intersection;
            double A = a*r->dir.x*r->dir.x + b*r->dir.y*r->dir.y + c*r->dir.z*r->dir.z + d*r->dir.x*r->dir.y + e*r->dir.x*r->dir.z + f*r->dir.y*r->dir.z;
            double B = 2*a*r->start.x*r->dir.x + 2*b*r->start.y*r->dir.y + 2*c*r->start.z*r->dir.z + d*(r->start.x*r->dir.y + r->start.y*r->dir.x) + e*(r->start.x*r->dir.z + r->start.z*r->dir.x) + f*(r->start.y*r->dir.z + r->start.z*r->dir.y) + g*r->dir.x + h*r->dir.y + i*r->dir.z;
            double C = a*r->start.x*r->start.x + b*r->start.y*r->start.y + c*r->start.z*r->start.z + d*r->start.x*r->start.y + e*r->start.x*r->start.z + f*r->start.y*r->start.z + g*r->start.x + h*r->start.y + i*r->start.z + j;
            double D = B*B - 4*A*C;
            if(D < 0){
                return -1;
            }
            double t1 = (-B + sqrt(D)) / (2*A);
            double t2 = (-B - sqrt(D)) / (2*A);
            if(t1 < 0 && t2 < 0){
                return -1;
            }
            if(t1 < 0 || t2 < 0){
                double tao = t1 < 0 ? t2 : t1;
                intersection = r->start + r->dir*tao;
                if(((intersection.x < reference_point.x || intersection.x > reference_point.x + length) && fabs(length) > 1e-5) || ((intersection.y < reference_point.y || intersection.y > reference_point.y + width) && fabs(width) > 1e-5) || ((intersection.z < reference_point.z || intersection.z > reference_point.z + height) && fabs(height) > 1e-5)){
                    return -1;
                }
            }
            else{
                Vector i1 = r->start + r->dir*t1;
                Vector i2 = r->start + r->dir*t2;
                if(((i1.x < reference_point.x || i1.x > reference_point.x + length) && fabs(length) > 1e-5) || ((i1.y < reference_point.y || i1.y > reference_point.y + width) && fabs(width) > 1e-5) || ((i1.z < reference_point.z || i1.z > reference_point.z + height) && fabs(height) > 1e-5)){
                    if(((i2.x < reference_point.x || i2.x > reference_point.x + length) && fabs(length) > 1e-5) || ((i2.y < reference_point.y || i2.y > reference_point.y + width) && fabs(width) > 1e-5) || ((i2.z < reference_point.z || i2.z > reference_point.z + height) && fabs(height) > 1e-5)){
                        return -1;
                    }
                    else{
                        intersection = i2;
                    }
                }
                else{
                    if(((i2.x < reference_point.x || i2.x > reference_point.x + length) && fabs(length) > 1e-5) || ((i2.y < reference_point.y || i2.y > reference_point.y + width) && fabs(width) > 1e-5) || ((i2.z < reference_point.z || i2.z > reference_point.z + height) && fabs(height) > 1e-5)){
                        intersection = i1;
                    }
                    else{
                        double d1 = r->start.distance(i1);
                        double d2 = r->start.distance(i2);
                        intersection = d1 < d2 ? i1 : i2;
                    }
                }
            }

            tmin = r->start.distance(intersection);

            if(level == 0){
                return tmin;
            }
            Vector normal = getNormal(intersection);
            if(normal.dot(r->dir) > 0){
                normal = normal * (-1);
            }
            double *surf_color = getColor(intersection);
            color[0] = surf_color[0] * coEfficients[0];
            color[1] = surf_color[1] * coEfficients[0];
            color[2] = surf_color[2] * coEfficients[0];

            for(int i=0; i<pointLights.size(); i++){
                Ray *shadow = new Ray(pointLights[i]->position, intersection - pointLights[i]->position);
                int nearest = -1;
                double tshadow = DBL_MAX;
                for(int j=0; j<objects.size(); j++){
                    double t = objects[j]->intersect(shadow, color, 0);
                    if(t >= 0 && t < tshadow){
                        tshadow = t;
                        nearest = j;
                    }
                }
                if(nearest == id){
                    double lambert = (-1)*normal.dot(shadow->dir);
                    if(lambert > 0){
                        color[0] += lambert * surf_color[0] * coEfficients[1] * pointLights[i]->color[0];
                        color[1] += lambert * surf_color[1] * coEfficients[1] * pointLights[i]->color[1];
                        color[2] += lambert * surf_color[2] * coEfficients[1] * pointLights[i]->color[2];
                    }
                    Vector reflection = shadow->dir - normal * (2 * shadow->dir.dot(normal));
                    reflection.normalize();
                    double phong = (-1)*reflection.dot(r->dir);
                    if(phong > 0){
                        phong = pow(phong, shine);
                        color[0] += phong * coEfficients[2] * pointLights[i]->color[0] * surf_color[0];
                        color[1] += phong * coEfficients[2] * pointLights[i]->color[1] * surf_color[1];
                        color[2] += phong * coEfficients[2] * pointLights[i]->color[2] * surf_color[2];
                    }
                }
                delete shadow;
            }

            for(int i=0; i<spotLights.size(); i++){
                Ray *shadow = new Ray(spotLights[i]->point_light.position, intersection - spotLights[i]->point_light.position);
                int nearest = -1;
                double tshadow = DBL_MAX;
                double dot = shadow->dir.dot(spotLights[i]->direction);
                if(getDegree(acos(dot)) > spotLights[i]->cut_off){
                    continue;
                }
                for(int j=0; j<objects.size(); j++){
                    double t = objects[j]->intersect(shadow, color, 0);
                    if(t >= 0 && t < tshadow){
                        tshadow = t;
                        nearest = j;
                    }
                }
                if(nearest == id){
                    double lambert = (-1)*normal.dot(shadow->dir);
                    if(lambert > 0){
                        color[0] += lambert * surf_color[0] * coEfficients[1] * spotLights[i]->point_light.color[0] * pow(dot, 2);
                        color[1] += lambert * surf_color[1] * coEfficients[1] * spotLights[i]->point_light.color[1] * pow(dot, 2);
                        color[2] += lambert * surf_color[2] * coEfficients[1] * spotLights[i]->point_light.color[2] * pow(dot, 2);
                    }
                    Vector reflection = shadow->dir - normal * (2 * shadow->dir.dot(normal));
                    reflection.normalize();
                    double phong = (-1)*reflection.dot(r->dir);
                    if(phong > 0){
                        phong = pow(phong, shine);
                        color[0] += phong * coEfficients[2] * spotLights[i]->point_light.color[0] * pow(dot, 2) * surf_color[0];
                        color[1] += phong * coEfficients[2] * spotLights[i]->point_light.color[1] * pow(dot, 2) * surf_color[1];
                        color[2] += phong * coEfficients[2] * spotLights[i]->point_light.color[2] * pow(dot, 2) * surf_color[2];
                    }
                }
                delete shadow;
            }
            
            if(level >= recursionLevel){
                return tmin;
            }

            Vector reflection = r->dir - normal * (2 * r->dir.dot(normal));
            reflection.normalize();
            Vector start = intersection + reflection * epsilon;
            Ray *reflected = new Ray(start, reflection);
            double *reflected_color = new double[3];
            reflected_color[0] = 0;
            reflected_color[1] = 0;
            reflected_color[2] = 0;
            double t = DBL_MAX;
            int nearest = -1;
            for(int i=0; i<objects.size(); i++){
                double t1 = objects[i]->intersect(reflected, reflected_color, 0);
                if(t1 >= 0 && t1 < t){
                    t = t1;
                    nearest = i;
                }
            }
            if(nearest != -1){
                t = objects[nearest]->intersect(reflected, reflected_color, level+1);
                color[0] += reflected_color[0] * coEfficients[3];
                color[1] += reflected_color[1] * coEfficients[3];
                color[2] += reflected_color[2] * coEfficients[3];
            }
            delete reflected;
            delete[] reflected_color;
            return tmin;
        }
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

        Vector getNormal(Vector point)
        {
            return Vector(0, 0, 1);
        }

        double* getColor(Vector point)
        {
            int i = (point.x - reference_point.x) / tileWidth;
            int j = (point.y - reference_point.y) / tileWidth;
            if((i+j)%2 == 0){
                return color;
            }
            else{
                return altColor;
            }
        }

        double intersect(Ray *r, double *color, int level)
        {
            double tmin;
            double tao = - (r->start.z / r->dir.z);
            if(tao < 0){
                return -1;
            }
            Vector intersection(r->start + r->dir*tao);
            if(intersection.x < reference_point.x || intersection.x > reference_point.x + length || intersection.y < reference_point.y || intersection.y > reference_point.y + width){
                return -1;
            }
            tmin = r->start.distance(intersection);

            if(level == 0){
                return tmin;
            }
            Vector normal = getNormal(intersection);
            if(normal.dot(r->dir) > 0){
                normal = normal * (-1);
            }
            double *surf_color = getColor(intersection);
            color[0] = surf_color[0] * coEfficients[0];
            color[1] = surf_color[1] * coEfficients[0];
            color[2] = surf_color[2] * coEfficients[0];

            for(int i=0; i<pointLights.size(); i++){
                Ray *shadow = new Ray(pointLights[i]->position, intersection - pointLights[i]->position);
                int nearest = -1;
                double tshadow = DBL_MAX;
                for(int j=0; j<objects.size(); j++){
                    double t = objects[j]->intersect(shadow, color, 0);
                    if(t >= 0 && t < tshadow){
                        tshadow = t;
                        nearest = j;
                    }
                }
                if(nearest == id){
                    double lambert = (-1)*normal.dot(shadow->dir);
                    if(lambert > 0){
                        color[0] += lambert * surf_color[0] * coEfficients[1] * pointLights[i]->color[0];
                        color[1] += lambert * surf_color[1] * coEfficients[1] * pointLights[i]->color[1];
                        color[2] += lambert * surf_color[2] * coEfficients[1] * pointLights[i]->color[2];
                    }
                    Vector reflection = shadow->dir - normal * (2 * shadow->dir.dot(normal));
                    reflection.normalize();
                    double phong = (-1)*reflection.dot(r->dir);
                    if(phong > 0){
                        phong = pow(phong, shine);
                        color[0] += phong * coEfficients[2] * pointLights[i]->color[0] * surf_color[0];
                        color[1] += phong * coEfficients[2] * pointLights[i]->color[1] * surf_color[1];
                        color[2] += phong * coEfficients[2] * pointLights[i]->color[2] * surf_color[2];
                    }
                }
                delete shadow;
            }

            for(int i=0; i<spotLights.size(); i++){
                Ray *shadow = new Ray(spotLights[i]->point_light.position, intersection - spotLights[i]->point_light.position);
                int nearest = -1;
                double tshadow = DBL_MAX;
                double dot = shadow->dir.dot(spotLights[i]->direction);
                if(getDegree(acos(dot)) > spotLights[i]->cut_off){
                    continue;
                }
                for(int j=0; j<objects.size(); j++){
                    double t = objects[j]->intersect(shadow, color, 0);
                    if(t >= 0 && t < tshadow){
                        tshadow = t;
                        nearest = j;
                    }
                }
                if(nearest == id){
                    double lambert = (-1)*normal.dot(shadow->dir);
                    if(lambert > 0){
                        color[0] += lambert * surf_color[0] * coEfficients[1] * spotLights[i]->point_light.color[0] * pow(dot, 2);
                        color[1] += lambert * surf_color[1] * coEfficients[1] * spotLights[i]->point_light.color[1] * pow(dot, 2);
                        color[2] += lambert * surf_color[2] * coEfficients[1] * spotLights[i]->point_light.color[2] * pow(dot, 2);
                    }
                    Vector reflection = shadow->dir - normal * (2 * shadow->dir.dot(normal));
                    reflection.normalize();
                    double phong = (-1)*reflection.dot(r->dir);
                    if(phong > 0){
                        phong = pow(phong, shine);
                        color[0] += phong * coEfficients[2] * spotLights[i]->point_light.color[0] * pow(dot, 2) * surf_color[0];
                        color[1] += phong * coEfficients[2] * spotLights[i]->point_light.color[1] * pow(dot, 2) * surf_color[1];
                        color[2] += phong * coEfficients[2] * spotLights[i]->point_light.color[2] * pow(dot, 2) * surf_color[2];
                    }
                }
                delete shadow;
            }
            
            if(level >= recursionLevel){
                return tmin;
            }

            Vector reflection = r->dir - normal * (2 * r->dir.dot(normal));
            reflection.normalize();
            Vector start = intersection + reflection * epsilon;
            Ray *reflected = new Ray(start, reflection);
            double *reflected_color = new double[3];
            reflected_color[0] = 0;
            reflected_color[1] = 0;
            reflected_color[2] = 0;
            double t = DBL_MAX;
            int nearest = -1;
            for(int i=0; i<objects.size(); i++){
                double t1 = objects[i]->intersect(reflected, reflected_color, 0);
                if(t1 >= 0 && t1 < t){
                    t = t1;
                    nearest = i;
                }
            }
            if(nearest != -1){
                t = objects[nearest]->intersect(reflected, reflected_color, level+1);
                color[0] += reflected_color[0] * coEfficients[3];
                color[1] += reflected_color[1] * coEfficients[3];
                color[2] += reflected_color[2] * coEfficients[3];
            }
            delete reflected;
            delete[] reflected_color;
            return tmin;
        }
};
#endif