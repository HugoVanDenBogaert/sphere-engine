#include "Camera.hpp"
float lerp(float a, float b, float t) {
    return a + t*(b - a);
}
void Camera::processImage() {
    geom::Vec3f lightDir = {0.267261241912f, 0.534522483825f, 0.801783725737f};
    geom::Vec3f centerScreen = pos + dir * dist;
    float Hmax = std::tan(fovH/2)*dist;
    float Hmin = -Hmax;
    float Wmax = std::tan(fovW/2)*dist;
    float Wmin = -Wmax;
    geom::Ray ray;
    for (int h = 0; h < height; h++){
        float y = lerp(Hmin, Hmax, (float) h / (height - 1));
        for (int w = 0; w < width; w++) {
            float x = lerp(Wmin, Wmax, (float) w / (width - 1));
            ray.pos = centerScreen + up * y + right * x;
            ray.dir = pos - ray.pos;
            ray.dir.normalize();
            geom::Vec3f p1 = {0, 0, 0};
            geom::Vec3f p2 = {0, 0, 0};
            unsigned int nbPoints = scene.intersect(ray,p1,p2);
            geom::Vec3f dirBounce = {1, 0, 0};
            float brightness;
            float brightness1;
            float brightness2;
            int alpha = 255;
            int alphaN;
            switch (nbPoints)
            {
            case 1:
                dirBounce = bounceDir(ray.dir,getNormal(scene,p1));
                brightness = std::max(dirBounce * lightDir, 0.0f);
                pixBuffer[4 * (w + h * width)] = 255 * brightness;
                pixBuffer[4 * (w + h * width)+1] = 0 * brightness;
                pixBuffer[4 * (w + h * width)+2] = 128 * brightness;
                pixBuffer[4 * (w + h * width)+3] = alpha;
                break;
            case 2:
                dirBounce = bounceDir(ray.dir,getNormal(scene,p1));
                brightness1 = std::max(dirBounce * lightDir, 0.0f);
                dirBounce = bounceDir(ray.dir,getNormal(scene,p2));
                brightness2 = std::max(dirBounce * lightDir, 0.0f);
                alphaN = 2 * alpha - alpha * alpha / 255;
                pixBuffer[4 * (w + h * width)] = alpha * 255 * (brightness1 + brightness2 * (1.0f - alpha / 255.0f)) / 255;
                pixBuffer[4 * (w + h * width)+1] = alpha * 0 * (brightness1 + brightness2 * (1.0f - alpha / 255.0f)) / 255;
                pixBuffer[4 * (w + h * width)+2] = alpha * 128 * (brightness1 + brightness2 * (1.0f - alpha / 255.0f)) / 255;
                pixBuffer[4 * (w + h * width)+3] = alphaN;
                break;
            default:
                pixBuffer[4 * (w + h * width)] = 0;
                pixBuffer[4 * (w + h * width)+1] = 0;
                pixBuffer[4 * (w + h * width)+2] = 255;
                pixBuffer[4 * (w + h * width)+3] = 128;
                break;
            }
        }
    }
}

geom::Vec3f Camera::getNormal(const geom::Sphere sphere,const geom::Vec3f point) {
    geom::Vec3f res = point - sphere.pos;
    res.normalize();
    return res;
}

geom::Vec3f Camera::bounceDir(const geom::Vec3f dir, const geom::Vec3f normal) {
    bool opposit = signbit(dir * normal);
    geom::Vec3f res = normal * 2 - dir;
    res.normalize();
    return (res * opposit) - (res * (!opposit));
}