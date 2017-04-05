/*
TDT4102, Spring 2017
C++ Asteroid example using SFML

Any questions, please email me at:
fredrikpei@gmail.com
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <memory>

#include "gameobjects.h"

using namespace std;

int main() {
    srand(time(NULL));
    
	int width = 1000, height = 800;
    GameObject::width = width;
    GameObject::height = height;

	string windowName = "Spaceship";
	sf::RenderWindow window(sf::VideoMode(width, height), windowName);
    sf::Font font;
    if (!font.loadFromFile("OpenSans-Bold.ttf")) {
        cout << "Couldn't open font!\n";
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setStyle(sf::Text::Bold);
    text.setColor(sf::Color::White);
    text.setPosition(50,50);

    int score = 0;
    
    // Vector of pointers to all game objects
    vector<unique_ptr<GameObject>> gameObjects;

    gameObjects.push_back(make_unique<Spaceship>());

    unsigned c = 0;
    bool gameOver = false;

	while (window.isOpen()) {
        c++;
        if (c % 8 == 0) {  // Adds a random asteroid every 8th iteration
            int size = rand() % 3;
            int startX = -50 + (rand() % 2) * (width + 100);
            int startY = -50 + (rand() % 2) * (height + 100);
            gameObjects.push_back(make_unique<Asteroid>(size, sf::Vector2f(startX, startY)));
        }
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Key::Escape)
					window.close();
                else if (event.key.code == sf::Keyboard::Key::Space) {
                    auto spawn = gameObjects[0]->spawnObject();  // Spaceship is allways at index 0
                    gameObjects.push_back(move(spawn));
                }
                else if (event.key.code == sf::Keyboard::Key::Return) {  // Begin anew
                    gameObjects.clear();
                    gameObjects.push_back(make_unique<Spaceship>());
                    score = 0;
                    text.setCharacterSize(20);
                    text.setColor(sf::Color::White);
                    text.setPosition(50,50);
                    gameOver = false;
                }
                break;
			}
		}

        vector<unique_ptr<Asteroid>> newAsteroids;
        for (auto& obj : gameObjects) {
            obj->update();
            for (auto& rhs : gameObjects) {
                if (obj != rhs && obj->collision(*rhs)) {
                    if (obj->getType() == SPACESHIP || rhs->getType() == SPACESHIP) {
                        gameOver = true;
                    } else if (obj->getType() == BULLET || rhs->getType() == BULLET) {
                        score += 1;
                        obj->deleteMe = true;
                        rhs->deleteMe = true;
                        if (obj->getType() == ASTEROID) {
                            gameObjects.push_back(obj->spawnObject());
                            gameObjects.push_back(obj->spawnObject());
                        }
                        if (rhs->getType() == ASTEROID) {
                            gameObjects.push_back(rhs->spawnObject());
                            gameObjects.push_back(rhs->spawnObject());
                        }
                    }
                    break;
                }
            }
        }

        for (auto& a : newAsteroids) {
            gameObjects.push_back(move(a));
        }

        auto it = gameObjects.begin();
        while (it != gameObjects.end()) {
            if ((*it)->deleteMe) {
                it = gameObjects.erase(it);
            } else {
                ++it;
            }
        }

        if (!gameOver) {
            window.clear();
            for (const auto& obj : gameObjects) {
                window.draw(obj->getSprite());
            }
            text.setString(to_string(score));
        } else {
            text.setString("Game Over!");
            text.setCharacterSize(80);
            text.setColor(sf::Color::Green);
            text.setPosition(170,300);
        }
        window.draw(text);
        window.display();
	}

	return 0;
}
