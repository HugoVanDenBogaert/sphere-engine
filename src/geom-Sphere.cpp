#include "Geometry.hpp"

unsigned int geom::Sphere::intersect(const Ray ray, Vec3f &point1, Vec3f &point2) const
{
    // if dir is a unit vector, then a = 1
    float b = ray.dir * (ray.pos - pos) * 2;
    float c = (ray.pos - pos)*(ray.pos - pos) - (radius * radius);
    float delta = b * b - 4 * c;
    float s;
    // no intersection
    if (delta < 0)
        return 0;
    // 1 intersection
    if (delta == 0)
    {
        s = -.5 * b;
        // point is on the opposit direction
        if (s < 0)
            return 0;
        point1 = ray.pos + ray.dir * s;
        return 1;
    }
    c = std::sqrt(delta);
    s = .5 * (-b + c);
    // (furthest) point is on the opposit direction of the ray
    if (s < 0)
    {
        return 0;
    }
    point2 = ray.pos + ray.dir * s;
    s = .5 * (-b - c);
    // (closest) point is on the opposit direction of the ray
    if (s < 0) {
        point1 = point2;
        return 1;
    }
    // 2 intersection
    point1 = ray.pos + ray.dir * s;
    return 2;
}