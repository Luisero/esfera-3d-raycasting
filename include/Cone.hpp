#ifndef CONE
#define CONE
#include "./Object.hpp"
#include <vector>

class Cone : public Object {
  public:
  Point3 center;
  float radius;
  bool has_bottom;
  std::vector<std::vector<float>> M_star;
  Point3 vertice;
  Vector3 dc;
  float height;
  Point3 color;
  Point3 diffuse_color;
  Point3 specular_color;

  Cone(Point3 &center, float radius, bool has_bottom, Point3 &vertice, Point3 &color, Point3 &dif_color, Point3 &spec_color);

  const Point3& getColor() const override { return color; };
  const Point3& getDiffuse() const override { return diffuse_color; };
  const Point3& getSpecular() const override { return specular_color; };
  
  bool Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const override;
  Vector3 getSurfaceNormal(const Point3 &p_int) const;
};

#endif