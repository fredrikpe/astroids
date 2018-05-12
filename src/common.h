
#pragma once

#include <string>

static const int WindowHeight = 1000;
static const int WindowWidth = 1000;

static const std::string WindowTitle{"Asteroids"};

struct Actions {
  bool moveStraight = false;
  bool rotateLeft = false;
  bool rotateRight = false;
  bool shoot = false;
  bool restart = false;
};
