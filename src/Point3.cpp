#include "../include/Point3.hpp"

Point3::Point3(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

Point3::Point3(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
}