#include "core/headers/GameWorld.h"
#include "core/headers/constants.h"
#include "ia/headers/MLP.h"
#include <vector>
#include <cmath>
#include <raylib.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

using namespace std;

struct Cloud {
  float x;
  float y;
  float speed;
};

struct AppState {
  GameWorld game;
  MLP mlp;
  vector<vector<double>> X;
  vector<vector<double>> Y;
  int generation = 1;
  int generationTotal = 200;
  int speed_multiplier = 1;
  vector<Cloud> clouds;
  float groundX = 0.0f;
  Texture2D smallCactusSprite;
  Texture2D bigCactusSprite;
  Texture2D ptSprite;
  Texture2D trexSprite;
  Texture2D trexDuckSprite;
  Texture2D cloudSprite;
  Texture2D groundSprite;

  AppState() : mlp({INPUT_NEURONS, HIDDEN_NEURONS, OUTPUT_NEURONS}, LEARNING_RATE) {}
};

void updateDrawFrame(double deltaTime, GameWorld *game, MLP *mlp,
                     int &generation, int generationTotal,
                     vector<vector<double>> &X, vector<vector<double>> &Y,
                     int speed_multiplier);

void drawNeuralNetwork(MLP *mlp, const vector<double> &inputs, int startX,
                       int startY);

void main_loop(void* arg) {
  AppState* state = (AppState*)arg;
  float deltaTime = GetFrameTime();

  Vector2 mousePoint = GetMousePosition();
  Rectangle btnMinus = {WINDOW_WIDTH - 300, 10, 40, 40};
  Rectangle btnPlus = {WINDOW_WIDTH - 150, 10, 40, 40};

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (CheckCollisionPointRec(mousePoint, btnPlus)) {
      state->speed_multiplier += 10;
    } else if (CheckCollisionPointRec(mousePoint, btnMinus)) {
      state->speed_multiplier = (state->speed_multiplier > 1) ? state->speed_multiplier - 10 : 1;
    }
  }

  state->game.timeDecrement(deltaTime);

  BeginDrawing();
  ClearBackground(RAYWHITE);

  updateDrawFrame(deltaTime, &state->game, &state->mlp, state->generation, state->generationTotal, state->X, state->Y, state->speed_multiplier);

  DrawText(TextFormat("Generation: %i", state->generation), 10, 10, 20, BLACK);
  DrawText(TextFormat("Speed: x%i", state->speed_multiplier), WINDOW_WIDTH - 250, 20, 20, BLACK);

  DrawRectangleRec(btnMinus, LIGHTGRAY);
  DrawText("-", btnMinus.x + 15, btnMinus.y + 10, 20, BLACK);
  DrawRectangleRec(btnPlus, LIGHTGRAY);
  DrawText("+", btnPlus.x + 15, btnPlus.y + 10, 20, BLACK);

  DrawLine(0, 450, WINDOW_WIDTH, 450, LIGHTGRAY);
  DrawText("DASHBOARD NEURAL", 10, 40, 30, DARKGRAY);

  drawNeuralNetwork(&state->mlp, state->game.getInputs(), WINDOW_WIDTH / 2 - 150, 280);

  for (Cloud &cloud : state->clouds) {
    cloud.x -= cloud.speed * deltaTime * state->speed_multiplier;
    if (cloud.x < -state->cloudSprite.width) {
      cloud.x = WINDOW_WIDTH + GetRandomValue(10, 200);
      cloud.y = GetRandomValue(460, 600);
    }
    DrawTexture(state->cloudSprite, cloud.x, cloud.y, WHITE);
  }

  state->groundX -= state->game.getInputs()[1] * 1000.0 * deltaTime * state->speed_multiplier;
  if (state->groundX <= -state->groundSprite.width) {
    state->groundX = 0.0f;
  }
  DrawTexture(state->groundSprite, state->groundX, GROUND_Y, WHITE);
  DrawTexture(state->groundSprite, state->groundX + state->groundSprite.width, GROUND_Y, WHITE);

  TRex trex = state->game.getTrex();
  Texture2D currentTrexSprite = (trex.getHeight() < TREX_HEIGHT) ? state->trexDuckSprite : state->trexSprite;
  Rectangle trex_source = {0, 0, (float)currentTrexSprite.width, (float)currentTrexSprite.height};
  Rectangle trex_dest = {
      (float)(trex.getX() - (currentTrexSprite.width - trex.getWidth()) / 2.0),
      (float)(trex.getY() + trex.getHeight() - currentTrexSprite.height),
      (float)currentTrexSprite.width, (float)currentTrexSprite.height};
  DrawTexturePro(currentTrexSprite, trex_source, trex_dest, {0, 0}, 0.0f, WHITE);
  DrawRectangle(trex.getX(), trex.getY(), trex.getWidth(), trex.getHeight(), Fade(GREEN, 0.2f));
  DrawRectangleLines(trex.getX(), trex.getY(), trex.getWidth(), trex.getHeight(), GREEN);

  deque<Obstacle> obstacles = state->game.getObstacles();
  for (Obstacle obstacle : obstacles) {
    Texture2D currentSprite;
    if (obstacle.getWidth() == SMALL_CACTUS_WIDTH) currentSprite = state->smallCactusSprite;
    else if (obstacle.getWidth() == BIG_CACTUS_WIDTH) currentSprite = state->bigCactusSprite;
    else currentSprite = state->ptSprite;

    Rectangle source_rect = {0, 0, (float)currentSprite.width, (float)currentSprite.height};
    Rectangle dest_rect = {
        (float)(obstacle.getX() - (currentSprite.width - obstacle.getWidth()) / 2.0),
        (float)(obstacle.getY() + obstacle.getHeight() - currentSprite.height),
        (float)currentSprite.width, (float)currentSprite.height};

    DrawTexturePro(currentSprite, source_rect, dest_rect, {0, 0}, 0.0f, WHITE);
    DrawRectangle(obstacle.getX(), obstacle.getY(), obstacle.getWidth(), obstacle.getHeight(), Fade(RED, 0.2f));
    DrawRectangleLines(obstacle.getX(), obstacle.getY(), obstacle.getWidth(), obstacle.getHeight(), RED);
  }

  EndDrawing();
}

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Neural Network TRex");

  AppState* state = new AppState();
  
  for (int i = 0; i < 5; ++i) {
    state->clouds.push_back(Cloud{(float)(i * 300 + 2000), (float)GetRandomValue(460, 600),
                           (float)(200 + GetRandomValue(-50, 100))});
  }

  state->smallCactusSprite = LoadTexture("assets/smallCactus.png");
  state->bigCactusSprite = LoadTexture("assets/bigCactus.png");
  state->ptSprite = LoadTexture("assets/pterodatyl.png");
  state->trexSprite = LoadTexture("assets/trex.png");
  state->trexDuckSprite = LoadTexture("assets/trexDuck.png");
  state->cloudSprite = LoadTexture("assets/cloud.png");
  state->groundSprite = LoadTexture("assets/ground.png");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(main_loop, state, 0, 1);
