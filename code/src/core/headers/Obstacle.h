#pragma once
#include "constants.h"

class Obstacle {

private:
  double x;
  double y;
  double width;
  double height;

public:
  Obstacle(double x, double y, double width, double height);
  ~Obstacle();

  void update(double deltaTime, double gameSpeed);

  // Getters
  double getX() const { return x; }
  double getY() const { return y; }
  double getWidth() const { return width; }
  double getHeight() const { return height; }
};