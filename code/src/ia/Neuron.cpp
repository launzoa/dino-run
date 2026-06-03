#include "headers/Neuron.h"
#include <cmath>
#include <random>

using namespace std;

Neuron::Neuron(int input_size) {
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<double> dist(-1.0, 1.0);

  this->b = dist(gen);
  for (int i = 0; i < input_size; i++) {
    this->w.push_back(dist(gen));
  }

  this->u = 0.0;
  this->delta = 0.0;
}

Neuron::~Neuron() {}

void Neuron::soma(const vector<double> &x) {
  this->u = this->b;

  for (int i = 0; i < x.size(); ++i) {
    this->u += this->w[i] * x[i];
  }
}

double Neuron::sigmoid() { return 1.0 / (1.0 + exp(-this->u)); }

double Neuron::sigmoid_derivative() {
  return this->sigmoid() * (1.0 - this->sigmoid());
}

void Neuron::calculate_delta(double err) {
  this->delta = err * this->sigmoid_derivative();
}

vector<double> Neuron::delta_error() {
  vector<double> delta_err;
  for (int i = 0; i < w.size(); ++i) {
    delta_err.push_back(this->delta * this->w[i]);
  }
  return delta_err;
}

void Neuron::update_weights(const vector<double> &x, double lr) {
  for (int i = 0; i < x.size(); ++i) {
    this->w[i] += lr * this->delta * x[i];
  }
  this->b += lr * this->delta;
}

const std::vector<double>& Neuron::getWeights() const {
  return this->w;
}

double Neuron::getActivation() const {
  return 1.0 / (1.0 + exp(-this->u));
}
