#include "../include/Plain.hpp"
#include "../include/Vector3.hpp"
#include "../include/Point3.hpp"
#include <cmath>

Plain::Plain(Point3 &p0, Point3 &p1, Point3 &p2, Point3 &color, Point3 &diffuse_color, Point3 &specular_color){
    this->p0 = p0;
    this->p1 = p1;
    this->p2 = p2;
    this->color = color;
    this->diffuse_color = diffuse_color;
    this->specular_color = specular_color;

    Vector3 s1(p0, p1);
    Vector3 s2(p0, p2);
    Vector3 plain_normal(s1.y*s2.z-s1.z*s2.y, s1.z*s2.x-s1.x*s2.z, s1.x*s2.y-s1.y*s2.x);
    this->normal = plain_normal;
}

Vector3 Plain::getSurfaceNormal(const Point3 &p_int) const{
    return this->normal;
}

float Plain::Intersect(const Point3 &origin, const Vector3 &dir) const{
    float denominator = dot(this->normal, dir);

    if (std::abs(denominator) > 0.0001f) {
        Vector3 p0_to_origin(origin, this->p0);
        float t = dot(p0_to_origin, this->normal) / denominator;
        
        if (t >= 0) {
            return t;
        }
    }

    return -1.0f;
}

Point3 Plain::getCenter() const{
    return this->p0;
}

Point3 Plain::getColor() const {
    return this->color;
}

Point3 Plain::getDiffuse() const {
    return this->diffuse_color;
}

Point3 Plain::getSpecular() const {
    return this->specular_color;
}