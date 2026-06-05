#pragma once
#include "Neuron.h"
#include <vector>

// ============================================================================
// CLASSE LAYER (Camada de Neurônios)
// ============================================================================
// Uma rede neural é formada por camadas. Esta classe agrupa vários neurônios
// que processam informações paralelamente. O trabalho em equipe deles
// é o que permite à rede aprender padrões complexos.
class Layer {
private:
  std::vector<Neuron> neurons; // A equipe de neurônios desta camada
  std::vector<double> x;       // As entradas que a camada recebeu (Memória de curto prazo para o treino)
  std::vector<double> y;       // As saídas produzidas (Para passar para a próxima camada)

public:
  // Inicializa a camada definindo quantos dados entram e quantos neurônios existem nela
  Layer(int input_size, int num_neurons);
  ~Layer();

  // O fluxo de raciocínio (Da esquerda para a direita)
  // Recebe dados, passa por todos os neurônios e gera o pulso da camada
  std::vector<double> forward(const std::vector<double> &x);
  
  // O fluxo de aprendizado (Da direita para a esquerda - Backpropagation)
  // Repassa a culpa do erro para a camada anterior e treina os neurônios locais
  std::vector<double> backward(const std::vector<double> &err, double lr);
  
  // Acesso de leitura visual
  const std::vector<Neuron>& getNeurons() const;
};