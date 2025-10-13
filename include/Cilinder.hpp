#ifndef CILINDER
#define CILINDER
#include "Object.hpp"
#include "Point3.hpp"
#include "Vector3.hpp"
#include <vector>

class Cilinder : public Object {
    public:
        Point3 center;
        float height;
        float radius;
        Vector3 dc;
        std::vector<std::vector<float>> M;
        bool bottom_lid;
        bool upper_lid;
        Point3 color;
        Point3 diffuse_color;
        Point3 specular_color;
        Point3 getCenter() const override;
        Point3 getColor() const override;
        Point3 getDiffuse() const override;
        Point3 getSpecular() const override;
        Vector3 getSurfaceNormal(const Point3 &p_int) const override;

        Cilinder(Point3 &c_base, float height, float radius, Vector3 &dc, bool bottom_lid, bool upper_lid, Point3 &color, Point3 &dif_color, Point3 &esp_color);
        bool Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const override;
};

#endif