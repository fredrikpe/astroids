
#pragma once

#include <chrono>

using namespace std::chrono;

struct FrameTimer {

  void updateFrameTime() {
    const auto now = high_resolution_clock::now();
    frame_time = duration<double>(now - prev_time).count();
    prev_time = now;
  }

  double frame_time = 0.0;
  const double dt = 0.01;

  time_point<high_resolution_clock> prev_time = high_resolution_clock::now();
};
