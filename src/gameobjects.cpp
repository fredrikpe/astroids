
#include <iostream> 
#include <cstdlib> 
#include <cmath>
#include <memory>
#include <algorithm>
#include <cassert>

#include "gameobjects.h"



using namespace std;

const int width = 1000;
const int height = 1000;

Spaceship::Spaceship()
{
  assert(width != 0);
  form.pos_x = width / 2;
  form.pos_y = height / 2;
  form.radius = 20;
  form.velocity = 3.0;
}

unique_ptr<GameObject> Spaceship::shoot()
{
  double angle = form.rotation / 180.0 * PI;
  double x = form.pos_x + 35*sin(angle);
  double y = form.pos_y - 35*cos(angle);

  return make_unique<GameObject>(Bullet{form.rotation, x, y});
}

std::unique_ptr<GameObject> Spaceship::update(const Actions& actions) {
  if (actions.moveStraight) {
    form.goStraight();
  }
  if (actions.rotateLeft) {
    form.rotate(rotationSpeed);
  }
  if (actions.rotateRight) {
    form.rotate(-rotationSpeed);
  }
  if (actions.shoot) {
    return shoot();
  }
  return nullptr;
}

Bullet::Bullet(double rotation, double x, double y) 
{
  form.pos_x = x;
  form.pos_y = y;
  form.rotation = rotation;
  form.velocity = 5;
  form.radius = 2;
}

bool Bullet::isOutside() const
{
  return form.pos_x > width || form.pos_x < 0 ||
    form.pos_y > width || form.pos_y < 0;
}

Asteroid Asteroid::randomAsteroid() {
  int size = rand() % 3;
  double startX = -50 + (rand() % 2) * (width + 100);
  double startY = -50 + (rand() % 2) * (height + 100);
  return Asteroid{size, startX, startY};
}

// Protip: use a picture of your enemy to make the game more fun
//  : sprite(constructSprite("res/asteroid.png", 1 * 0.1 * (size + 2));
Asteroid::Asteroid(int _size, double x, double y)
{
  size = _size;
  form.pos_x = x;
  form.pos_y = y;

  if (size < 0) deleted = true;

  form.rotation = rand() % 360;
  size = size;

  form.velocity = (3 - size)*0.3;
  form.radius = (size + 2) * 8;
}

unique_ptr<GameObject> Asteroid::spawnObject()
{
  return make_unique<GameObject>(smallerAsteroid());
}

bool Asteroid::isOutside() const
{
  return form.pos_x > width + 100 || form.pos_x < -100 ||
    form.pos_y > 100 + height || form.pos_y < -100;
}

