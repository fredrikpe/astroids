/*
 * TDT4102, Spring 2017
 * C++ Asteroid example using SFML
 *
 * Any questions, please email me at:
 *   fredrikpei@gmail.com
 */

#pragma once

#include "common.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <variant>
#include <optional>

#define PI 3.14159265;

struct PhysicsForm {
  double pos_x = 0;
  double pos_y = 0;
  double rotation = 0;
  double velocity = 0;
  double radius = 0;

  bool isCollision(const PhysicsForm &other) const {
    double distance =
        sqrt(pow(pos_x - other.pos_x, 2) + pow(pos_y - other.pos_y, 2));
    return distance < radius + other.radius;
  }

  void goStraight(double dt) {
    double angle = rotation / 180.0 * PI;
    pos_x += velocity * sin(angle) * dt;
    pos_y += -velocity * cos(angle) * dt;
  }

  void rotate(double angle, double dt) { rotation += angle * dt; }
};

class Spaceship;
class Bullet;
class Asteroid;

using GameObject = std::variant<Spaceship, Bullet, Asteroid>;


class Spaceship {
public:
  Spaceship();

  std::optional<GameObject> update(const Actions &actions, double dt);

  bool isOutside(int, int) const { return false; }
  std::optional<GameObject> shoot();

  std::pair<std::string, double> fileAndScaling() const {
    return std::make_pair("res/spaceship.png", 0.5);
  }

  PhysicsForm form;

  bool deleted = false;

private:
  double rotationSpeed = 0.0;
  int last_shot = 0;
};

class Bullet {
public:
  Bullet(double rotation, double x, double y);

  std::optional<GameObject> update(const Actions &, double dt);

  bool isOutside() const;

  std::pair<std::string, double> fileAndScaling() const {
    return std::make_pair("res/bullet.png", 0.05);
  }

  PhysicsForm form;

  bool deleted = false;
};

class Asteroid {
public:
  Asteroid(int size, double x, double y);

  std::optional<GameObject> update(const Actions &, double dt);

  bool isOutside() const;
  std::pair<std::string, double> fileAndScaling() const {
    return std::make_pair("res/asteroid.png", 1 * 0.1 * (size + 2));
  }

  static Asteroid randomAsteroid();

  PhysicsForm form;
  bool deleted = false;

private:
  int size = 0;
};


void setDeleted(GameObject &o1);

bool isDeleted(const GameObject &o1);

void collision(GameObject &o1, GameObject &o2);

