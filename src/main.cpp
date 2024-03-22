#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "Geometry.hpp"
#include "Camera.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");

    Camera camera;
    camera.width = 1000;
    camera.height = 1000;
    camera.scene.pos = {-0.0f, 0.5f, 0.0f};
    camera.scene.radius = 1.0f;
    camera.pixBuffer = new unsigned char[4 * camera.width * camera.height];
    camera.processImage();
    sf::Texture texture;
    texture.create(1000,1000);
    texture.update((sf::Uint8*)camera.pixBuffer);
    sf::Sprite sprite(texture,sf::IntRect(0,0,1000,1000));

    /*sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    geom::Sphere ball;
    ball.radius = 1;
    ball.pos = {0.0f, 0.0f, 0.0f};
    geom::Ray ray;
    ray.pos = {-2.0f, 0.0f, 0.0f};
    ray.dir = {1.0f, 0.0f, 0.0f};*/

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
        camera.scene.pos.x += .001;
        texture.update((sf::Uint8*)camera.pixBuffer);
        camera.processImage();
    }

    return 0;
}