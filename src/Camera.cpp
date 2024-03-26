#include "Camera.hpp"
float lerp(float a, float b, float t) {
    return a + t*(b - a);
}
void Camera::processImage() {
    geom::Vec3f lightDir = {-0.267261241912f, 0.534522483825f, -0.801783725737f};
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
            ray.dir = ray.pos - pos;
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
                brightness = (dirBounce * lightDir + 1) / 2 + (dirBounce * geom::Vec3f({.0f, .0f, 1.0f}) + 1) / 2;
                pixBuffer[4 * (w + h * width)] = 255 * brightness ;
                pixBuffer[4 * (w + h * width)+1] = .0 * brightness;
                pixBuffer[4 * (w + h * width)+2] = 128 * brightness ;
                pixBuffer[4 * (w + h * width)+3] = alpha;
                break;
            case 2:
                dirBounce = bounceDir(ray.dir,getNormal(scene,p1));
                brightness1 = (dirBounce * lightDir + 1) / 2 + (dirBounce * geom::Vec3f({.0f, .0f, 1.0f}) + 1) / 2;
                dirBounce = bounceDir(ray.dir,getNormal(scene,p2));
                brightness2 = (dirBounce * lightDir + 1) / 2 + (dirBounce * geom::Vec3f({.0f, .0f, 1.0f}) + 1) / 2;
                alphaN = 2 * alpha - alpha * alpha / 255;
                pixBuffer[4 * (w + h * width)] = alpha * 255 * (brightness1 + brightness2 * (1.0f - alpha / 255.0f)) / 255;
                pixBuffer[4 * (w + h * width)+1] = alpha * 0 * (brightness1 + brightness2 * (1.0f - alpha / 255.0f)) / 255;
                pixBuffer[4 * (w + h * width)+2] = alpha * 128 * (brightness1 + brightness2 * (1.0f - alpha / 255.0f)) / 255;
                pixBuffer[4 * (w + h * width)+3] = alphaN;
                break;
            default:
                pixBuffer[4 * (w + h * width)] = 0;
                pixBuffer[4 * (w + h * width)+1] = 0;
                pixBuffer[4 * (w + h * width)+2] = 0;
                pixBuffer[4 * (w + h * width)+3] = 255;
                break;
            }
            if (nbPoints > 2){
                pixBuffer[4 * (w + h * width)] = std::min(pixBuffer[4 * (w + h * width)] + 50, 255);
                pixBuffer[4 * (w + h * width)+1] = std::min(pixBuffer[4 * (w + h * width)+1] + 50, 255);
                pixBuffer[4 * (w + h * width)+2] = std::min(pixBuffer[4 * (w + h * width)+2] + 50, 255);
            }
        }
    }
}

geom::Vec3f Camera::getNormal(const geom::Sphere sphere,const geom::Vec3f point) {
    geom::Vec3f res = point - sphere.pos;
    res.normalize();
    return res;
}

geom::Vec3f Camera::bounceDir(const geom::Vec3f direction, const geom::Vec3f normal) {
    float k = 2 * (direction * normal);
    return direction - normal * k;
}