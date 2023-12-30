#include <iostream>
#include <fstream>
#include <vector>
#include "matrix.h"
#include "transform.h"

using namespace std;

vector<Triangle> transformations()
{
    vector<Triangle> triangles;
    ifstream in("scene.txt");
    ofstream out1("stage1.txt"), out2("stage2.txt"), out3("stage3.txt");
    Model model;

    double eyex, eyey, eyez;
    double lookx, looky, lookz;
    double upx, upy, upz;
    in >> eyex >> eyey >> eyez;
    in >> lookx >> looky >> lookz;
    in >> upx >> upy >> upz;
    View view(eyex, eyey, eyez, lookx, looky, lookz, upx, upy, upz);

    double fovY, aspectRatio, near, far;
    in >> fovY >> aspectRatio >> near >> far;
    Projection projection(fovY, aspectRatio, near, far);

    string command;
    while(true){
        in >> command;
        if(command == "triangle"){
            double x1, y1, z1;
            double x2, y2, z2;
            double x3, y3, z3;
            in >> x1 >> y1 >> z1;
            in >> x2 >> y2 >> z2;
            in >> x3 >> y3 >> z3;
            Triangle triangle(Pointh(x1, y1, z1), Pointh(x2, y2, z2), Pointh(x3, y3, z3));
            triangle.transform(model.current);
            triangle.print(out1);
            out1 << endl;
            triangle.transform(view.m);
            triangle.print(out2);
            out2 << endl;
            triangle.transform(projection.m);
            triangle.print(out3);
            out3 << endl;
            triangles.push_back(triangle);
        }
        else if(command == "translate"){
            double tx, ty, tz;
            in >> tx >> ty >> tz;
            model.translate(tx, ty, tz);
        }
        else if(command == "scale"){
            double sx, sy, sz;
            in >> sx >> sy >> sz;
            model.scale(sx, sy, sz);
        }
        else if(command == "rotate"){
            double angle, rx, ry, rz;
            in >> angle >> rx >> ry >> rz;
            model.rotate(angle, rx, ry, rz);
        }
        else if(command == "push"){
            model.pushModel();
        }
        else if(command == "pop"){
            model.popModel();
        }
        else if(command == "end"){
            break;
        }
    }

    in.close();
    out1.close();
    out2.close();
    out3.close();
    return triangles;
}

int main()
{
    vector<Triangle> triangles = transformations();
    return 0;
}