#pragma once
#include "constants.h"

class TRex {

private:
  double x = TREX_X;
  double y = TREX_Y;
  double width = TREX_WIDTH;
  double height = TREX_HEIGHT;
  double vel_y = TREX_VEL_Y;
  const double GRAVITY = TREX_GRAVITY;

public:
  TRex() {}
  ~TRex() {}

  void update(double deltaTime, int action); // update vertical velocity of TRex
  void jump();                               // jump action
  void sneak();                              // sneak action

  void reset();

  // Getters
  double getX() const { return x; }
  double getY() const { return y; }
  double getWidth() const { return width; }
  double getHeight() const { return height; }
};