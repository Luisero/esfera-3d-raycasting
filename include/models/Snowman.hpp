#ifndef SNOWMAN
#define SNOWMAN
#include "../Object.hpp"
#include "../Sphere.hpp"
#include "../Cilinder.hpp"
#include "../Cone.hpp"
#include <vector>
#include <memory>

class Snowman : public Object {
  public:
  std::vector<std::unique_ptr<Object>> components;
  Point3 center;
  // how much the spheres are sunk into each other
  float sink_factor;
  
  // by convention, the spheres in the body are: 0 - "legs", 1 - "chest", 2 - head
  Snowman(Point3 &center, float sink_factor);
  bool Intersect(const Point3& origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const;
};

#endif