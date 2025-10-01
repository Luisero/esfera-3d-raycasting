#include "Point3.hpp"
#ifndef SPHERE
#define SPHERE
class Sphere{
    public:
        Point3 center;
        float radius;
        Sphere(Point3 center, float radius);
};

#endif