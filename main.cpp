#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <random>


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PARTICLE_COUNT = 50;
const float PARTICLE_SIZE = 3.0f;
const float GRAVITY = 50.0f; // Pixels per second squared
const float MAX_LIFE = 2.0f; // Seconds

std::random_device rd;
std::mt19937 generator(rd());
std::uniform_real_distribution<float> angleDist(0.0f, 360.0f);
std::uniform_real_distribution<float> speedDist(100.0f, 300.0f);


struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float life;
    sf::Color baseColor;
    sf::CircleShape shape;

    Particle(sf::Vector2f pos, sf::Color color)
        : position(pos), baseColor(color), life(MAX_LIFE) {

        float angle_deg = angleDist(generator);
        float angle_rad = angle_deg * (float)M_PI / 180.0f;
        float speed = speedDist(generator);

        velocity = sf::Vector2f(std::cos(angle_rad) * speed, std::sin(angle_rad) * speed);

        shape.setRadius(PARTICLE_SIZE);
        shape.setFillColor(baseColor);
        shape.setOrigin(PARTICLE_SIZE, PARTICLE_SIZE);
        shape.setPosition(position);
    }


    void update(float dt) {
        velocity.y += GRAVITY * dt;

        position += velocity * dt;

        life -= dt;

        shape.setPosition(position);

        float alpha = 255.0f * (life / MAX_LIFE);
        shape.setFillColor(sf::Color(
            baseColor.r,
            baseColor.g,
            baseColor.b,
            static_cast<sf::Uint8>(std::max(0.0f, alpha))
        ));
    }


    bool isAlive() const {
        return life > 0;
    }
};


void spawnParticleBurst(std::vector<Particle>& particles, sf::Vector2f position, int count) {
    sf::Color colors[] = {
        sf::Color(255, 100, 50),
        sf::Color(255, 180, 0),
        sf::Color(255, 255, 150)
    };
    int num_colors = sizeof(colors) / sizeof(colors[0]);

    std::uniform_int_distribution<int> colorIndexDist(0, num_colors - 1);

    for (int i = 0; i < count; ++i) {
        int color_index = colorIndexDist(generator);
        particles.emplace_back(position, colors[color_index]);
    }
}


int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Particle Simulator", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    std::vector<Particle> particles;
    sf::Clock clock;


    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    spawnParticleBurst(particles, mousePos, PARTICLE_COUNT);
                }
            }
        }


        for (auto it = particles.begin(); it != particles.end(); ) {
            it->update(dt);
            if (!it->isAlive() || it->position.y > WINDOW_HEIGHT) {
                it = particles.erase(it);
            } else {
                ++it;
            }
        }

        window.clear(sf::Color(20, 20, 30));

        for (const auto& particle : particles) {
            window.draw(particle.shape);
        }

        window.display();
    }

    return 0;
}