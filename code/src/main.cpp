#include "core/headers/GameWorld.h"
#include "core/headers/constants.h"
#include "ia/headers/MLP.h"
#include <cmath>
#include <raylib.h>
#include <vector>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

using namespace std;

// Structs
struct Cloud {
  float x;
  float y;
  float speed;
};

struct AppState {
  GameWorld game;
  MLP mlp;

  vector<vector<double>> X; // input dataset
  vector<vector<double>> Y; // output dataset

  int generation = 1;       // mlp generation
  int speed_multiplier = 1; // speed multiplier (for training more fast)
  int speedIdx = 0;         // index for speed_multiplier

  vector<Cloud> clouds;
  float groundX = 0.0f;

  // Textures
  Texture2D smallCactusSprite;
  Texture2D bigCactusSprite;
  Texture2D ptSprite;
  Texture2D trexSprite;
  Texture2D trexDuckSprite;
  Texture2D cloudSprite;
  Texture2D groundSprite;

  AppState()
      : mlp({INPUT_NEURONS, HIDDEN_NEURONS, HIDDEN_NEURONS, OUTPUT_NEURONS},
            LEARNING_RATE) {}
};

// @brief update the frame and draw the game
// @param deltaTime time passed since last frame
// @param game game instance
// @param mlp multi layer perceptron instance
// @param generation mlp generation
// @param X input dataset
// @param Y output dataset
// @param speed_multiplier speed multiplier (for training more fast)
void updateDrawFrame(double deltaTime, GameWorld *game, MLP *mlp,
                     int &generation, vector<vector<double>> &X,
                     vector<vector<double>> &Y, int speed_multiplier);

// @brief draw neural network
// @param mlp multi layer perceptron instance
// @param inputs inputs dataset
// @param startX start position at x axis
// @param startY start position at y axis
void drawNeuralNetwork(MLP *mlp, const vector<double> &inputs, int startX,
                       int startY);

