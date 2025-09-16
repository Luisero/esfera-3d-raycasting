#ifndef VECTOR3
#define VECTOR3
class Vector3{
    public:
        float x,y,z = 0;
        Vector3(float x, float y, float z);
        Vector3();
        void normalize();
        float dot(const Vector3& o) const { return x*o.x + y*o.y + z*o.z; };
        Vector3 operator-(const Vector3& o) const { return {x-o.x, y-o.y, z-o.z}; };


};

#endif