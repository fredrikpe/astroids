/*
   TDT4102, Spring 2017
   C++ Asteroid example using SFML

   Any questions, please email me at:
   fredrikpei@gmail.com
   */

#include "gameobjects.h"
#include "graphics.h"
#include "timer.h"

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

struct AsteroidsGame {
  AsteroidsGame() { objects.push_back(Spaceship{}); }

  void updatePositions(Actions actions, double dt) {
    auto size = objects.size();
    for (size_t i = 0; i < size; i++) {
      auto spawn = objects[i].update(actions, dt);
      if (spawn != nullptr) {
        objects.push_back(std::move(*spawn));
      }
    }
  }

  void checkCollisions() {
    auto size = objects.size();
    for (size_t i = 0; i < size; i++) {
      for (size_t j = i + 1; j < size; j++) {
        objects[i].collision(objects[j]);
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
  bool game_over = false;
};

int main() {
  srand(time(NULL));

  AsteroidsGame game{};
  Gui gui{};
  FrameTimer timer{};

  try {
    while (gui.isOpen()) {

      timer.updateFrameTime();

      game.objects.push_back(Asteroid::randomAsteroid());

      Actions actions = gui.parseInput();

      while (timer.frame_time > 0) {
        game.updatePositions(actions, timer.dt);

        game.checkCollisions();

        game.removeDeleted();

        timer.frame_time -= timer.dt;
      }

      if (!game.game_over) {
        gui.render(game.objects);
      }
    }
  } catch (...) {
  }
}
