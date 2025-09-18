#ifndef VECTOR3
#define VECTOR3

class Vector3 {
public:
  float x, y, z;

  // Construtores
  Vector3();
  Vector3(float x, float y, float z);

  // Métodos
  void normalize();
  float length() const;
  float dot(const Vector3 &o) const;

  // Operadores
  Vector3 operator+(const Vector3 &o) const;
  Vector3 operator-(const Vector3 &o) const;
  Vector3 operator*(float escalar) const;
  Vector3 operator/(float escalar) const;

  Vector3 &operator+=(const Vector3 &o);
  Vector3 &operator-=(const Vector3 &o);
  Vector3 &operator*=(float escalar);
  Vector3 &operator/=(float escalar);
};

#endif
