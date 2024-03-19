class Vec3f {
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float& operator[](const int n) {
        return *(((float*)this)+n);
    };
    const Vec3f operator+(Vec3f V) {
        return Vec3f(x + V.x, y + V.y, z + V.z);
    };
    const Vec3f operator-(const Vec3f V) {
        return Vec3f(x - V.x, y - V.y, z - V.z);
    };
    const Vec3f operator*(const Vec3f V) {
        return Vec3f(y*V.z - z*V.y, z*V.x - x*V.z, x*V.y - y*V.x);
    };
    const Vec3f operator*(const float s) {
        return Vec3f(s*x, s*y, s*z);
    }
    Vec3f operator=(const float point[3]) {
        (*this)[0] = point[0], (*this)[1] = point[1], (*this)[2] = point[2];
        return *this;
    }
    Vec3f(const float x, const float y, const float z) {
        this->x = x; this->y = y; this->z = z;
    };
};

class Sphere {
    public:
    float radius = 1.0f;
    Vec3f pos = {0.0f, 0.0f, 0.0f};
};

class Ray {
    public:
    Vec3f pos = {0.0f, 0.0f, 0.0f};
    Vec3f dir = {1.0f, 0.0f, 0.0f};
};