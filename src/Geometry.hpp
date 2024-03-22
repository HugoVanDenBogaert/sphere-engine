#pragma once
#include <math.h>
namespace geom
{
    class Vec3f
    {
    public:
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float &operator[](const int n)
        {
            return *(((float *)this) + n);
        };
        Vec3f operator+(const Vec3f V) const
        {
            return Vec3f(x + V.x, y + V.y, z + V.z);
        };
        Vec3f operator-(const Vec3f V) const
        {
            return Vec3f(x - V.x, y - V.y, z - V.z);
        };
        float operator*(const Vec3f V) const
        {
            return x*V.x + y*V.y + z*V.z;
        };
        Vec3f operator*(const float s) const
        {
            return Vec3f(s * x, s * y, s * z);
        };
        Vec3f operator^(const Vec3f V) const {
            return Vec3f(y * V.z - z * V.y, z * V.x - x * V.z, x * V.y - y * V.x);
        };
        Vec3f operator=(const float point[3])
        {
            (*this)[0] = point[0], (*this)[1] = point[1], (*this)[2] = point[2];
            return *this;
        };
        void normalize() {
            float norm = std::sqrt(x*x + y*y + z*z);
            x = x / norm;
            y = y / norm;
            z = z / norm;
        };
        Vec3f(const float x, const float y, const float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        };
    };

    class Ray
    {
    public:
        Vec3f pos = {0.0f, 0.0f, 0.0f};
        Vec3f dir = {1.0f, 0.0f, 0.0f};
    };

    class Sphere
    {
    public:
        float radius = 1.0f;
        Vec3f pos = {0.0f, 0.0f, 0.0f};
        unsigned int intersect(const Ray ray, Vec3f &point1, Vec3f &point2) const;
    };
}