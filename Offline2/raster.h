#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include "matrix.h"
#include "transform.h"

#ifndef RASTER_H
#define RASTER_H

static unsigned long int g_seed = 1;
inline int randomColor()
{
g_seed = (214013 * g_seed + 2531011);
return (g_seed >> 16) & 0x7FFF;
}

class Zbuffer
{
    public:
        double **zbuffer;
        int **red, **green, **blue;
        int width, height;
        double topY, leftX, dy, dx;

        Zbuffer(int width, int height)
        {
            this->width = width;
            this->height = height;
            dx = 2.00/width;
            dy = 2.00/height;
            topY = 1 - dy/2;
            leftX = (-1) + dx/2;
            zbuffer = new double*[height];
            red = new int*[height];
            green = new int*[height];
            blue = new int*[height];
            for(int i = 0; i < height; i++)
            {
                zbuffer[i] = new double[width];
                red[i] = new int[width];
                green[i] = new int[width];
                blue[i] = new int[width];
                for(int j = 0; j < width; j++)
                {
                    zbuffer[i][j] = 1;
                    red[i][j] = 0;
                    green[i][j] = 0;
                    blue[i][j] = 0;
                }
            }
        }

        Zbuffer()
        {
            width = 100;
            height = 100;
            dx = 2.00/width;
            dy = 2.00/height;
            topY = 1 - dy/2;
            leftX = (-1) + dx/2;
            zbuffer = new double*[height];
            red = new int*[height];
            green = new int*[height];
            blue = new int*[height];
            for(int i = 0; i < height; i++)
            {
                zbuffer[i] = new double[width];
                red[i] = new int[width];
                green[i] = new int[width];
                blue[i] = new int[width];
                for(int j = 0; j < width; j++)
                {
                    zbuffer[i][j] = 1;
                    red[i][j] = 0;
                    green[i][j] = 0;
                    blue[i][j] = 0;
                }
            }
        }

        ~Zbuffer()
        {
            for(int i = 0; i < height; i++)
            {
                delete[] zbuffer[i];
                delete[] red[i];
                delete[] green[i];
                delete[] blue[i];
            }
            delete[] zbuffer;
            delete[] red;
            delete[] green;
            delete[] blue;
        }

        double inline getRowY(int row)
        {
            return topY - row*dy;
        }
              
        double inline getColX(int col)
        {
            return leftX + col*dx;
        }

        void print(std::ostream &out)
        {
            for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    if(zbuffer[i][j] < 1 && zbuffer[i][j] > -1){
                        out << std::fixed << std::setprecision(6) << zbuffer[i][j] << '\t';
                    }
                }
                out << std::endl;
            }
        }

};

class Zhelper
{
    public:
        Zbuffer *zbuffer;

        Zhelper(Zbuffer *zbuffer)
        {
            this->zbuffer = zbuffer;
        }

        int getHighRow(double y1, double y2, double y3)
        {
            double y = std::max(y1, std::max(y2, y3));
            if(y > zbuffer->topY){
                return 0;
            }
            else{
                return round((zbuffer->topY - y)/(zbuffer->dy));
            }
        }
        
        int getLowRow(double y1, double y2, double y3)
        {
            double y = std::min(y1, std::min(y2, y3));
            double low = zbuffer->topY - ((zbuffer->height-1)*zbuffer->dy);
            if(y < low){
                return zbuffer->height - 1;
            }
            else{
                int row = round((y - low)/zbuffer->dy);
                return zbuffer->height - row - 1;
            }
        }

        int getLeftCol(double x1, double x2)
        {
            double x = std::min(x1, x2);
            if(x < zbuffer->leftX){
                return 0;
            }
            else{
                return round((x - zbuffer->leftX)/(zbuffer->dx));
            }
        }

        int getRightCol(double x1, double x2)
        {
            double x = std::max(x1, x2);
            double high = zbuffer->leftX + ((zbuffer->width-1)*zbuffer->dx);
            if(x > high){
                return zbuffer->width - 1;
            }
            else{
                int col = round((high - x)/zbuffer->dx);
                return zbuffer->width - col - 1;
            }
        }

