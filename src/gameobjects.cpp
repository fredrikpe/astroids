
#include <iostream> 
#include <cstdlib> 
#include <cmath>
#include <memory>
#include <algorithm>
#include <cassert>

#include "gameobjects.h"

#define PI 3.14159265;


using namespace std;

int GameObject::width = 0;
int GameObject::height = 0;

void GameObject::update() {
  if (isOutside()) {
    deleteMe = true;
  }
  goStraight();
}

bool GameObject::isOutside() const
{
  return pos_x > width || pos_x < 0 || pos_y > width || pos_y < 0;
}

void GameObject::goStraight()
{
  float angle = rotation / 180 * PI;
  pos_x += velocity*sin(angle);
  pos_y += -velocity*cos(angle);
}

bool GameObject::collision(GameObject& rhs) const 
{
  float distance = sqrt(pow(pos_x - rhs.pos_x, 2) + pow(pos_y - rhs.pos_y, 2));
  return distance < getRadius() + rhs.getRadius();
}

void Spaceship::update() {}

unique_ptr<GameObject> Spaceship::spawnObject()
{
  double angle = rotation / 180 * PI;
  float x = pos_x + 35*sin(angle);
  float y = pos_y - 35*cos(angle);

  return make_unique<Bullet>(rotation, x, y);
}

Spaceship::Spaceship()
{
  assert(width != 0);
  pos_x = GameObject::width / 2;
  pos_y = GameObject::height / 2;
  setRadius(20);
  scale = 0.5;
}

Bullet::Bullet(double rotation, float x, float y)
{
  pos_x = x;
  pos_y = y;
  rotation = rotation;
  velocity = 5;
  radius = 2;
  setVelocity(7);
  setRadius(2);
  scale = 0.05;
}

unique_ptr<GameObject> Asteroid::spawnObject()
{
  int size = getSize() - 1;
  return make_unique<Asteroid>(size, pos_x, pos_y);
}

bool Asteroid::isOutside() const
{
  return pos_x > width + 100 || pos_x < -100 || pos_y > 100 + height || pos_y < -100;
}

// Protip: use a picture of your enemy to make the game more fun
//  : sprite(constructSprite("res/asteroid.png", 1 * 0.1 * (size + 2));
Asteroid::Asteroid(int size, int x, int y)
{
  pos_x = x;
  pos_y = y;
  scale = 1 * 0.1 * (size + 2);

  if (size < 0) deleteMe = true;


  rotation = rand() % 360;
  setSize(size);

  setVelocity((3 - size)*0.1);
  setRadius((size + 2) * 8);
}

