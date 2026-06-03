#pragma once
// Window constants
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

// Game constants
const double SPAWN_TIME = 2.0;
const double GAME_SPEED = 400.0;
const double GAME_TIME_LIMIT = 1000;

// Ground constants
const int GROUND_Y = WINDOW_HEIGHT - 25;

// TRex constants
const double TREX_WIDTH = 88;
const double TREX_HEIGHT = 94;
const double TREX_X = 100;
const double TREX_Y = GROUND_Y - TREX_HEIGHT;
const double TREX_VEL_Y = 0;
const double TREX_GRAVITY = 1500.0;

// Small cactus constants
const int SMALL_CACTUS_WIDTH = 34;
const int SMALL_CACTUS_HEIGHT = 70;

// Big cactus constants
const int BIG_CACTUS_WIDTH = 50;
const int BIG_CACTUS_HEIGHT = 100;

// Pterodactyl constants
const int PTERODACTYL_WIDTH = 92;
const int PTERODACTYL_HEIGHT = 80;

// Actions constants
const double JUMP_IMPULSE = -800;
const double SNEAK_IMPULSE = 600;

// MLP constants
const int INPUT_NEURONS = 4;
const int HIDDEN_NEURONS = 8;
const int OUTPUT_NEURONS = 3;
const double LEARNING_RATE = 0.1;
const int EPOCHS = 5;