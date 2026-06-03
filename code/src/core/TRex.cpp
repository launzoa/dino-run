#include "headers/TRex.h"

void TRex::update(double deltaTime, int action) {
  switch (action) {
  case 0: // run
    this->height = TREX_HEIGHT;
    break;

  case 1: // jump
    if (this->y >= GROUND_Y - this->height) {
      this->jump();
    }
    break;

  case -1: // sneak
    this->sneak();
    break;
  }

  this->vel_y += this->GRAVITY * deltaTime;
  this->y += this->vel_y * deltaTime;

  if (this->y > GROUND_Y - this->height) {
    this->y = GROUND_Y - this->height;
    this->vel_y = 0;
  }
}

void TRex::jump() { this->vel_y = JUMP_IMPULSE; }

void TRex::sneak() {
  this->vel_y = SNEAK_IMPULSE;
  this->height = 60;
}

void TRex::reset() {
  this->x = TREX_X;
  this->y = TREX_Y;
  this->vel_y = TREX_VEL_Y;
  this->height = TREX_HEIGHT;
}