#pragma once
#include "Obstacle.h"
#include "TRex.h"
#include "constants.h"
#include <deque>
#include <vector>

// @brief: class that controls the game world and the main dinosaur
class GameWorld {
private:
  std::deque<Obstacle> obstacles; // queue of obstacles (those who leave the
                                  // screen on the left are deleted)
  TRex trex;                      // the main avatar
  double gameSpeed = GAME_SPEED;
  double spawnTimer = 2.0; // timer to spawn the next obstacle
  double gameTime = GAME_TIME_LIMIT;
  bool gameOverState = false; // control flag (did the dino crash?)

public:
  GameWorld();
  ~GameWorld();

  // @brief: updates the game world and the dinosaur's position
  void update(double deltaTime, int action);

  // @brief: generates new obstacles
  void insertObstacle();

  // @brief: removes obstacles that are off-screen
  void removeObstacle();

  // @brief: decrements the game time
  void timeDecrement(double deltaTime);

  // @brief: checks for collisions
  bool isColliding();

  // @brief: switches the game over state
  void switchGameOverState();

  // @brief: resets the game
  void resetGame();

  // @brief: returns the game over state
  bool getGameOverState();

  // @brief: returns the distance to the nearest obstacle
  double getDistanceToObstacle();

  // @brief: returns the time to collision
  double getTimeToCollision();

  // @brief: returns the game speed
  double getGameSpeed();

  // @brief: returns the dinosaur
  TRex getTrex();

  // @brief: returns the inputs for the neural network
  std::vector<double> getInputs();

  // @brief: returns the obstacles
  std::deque<Obstacle> getObstacles();
};