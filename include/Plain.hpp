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

        const Point3& getColor() const override { return color; };
        const Point3& getDiffuse() const override { return diffuse_color; };
        const Point3& getSpecular() const override { return specular_color; };

        Vector3 getSurfaceNormal(const Point3 &p_int) const;
        bool Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const override;
};

#endif