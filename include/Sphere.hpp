#include "Vector3.hpp"
#ifndef SPHERE
#define SPHERE
class Sphere{
    public:
        Vector3 center;
        float radius;
        Sphere(Vector3 center, float radius);
};

#endif