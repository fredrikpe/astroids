/*
* TDT4102, Spring 2017
* C++ Asteroid example using SFML
*
* Any questions, please email me at:
*   fredrikpei@gmail.com
*/

#pragma once

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <memory>


using namespace std;

class Bullet;
class Spaceship;
class GameObject;

enum Type { SPACESHIP, BULLET, ASTEROID };
enum class Direction { Up, Left, Right };

class GameObject {
  public:
    float rotationSpeed = 1.f;
    float velocity = 2.f;
    int radius = 2;
    int size = 0;
    double pos_x;
    double pos_y;
    double rotation;
    double scale;

  public:
    static int width, height;
    bool deleteMe = false;

    virtual ~GameObject() {}

    void setSize(int s) { size = s; }
    void setRotationSpeed(float speed) { rotationSpeed = speed; }
    void setVelocity(float speed) { velocity = speed; }
    void setRadius(int r) { radius = r; }

    float getRotationSpeed() const { return rotationSpeed; }
    float getVelocity() const { return velocity; }
    int getRadius() const { return radius; }
    virtual unique_ptr<GameObject> spawnObject() { return nullptr; }
    int getSize() const { return size; }
    double getScale() const { return scale; }

    void goStraight();
    void rotateLeft() { rotate(rotationSpeed); }
    void rotateRight() { rotate(-rotationSpeed); }
    void rotate(float angle) { rotation += angle; }

    virtual void update();
    virtual bool isOutside() const;
    virtual Type getType() const = 0;

    bool collision(GameObject& rhs) const;
};


class Spaceship : public GameObject {
  public:
    Spaceship();
    ~Spaceship() {}

    void update();

    bool isOutside(int, int) const { return false; }
    unique_ptr<GameObject> spawnObject();

    Type getType() const { return SPACESHIP; }
};


class Bullet : public GameObject {
  public:
    Bullet(double rotation, float x, float y);
    ~Bullet() {}

    Type getType() const { return BULLET; }
};

class Asteroid : public GameObject {
  public:
    Asteroid(int size, int x, int y);
    ~Asteroid() {}

    unique_ptr<GameObject> spawnObject();

    bool isOutside() const;
    Type getType() const { return ASTEROID; }
};


