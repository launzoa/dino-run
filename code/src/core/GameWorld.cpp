#include "headers/GameWorld.h"
#include "headers/Obstacle.h"
#include "headers/constants.h"
#include <random>
#include <vector>

using namespace std;

GameWorld::GameWorld() {}
GameWorld::~GameWorld() {}

// @brief: updates the game world
// @param: deltaTme -> time passed since the last update
// @param: acion -> action to be performed by the trex
void GameWorld::update(double deltaTime, int action) {
  // update trex position based on the action received from the brain
  this->trex.update(deltaTime, action);

  if (!this->obstacles.empty()) { // moves obstacles to the left
    for (Obstacle &obstacle : this->obstacles) {
      obstacle.update(deltaTime, this->gameSpeed);
    }

    if (this->isColliding()) {     // checks for collision
      this->switchGameOverState(); // if colide, game over
    }

    this->gameSpeed += 10.0 * deltaTime; // increases game speed

    // remove the obstacle if it's out of the screen
    if (this->obstacles.front().getX() + this->obstacles.front().getWidth() <
        0) {
      this->removeObstacle();
    }
  }

  // spawner of obstacles
  this->spawnTimer -= deltaTime;
  if (this->spawnTimer <= 0) { // inserts an obstacle when the timer reaches 0
    this->insertObstacle();
    // faster game needs faster obstacle spawn
    this->spawnTimer = SPAWN_TIME * (GAME_SPEED / this->gameSpeed);
  }
}

// @brief: creates cactus of different sizes or pterodactyls based on the
// random
void GameWorld::insertObstacle() {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, 2);

  int obstacleClass = dis(gen);

  switch (obstacleClass) { // small cactus (can come from 1 to 3 clusters)
  case 0: {
    int qntSmallCactus = (dis(gen) != 0) ? dis(gen) + 1 : 1;
    for (int i = 0; i < qntSmallCactus; i++) {
      this->obstacles.push_back(
          Obstacle(WINDOW_WIDTH + (i * SMALL_CACTUS_WIDTH),
                   GROUND_Y - SMALL_CACTUS_HEIGHT, SMALL_CACTUS_WIDTH,
                   SMALL_CACTUS_HEIGHT));
    }
    break;
  }
  case 1: { // big cactus (can come from 1 to 2 clusters)
    int qntBigCactus = (dis(gen) != 0) ? dis(gen) : 1;
    for (int i = 0; i < qntBigCactus; i++) {
      this->obstacles.push_back(Obstacle(WINDOW_WIDTH + (i * BIG_CACTUS_WIDTH),
                                         GROUND_Y - BIG_CACTUS_HEIGHT,
                                         BIG_CACTUS_WIDTH, BIG_CACTUS_HEIGHT));
    }
    break;
  }
  case 2: { // pterodactyl (always 1 cluster)
    int whichPterodactyl = dis(gen);

    int spawnY;
    if (whichPterodactyl == 0) { // high pterodactyl
      spawnY = GROUND_Y - 205;
    } else if (whichPterodactyl == 1) { // mid pterodactyl
      spawnY = GROUND_Y - 155;
    } else { // low pterodactyl
      spawnY = GROUND_Y - 95;
    }
    this->obstacles.push_back(Obstacle(WINDOW_WIDTH + (PTERODACTYL_WIDTH),
                                       spawnY, PTERODACTYL_WIDTH,
                                       PTERODACTYL_HEIGHT));
    break;
  }
  }
}

// @brief: removes the obstacle from the game world (when it's out of the
// screen)
void GameWorld::removeObstacle() { this->obstacles.pop_front(); }

// @brief: decrements the game time
// @param: deltaTime -> time passed since the last update
void GameWorld::timeDecrement(double deltaTime) { this->gameTime -= deltaTime; }

// @brief: checks for collision (the trex "invade" the obstacle area in X and Y)
bool GameWorld::isColliding() {
  Obstacle front = this->obstacles.front();
  // checks if the trex's hitbox overlaps with the obstacle's hitbox
  // (Axis-Aligned Bounding Box collision detection)

  bool overlapX = (this->trex.getX() < front.getX() + front.getWidth()) &&
                  (this->trex.getX() + this->trex.getWidth() > front.getX());
  bool overlapY = (this->trex.getY() < front.getY() + front.getHeight()) &&
                  (this->trex.getY() + this->trex.getHeight() > front.getY());

  return overlapX && overlapY;
}

// @brief: switches the game over state
void GameWorld::switchGameOverState() {
  this->gameOverState = !this->gameOverState;
}

// @brief: resets the game world (calls this function when the game is over)
void GameWorld::resetGame() {
  this->gameOverState = false;
  this->gameSpeed = GAME_SPEED;
  this->gameTime = GAME_TIME_LIMIT;
  this->spawnTimer = SPAWN_TIME;
  this->obstacles.clear();
  this->trex.reset();
}

// @brief: returns the game over state
bool GameWorld::getGameOverState() { return this->gameOverState; }

// @brief: returns the distance to the obstacle based on the trex's and front
// obstacle's.
// @returns: -1.0 if there are no obstacles, otherwise the distance to the
// obstacle
double GameWorld::getDistanceToObstacle() {
  if (this->obstacles.empty()) {
    return -1.0;
  }

  return this->obstacles.front().getX() - this->trex.getX();
}

// @brief: returns the time to collision based on the distance and game speed
// @returns: -1.0 if there are no obstacles, otherwise the time to collision
double GameWorld::getTimeToCollision() {
  double distance = this->getDistanceToObstacle();

  if (distance < 0) {
    return -1.0;
  }

  return distance / this->gameSpeed;
}

// @brief: returns the trex instace
TRex GameWorld::getTrex() { return this->trex; }

// @brief: create the input vector for the neuron (normalized in 0.0 and 1.0)
// @returns: vector of 5 values (distance, game speed, obstacle y
// position, obstacle width, trex velocity)
std::vector<double> GameWorld::getInputs() {
  if (this->obstacles.empty()) { // just to fill the vector when there are no
                                 // obstacles
    return {1.0, this->gameSpeed / 1000.0, (double)GROUND_Y / WINDOW_HEIGHT,
            0.0, TREX_VEL_Y};
  }

  Obstacle front = this->obstacles.front();
  TRex trex = this->getTrex();

  // normalizes the input vector (0.0 and 1.0)
  return {this->getTimeToCollision() / (WINDOW_WIDTH / GAME_SPEED),
          this->gameSpeed / 1000.0, front.getY() / WINDOW_HEIGHT,
          (double)front.getWidth() / 100.0, trex.getVelY()};
}

// @brief: returns the obstacles deque
std::deque<Obstacle> GameWorld::getObstacles() { return this->obstacles; }