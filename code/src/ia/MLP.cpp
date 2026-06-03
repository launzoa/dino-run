#include "headers/MLP.h"

using namespace std;

MLP::MLP(const vector<int> &topology, double lr) {
  this->lr = lr;

  for (int i = 0; i < topology.size() - 1; ++i) {
    this->layers.push_back(Layer(topology[i], topology[i + 1]));
  }
}

MLP::~MLP() {}

void MLP::train(const vector<vector<double>> &X,
                const vector<vector<double>> &y, int num_epochs) {

  for (int epoch = 0; epoch < num_epochs; ++epoch) {
    for (int i = 0; i < X.size(); ++i) {
      vector<double> output = X[i];
      for (Layer &layer : this->layers) {
        output = layer.forward(output);
      }

      vector<double> err_back;
      for (int j = 0; j < y[i].size(); ++j) {
        err_back.push_back(y[i][j] - output[j]);
      }

      for (int k = this->layers.size() - 1; k >= 0; --k) {
        err_back = this->layers[k].backward(err_back, this->lr);
      }
    }
  }
}

vector<double> MLP::predict(const vector<double> &x) {
  vector<double> y = x;
  for (Layer &layer : this->layers) {
    y = layer.forward(y);
  }

  return y;
}

const std::vector<Layer>& MLP::getLayers() const {
  return this->layers;
}