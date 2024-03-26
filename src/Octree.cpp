#include "Octree.hpp"

// Bitmask telling if the point is in front or
// behind of each face, with 0 being on the
// positive side, and 1 on the negative side.
// The bit as a value of 0 if the point is on
// the plane. The bitmask is of the form:
// xl yl zl xh yh zh
// (with l for the low-coordonates of the cube,
// and h for the high-coordonates of the cube).
unsigned int pointPosForCubeBitmap(const geom::Vec3f &point, const geom::Vec3f &minCoordCube, const geom::Vec3f &maxCoordCube)
{
    geom::Vec3f deltaLow = minCoordCube - point;
    geom::Vec3f deltaHigh = maxCoordCube - point;
    unsigned int bitmask = (std::signbit(deltaLow.x) << 5) +
                           (std::signbit(deltaLow.y) << 4) +
                           (std::signbit(deltaLow.z) << 3) +
                           (std::signbit(deltaHigh.x) << 2) +
                           (std::signbit(deltaHigh.y) << 1) +
                           (int)std::signbit(deltaHigh.z);
    return bitmask;
}

bool isInSquare(const float &x, const float &y, const float &xm, const float &ym, const float &xM, const float &yM)
{
    return (x >= xm) && (x < xM) && (y >= ym) && (y < yM);
}

bool passInXPFace(const geom::Ray &ray, const geom::Vec3f &minCube, const geom::Vec3f &maxCube)
{
    float d = (maxCube.x - ray.pos.x) / ray.dir.x;
    float y = ray.pos.y + d * ray.dir.y;
    float z = ray.pos.z + d * ray.dir.z;
    return (!signbit(d)) && isInSquare(y, z, minCube.y, minCube.z, maxCube.y, maxCube.z);
}
bool passInYPFace(const geom::Ray &ray, const geom::Vec3f &minCube, const geom::Vec3f &maxCube)
{
    float d = (maxCube.y - ray.pos.y) / ray.dir.y;
    float x = ray.pos.x + d * ray.dir.x;
    float z = ray.pos.z + d * ray.dir.z;
    return (!signbit(d)) && isInSquare(x, z, minCube.x, minCube.z, maxCube.x, maxCube.z);
}
bool passInZPFace(const geom::Ray &ray, const geom::Vec3f &minCube, const geom::Vec3f &maxCube)
{
    float d = (maxCube.z - ray.pos.z) / ray.dir.z;
    float x = ray.pos.x + d * ray.dir.x;
    float y = ray.pos.y + d * ray.dir.y;
    return (!signbit(d)) && isInSquare(x, y, minCube.x, minCube.y, maxCube.x, maxCube.y);
}
bool passInXNFace(const geom::Ray &ray, const geom::Vec3f &minCube, const geom::Vec3f &maxCube)
{
    float d = (minCube.x - ray.pos.x) / ray.dir.x;
    float y = ray.pos.y + d * ray.dir.y;
    float z = ray.pos.z + d * ray.dir.z;
    return (!signbit(d)) && isInSquare(y, z, minCube.y, minCube.z, maxCube.y, maxCube.z);
}
bool passInYNFace(const geom::Ray &ray, const geom::Vec3f &minCube, const geom::Vec3f &maxCube)
{
    float d = (minCube.y - ray.pos.y) / ray.dir.y;
    float x = ray.pos.x + d * ray.dir.x;
    float z = ray.pos.z + d * ray.dir.z;
    return (!signbit(d)) && isInSquare(x, z, minCube.x, minCube.z, maxCube.x, maxCube.z);
}
bool passInZNFace(const geom::Ray &ray, const geom::Vec3f &minCube, const geom::Vec3f &maxCube)
{
    float d = (minCube.z - ray.pos.z) / ray.dir.z;
    float x = ray.pos.x + d * ray.dir.x;
    float y = ray.pos.y + d * ray.dir.y;
    return (!signbit(d)) && isInSquare(x, y, minCube.x, minCube.y, maxCube.x, maxCube.y);
}

