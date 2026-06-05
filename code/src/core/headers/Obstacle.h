#pragma once
#include "constants.h"

// @brief: class that represents the obstacles
class Obstacle {

private:
  double x;
  double y;
  double width;  // hitbox width
  double height; // hitbox height

public:
  Obstacle(double x, double y, double width, double height);
  ~Obstacle();

  // @brief: "moves" obstacle to the left.
  // actually the dino is stopped on the x axis, it's the world (obstacle) that
  // runs against it
  void update(double deltaTime, double gameSpeed);

  // @brief: returns the obstacle's x position
  double getX() const { return x; }

  // @brief: returns the obstacle's y position
  double getY() const { return y; }

  // @brief: returns the obstacle's width
  double getWidth() const { return width; }

  // @brief: returns the obstacle's height
  double getHeight() const { return height; }
};