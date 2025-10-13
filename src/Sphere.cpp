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

bool Sphere::Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const{
    Vector3 L(center, origin);
    float b = 2.0f * dot(dir, L);
    float c = dot(L, L) - radius * radius;

    float discriminant = b * b - 4.0f * c; // a=1
    if (discriminant < 0)
        return false;

    float sqrtD = std::sqrt(discriminant);
    float t1 = (-b + sqrtD) * 0.5f;
    float t2 = (-b - sqrtD) * 0.5f;

    // checando se t1 ou t2 estão no limite aceitável
    // primeiro t2 pq ele é menor (está mais perto do observador)
    if (t2 > t_min && t2 < t_max){
        // a distancia ate a colisao
        hr.t = t2;
        // o ponto da colisão
        hr.p_int = Point3(origin + t2*dir);
        Vector3 normal_ponto(center, hr.p_int);
        normal_ponto.normalize();
        hr.normal = normal_ponto;
        hr.obj_ptr = this;
    } 
    else if(t1 > t_min && t1 < t_max) {
        hr.t = t1;
        // o ponto da colisão
        hr.p_int = Point3(origin + t1*dir);
        Vector3 normal_ponto(center, hr.p_int);
        normal_ponto.normalize();
        hr.normal = normal_ponto;
        hr.obj_ptr = this;
    } else return false;

    return true;
}

Vector3 Sphere::getSurfaceNormal(const Point3 &p_int) const {
    Vector3 surface_normal(this->getCenter(), p_int);
    return surface_normal;
}