#pragma once
#include "Geometry.hpp"
enum id {sphere, ellipsoid, triangle};
class VisibleShape {
public:
    virtual geom::Sphere boundingSphere() const {return geom::Sphere();};
    virtual geom::Vec3f normal(geom::Vec3f point) const {return point - pos;};
    virtual geom::Vec3f getRotation() const {return rotation;};
    virtual geom::Vec3f getPos() const {return pos;};
    virtual void setRotation(const geom::Vec3f rotation) {this->rotation = rotation;};
    virtual void setPosition(const geom::Vec3f point) {pos = point;};
    virtual void rotate(const geom::Vec3f rotation) {this->rotation = this->rotation + rotation;};
    virtual void translate(const geom::Vec3f vector) {pos = pos + vector;};
protected:
    id shapeId;
    geom::Vec3f rotation = {.0f, .0f, .0f};
    geom::Vec3f pos = {.0f, .0f, .0f};
};

class Sphere : public VisibleShape {
private:
    float radius = 1.0f;
public:
    Sphere() {shapeId = id::sphere;};
    geom::Sphere boundingSphere() const {return geom::Sphere(radius, VisibleShape::pos);};
};