bool intersectCube(geom::Ray ray, geom::Vec3f minCube, geom::Vec3f maxCube)
{
    unsigned int bitmask = pointPosForCubeBitmap(ray.pos, minCube, maxCube);
    float c, x, y, z;
    switch (bitmask)
    {
        /// 3 coord inside cube ///

    case 0b000111:
        return true;

        /// 2 coord inside cube ///

    // x on positive side of the cube, y and z inside cube coord
    case 0b000011:
        return passInXPFace(ray, minCube, maxCube);
    // y on positive side of the cube, x and z inside cube coord
    case 0b000101:
        return passInYPFace(ray, minCube, maxCube);
    // z on positive side of the cube, x and y inside cube coord
    case 0b000110:
        return passInZPFace(ray, minCube, maxCube);
    // x on negative side of the cube, y and z inside cube coord
    case 0b100111:
        return passInXNFace(ray, minCube, maxCube);
    // y on negative side of the cube, x and z inside cube coord
    case 0b010111:
        return passInYNFace(ray, minCube, maxCube);
    // z on negative side of the cube, x and y inside cube coord
    case 0b001111:
        return passInZNFace(ray, minCube, maxCube);

        /// 1 coord inside cube ///

    // x and y on positive side of the cube, z inside cube coord
    case 0b000001:
        return passInXPFace(ray, minCube, maxCube) || passInYPFace(ray, minCube, maxCube);
    // x and z on positive side of the cube, y inside cube coord
    case 0b000010:
        return passInXPFace(ray, minCube, maxCube) || passInZPFace(ray, minCube, maxCube);
    // y and z on positive side of the cube, x inside cube coord
    case 0b000100:
        return passInYPFace(ray, minCube, maxCube) || passInZPFace(ray, minCube, maxCube);
    // x and y on negative side of the cube, z inside cube coord
    case 0b110111:
        return passInXNFace(ray, minCube, maxCube) || passInYNFace(ray, minCube, maxCube);
    // x and z on negative side of the cube, y inside cube coord
    case 0b101111:
        return passInXNFace(ray, minCube, maxCube) || passInZNFace(ray, minCube, maxCube);
    // y and z on negative side of the cube, x inside cube coord
    case 0b011111:
        return passInYNFace(ray, minCube, maxCube) || passInZNFace(ray, minCube, maxCube);

        /// 0 coord inside cube ///

    // x, y and z on positive side of the cube
    case 0b000000:
        return passInXPFace(ray, minCube, maxCube) || passInYPFace(ray, minCube, maxCube) || passInZPFace(ray, minCube, maxCube);
    // x and y on positive side of the cube, z on negative side of the cube
    case 0b001001:
        return passInXPFace(ray, minCube, maxCube) || passInYPFace(ray, minCube, maxCube) || passInZNFace(ray, minCube, maxCube);
    // x and z on positive side of the cube, y on negative side of the cube
    case 0b010010:
        return passInXPFace(ray, minCube, maxCube) || passInYNFace(ray, minCube, maxCube) || passInZPFace(ray, minCube, maxCube);
    // y and z on positive side of the cube, x on negative side of the cube
    case 0b100100:
        return passInXNFace(ray, minCube, maxCube) || passInYPFace(ray, minCube, maxCube) || passInZPFace(ray, minCube, maxCube);
    // x on positive side of the cube, y and z on negative side of the cube
    case 0b011011:
        return passInXPFace(ray, minCube, maxCube) || passInYNFace(ray, minCube, maxCube) || passInZNFace(ray, minCube, maxCube);
    // y on positive side of the cube, x and z on negative side of the cube
    case 0b101101:
        return passInXNFace(ray, minCube, maxCube) || passInYPFace(ray, minCube, maxCube) || passInZNFace(ray, minCube, maxCube);
    // z on positive side of the cube, x and y on negative side of the cube
    case 0b110110:
        return passInXNFace(ray, minCube, maxCube) || passInYNFace(ray, minCube, maxCube) || passInZPFace(ray, minCube, maxCube);

    // transitivity of inequation is not respected
    default:
        return false;
    }
}

template <class T>
T *Octree<T>::getNext(const geom::Ray &ray)
{
    static std::size_t index = 0;
    static const geom::Ray *lastRay = nullptr; // pointer to constant Ray (pointer itself is not const)

    // if the ray is new, we reset
    bool isNew = &ray != lastRay;
    if (isNew)
    {
        lastRay = &ray;
    }
    if (childs[0] == nullptr)
    {
        return getNextNoChild(ray, isNew);
    }
    return getNextHasChild(ray, isNew);
}

