#include "../include/Vector3.hpp"
#include "../include/Point3.hpp"
#include <cmath>
Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::Vector3(const Point3 &origin, const Point3 &destiny){
    this->x = destiny.x - origin.x;
    this->y = destiny.y - origin.y;
    this->z = destiny.z - origin.z;
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
}

Vector3 normalize(const Vector3 &o) {
    float length = std::sqrt(o.x * o.x + o.y * o.y + o.z * o.z);
    if(length != 0.0f){
        Vector3 unit_vector(o.x/length, o.y/length, o.z/length);
        return unit_vector;
    }
    return o;
}

float Vector3::length(){
    return std::sqrt(x * x + y * y + z * z);
}

const float& Vector3::operator[](int i) const {
    if(i == 0) return this->x;
    if(i == 1) return this->y;
    return this->z;
}