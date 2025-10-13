#include "Point3.hpp"
#include "Vector3.hpp"
#include "Object.hpp"
#ifndef SPHERE
#define SPHERE

class Sphere : public Object {
    public:
        Point3 center;
        Point3 color;
        Point3 diffuse_color;
        Point3 specular_color;
        float radius;
        Sphere(Point3 &center, float radius, Point3 &color, Point3 &diffuse_color, Point3 &specular_color);

        bool Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const override;
        Point3 getCenter() const override { return center; };
        Vector3 getSurfaceNormal(const Point3 &p_int) const override;
        Point3 getColor() const override { return color; };
        Point3 getDiffuse() const override { return diffuse_color; };
        Point3 getSpecular() const override { return specular_color; };
};

#endif