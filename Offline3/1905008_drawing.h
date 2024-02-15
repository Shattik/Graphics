#ifdef __linux__
 #include <GL/glut.h>
#elif WIN32
 #include <windows.h>
 #include <GL/glut.h>
#endif

#ifndef DRAWING_H
#define DRAWING_H
void drawTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3)
{
    glBegin(GL_TRIANGLES);{
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
    }glEnd();
}

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
#endif