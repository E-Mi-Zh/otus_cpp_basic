#pragma once
#include <iostream>

class Color {
  public:
    Color();
    Color(double red, double green, double blue);
    double red() const;
    double green() const;
    double blue() const;

  private:
    double r{};
    double g{};
    double b{};
};

inline std::istream& operator>>(std::istream& stream, Color& variable) {
    double r, g, b;
    stream >> r >> g >> b;
    variable = Color(r, g, b);
    return stream;
}
