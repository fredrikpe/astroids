/*
   TDT4102, Spring 2017
   C++ Asteroid example using SFML

   Any questions, please email me at:
   fredrikpei@gmail.com
   */

#pragma once

#include "common.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

struct Sprite {
  explicit Sprite(const std::string &filename) {
    if (!image.loadFromFile(filename)) {
      throw std::exception();
    }

    image.createMaskFromColor(sf::Color::White);
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    sprite.setOrigin(sprite.getTextureRect().width / 2,
                     sprite.getTextureRect().height / 2);

    sprite.setPosition(WindowWidth / 2, WindowHeight / 2);
  }

  sf::Image image;
  sf::Texture texture;
  sf::Sprite sprite;
};

class Gui {
public:
  bool isOpen() { return window.isOpen(); }

  Actions parseInput() {
    sf::Event event{};
    window.pollEvent(event);

    if ( // event.type == sf::Event::Closed ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
      window.close();
    }

    return Actions{sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up),
                   sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left),
                   sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right),
                   sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space),
                   sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return)};
  }

  void render(const std::vector<GameObject> &objects) {
    window.clear();
    for (const auto &obj : objects) {
      window.drawGameObject(obj);
    }
    window.updateFrame();
  }

private:
  class Window {
  public:
    Window()
        : render_window(sf::VideoMode(WindowWidth, WindowHeight), WindowTitle) {
      if (!font.loadFromFile("res/OpenSans-Bold.ttf")) {
        render_window.close();
      }
    }

    void updateFrame() { render_window.display(); }

    void drawGameObject(const GameObject &object) {
      const auto filename = std::visit([](auto&& o) { return o.fileAndScaling(); }, object).first;
      const auto scaling = std::visit([](auto&& o) { return o.fileAndScaling(); }, object).second;

      try {
        auto sprite = Sprite(filename);
        const auto physicsForm = std::visit([](auto&& o) { return o.form; },  object);

        sprite.sprite.scale(scaling, scaling);
        sprite.sprite.setPosition(physicsForm.pos_x, physicsForm.pos_y);
        sprite.sprite.setRotation(physicsForm.rotation);

        render_window.draw(sprite.sprite);

        sprites.push_back(std::move(sprite));
      } catch (...) {
        render_window.close();
        return;
      }
    }

    void clear() {
      render_window.clear();
      sprites.clear();
    }

    bool isOpen() { return render_window.isOpen(); }
    void close() { render_window.close(); }
    bool pollEvent(auto &event) { return render_window.pollEvent(event); }

  private:
    sf::RenderWindow render_window;
    sf::Font font;
    std::vector<Sprite> sprites;
  };

  Window window;
};
