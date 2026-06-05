#pragma once
#include "Layer.h"

// ============================================================================
// CLASSE MLP (Multilayer Perceptron / O Cérebro Completo)
// ============================================================================
// Esta classe é a orquestradora final. Ela empilha várias camadas (Layers)
// e coordena o fluxo de dados entre elas, do input até o output.
class MLP {
private:
  std::vector<Layer> layers; // As camadas empilhadas do nosso cérebro
  double lr;                 // Learning Rate: A rapidez com que a IA muda de ideia (pesos)

public:
  // Inicializa o cérebro com uma 'topologia' (ex: {4, 16, 3} -> 4 entradas, 16 ocultos, 3 saídas)
  MLP(const std::vector<int> &topology, double lr);
  ~MLP();

  // A Sala de Aula: Repete várias vezes (num_epochs) o treino usando os pares de [Visão] e [Resposta Certa]
  void train(const std::vector<std::vector<double>> &X,
             const std::vector<std::vector<double>> &y, int num_epochs);
             
  // O Exame Prático: Recebe o que a IA está vendo e cospe a ação probabilística
  std::vector<double> predict(const std::vector<double> &x);
  
  // Acesso para desenharmos a rede na tela
  const std::vector<Layer>& getLayers() const;
};