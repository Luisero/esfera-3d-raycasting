#include "../include/Vector3.hpp"
#include "../include/Point3.hpp"
#include <cmath>
Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::Vector3(const Point3 &o, const Point3 &a){
    this->x = o.x - a.x;
    this->y = o.y - a.y;
    this->z = o.z - a.z;
}

Vector3::Vector3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;

}

void Vector3::normalize()
{
    float length = std::sqrt(x * x + y * y + z * z);
    if (length != 0.0f) {
        x /= length;
        y /= length;
        z /= length;
    }
    this->x = x;
    this->y = y;
    this->z = z;
}