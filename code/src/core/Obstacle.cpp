#include "headers/Obstacle.h"

Obstacle::Obstacle(double x, double y, double width, double height) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}

Obstacle::~Obstacle() {}

void Obstacle::update(double deltaTime, double gameSpeed) {
  this->x -= deltaTime * gameSpeed;
}