#else
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    main_loop(state);
  }
#endif

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

void updateDrawFrame(double deltaTime, GameWorld *game, MLP *mlp,
                     int &generation, int generationTotal,
                     vector<vector<double>> &X, vector<vector<double>> &Y,
                     int speed_multiplier) {
  int SPEED_UP = speed_multiplier;

  for (int i = 0; i < SPEED_UP; ++i) {
    vector<double> x = game->getInputs();
    X.push_back(x);

    vector<double> action_pred = mlp->predict(x);
    if (game->getObstacles().empty() || game->getTimeToCollision() > 1.2) {
      Y.push_back({0, 1, 0});
    } else {
      Y.push_back(action_pred);
    }

    if (X.size() > 10000) {
      X.erase(X.begin());
      Y.erase(Y.begin());
    }

    int max_idx = 0;
    if (action_pred[1] > action_pred[max_idx])
      max_idx = 1;
    if (action_pred[2] > action_pred[max_idx])
      max_idx = 2;

    int action = 0;
    if (max_idx == 0)
      action = -1; // duck
    else if (max_idx == 1)
      action = 0; // run
    else
      action = 1; // jump

    game->update(deltaTime, action);

    if (game->getGameOverState()) {
      vector<double> action_correct = {0, 0, 1}; // jump default
      if (game->getObstacles().front().getWidth() == PTERODACTYL_WIDTH) {
        action_correct = {1, 0, 0}; // duck
      }

      vector<vector<double>> batch_X;
      vector<vector<double>> batch_Y;

      int back_frames = (Y.size() > 15) ? 15 : Y.size();
      for (int j = 1; j <= back_frames; ++j) {
        Y[Y.size() - j] = action_correct;
        batch_X.push_back(X[X.size() - j]);
        batch_Y.push_back(action_correct);
      }

      // Complete o lote com memórias aleatórias (A essência do Experience
      // Replay!)
      int batch_size = (X.size() > 256) ? 256 : X.size();
      for (int i = back_frames; i < batch_size; ++i) {
        int rand_idx = GetRandomValue(0, X.size() - 1);
        batch_X.push_back(X[rand_idx]);
        batch_Y.push_back(Y[rand_idx]);
      }

      mlp->train(batch_X, batch_Y, EPOCHS);

      game->resetGame();
      generation++;
    }
  }
}