template <class T>
T *Octree<T>::getNextNoChild(const geom::Ray &ray, const bool &isNew)
{
    static std::size_t index = 0;
    if (isNew)
    {
        index = 0;
    }
    return index < objects.size() ? objects.at(index++) : nullptr;
}

typedef union
{
    unsigned char byte[4];
} byteAr4;

// mask from boolean, use with &
unsigned char mFB[2] = {0x00, 0xff};

byteAr4 passOrder(const unsigned int &mask, const bool (&intersect)[8])
{
    byteAr4 bytes;
    unsigned char *byte = bytes.byte;
    unsigned char masks[8] = {
        (unsigned char)(mFB[intersect[0b000]] & 0b11111000),
        (unsigned char)(mFB[intersect[0b001]] & 0b11111001),
        (unsigned char)(mFB[intersect[0b010]] & 0b11111010),
        (unsigned char)(mFB[intersect[0b011]] & 0b11111011),
        (unsigned char)(mFB[intersect[0b100]] & 0b11111100),
        (unsigned char)(mFB[intersect[0b101]] & 0b11111101),
        (unsigned char)(mFB[intersect[0b110]] & 0b11111110),
        (unsigned char)(mFB[intersect[0b111]] & 0b11111111)};
    byte[0] = masks[mask ^ 0b111];
    byte[1] = masks[mask ^ 0b011] | masks[mask ^ 0b101] | masks[mask ^ 0b110];
    byte[2] = masks[mask ^ 0b001] | masks[mask ^ 0b010] | masks[mask ^ 0b100];
    byte[3] = masks[mask ^ 0b000];
    return bytes;
}

template <class T>
T *Octree<T>::getNextHasChild(const geom::Ray &ray, const bool &isNew)
{
    float &l = length;
    static geom::Vec3f cubes[8][2] = {
        {center - geom::Vec3f({l, l, l}), center},
        {center - geom::Vec3f({l, l, 0.0f}), center + geom::Vec3f({0.0f, 0.0f, l})},
        {center - geom::Vec3f({l, 0.0f, l}), center + geom::Vec3f({0.0f, l, 0.0f})},
        {center - geom::Vec3f({l, 0.0f, 0.0f}), center + geom::Vec3f({0.0f, l, l})},
        {center - geom::Vec3f({0.0f, l, l}), center + geom::Vec3f({l, 0.0f, 0.0})},
        {center - geom::Vec3f({0.0f, l, 0.0f}), center + geom::Vec3f({l, 0.0f, l})},
        {center - geom::Vec3f({0.0f, 0.0f, l}), center + geom::Vec3f({l, l, 0.0f})},
        {center - geom::Vec3f({0.0f, 0.0f, 0.0f}), center + geom::Vec3f({l, l, l})}};
    static std::size_t nbPass = 0;
    unsigned char cubePassed[4] = {0xff, 0xff, 0xff, 0xff};
    if (isNew)
    {
        bool intersect[8] = {false, false, false, false, false, false, false, false};
        for (std::size_t i = 0; i < 8; i++)
        {
            intersect[i] = intersectCube(ray, cubes[i][0], cubes[i][1]);
        }
        unsigned int mask = (std::signbit(ray.dir.x) << 2) + (std::signbit(ray.dir.y) << 1) + (unsigned int)std::signbit(ray.dir.z);
        byteAr4 pass = passOrder(mask, intersect);
        *(byteAr4*)&cubePassed = pass;
        nbPass = ((cubePassed[0] & 0xf0) >> 7) + ((cubePassed[1] & 0xf0) >> 7) + ((cubePassed[2] & 0xf0) >> 7) + ((cubePassed[3] & 0xf0) >> 7);
        while(!(cubePassed[0] & 0xf0) >> 7) {
            (*(int*)&cubePassed) = (*(int*)&cubePassed) << 8;
        }
    }
    if (nbPass == 0) {
        return nullptr;
    }
    nbPass--;
    unsigned char nextCube = cubePassed[0] & 0b111;
    (*(int*)&cubePassed) = (*(int*)&cubePassed) << 8;
    return childs[nextCube]->getNext(ray);
}