// @brief main loop (callback for web)
// @param arg app state
void main_loop(void *arg) {
  AppState *state = (AppState *)arg;
  float deltaTime = GetFrameTime();

  // velocity control buttons
  Vector2 mousePoint = GetMousePosition();
  Rectangle btnMinus = {WINDOW_WIDTH - 225, 10, 40, 40};
  Rectangle btnPlus = {WINDOW_WIDTH - 50, 10, 40, 40};

  // mouse click events
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { // click on button
    if (CheckCollisionPointRec(mousePoint,
                               btnPlus)) { // click on (+) button
      state->speedIdx =
          (state->speedIdx < 3) ? state->speedIdx = state->speedIdx + 1 : 3;
      state->speed_multiplier =
          SPEED_ARR[state->speedIdx]; // increase game speed
    } else if (CheckCollisionPointRec(mousePoint,
                                      btnMinus)) { // click on (-) button
      state->speedIdx =
          (state->speedIdx > 0) ? state->speedIdx = state->speedIdx - 1 : 0;
      state->speed_multiplier =
          SPEED_ARR[state->speedIdx]; // decrease game speed
    }
  }

  state->game.timeDecrement(deltaTime); // time limit

  BeginDrawing();
  ClearBackground(RAYWHITE);

  // game loop
  updateDrawFrame(deltaTime, &state->game, &state->mlp, state->generation,
                  state->X, state->Y, state->speed_multiplier);

  // draw generation
  DrawText(TextFormat("Generation: %i", state->generation), 10, 10, 20, BLACK);

  // draw speed
  DrawText(TextFormat("Speed: x%i", state->speed_multiplier),
           WINDOW_WIDTH - 175, 20, 20, BLACK);

  // draw velocity control buttons
  DrawRectangleRec(btnMinus, LIGHTGRAY);
  DrawText("-", btnMinus.x + 15, btnMinus.y + 10, 20, BLACK);
  DrawRectangleRec(btnPlus, LIGHTGRAY);
  DrawText("+", btnPlus.x + 15, btnPlus.y + 10, 20, BLACK);

  // draw dashboard
  DrawLine(0, 450, WINDOW_WIDTH, 450, LIGHTGRAY);
  DrawText("GitHub: @launzoa", 10, 40, 20, DARKGRAY);

  // draw neural network
  drawNeuralNetwork(&state->mlp, state->game.getInputs(),
                    WINDOW_WIDTH / 2 - 150, 240);

  // draw clouds
  for (Cloud &cloud : state->clouds) {
    cloud.x -= cloud.speed * deltaTime * state->speed_multiplier;
    if (cloud.x < -state->cloudSprite.width) {
      cloud.x = WINDOW_WIDTH + GetRandomValue(10, 200);
      cloud.y = GetRandomValue(460, 600);
    }
    DrawTexture(state->cloudSprite, cloud.x, cloud.y, WHITE);
  }

  // draw ground
  state->groundX -=
      state->game.getInputs()[1] * 1000.0 * deltaTime * state->speed_multiplier;
  if (state->groundX <= -state->groundSprite.width) {
    state->groundX = 0.0f;
  }
  DrawTexture(state->groundSprite, state->groundX, GROUND_Y, WHITE);
  DrawTexture(state->groundSprite, state->groundX + state->groundSprite.width,
              GROUND_Y, WHITE);

  // draw trex
  TRex trex = state->game.getTrex();

  // current sprite
  Texture2D currentTrexSprite = (trex.getHeight() < TREX_HEIGHT)
                                    ? state->trexDuckSprite
                                    : state->trexSprite;
  // source rectangle
  Rectangle trex_source = {0, 0, (float)currentTrexSprite.width,
                           (float)currentTrexSprite.height};

  // destination rectangle
  Rectangle trex_dest = {
      (float)(trex.getX() - (currentTrexSprite.width - trex.getWidth()) / 2.0),
      (float)(trex.getY() + trex.getHeight() - currentTrexSprite.height),
      (float)currentTrexSprite.width, (float)currentTrexSprite.height};
  DrawTexturePro(currentTrexSprite, trex_source, trex_dest, {0, 0}, 0.0f,
                 WHITE);

  // draw trex hitboxes
  DrawRectangle(trex.getX(), trex.getY(), trex.getWidth(), trex.getHeight(),
                Fade(GREEN, 0.2f));
  DrawRectangleLines(trex.getX(), trex.getY(), trex.getWidth(),
                     trex.getHeight(), GREEN);

  // draw obstacles
  deque<Obstacle> obstacles = state->game.getObstacles();
  for (Obstacle obstacle : obstacles) {
    Texture2D currentSprite;
    if (obstacle.getWidth() == SMALL_CACTUS_WIDTH) // small cactus
      currentSprite = state->smallCactusSprite;
    else if (obstacle.getWidth() == BIG_CACTUS_WIDTH) // big cactus
      currentSprite = state->bigCactusSprite;
    else // pterodactyl
      currentSprite = state->ptSprite;
    // source rectangle
    Rectangle source_rect = {0, 0, (float)currentSprite.width,
                             (float)currentSprite.height};
    // destination rectangle
    Rectangle dest_rect = {
        (float)(obstacle.getX() -
                (currentSprite.width - obstacle.getWidth()) / 2.0),
        (float)(obstacle.getY() + obstacle.getHeight() - currentSprite.height),
        (float)currentSprite.width, (float)currentSprite.height};

    // draw obstacle
    DrawTexturePro(currentSprite, source_rect, dest_rect, {0, 0}, 0.0f, WHITE);

    // draw obstacle hitboxes
    DrawRectangle(obstacle.getX(), obstacle.getY(), obstacle.getWidth(),
                  obstacle.getHeight(), Fade(RED, 0.2f));
    DrawRectangleLines(obstacle.getX(), obstacle.getY(), obstacle.getWidth(),
                       obstacle.getHeight(), RED);
  }

  EndDrawing();
}

// @brief: main function
int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dino-run AI");

  AppState *state = new AppState();

  // generate initial clouds
  for (int i = 0; i < 5; ++i) {
    state->clouds.push_back(Cloud{(float)(i * 300 + 2000),
                                  (float)GetRandomValue(460, 600),
                                  (float)(200 + GetRandomValue(-50, 100))});
  }

  // load textures
  state->smallCactusSprite = LoadTexture("assets/smallCactus.png");
  state->bigCactusSprite = LoadTexture("assets/bigCactus.png");
  state->ptSprite = LoadTexture("assets/pterodatyl.png");
  state->trexSprite = LoadTexture("assets/trex.png");
  state->trexDuckSprite = LoadTexture("assets/trexDuck.png");
  state->cloudSprite = LoadTexture("assets/cloud.png");
  state->groundSprite = LoadTexture("assets/ground.png");

#if defined(PLATFORM_WEB) // web platform
  emscripten_set_main_loop_arg(main_loop, state, 0, 1);
