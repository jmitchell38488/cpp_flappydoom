#pragma once

#include <chrono>
#include <ctime>
#include <cctype>
#include <string>
#include <iostream>
#include <cstring>
#include <codecvt>
#include <cmath>

using namespace std;

float mean(float si_a, float si_b)
{
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

float sine_w(float time, float period)
{
  return std::sin(time * 2 * M_PI / period);
}

float sine_wave(float time, float period, float amplitude, float midpoint)
{
  return std::sin(time * 2 * M_PI / period) * amplitude + midpoint;
}

float sine_between(float time, float period, float minimum, float maximum)
{
  float midpoint = mean(minimum, maximum);
  float amplitude = maximum - midpoint;
  return sine_wave(time, period, amplitude, midpoint);
}

float time_epoch_ms()
{
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

std::u32string convertToUTF32(const std::string &input)
{
  std::u32string result;

  // Manual conversion from UTF-8 to UTF-32
  for (size_t i = 0; i < input.size();)
  {
    char32_t cp = 0;
    unsigned char c = static_cast<unsigned char>(input[i]);
    if (c <= 0x7F)
    {
      cp = c;
      i += 1;
    }
    else if (c <= 0xDF && i + 1 < input.size())
    {
      cp = (c & 0x1F) << 6 | (input[i + 1] & 0x3F);
      i += 2;
    }
    else if (c <= 0xEF && i + 2 < input.size())
    {
      cp = (c & 0x0F) << 12 | (input[i + 1] & 0x3F) << 6 | (input[i + 2] & 0x3F);
      i += 3;
    }
    else if (c <= 0xF4 && i + 3 < input.size())
    {
      cp = (c & 0x07) << 18 | (input[i + 1] & 0x3F) << 12 | (input[i + 2] & 0x3F) << 6 | (input[i + 3] & 0x3F);
      i += 4;
    }
    else
    {
      // Handle invalid UTF-8 sequence
      break;
    }
    result += cp;
  }

  return result;
}

void string_to_lower(std::string str)
{
  // Convert the string to lowercase
  for (char &c : str)
  {
    if (std::isalpha(c))
    {
      c = std::tolower(c);
    }
  }
}

float ease_in_ease_out(float normTime) {
    return normTime < 0.5f ? 0.5f * pow(2 * normTime, 2) : -0.5f * (pow(2 * normTime - 2, 2) - 2);
}