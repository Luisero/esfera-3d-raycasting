#ifndef TRIANGLE
#define TRIANGLE
#include "Object.hpp"

class Triangle : public Object {
  public:
    Point3 p1, p2, p3, color, diffuse_color, specular_color;
    Vector3 normal;
    double area;

    Triangle(Point3 &p1, Point3 &p2, Point3 &p3, Point3 &color, Point3 &diffuse_color, Point3 &specular_color);

    bool Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const override;

    const Point3& getColor() const override { return color; };
    const Point3& getDiffuse() const override { return diffuse_color; };
    const Point3& getSpecular() const override { return specular_color; };
};

#endif