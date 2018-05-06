/*
   TDT4102, Spring 2017
   C++ Asteroid example using SFML

   Any questions, please email me at:
   fredrikpei@gmail.com
   */

#include "gameobjects.h"
#include "graphics.h"

#include <SFML/Graphics.hpp>

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <time.h>

using namespace std;

struct AsteroidsGame {
  AsteroidsGame() {
    objects.clear();
    objects.push_back(Spaceship{});
    score = 0;
    gameOver = false;
  }

  std::vector<GameObject> objects;
  int score = 0;
  bool gameOver = false;
};

// void CheckCollisions(std::vector<GameObject>& objects)

Actions parseActions() {
  return Actions{
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up),
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left),
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right),
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space),
  };
}

void updatePositions(std::vector<GameObject>& objects) {
  auto size = objects.size();
  for (size_t i=0; i<size; i++) {
    auto spawn = objects[i].update(parseActions());
    if (spawn != nullptr) {
      objects.push_back(std::move(*spawn));
    }
  }
}

void checkCollisions(std::vector<GameObject>& objects) {
  auto size = objects.size();
  for (size_t i=0; i<size; i++) {
    for (size_t j=i + 1; j<size; j++) {
      auto spawn = objects[i].collision(objects[j]);
      if (spawn != nullptr) {
        objects.push_back(std::move(*spawn));
      }
    }
  }
}


int main() {
  srand(time(NULL));

  AsteroidsGame game{};
  Window window{};

  int tick = 0;

  for (; window.isOpen(); tick++) {

    sf::Event event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return)) {
        window.formatScoreText();
        game = AsteroidsGame{};
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        window.close();
      }
    }

    updatePositions(game.objects);

    if (tick % 16 == 0) {
      game.objects.push_back(Asteroid::randomAsteroid());
      if (tick % 32 == 0) {
        checkCollisions(game.objects);
      }
    }

    // Remove deleted objects
    game.objects.erase(
        std::remove_if(game.objects.begin(), game.objects.end(),
          [](const auto &object) { return object.isDeleted(); }),
        game.objects.end());

    if (!game.gameOver) {
      window.clear();
      for (const auto &obj : game.objects) {
        window.drawGameObject(obj);
      }
      window.setText(std::to_string(game.score));
    } else {
      window.formatEndText();
      window.setText("Game Over!");
    }
    window.updateFrame();
  }

  return 0;
}