        std::vector<std::pair<double, double>> getXZps(Pointh p1, Pointh p2, Pointh p3, double yp)
        {
            std::vector<std::pair<double, double>> xzps;
            double epsilon = 0.0000001;
            if(fabs(yp - p1.y) < epsilon){
                xzps.push_back(std::make_pair(p1.x, p1.z));
            }
            if(fabs(yp - p2.y) < epsilon){
                xzps.push_back(std::make_pair(p2.x, p2.z));
            }
            if(fabs(yp - p3.y) < epsilon){
                xzps.push_back(std::make_pair(p3.x, p3.z));
            }
            if((yp > p1.y && yp < p2.y) || (yp < p1.y && yp > p2.y)){
                double x = p2.x + ((yp - p2.y)/(p1.y - p2.y))*(p1.x - p2.x);
                double z = p2.z + ((yp - p2.y)/(p1.y - p2.y))*(p1.z - p2.z);
                xzps.push_back(std::make_pair(x, z));
            }
            if((yp > p1.y && yp < p3.y) || (yp < p1.y && yp > p3.y)){
                double x = p3.x + ((yp - p3.y)/(p1.y - p3.y))*(p1.x - p3.x);
                double z = p3.z + ((yp - p3.y)/(p1.y - p3.y))*(p1.z - p3.z);
                xzps.push_back(std::make_pair(x, z));
            }
            if((yp > p2.y && yp < p3.y) || (yp < p2.y && yp > p3.y)){
                double x = p3.x + ((yp - p3.y)/(p2.y - p3.y))*(p2.x - p3.x);
                double z = p3.z + ((yp - p3.y)/(p2.y - p3.y))*(p2.z - p3.z);
                xzps.push_back(std::make_pair(x, z));
            }
            return xzps;
        }

        double getZval(double x1, double z1, double x2, double z2, double xp)
        {
            double epsilon = 0.0000001;
            if(fabs(xp - x1) < epsilon)
                return z1;
            if(fabs(xp - x2) < epsilon)
                return z2;
            return z1 + ((xp - x1)/(x2 - x1))*(z2 - z1);
        }

        void populateZbuffer(std::vector<Triangle> triangles)
        {
            for(int i = 0; i < triangles.size(); i++){
                Triangle t = triangles[i];
                int highRow = getHighRow(t.points[0].y, t.points[1].y, t.points[2].y);
                int lowRow = getLowRow(t.points[0].y, t.points[1].y, t.points[2].y);
                int red = randomColor()%256;
                int green = randomColor()%256;
                int blue = randomColor()%256;

                for(int j=highRow; j<=lowRow; j++){
                    double yp = zbuffer->getRowY(j);
                    std::vector<std::pair<double, double>> xzps = getXZps(t.points[0], t.points[1], t.points[2], yp);
                    int leftCol, rightCol;
                    if(xzps.size() == 0){
                        continue;
                    }
                    else if(xzps.size() == 1){
                        leftCol = getLeftCol(xzps[0].first, xzps[0].first);
                        rightCol = getRightCol(xzps[0].first, xzps[0].first);
                    }
                    else{
                        leftCol = getLeftCol(xzps[0].first, xzps[1].first);
                        rightCol = getRightCol(xzps[1].first, xzps[0].first);
                    }    
                    
                    for(int k=leftCol; k<=rightCol; k++){
                        double xp = zbuffer->getColX(k);
                        double zp = getZval(xzps[0].first, xzps[0].second, xzps[1].first, xzps[1].second, xp);
                        if(zp < zbuffer->zbuffer[j][k] && zp > -1 && zp < 1){
                            zbuffer->zbuffer[j][k] = zp;
                            zbuffer->red[j][k] = red;
                            zbuffer->green[j][k] = green;
                            zbuffer->blue[j][k] = blue;
                        }
                    }
                }
            }
        }

};

#endif