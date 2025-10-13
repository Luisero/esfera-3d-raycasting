#ifndef OBJECT
#define OBJECT
#include "Point3.hpp"
#include "Vector3.hpp"
#include "HitRecord.hpp"

class Object {
    public:
        Point3 center;
        Point3 color;
        Point3 diffuse_color;
        Point3 specular_color;
        virtual ~Object();
        virtual bool Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const = 0;
        virtual Point3 getCenter() const = 0;
        virtual Vector3 getSurfaceNormal(const Point3 &p_int) const = 0;
        virtual Point3 getColor() const = 0;
        virtual Point3 getDiffuse() const = 0;
        virtual Point3 getSpecular() const = 0;
};

#endif