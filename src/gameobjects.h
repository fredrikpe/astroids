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
#include <memory>
#include <string>
#include <tuple>
#include <vector>

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

class GameObject {
public:
  template <typename T>
  GameObject(T t) : model(std::make_unique<Model<T>>(std::move(t))) {}

  std::unique_ptr<GameObject> update(const Actions &actions, double dt) {
    return model->update(actions, dt);
  }

  void collision(GameObject &other) { return model->collision(other); }

  PhysicsForm form() const { return model->form(); }

  bool isDeleted() const { return model->isDeleted(); }

  void setDeleted() { model->setDeleted(); }

  std::pair<std::string, double> fileAndScaling() const {
    return model->fileAndScaling();
  }

private:
  struct Concept {
    virtual ~Concept() = default;
    virtual std::unique_ptr<GameObject> update(const Actions &actions,
                                               double dt) = 0;
    virtual void collision(GameObject &other) = 0;
    virtual PhysicsForm form() const = 0;
    virtual bool isDeleted() const = 0;
    virtual void setDeleted() = 0;
    virtual std::pair<std::string, double> fileAndScaling() const = 0;
  };

  template <typename T> struct Model final : Concept {
    Model(T t) : data(std::move(t)) {}

    std::unique_ptr<GameObject> update(const Actions &actions,
                                       double dt) override {
      return data.update(actions, dt);
    }

    PhysicsForm form() const override { return data.form; }

    std::pair<std::string, double> fileAndScaling() const override {
      return data.fileAndScaling();
    }

    bool isDeleted() const override { return data.deleted; }

    void setDeleted() override { data.deleted = true; }

    void collision(GameObject &other) override {
      if (form().isCollision(other.form())) {
        setDeleted();
        other.setDeleted();
      }
    }

    T data;
  };

  std::unique_ptr<Concept> model;
};

class Spaceship {
public:
  Spaceship();

  std::unique_ptr<GameObject> update(const Actions &actions, double dt);

  bool isOutside(int, int) const { return false; }
  std::unique_ptr<GameObject> shoot();

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

  std::unique_ptr<GameObject> update(const Actions &, double dt);

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

  std::unique_ptr<GameObject> update(const Actions &, double dt);

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