#else // normal platform
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    main_loop(state);
  }
#endif

  // unload textures
  UnloadTexture(state->smallCactusSprite);
  UnloadTexture(state->bigCactusSprite);
  UnloadTexture(state->ptSprite);
  UnloadTexture(state->trexSprite);
  UnloadTexture(state->trexDuckSprite);
  UnloadTexture(state->cloudSprite);
  UnloadTexture(state->groundSprite);

  delete state;
  CloseWindow();
  return 0;
}

// @brief This function is executed constantly. It observes the environment,
// makes a decision, updates the physics and trains the AI if there is a
// collision.
void updateDrawFrame(double deltaTime, GameWorld *game, MLP *mlp,
                     int &generation, vector<vector<double>> &X,
                     vector<vector<double>> &Y, int speed_multiplier) {
  int SPEED_UP = speed_multiplier;

  // runs the game several times per visual frame to speed up training
  for (int i = 0; i < SPEED_UP; ++i) {
    vector<double> x = game->getInputs();
    X.push_back(x); // stores the observation

    vector<double> action_pred = mlp->predict(x); // predicts the action

    // if there is no immediate danger, the "correct" action is to keep running
    if (game->getObstacles().empty() || game->getTimeToCollision() > 1.2) {
      Y.push_back({0, 1, 0});
    } else {
      Y.push_back(action_pred); // otherwise, store what the brain decided to do
    }

    // avoids RAM overflow by limiting the history to 20,000 records
    if (X.size() > RAM_MAX_RECORDS) {
      X.erase(X.begin());
      Y.erase(Y.begin());
    }

    // finds the neuron with the highest activation
    int max_idx = 0;
    if (action_pred[1] > action_pred[max_idx])
      max_idx = 1;
    if (action_pred[2] > action_pred[max_idx])
      max_idx = 2;

    // convert the winner neuron index to an action
    int action = 0;
    if (max_idx == 0)
      action = -1; // winner neuron 0 -> duck
    else if (max_idx == 1)
      action = 0; // winner neuron 1 -> run
    else
      action = 1; // winner neuron 2 -> jump

    // updates the physics
    game->update(deltaTime, action);

    // punishes and trains (if the dinosaur dies)
    if (game->getGameOverState()) {
      vector<double> action_correct;

      Obstacle obstacle = game->getObstacles().front();
      TRex trex = game->getTrex();

      if (trex.getVelY() > 0) {
        // collided with a obstacle while falling
        // correct action (for the past frames): Fast Fall!
        action_correct = {1, 0, 0}; // duck
      } else if (obstacle.getY() + obstacle.getHeight() <
                 trex.getY() + trex.getHeight()) {
        // collided with a bird (duck solves the problem)
        action_correct = {1, 0, 0}; // duck
      } else {
        // collided with a cactus (Jump solves the problem)
        action_correct = {0, 0, 1}; // Jump
      }

      vector<vector<double>> batch_X; // mini-batch for training (inputs)
      vector<vector<double>>
          batch_Y; // mini-batch for training (correct outputs)

      // how many frames to correct (15 in this case)
      int back_frames = (Y.size() > 15) ? 15 : Y.size();
      for (int j = back_frames; j > 0; --j) {
        // overwrite the history with the correct output
        Y[Y.size() - j] = action_correct;
        batch_Y.push_back(action_correct);  // the same for mini-batch outputs
        batch_X.push_back(X[X.size() - j]); // add the last input
      }

      // expericence replay: adds random frames to the mini-batch (prevents
      // catastrophic forgetting)
      int batch_size = (X.size() > 512) ? 512 : X.size();
      for (int i = back_frames; i < batch_size; ++i) {
        int rand_idx = GetRandomValue(0, X.size() - 1);
        batch_X.push_back(X[rand_idx]);
        batch_Y.push_back(Y[rand_idx]);
      }

      mlp->train(batch_X, batch_Y, EPOCHS);

      // resurrect the dinosaur for the next life (now a bit smarter)
      game->resetGame();
      generation++;
    }
  }
}

