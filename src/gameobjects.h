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
#include <tuple>
#include <typeinfo>
#include <cstdlib> 
#include <cmath>
#include <algorithm>

#define PI 3.14159265;

using namespace std;

enum class ObjectType { Spaceship, Bullet, Asteroid };

struct Actions {
  bool moveStraight = false;
  bool rotateLeft = false;
  bool rotateRight = false;
  bool shoot = false;
};

struct PhysicsForm {
  double pos_x = 0;
  double pos_y = 0;
  double rotation = 0;
  double velocity = 0;
  double radius = 0;

  bool isCollision(const PhysicsForm& other) const {
    double distance = sqrt(pow(pos_x - other.pos_x, 2) +
        pow(pos_y - other.pos_y, 2));
    return distance < radius + other.radius;
  }

  void goStraight() {
    double angle = rotation / 180.0 * PI;
    pos_x += velocity*sin(angle);
    pos_y += -velocity*cos(angle);
  }
  
  void rotate(double angle) {
    rotation += angle;
  }
};


class GameObject {
  public:
    template <typename T>
    GameObject(T t)
    : model(std::make_unique<Model<T>>(std::move(t)))
    {}

    std::unique_ptr<GameObject> update(const Actions& actions) {
      return model->update(actions);
    }
    std::unique_ptr<GameObject> collision(const GameObject& other) {
      return model->collision(other);
    }
    PhysicsForm form() const {
      return model->form();
    }
    bool isDeleted() const {
      return model->isDeleted();
    }
    std::pair<std::string, double> fileAndScaling() const {
      return model->fileAndScaling();
    }

    ObjectType type() const {
      return model->type();
    }

  private:
    struct Concept {
      virtual ~Concept() = default;
      virtual std::unique_ptr<GameObject> update(const Actions& actions) = 0;
      virtual std::unique_ptr<GameObject> collision(const GameObject& other) = 0;
      virtual PhysicsForm form() const = 0;
      virtual bool isDeleted() const = 0;
      virtual std::pair<std::string, double> fileAndScaling() const = 0;
      virtual ObjectType type() const = 0;
    };

    template <typename T>
    struct Model final : Concept {
      Model(T t)
      : data(std::move(t))
      {}

      std::unique_ptr<GameObject> update(const Actions& actions) override {
        return data.update(actions);
      }

      PhysicsForm form() const override {
        return data.form;
      }

      std::pair<std::string, double> fileAndScaling() const override {
        return data.fileAndScaling();
      }

      bool isDeleted() const override {
        return data.deleted;
      }

      std::unique_ptr<GameObject> collision(const GameObject& other) override {
        return data.collision(other);
      }

      ObjectType type() const override {
        return data.type();
      }

      T data;
    };

    std::unique_ptr<Concept> model;
};

class Spaceship {
  public:
    Spaceship();

    std::unique_ptr<GameObject> update(const Actions& actions);

    std::unique_ptr<GameObject> collision(const GameObject& other) {
      if (form.isCollision(other.form())) {
        deleted = true;
      }
      return nullptr;
    }

    bool isOutside(int, int) const { return false; }
    unique_ptr<GameObject> shoot();

    std::pair<std::string, double> fileAndScaling() const {
      return std::make_pair("res/spaceship.png", 0.5);
    }

    ObjectType type() const { return ObjectType::Spaceship; }

    PhysicsForm form;

    bool deleted = false;
  private:
    double rotationSpeed = 3.0;
};

class Bullet {
  public:
    Bullet(double rotation, double x, double y);

    std::unique_ptr<GameObject> update(const Actions&) {
      if (isOutside()) {
        deleted = true;
      }
      form.goStraight();
      return nullptr;
    }

    std::unique_ptr<GameObject> collision(const GameObject& other) {
      if (form.isCollision(other.form())) {
        deleted = true;
      }
      return nullptr;
    }

    bool isOutside() const;

    std::pair<std::string, double> fileAndScaling() const {
      return std::make_pair("res/bullet.png", 0.05);
    }

    ObjectType type() const { return ObjectType::Bullet; }

    PhysicsForm form;

    bool deleted = false;
};

class Asteroid {
  public:
    Asteroid(int size, double x, double y);

    std::unique_ptr<GameObject> update(const Actions&) {
      if (isOutside()) {
        deleted = true;
      }
      form.goStraight();
      return nullptr;
    }

    std::unique_ptr<GameObject> collision(const GameObject& other) {
      if (form.isCollision(other.form())) {
        if (other.type() != type()) {
          *this = smallerAsteroid();
          return spawnObject();
        }
      }
      return nullptr;
    }

    unique_ptr<GameObject> spawnObject();

    Asteroid smallerAsteroid() const {
      return Asteroid{size - 1, form.pos_x, form.pos_y};
    }

    bool isOutside() const;
    std::pair<std::string, double> fileAndScaling() const {
      return std::make_pair("res/asteroid.png", 1 * 0.1 * (size + 2));
    }

    static Asteroid randomAsteroid();

    ObjectType type() const { return ObjectType::Asteroid; }

    PhysicsForm form;
    bool deleted = false;

  private:
    int size = 0;
};

