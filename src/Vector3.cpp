#include "../include/Vector3.hpp"
#include <cmath>

Vector3::Vector3() {
  this->x = 0;
  this->y = 0;
  this->z = 0;
}

Vector3::Vector3(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

float Vector3::length() const { return std::sqrt(x * x + y * y + z * z); }

void Vector3::normalize() {
  float len = length();
  if (len > 0.00001f) {
    x /= len;
    y /= len;
    z /= len;
  }
}

// Produto escalar
float Vector3::dot(const Vector3 &o) const {
  return x * o.x + y * o.y + z * o.z;
}

// Operadores
Vector3 Vector3::operator+(const Vector3 &o) const {
  return Vector3(x + o.x, y + o.y, z + o.z);
}

Vector3 Vector3::operator-(const Vector3 &o) const {
  return Vector3(x - o.x, y - o.y, z - o.z);
}

Vector3 Vector3::operator*(float escalar) const {
  return Vector3(x * escalar, y * escalar, z * escalar);
}

Vector3 Vector3::operator/(float escalar) const {
  return Vector3(x / escalar, y / escalar, z / escalar);
}

Vector3 &Vector3::operator+=(const Vector3 &o) {
  x += o.x;
  y += o.y;
  z += o.z;
  return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &o) {
  x -= o.x;
  y -= o.y;
  z -= o.z;
  return *this;
}

Vector3 &Vector3::operator*=(float escalar) {
  x *= escalar;
  y *= escalar;
  z *= escalar;
  return *this;
}

Vector3 &Vector3::operator/=(float escalar) {
  x /= escalar;
  y /= escalar;
  z /= escalar;
  return *this;
}
