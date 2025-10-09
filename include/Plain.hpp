#ifndef PLAIN
#define PLAIN
#include "Object.hpp"

class Plain : public Object {
    public:
        Point3 p0, p1, p2;
        Vector3 normal;
        Point3 color;
        Point3 diffuse_color;
        Point3 specular_color;

        Plain(Point3 &p0, Point3 &p1, Point3 &p2, Point3 &color, Point3 &diffuse_color, Point3 &specular_color);

        Vector3 getSurfaceNormal(const Point3 &p_int) const override;
        float Intersect(const Point3 &origin, const Vector3 &dir) const override;
        Point3 getCenter() const override;
        Point3 getColor() const override;
        Point3 getDiffuse() const override;
        Point3 getSpecular() const override;
};

#endif