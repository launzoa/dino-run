#pragma once
#include <vector>

// @brief: visual configurations
const int WINDOW_WIDTH = 1200;           // max width for window at x axis
const int WINDOW_HEIGHT = 800;           // max height for window at y axis
const int GROUND_Y = WINDOW_HEIGHT - 25; // ground position on screen

// @brief: global parameters
const double SPAWN_TIME = 2.0;       // spawn interval for obstacles
const double GAME_SPEED = 400.0;     // game initial velocity
const double GAME_TIME_LIMIT = 1000; // game time limit
const std::vector<int> SPEED_ARR = {1, 2, 10, 100}; // speed multiplier
const int RAM_MAX_RECORDS = 20000;                  // max records for ram

// @brief: physics actions
const double JUMP_IMPULSE = -800; // force applied on y axis for jump
const double SNEAK_IMPULSE = 800; // force applied on y axis for get jump down

// @brief: neural network hyperparamters
const int INPUT_NEURONS =
    5; // distance, velocity, height, width (obstacle), y velocity (trex)
const int HIDDEN_NEURONS = 16;     // neuron quantity for hidden layer
const int OUTPUT_NEURONS = 3;      // duck, run, jump (action)
const double LEARNING_RATE = 0.05; // learning rate
const int EPOCHS = 5;              // epochs for training

// T-Rex (hitboxes)
const double TREX_WIDTH = 88;                 // rectangle width
const double TREX_HEIGHT = 94;                // rectangle height
const double TREX_SNEAK_HEIGHT = 60;          // rectangle height while sneaking
const double TREX_X = 100;                    // initial position at x axis
const double TREX_Y = GROUND_Y - TREX_HEIGHT; // initial position at y axis (in
                                              // ground)
const double TREX_VEL_Y = 0;                  // velocity at y axis (down or up)
const double TREX_GRAVITY = 1500.0; // gravitational constant (trex get down)

// @brief: obstacles (hitboxes)
const int SMALL_CACTUS_WIDTH = 34;  // rectangle (sc) width
const int SMALL_CACTUS_HEIGHT = 70; // rectangle (sc) height
const int BIG_CACTUS_WIDTH = 50;    // rectangle (bc) width
const int BIG_CACTUS_HEIGHT = 100;  // rectangle (bc) height
const int PTERODACTYL_WIDTH = 92;   // rectangle (pte) width
const int PTERODACTYL_HEIGHT = 80;  // rectangle (pte) height
