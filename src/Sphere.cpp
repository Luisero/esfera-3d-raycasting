#include "../include/Sphere.hpp"
#include "../include/Point3.hpp"
#include "../include/Vector3.hpp"
#include <cmath>

Sphere::Sphere(Point3 &center, float radius, Point3 &color, Point3 &diffuse_color, Point3 &specular_color)
{
    this->center = center;
    this->radius = radius;
    this->color = color;
    this->diffuse_color = diffuse_color;
    this->specular_color = specular_color;
}

float Sphere::Intersect(const Point3 &origin, const Vector3 &dir) const{
    Vector3 L(center, origin);
    float b = 2.0f * dot(dir, L);
    float c = dot(L, L) - radius * radius;

    float discriminant = b * b - 4.0f * c; // a=1
    if (discriminant < 0)
        return -1.0f;

    float sqrtD = std::sqrt(discriminant);
    float t1 = (-b - sqrtD) * 0.5f;
    float t2 = (-b + sqrtD) * 0.5f;

    float t = std::min(t1, t2); // pega a interseção menor
    if (t < 0)
        return -1.0f; // está atrás do raio

    return t;
}

Vector3 Sphere::getSurfaceNormal(const Point3 &p_int) const {
    Vector3 surface_normal(this->getCenter(), p_int);
    return surface_normal;
}