void drawNeuralNetwork(MLP *mlp, const vector<double> &inputs, int startX,
                       int startY) {
  // visual configurations
  int layerSpacingX = 200;
  int maxNetworkHeight = 340; // espaço vertical máximo para a rede no dashboard

  // gets mlp topology
  const vector<Layer> &layers = mlp->getLayers();

  vector<int> topology;
  topology.push_back(inputs.size());
  for (const Layer &l : layers) {
    topology.push_back(l.getNeurons().size());
  }

  vector<Vector2> prevLayerNodes;

  // labels
  const char *inputLabels[] = {"Distance", "Velocity", "[Obstacle] Height",
                               "[Obstacle] Width", "[TRex] Velocity (y)"};
  const char *outputLabels[] = {"Duck", "Run", "Jump"};

  // draw layers
  for (int i = 0; i < topology.size(); ++i) {
    // Cálculo dinâmico para não vazar a tela (fanning out effect)
    int currentSpacingY =
        maxNetworkHeight / (topology[i] > 1 ? topology[i] : 2);
    if (currentSpacingY > 45)
      currentSpacingY = 45; // limite máximo

    int currentRadius = currentSpacingY / 3;
    if (currentRadius > 12)
      currentRadius = 12;
    if (currentRadius < 3)
      currentRadius = 3;

    int layerHeight = (topology[i] - 1) * currentSpacingY;
    int currentY = startY - layerHeight / 2;
    int currentX = startX + i * layerSpacingX;

    vector<Vector2> currentLayerNodes;

    // get nodes positions
    for (int j = 0; j < topology[i]; ++j) {
      currentLayerNodes.push_back(
          {(float)currentX, (float)(currentY + j * currentSpacingY)});
    }

    // draw connections
    if (i > 0) {
      const Layer &layer = layers[i - 1];
      const vector<Neuron> &neurons = layer.getNeurons();

      // Ajuste de opacidade para evitar borrões gigantes de linhas
      float opacity = 0.4f;
      int totalConnections = neurons.size() * prevLayerNodes.size();
      if (totalConnections > 50)
        opacity = 0.15f;
      if (totalConnections > 150)
        opacity = 0.05f; // 16x16 precisa de opacidade bem baixa

      for (int n = 0; n < neurons.size(); ++n) {
        const vector<double> &w = neurons[n].getWeights();
        Vector2 cNode = currentLayerNodes[n];

        for (int p = 0; p < prevLayerNodes.size(); ++p) {
          Vector2 pNode = prevLayerNodes[p];
          double weight = w[p];
          Color lineColor = (weight > 0.0) ? GREEN : RED;

          float thickness = (float)(std::abs(weight) * 1.5);
          if (thickness > 4.0f)
            thickness = 4.0f;

          DrawLineEx(pNode, cNode, thickness, Fade(lineColor, opacity));
        }
      }
    }

    // draw nodes
    for (int j = 0; j < currentLayerNodes.size(); ++j) {
      Vector2 cNode = currentLayerNodes[j];

      // get activation
      double activation = 0.0;
      if (i == 0) {
        activation = inputs[j];
      } else {
        activation = layers[i - 1].getNeurons()[j].getActivation();
      }

      // clamp activation
      if (activation < 0.0)
        activation = 0.0;
      if (activation > 1.0)
        activation = 1.0;

      // glow effect for high activations
      if (activation > 0.5) {
        float glowRadius = currentRadius + (activation * 15.0f);
        DrawCircleGradient(cNode.x, cNode.y, glowRadius,
                           Fade(YELLOW, activation * 0.8f), Fade(YELLOW, 0.0f));
      }

      Color nodeColor = {(unsigned char)(0), (unsigned char)(0),
                         (unsigned char)(activation * 255), 255};

      DrawCircleV(cNode, currentRadius, nodeColor);
      DrawCircleLinesV(cNode, currentRadius, BLACK);

      // draw Labels
      if (i == 0 && j < 5) { // input layer (5 labels)
        DrawText(TextFormat("%s: %.2f", inputLabels[j], inputs[j]),
                 cNode.x - 300, cNode.y - 10, 20, DARKGRAY);
        DrawLine(cNode.x - 40, cNode.y, cNode.x - 15, cNode.y,
                 DARKGRAY);                           // arrow line
      } else if (i == topology.size() - 1 && j < 3) { // output layer
        Color textColor = (activation > 0.8) ? BLACK : GRAY;
        DrawText(outputLabels[j], cNode.x + 40, cNode.y - 10, 20, textColor);
        DrawLine(cNode.x + 15, cNode.y, cNode.x + 30, cNode.y,
                 DARKGRAY); // arrow line
      }
    }

    prevLayerNodes = currentLayerNodes;
  }
}