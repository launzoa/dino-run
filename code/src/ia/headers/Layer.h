#pragma once
#include "Neuron.h"
#include <vector>

class Layer {
private:
  std::vector<Neuron> neurons;
  std::vector<double> x;
  std::vector<double> y;

public:
  Layer(int input_size, int num_neurons);
  ~Layer();

  std::vector<double> forward(const std::vector<double> &x);
  std::vector<double> backward(const std::vector<double> &err, double lr);
  
  const std::vector<Neuron>& getNeurons() const;
};