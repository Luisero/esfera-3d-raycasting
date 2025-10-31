#ifndef OBJECT
#define OBJECT
#include "Point3.hpp"
#include "Vector3.hpp"
#include "HitRecord.hpp"

class Object {
    public:
        const Point3 center;
        float radius;
        const Point3 color;
        const Point3 diffuse_color;
        const Point3 specular_color;
        virtual ~Object();
        virtual bool Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const = 0;
        virtual const Point3& getCenter() const { return center; };
        virtual const Point3& getColor() const { return color; };
        virtual const Point3& getDiffuse() const { return diffuse_color; };
        virtual const Point3& getSpecular() const { return specular_color; };
};

#endif