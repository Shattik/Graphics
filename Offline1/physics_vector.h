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