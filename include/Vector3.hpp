#ifndef VECTOR3
#define VECTOR3

class Point3;

class Vector3 {
public:
  float x, y, z = 0;
  Vector3(float x, float y, float z);
  Vector3(const Point3 &origin, const Point3 &destiny);
  Vector3();
  void normalize();
  float length();
};

// operadores
inline Vector3 operator*(float e, const Vector3 &o) {
  return { o.x*e, o.y*e, o.z*e };
};
inline Vector3 operator*(const Vector3 &o, float e) {
  return { o.x*e, o.y*e, o.z*e };
};

inline Vector3 operator+(float e, const Vector3 &o) {
  return { e+o.x, e+o.y, e+o.z };
};
inline Vector3 operator+(const Vector3 &o, float e) {
  return { o.x+e, o.y+e, o.z+e };
};
inline Vector3 operator+(const Vector3& o, const Vector3 &a) {
  return { o.x+a.x, o.y+a.y, o.z+a.z };
};

inline Vector3 operator-(float e, const Vector3 &o) {
  return { e-o.x, e-o.y, e-o.z };
};
inline Vector3 operator-(const Vector3 &o, float e) {
  return { o.x-e, o.y-e, o.z-e };
};
inline Vector3 operator-(const Vector3 &o, const Vector3 &a) {
  return { o.x-a.x, o.y-a.y, o.z-a.z };
};

inline float dot(const Vector3 &o, const Vector3 &a) { 
  return o.x*a.x + o.y*a.y + o.z*a.z; 
};

inline Vector3 reflect(const Vector3 &normal, const Vector3 &luz) {
  return 2*dot(normal, luz)*normal-luz;
}

#endif
