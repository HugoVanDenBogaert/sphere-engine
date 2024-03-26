#pragma once
#include <vector>
#include "Geometry.hpp"

template<class T>
class Octree {
private:
    T* getNextNoChild(const geom::Ray &ray, const bool &isNew);
    T* getNextHasChild(const geom::Ray &ray, const bool &isNew);
    int lvl = 0;
    Octree* childs[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    geom::Vec3f center = {.0f, .0f, .0f};
    float length = 1024.0f;
    std::vector<T*> objects;
public:
    void add(T* object);
    void del(T* object);
    void mov(T* object);
    T* getNext(const geom::Ray &ray);
};