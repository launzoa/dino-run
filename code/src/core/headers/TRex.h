#pragma once
#include "constants.h"

// @brief: class that controls the main dinosaur
class TRex {

private:
  double x = TREX_X;                   // x position
  double y = TREX_Y;                   // y position
  double width = TREX_WIDTH;           // width
  double height = TREX_HEIGHT;         // height
  double vel_y = TREX_VEL_Y;           // vertical velocity
  const double GRAVITY = TREX_GRAVITY; // gravitational constant

public:
  TRex() {}
  ~TRex() {}

  // @brief: receives an integer command (-1 = duck, 0 = run, 1 = jump) and
  // updates the physics
  void update(double deltaTime, int action);

  // @brief: applies a vertical force contrary to gravity
  void jump();

  // @brief: shortens the hitbox (collision box)
  void sneak();

  // @brief: returns to life after the game is over
  void reset();

  // @brief: returns the x position of the trex
  double getX() const { return x; }

  // @brief: returns the y position of the trex
  double getY() const { return y; }

  // @brief: returns the width of the trex
  double getWidth() const { return width; }

  // @brief: returns the height of the trex
  double getHeight() const { return height; }

  // @brief: returns the vertical velocity of the trex
  double getVelY() const { return vel_y; }
};