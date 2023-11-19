#pragma once

#include <chrono>
#include <ctime>

float mean(float si_a, float si_b) {
  if (((si_b > 0) && (si_a > (INT_MAX - si_b))) ||
    ((si_b < 0) && (si_a < (INT_MIN - si_b))))
  {
    /* will overflow, so use difference method */
    return si_b + (si_a - si_b) / 2;
  } 
  else
  {
  /* the addition will not overflow */
    return (si_a + si_b) / 2;
  }
}

float sine_w(float time, float period) {
    return std::sin(time * 2 * M_PI / period);
}

float sine_wave(float time, float period, float amplitude, float midpoint) {
    return std::sin(time * 2 * M_PI / period) * amplitude + midpoint;
}

float sine_between(float time, float period, float minimum, float maximum) {
    float midpoint = mean(minimum, maximum);
    float amplitude = maximum - midpoint;
    return sine_wave(time, period, amplitude, midpoint);
}

float time_epoch_ms() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}