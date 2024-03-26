#pragma once
#include "Geometry.hpp"
#include <cmath>
#include "Scene.hpp"
class Camera {
public:
    geom::Vec3f pos = {-2.0f, 0.0f, 0.0f};
    geom::Vec3f dir = {1.0f, 0.0f, 0.0f};
    geom::Vec3f up = {0.0f, 0.0f, 1.0f};
    float dist = 1.0f;
    float fovW = M_PI_2;
    unsigned int width = 1920;
    unsigned int height = 1080;
    float fovH = fovW * height / width;
    unsigned char* pixBuffer;
    char* zBuffer;
    geom::Sphere scene;
    void processImage();
private:
    geom::Vec3f right = {0.0f, 1.0f, 0.0f};
    static geom::Vec3f getNormal(const geom::Sphere sphere,const geom::Vec3f point);
    static geom::Vec3f bounceDir(const geom::Vec3f dir, const geom::Vec3f normal);
};