#include <bits/stdc++.h>

#ifdef __linux__
 #include <GL/glut.h>
#elif WIN32
 #include <windows.h>
 #include <GL/glut.h>
#endif

#include "1905008_vector.h"
#include "1905008_camera.h"
#include "1905008_classes.h"
#include "bitmap_image.hpp"

#define rad(x) ((x) * acos(-1) / 180)
#define pi acos(-1)

using namespace std;

vector<Object*> objects;
vector<PointLight*> pointLights;
vector<SpotLight*> spotLights;
int recursionLevel, imageWidth, imageHeight;
int windowWidth = 640, windowHeight = 640;
int imageNum = 0;
double viewAngle = 80;

void loadData()
{
    ifstream fin("scene.txt");
    fin >> recursionLevel >> imageWidth;
    imageHeight = imageWidth;

    int n;
    fin >> n;
    for(int i = 0; i < n; i++)
    {
        string type;
        fin >> type;
        if(type == "sphere")
        {
            Vector center;
            double radius;
            fin >> center.x >> center.y >> center.z;
            fin >> radius;
            Sphere *s = new Sphere(center, radius);
            fin >> s->color[0] >> s->color[1] >> s->color[2];
            fin >> s->coEfficients[0] >> s->coEfficients[1] >> s->coEfficients[2] >> s->coEfficients[3];
            fin >> s->shine;
            objects.push_back(s);
        }
        else if(type == "triangle")
        {
            Vector a, b, c;
            fin >> a.x >> a.y >> a.z;
            fin >> b.x >> b.y >> b.z;
            fin >> c.x >> c.y >> c.z;
            Triangle *t = new Triangle(a, b, c);
            fin >> t->color[0] >> t->color[1] >> t->color[2];
            fin >> t->coEfficients[0] >> t->coEfficients[1] >> t->coEfficients[2] >> t->coEfficients[3];
            fin >> t->shine;
            objects.push_back(t);
        }
        else if(type == "general")
        {
            double a, b, c, d, e, f, g, h, i, j;
            double refx, refy, refz, len, wid, hei;
            fin >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j;
            General *gen = new General(a, b, c, d, e, f, g, h, i, j);
            fin >> refx >> refy >> refz;
            fin >> len >> wid >> hei;
            gen->setDimensions(refx, refy, refz, len, wid, hei);
            fin >> gen->color[0] >> gen->color[1] >> gen->color[2];
            fin >> gen->coEfficients[0] >> gen->coEfficients[1] >> gen->coEfficients[2] >> gen->coEfficients[3];
            fin >> gen->shine;
            objects.push_back(gen);
        }
    }
    fin >> n;
    for(int i = 0; i < n; i++)
    {
        PointLight *pl = new PointLight();
        fin >> pl->position.x >> pl->position.y >> pl->position.z;
        fin >> pl->color[0] >> pl->color[1] >> pl->color[2];
        pointLights.push_back(pl);
    }

    fin >> n;
    for(int i = 0; i < n; i++)
    {
        PointLight p1;
        Vector direction;
        double angle;
        fin >> p1.position.x >> p1.position.y >> p1.position.z;
        fin >> p1.color[0] >> p1.color[1] >> p1.color[2];
        fin >> direction.x >> direction.y >> direction.z;
        fin >> angle;
        SpotLight *sl = new SpotLight(p1, direction, angle);
        spotLights.push_back(sl);
    }

    Floor *f = new Floor(20, 1000);
    f->setCoEfficients(0.4, 0.2, 0.2, 0.2);
    f->setShine(5);
    f->setColor(1, 1, 1);
    f->setAltColor(0, 0, 0);
    objects.push_back(f);

    for(int i=0; i<objects.size(); i++){
        objects[i]->id = i;
    }

    fin.close();
}

void capture()
{
    bitmap_image *image = new bitmap_image(imageWidth, imageHeight);
    double planeDistance = (windowHeight / 2.0) / tan(rad(viewAngle / 2.0));
    Vector eye(look.eyeX, look.eyeY, look.eyeZ);
    Vector l(look.eyeX, look.eyeY, look.eyeZ, look.centerX, look.centerY, look.centerZ);
    l.normalize();
    Vector u(look.upX, look.upY, look.upZ);
    u.normalize();
    Vector r = l.cross(u);
    r.normalize();
    Vector topleft = eye + l*planeDistance - r*(windowWidth/2.0) + u*(windowHeight/2.0);
    double du = (windowWidth*1.0)/imageWidth;
    double dv = (windowHeight*1.0)/imageHeight;

    topleft = topleft + r*(0.5*du) - u*(0.5*dv);

    for(int i=0; i<imageWidth; i++){
        for(int j=0; j<imageHeight; j++){
            int nearest;
            double t, tmin = DBL_MAX;
            Vector curPix = topleft + (du*i)*r - (dv*j)*u;
            Ray *r = new Ray(eye, curPix-eye);
            double *color = new double[3];
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
            for(int k=0; k<objects.size(); k++){
                t = objects[k]->intersect(r, color, 0);
                if(t >= 0 && t < tmin){
                    tmin = t;
                    nearest = k;
                }
            }
            if(tmin == DBL_MAX){
                color[0] = 0;
                color[1] = 0;
                color[2] = 0;
            }
            else{
                tmin = objects[nearest]->intersect(r, color, 1);
            }
            image->set_pixel(i, j, color[0]*255, color[1]*255, color[2]*255);
            delete[] color;
            delete r;
        }
    }
    imageNum++;
    image->save_image("Output_1"+to_string(imageNum)+".bmp");
    delete image;
}

void display()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    look.lookAt();

    for(int i = 0; i < objects.size(); i++)
    {
        objects[i]->draw();
    }
    for(int i = 0; i < pointLights.size(); i++)
    {
        pointLights[i]->draw();
    }
    for(int i = 0; i < spotLights.size(); i++)
    {
        spotLights[i]->draw();
    }

    glFlush();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, 1, 1, 500);
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
        case '0':
            capture();
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
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Ray Tracing");

    loadData();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutTimerFunc(50, Timer, 0);

    glutMainLoop();
    for(int i = 0; i < objects.size(); i++)
    {
        delete objects[i];
    }
    for(int i = 0; i < pointLights.size(); i++)
    {
        delete pointLights[i];
    }
    for(int i = 0; i < spotLights.size(); i++)
    {
        delete spotLights[i];
    }
    return 0;
}