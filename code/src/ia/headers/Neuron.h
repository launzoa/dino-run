#pragma once
#include <vector>

class Neuron {
private:
  std::vector<double> w;
  double b;
  double u;
  double delta;

public:
  Neuron(int input_size);
  ~Neuron();

  void soma(const std::vector<double> &x);
  double sigmoid();
  double sigmoid_derivative();
  void calculate_delta(double err);
  std::vector<double> delta_error();
  void update_weights(const std::vector<double> &x, double lr);

  const std::vector<double>& getWeights() const;
  double getActivation() const;
};
