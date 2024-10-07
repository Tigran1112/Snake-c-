#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

std::vector<sf::RectangleShape> snake = {};
sf::RectangleShape apple;

int direction = 0;
bool Pause = false;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distX(0, 790);
std::uniform_int_distribution<> distY(0, 590);

void New() {
    sf::RectangleShape zveno;
    zveno.setSize({10, 10});
    zveno.setFillColor(sf::Color(26, 158, 0));
    if (!snake.empty()) {
        zveno.setPosition(snake.back().getPosition());
    } else {
        zveno.setPosition(400, 300);
        zveno.setFillColor(sf::Color(37, 219, 0));
    }
    snake.push_back(zveno);
}

void CreateApple() {
    apple.setSize({10, 10});
    apple.setFillColor(sf::Color::Red);
    apple.setPosition(distX(gen), distY(gen));
}

void Move() {
    if (!snake.empty()) {
        sf::Vector2f prevPos = snake[0].getPosition();

        switch (direction) {
            case 0: prevPos.x += 10; break;
            case 1: prevPos.y += 10; break;
            case 2: prevPos.x -= 10; break;
            case 3: prevPos.y -= 10; break;
        }
        if (prevPos.x < 0 || prevPos.x >= 800 || prevPos.y < 0 || prevPos.y >= 600) {
            std::exit(1);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        for (size_t i = snake.size() - 1; i > 0; --i) {
            snake[i].setPosition(snake[i - 1].getPosition());
        }

        snake[0].setPosition(prevPos);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Snake");

    New();
    New();
    CreateApple();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Постоянно проверяем нажатие клавиши P для паузы
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            Pause = !Pause;
            std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Чтобы избежать слишком частого переключения
        }

        if (!Pause) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction != 1) {
                direction = 3;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction != 0) {
                direction = 2;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction != 3) {
                direction = 1;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction != 2) {
                direction = 0;
            }

            Move();
        }

        window.clear();

        for (const auto& segment : snake) {
            window.draw(segment);
        }

        window.draw(apple);

        window.display();

        if (!Pause && snake[0].getGlobalBounds().intersects(apple.getGlobalBounds())) {
            New();
            CreateApple();
        }

        for (size_t i = 1; i < snake.size(); ++i) {
            if (snake[0].getGlobalBounds().intersects(snake[i].getGlobalBounds())) {
                window.close();
            }
        }
    }

    return 0;
}