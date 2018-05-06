/*
   TDT4102, Spring 2017
   C++ Asteroid example using SFML

   Any questions, please email me at:
   fredrikpei@gmail.com
   */

#pragma once

#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <cassert>

const std::string WindowTitle{ "Asteroids" };

const int WindowHeight = 1000;
const int WindowWidth = 1000;

struct Sprite {
  explicit Sprite(const std::string& filename) {
    if (!image.loadFromFile(filename)) {
      //throw std::exception("Failed to load " + filename + ".");
        assert(false);
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

class Window {
  public:
    Window()
      : render_window(sf::VideoMode(WindowWidth, WindowHeight), WindowTitle)
    {
      if (!font.loadFromFile("res/OpenSans-Bold.ttf")) {
        cout << "Couldn't open font!\n";
        //throw std::exception("Couldn't open font!\n");
        //throw std::exception();
        assert(false);
      }

      text.setFont(font);
      text.setStyle(sf::Text::Bold);
      formatScoreText();
    }

    void updateFrame() {
      render_window.draw(text);
      render_window.display();
    }

    void drawGameObject(const GameObject& object) {
      const auto filename = object.fileAndScaling().first;
      const auto scaling = object.fileAndScaling().second;

      auto sprite = Sprite(filename);
      const auto physicsForm = object.form();

      sprite.sprite.scale(scaling, scaling);
      sprite.sprite.setPosition(physicsForm.pos_x, physicsForm.pos_y);
      sprite.sprite.setRotation(physicsForm.rotation);

      render_window.draw(sprite.sprite);

      sprites.push_back(std::move(sprite));

    }

    void clear() { 
      render_window.clear();
      sprites.clear();
    }

    bool isOpen() { return render_window.isOpen(); }
    void close() { render_window.close(); }
    bool pollEvent(auto& event) { return render_window.pollEvent(event); }

    void setText(const std::string& txt) {
      text.setString(txt);
    }

    void formatScoreText() {
      formatText(50, 50, 20, sf::Color::White);
    }

    void formatEndText() {
      formatText(170, 300, 80, sf::Color::Green);
    }

  private:
    void formatText(int pos_x, int pos_y, int size, auto color) {
      text.setCharacterSize(size);
      text.setColor(color);
      text.setPosition(pos_x, pos_y);
    }

    sf::RenderWindow render_window;
    sf::Text         text;
    sf::Font         font;
    std::vector<Sprite> sprites;
};
