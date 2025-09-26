#ifndef VECTOR3
#define VECTOR3
class Vector3 {
public:
  float x, y, z = 0;
  Vector3(float x, float y, float z);
  Vector3();
  void normalize();
  float dot(const Vector3 &o) const { return x * o.x + y * o.y + z * o.z; };
  Vector3 operator-(const Vector3 &o) const {
    return {x - o.x, y - o.y, z - o.z};
  };
  Vector3 operator+(const Vector3 &o) const {
    return {x + o.x, y + o.y, z + o.z};
  };
};

#endif

#ifndef VECTOR4
#define VECTOR4

class Vector4 {
public:
  float x, y, z, w = 0;
  Vector4(float x, float y, float z, float w);
  Vector4();
  void normalize();
  float dot(const Vector4 &o) { return x * o.x + y * o.y + z * o.z + w * o.w; };
  Vector4 operator-(const Vector4 &o) const {
    return {x - o.x, y - o.y, z - o.z, w - o.w};
  };
  Vector4 operator+(const Vector4 &o) const {
    return {x + o.x, y + o.y, z + o.z, w + o.w};
  }
};
#endif // !VECTOR4
#define VECTOR4
