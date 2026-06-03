#include "headers/Layer.h"

using namespace std;

Layer::Layer(int input_size, int num_neurons) {
  for (int i = 0; i < num_neurons; ++i) {
    this->neurons.push_back(Neuron(input_size));
  }

  this->x = vector<double>(input_size);
  this->y = vector<double>(num_neurons);
}

Layer::~Layer() {};

vector<double> Layer::forward(const vector<double> &x) {
  this->x = x;

  for (int i = 0; i < this->neurons.size(); ++i) {
    this->neurons[i].soma(x);
    this->y[i] = this->neurons[i].sigmoid();
  }
  return this->y;
}

vector<double> Layer::backward(const vector<double> &err, double lr) {
  vector<double> err_back(this->x.size(), 0.0);

  for (int i = 0; i < this->neurons.size(); ++i) {
    this->neurons[i].calculate_delta(err[i]);
    this->neurons[i].update_weights(this->x, lr);

    vector<double> delta_err = this->neurons[i].delta_error();
    for (int j = 0; j < delta_err.size(); ++j) {
      err_back[j] += delta_err[j];
    }
  }

  return err_back;
}

const std::vector<Neuron>& Layer::getNeurons() const {
  return this->neurons;
}