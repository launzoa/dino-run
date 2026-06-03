#pragma once
#include "Obstacle.h"
#include "TRex.h"
#include "constants.h"
#include <deque>
#include <vector>

class GameWorld {
private:
  std::deque<Obstacle> obstacles;
  TRex trex;
  double gameSpeed = GAME_SPEED;
  double spawnTimer = 2.0;
  double gameTime = GAME_TIME_LIMIT;
  bool gameOverState = false;

public:
  GameWorld();
  ~GameWorld();
  void update(double deltaTime, int action);

  void insertObstacle();
  void removeObstacle();

  void timeDecrement(double deltaTime);

  bool isColliding();
  void switchGameOverState();
  void resetGame();

  // Getters
  bool getGameOverState();
  double getDistanceToObstacle();
  double getTimeToCollision();
  double getGameSpeed();
  TRex getTrex();
  std::vector<double> getInputs();
  std::deque<Obstacle> getObstacles();
};