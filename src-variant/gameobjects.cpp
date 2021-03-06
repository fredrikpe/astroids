
#include "gameobjects.h"
#include "common.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

const double SpaceshipVelocity = 240.0;
const double SpaceshipRotationSpeed = 200.0;
const double BulletVelocity = 300.0;
const double AsteroidVelocity = 25.0;

Spaceship::Spaceship() {
  assert(WindowWidth != 0);
  form.pos_x = WindowWidth / 2;
  form.pos_y = WindowHeight / 2;
  form.radius = 20;
  form.velocity = SpaceshipVelocity;
  rotationSpeed = SpaceshipRotationSpeed;
}

std::optional<GameObject> Spaceship::shoot() {
  if (last_shot > 0) {
    return std::nullopt;
  }
  last_shot = 10;

  double angle = form.rotation / 180.0 * PI;
  double x = form.pos_x + 35 * sin(angle);
  double y = form.pos_y - 35 * cos(angle);

  return GameObject(Bullet{form.rotation, x, y});
}

std::optional<GameObject> Spaceship::update(const Actions &actions,
                                              double dt) {
  last_shot--;

  if (actions.moveStraight) {
    form.goStraight(dt);
  }
  if (actions.rotateLeft) {
    form.rotate(rotationSpeed, dt);
  }
  if (actions.rotateRight) {
    form.rotate(-rotationSpeed, dt);
  }
  if (actions.shoot) {
    return shoot();
  }
  return std::nullopt;
}

Bullet::Bullet(double rotation, double x, double y) {
  form.pos_x = x;
  form.pos_y = y;
  form.rotation = rotation;
  form.velocity = BulletVelocity;
  form.radius = 2;
}

std::optional<GameObject> Bullet::update(const Actions &, double dt) {
  if (isOutside()) {
    deleted = true;
  }
  form.goStraight(dt);
  return std::nullopt;
}

bool Bullet::isOutside() const {
  return form.pos_x > WindowWidth || form.pos_x < 0 ||
         form.pos_y > WindowWidth || form.pos_y < 0;
}

Asteroid Asteroid::randomAsteroid() {
  int size = rand() % 3;
  int side = rand() % 4;

  double pos_x = -50;
  double pos_y = -50;

  switch (side) {
  case 0:
    pos_x = WindowWidth + 50;
    pos_y = -50 + (rand() % 11) * (WindowHeight / 10);
    break;
  case 1:
    pos_x = -50 + (rand() % 11) * (WindowWidth / 10);
    pos_y = WindowHeight + 50;
    break;
  case 2:
    pos_y = -50 + (rand() % 11) * (WindowHeight / 10);
    break;
  case 3:
    pos_x = -50 + (rand() % 11) * (WindowWidth / 10);
    break;
  default:
    assert(false);
  }

  return Asteroid(size, pos_x, pos_y);
}

std::optional<GameObject> Asteroid::update(const Actions &, double dt) {
  if (isOutside()) {
    deleted = true;
  }
  form.goStraight(dt);
  return std::nullopt;
}

// Protip: use a picture of your enemy to make the game more fun
//  : sprite(constructSprite("res/asteroid.png", 1 * 0.1 * (size + 2));
Asteroid::Asteroid(int _size, double x, double y) {
  size = _size;
  form.pos_x = x;
  form.pos_y = y;

  if (size < 0)
    deleted = true;

  form.rotation = rand() % 360;

  form.velocity = (3 - size) * AsteroidVelocity;
  form.radius = (size + 2) * 8;
}

bool Asteroid::isOutside() const {
  return form.pos_x > WindowWidth + 100 || form.pos_x < -100 ||
         form.pos_y > 100 + WindowHeight || form.pos_y < -100;
}

void setDeleted(GameObject &o1) {
  std::visit([](auto&& o) { o.deleted = true; }, o1);
}

bool isDeleted(const GameObject &o1) {
  return std::visit([](auto&& o) { return o.deleted; }, o1);
}

void collision(GameObject &o1, GameObject &o2) {
  const auto o1form = std::visit([](auto&& o) { return o.form; }, o1);
  const auto o2form = std::visit([](auto&& o) { return o.form; }, o2);
  if (o1form.isCollision(o2form)) {
    setDeleted(o1);
    setDeleted(o2);
  }
}


