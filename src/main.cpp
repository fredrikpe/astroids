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


Actions parseInput() {
  return Actions{
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up),
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left),
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right),
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space),
  };
}

struct AsteroidsGame {
  AsteroidsGame() {
    objects.clear();
    objects.push_back(Spaceship{});
    score = 0;
    gameOver = false;
  }

  void updatePositions(Actions actions, double dt) {
    auto size = objects.size();
    for (size_t i=0; i<size; i++) {
      auto spawn = objects[i].update(actions, dt);
      if (spawn != nullptr) {
        objects.push_back(std::move(*spawn));
      }
    }
  }

  void checkCollisions() {
    auto size = objects.size();
    for (size_t i=0; i<size; i++) {
      for (size_t j=i + 1; j<size; j++) {
        auto spawn = objects[i].collision(objects[j]);
        if (spawn != nullptr) {
          score++;
          objects.push_back(std::move(*spawn));
        }
      }
    }
  }

  void removeDeleted() {
    objects.erase(
      std::remove_if(objects.begin(), objects.end(),
        [](const auto &object) { return object.isDeleted(); }),
      objects.end());
  }

  std::vector<GameObject> objects;
  int score = 0;
  bool gameOver = false;
};

void render(const AsteroidsGame& game, Window& window) {
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

int main() {
  srand(time(NULL));

  AsteroidsGame game{};
  Window        window{};

  double       t = 0.0;
  const double dt = 0.01;
  double       accumulator = 0.0;
  auto         current_time = std::chrono::high_resolution_clock::now();
        
  while (window.isOpen()) {
    
    auto new_time = std::chrono::high_resolution_clock::now();
    auto frame_time = std::chrono::duration<double>(
        new_time - current_time).count();

    current_time = new_time;
    accumulator += frame_time;

    game.objects.push_back(Asteroid::randomAsteroid());

    while (accumulator >= dt) {

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

      game.updatePositions(parseInput(), dt);

      game.checkCollisions();
        
      game.removeDeleted();

      accumulator -= dt;
      t += dt;
    }

    render(game, window);
  }
}