void drawNeuralNetwork(MLP *mlp, const vector<double> &inputs, int startX,
                       int startY) {
  int layerSpacingX = 200;
  int nodeSpacingY = 45;
  int radius = 12;

  const vector<Layer> &layers = mlp->getLayers();

  vector<int> topology;
  topology.push_back(inputs.size());
  for (const Layer &l : layers) {
    topology.push_back(l.getNeurons().size());
  }

  vector<Vector2> prevLayerNodes;

  const char *inputLabels[] = {"Distance", "Velocity", "[Obstacle] Height (y)",
                               "[Obstacle] Width (x)"};
  const char *outputLabels[] = {"Duck", "Run", "Jump"};

  for (int i = 0; i < topology.size(); ++i) {
    int layerHeight = (topology[i] - 1) * nodeSpacingY;
    int currentY = startY - layerHeight / 2;
    int currentX = startX + i * layerSpacingX;

    vector<Vector2> currentLayerNodes;

    for (int j = 0; j < topology[i]; ++j) {
      currentLayerNodes.push_back(
          {(float)currentX, (float)(currentY + j * nodeSpacingY)});
    }

    if (i > 0) {
      const Layer &layer = layers[i - 1];
      const vector<Neuron> &neurons = layer.getNeurons();

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

          DrawLineEx(pNode, cNode, thickness, Fade(lineColor, 0.4f));
        }
      }
    }

    for (int j = 0; j < currentLayerNodes.size(); ++j) {
      Vector2 cNode = currentLayerNodes[j];

      double activation = 0.0;
      if (i == 0) {
        activation = inputs[j];
      } else {
        activation = layers[i - 1].getNeurons()[j].getActivation();
      }

      if (activation < 0.0)
        activation = 0.0;
      if (activation > 1.0)
        activation = 1.0;

      // Glow effect for high activations
      if (activation > 0.5) {
        float glowRadius = radius + (activation * 15.0f);
        DrawCircleGradient(cNode.x, cNode.y, glowRadius,
                           Fade(YELLOW, activation * 0.8f), Fade(YELLOW, 0.0f));
      }

      Color nodeColor = {(unsigned char)(0), (unsigned char)(0),
                         (unsigned char)(activation * 255), 255};

      DrawCircleV(cNode, radius, nodeColor);
      DrawCircleLinesV(cNode, radius, BLACK);

      // Draw Labels
      if (i == 0 && j < 4) { // Input layer
        DrawText(TextFormat("%s: %.2f", inputLabels[j], inputs[j]),
                 cNode.x - 300, cNode.y - 10, 20, DARKGRAY);
        DrawLine(cNode.x - 40, cNode.y, cNode.x - 15, cNode.y,
                 DARKGRAY);                           // Arrow line
      } else if (i == topology.size() - 1 && j < 3) { // Output layer
        Color textColor = (activation > 0.8) ? BLACK : GRAY;
        DrawText(outputLabels[j], cNode.x + 40, cNode.y - 10, 20, textColor);
        DrawLine(cNode.x + 15, cNode.y, cNode.x + 30, cNode.y,
                 DARKGRAY); // Arrow line
      }
    }

    prevLayerNodes = currentLayerNodes;
  }
}