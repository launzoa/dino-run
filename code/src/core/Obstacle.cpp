#include "headers/Obstacle.h"

// @brief: creates an obstacle with the given position and dimensions
Obstacle::Obstacle(double x, double y, double width, double height) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}

Obstacle::~Obstacle() {}

// @brief: updates the obstacle's position based on the game speed
// @param deltaTime: time elapsed since the last update
// @param gameSpeed: global speed of the game
void Obstacle::update(double deltaTime, double gameSpeed) {
  this->x -= deltaTime * gameSpeed;
}