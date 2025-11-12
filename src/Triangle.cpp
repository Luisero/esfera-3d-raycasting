#include "../include/Triangle.hpp"

Triangle::Triangle(Point3 &p1, Point3 &p2, Point3 &p3, Point3 &color, Point3 &diffuse_color, Point3 &specular_color) {
  this->p1 = p1;
  this->p2 = p2;
  this->p3 = p3;
  Vector3 r1(p1, p2);
  Vector3 r2(p1, p3);
  this->normal = cross(r1, r2);
  this->area = this->normal.length()/2;
}

bool Triangle::Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const {
  Vector3 inv_w(origin, this->p1);
  double denominator = dot(dir, normalize(this->normal));
  double t = 0;
  if(denominator != 0){
    t = dot(normalize(inv_w), this->normal)/denominator; 
  }
  Point3 p_int = origin + t*dir;

  Vector3 s1(p_int, p1);
  Vector3 s2(p_int, p2);
  Vector3 s3(p_int, p3);

  double c1 = dot(this->normal, cross(s3, s1))/(2*this->area);
  double c2 = dot(this->normal, cross(s1, s2))/(2*this->area);
  double c3 = 2*this->area - c2 - c1;

  if(c1 < 0 || c2 < 0 || c3 < 0){
    return false;
  } else {
    return true;
  }
}