#include "headers/TRex.h"

//@brief: updates the trex's position based on the game speed and action
//@param deltaTime: time elapsed since the last update
//@param action: action to be performed by the trex
void TRex::update(double deltaTime, int action) {

  // updates the trex's position based on the action
  switch (action) {
  case 0:                       // run
    this->height = TREX_HEIGHT; // restores the normal height if it was crouched
    break;

  case 1: // jump
    // there is no "double jump" in the air.
    // only jump if it is touching the ground
    if (this->y >= GROUND_Y - this->height) {
      this->jump();
    }
    break;

  case -1: // sneak
    this->sneak();
    break;
  }

  // acceleration alters speed... and speed alters position on the Y axis
  this->vel_y += this->GRAVITY * deltaTime;
  this->y += this->vel_y * deltaTime;

  // prevents the trex from falling off the screen
  if (this->y > GROUND_Y - this->height) {
    this->y = GROUND_Y - this->height;
    this->vel_y = 0;
  }
}

// @brief: injects an abrupt force upwards (on the y axis)
void TRex::jump() { this->vel_y = JUMP_IMPULSE; }

// @brief: shortens the visual hitbox to avoid flying dinossaurs
void TRex::sneak() {
  this->vel_y = SNEAK_IMPULSE;
  if (this->height != TREX_SNEAK_HEIGHT) {
    // Se estava de pé no chão, a cabeça deve descer instantaneamente
    if (this->y >= GROUND_Y - TREX_HEIGHT) {
      this->y = GROUND_Y - TREX_SNEAK_HEIGHT;
    }
    this->height = TREX_SNEAK_HEIGHT;
  }
}

// @brief: resets the trex to its initial position
void TRex::reset() {
  this->x = TREX_X;
  this->y = TREX_Y;
  this->vel_y = TREX_VEL_Y;
  this->height = TREX_HEIGHT;
}