#include <SFML/Graphics.hpp>
#include "Geometry.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    Sphere ball;
    ball.radius = 1;
    ball.pos = {0.0f, 0.0f, 0.0f};
    Ray ray;
    ray.pos = {-2.0f, 0.0f, 0.0f};
    ray.dir = {1.0f, 0.0f, 0.0f};

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}