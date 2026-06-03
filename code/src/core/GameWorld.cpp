#include "headers/GameWorld.h"
#include "headers/Obstacle.h"
#include "headers/constants.h"
#include <cstdlib>
#include <vector>

using namespace std;

GameWorld::GameWorld() {}
GameWorld::~GameWorld() {}

void GameWorld::update(double deltaTime, int action) {
  this->trex.update(deltaTime, action);

  if (!this->obstacles.empty()) {

    for (Obstacle &obstacle : this->obstacles) {
      obstacle.update(deltaTime, this->gameSpeed);
    }

    if (this->isColliding()) {
      this->switchGameOverState();
    }

    // Aumenta gradativamente a velocidade do jogo com o tempo
    this->gameSpeed += 10.0 * deltaTime;

    if (this->obstacles.front().getX() + this->obstacles.front().getWidth() <
        0) {
      this->removeObstacle();
    }
  }

  this->spawnTimer -= deltaTime;
  if (this->spawnTimer <= 0) {
    this->insertObstacle();
    // Quanto mais rápido o jogo, menor o tempo de spawn (mantém a distância entre obstáculos consistente)
    this->spawnTimer = SPAWN_TIME * (GAME_SPEED / this->gameSpeed);
  }
}

void GameWorld::insertObstacle() {
  int obstacleClass = rand() % 3;

  switch (obstacleClass) {
  case 0: { // small cactus
    int qntSmallCactus = rand() % 3 + 1;
    for (int i = 0; i < qntSmallCactus; i++) {
      this->obstacles.push_back(
          Obstacle(WINDOW_WIDTH + (i * SMALL_CACTUS_WIDTH),
                   GROUND_Y - SMALL_CACTUS_HEIGHT, SMALL_CACTUS_WIDTH,
                   SMALL_CACTUS_HEIGHT));
    }
    break;
  }
  case 1: { // big cactus
    int qntBigCactus = rand() % 2 + 1;
    for (int i = 0; i < qntBigCactus; i++) {
      this->obstacles.push_back(Obstacle(WINDOW_WIDTH + (i * BIG_CACTUS_WIDTH),
                                         GROUND_Y - BIG_CACTUS_HEIGHT,
                                         BIG_CACTUS_WIDTH, BIG_CACTUS_HEIGHT));
    }
    break;
  }
  case 2: { // pterodactyl
    int qntPterodactyl = rand() % 1 + 1;

    for (int i = 0; i < qntPterodactyl; i++) {
      this->obstacles.push_back(Obstacle(WINDOW_WIDTH + (i * PTERODACTYL_WIDTH),
                                         0, PTERODACTYL_WIDTH, GROUND_Y - 65));
    }
    break;
  }
  }
}

void GameWorld::removeObstacle() { this->obstacles.pop_front(); }

void GameWorld::timeDecrement(double deltaTime) { this->gameTime -= deltaTime; }

bool GameWorld::isColliding() {
  Obstacle front = this->obstacles.front();
  bool overlapX = (this->trex.getX() < front.getX() + front.getWidth()) &&
                  (this->trex.getX() + this->trex.getWidth() > front.getX());
  bool overlapY = (this->trex.getY() < front.getY() + front.getHeight()) &&
                  (this->trex.getY() + this->trex.getHeight() > front.getY());

  return overlapX && overlapY;
}

void GameWorld::switchGameOverState() {
  this->gameOverState = !this->gameOverState;
}

void GameWorld::resetGame() {
  this->gameOverState = false;
  this->gameSpeed = GAME_SPEED;
  this->gameTime = GAME_TIME_LIMIT;
  this->spawnTimer = SPAWN_TIME;

  this->obstacles.clear();

  this->trex.reset();
}

// Getters

bool GameWorld::getGameOverState() { return this->gameOverState; }
double GameWorld::getDistanceToObstacle() {
  if (this->obstacles.empty()) {
    return -1.0;
  }

  return this->obstacles.front().getX() - this->trex.getX();
}

double GameWorld::getTimeToCollision() {
  double distance = this->getDistanceToObstacle();

  if (distance < 0) {
    return -1.0;
  }

  return distance / this->gameSpeed;
}

TRex GameWorld::getTrex() { return this->trex; }

std::vector<double> GameWorld::getInputs() {
  if (this->obstacles.empty()) {
    return {1.0, this->gameSpeed / 1000.0, (double)GROUND_Y / WINDOW_HEIGHT,
            0.0};
  }

  Obstacle front = this->obstacles.front();
  double visual_y = front.getY();
  if (front.getWidth() == PTERODACTYL_WIDTH) {
      visual_y = GROUND_Y - 55; // Altura visual original do Pterodáctilo
  }

  return {this->getTimeToCollision() / (WINDOW_WIDTH / GAME_SPEED),
          this->gameSpeed / 1000.0, visual_y / WINDOW_HEIGHT,
          front.getWidth() / 100.0};
}

std::deque<Obstacle> GameWorld::getObstacles() { return this->obstacles; }