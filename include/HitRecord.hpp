#include "Point3.hpp"
#include "Vector3.hpp"

class Object;

struct HitRecord {
    double t;
    Point3 p_int;
    Vector3 normal;
    const Object *obj_ptr;   
};