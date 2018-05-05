/*
   TDT4102, Spring 2017
   C++ Asteroid example using SFML

   Any questions, please email me at:
   fredrikpei@gmail.com
   */

#include "gameobjects.h"
#include "graphics.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <memory>
#include <thread>
#include <chrono>
#include <cassert>


using namespace std;


int main() {
  GameObject::width = WindowWidth;
  GameObject::height = WindowHeight;

  srand(time(NULL));

  Window window{};

  int score = 0;

  // Vector of pointers to all game objects
  vector<unique_ptr<GameObject>> gameObjects;

  gameObjects.push_back(make_unique<Spaceship>());


  unsigned c = 0;
  bool gameOver = false;
  try {

  while (window.isOpen()) {
    c++;
    if (c % 64 == 0) {  // Adds a random asteroid every 8th iteration
      int size = rand() % 3;
      int startX = -50 + (rand() % 2) * (WindowWidth + 100);
      int startY = -50 + (rand() % 2) * (WindowHeight + 100);
      gameObjects.push_back(make_unique<Asteroid>(size, startX, startY));
    }

    sf::Event event{};

    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          {
          window.close();
          break;
          }
        case sf::Event::KeyPressed:
          {
          if (event.key.code == sf::Keyboard::Key::Escape) {
            window.close();
          } else if (event.key.code == sf::Keyboard::Key::Space) {

            auto spawn = gameObjects[0]->spawnObject();  // Spaceship is allways at index 0
            assert(gameObjects[0]->getType() == SPACESHIP);
            assert(spawn != nullptr);
            gameObjects.push_back(move(spawn));
          }
          else if (event.key.code == sf::Keyboard::Key::Return) {  // Begin anew
            gameObjects.clear();
            gameObjects.push_back(make_unique<Spaceship>());
            score = 0;
            window.formatScoreText();
            gameOver = false;
          }

          if (event.key.code == sf::Keyboard::Key::Left) {
            //rotate left
            gameObjects[0]->rotateLeft();
          }
          else if (event.key.code == sf::Keyboard::Key::Right) {
            //rotate right
            gameObjects[0]->rotateRight();
          }
          //go forward
          if (event.key.code == sf::Keyboard::Key::Up) {
            gameObjects[0]->goStraight();
          }
          break;
          }
        default:
          break;
      }
    }

    vector<unique_ptr<Asteroid>> newAsteroids;
    for (auto& obj : gameObjects) {
      obj->update();
      for (auto& rhs : gameObjects) {
        assert(rhs != nullptr && obj != nullptr);
        if (obj != rhs && obj->collision(*rhs)) {
          if (obj->getType() == SPACESHIP || rhs->getType() == SPACESHIP) {
            //gameOver = true;
          } else if (obj->getType() == BULLET || rhs->getType() == BULLET) {
            if (obj->getType() == SPACESHIP || rhs->getType() == SPACESHIP) {
              continue;
            }
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
        window.drawGameObject(obj.get());
      }
      window.setText(std::to_string(score));
    } else {
      window.formatEndText();
      window.setText("Game Over!");
    }
    window.updateFrame();
  }
  }
  catch (...) {
    std::cout << "Exception caught!\n";
    return -1;
  }

  return 0;
}
