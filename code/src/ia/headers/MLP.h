#pragma once
#include "Layer.h"

class MLP {
private:
  std::vector<Layer> layers;
  double lr;

public:
  MLP(const std::vector<int> &topology, double lr);
  ~MLP();

  void train(const std::vector<std::vector<double>> &X,
             const std::vector<std::vector<double>> &y, int num_epochs);
  std::vector<double> predict(const std::vector<double> &x);
  
  const std::vector<Layer>& getLayers() const;
};