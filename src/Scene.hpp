#pragma once
#include "VisibleShape.hpp"
#include "Camera.hpp"
#include <vector>
class Camera;
class Scene {
    unsigned int nbShape = 0;
    unsigned int nbCamera = 0;
    unsigned int nbLight = 0;
    std::vector<std::vector<VisibleShape*>*> shapes;
    std::vector<Camera> cameras;
};