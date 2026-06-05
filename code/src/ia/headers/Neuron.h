#pragma once
#include <vector>

// ============================================================================
// CLASSE NEURON (A Unidade Básica do Cérebro)
// ============================================================================
// Na biologia, um neurônio recebe sinais, processa e dispara um pulso.
// Na matemática, ele é apenas um contêiner para uma soma ponderada.
class Neuron {
private:
  std::vector<double> w; // Pesos (Weights): A importância de cada conexão de entrada
  double b;              // Viés (Bias): O limiar de disparo padrão (mesmo sem entradas)
  double u;              // Potencial de Ativação: O resultado bruto da soma
  double delta;          // Erro local (Delta): Usado no treinamento (Backpropagation) para saber o erro

public:
  Neuron(int input_size);
  ~Neuron();

  // Fase 1: Recebe o estímulo e calcula o "pulso" bruto
  void soma(const std::vector<double> &x);
  
  // Fase 2: Converte o pulso bruto em uma probabilidade suave entre 0.0 e 1.0
  double sigmoid();
  
  // Derivada matemática: Usada para saber "para qual lado" os pesos devem ser ajustados
  double sigmoid_derivative();
  
  // Fase de Treino (Backpropagation): Calcula a culpa deste neurônio pelo erro
  void calculate_delta(double err);
  std::vector<double> delta_error();
  
  // O Aprendizado Real: Altera os pesos para não cometer o mesmo erro no futuro
  void update_weights(const std::vector<double> &x, double lr);

  // Funções de leitura para renderização visual
  const std::vector<double>& getWeights() const;
  double getActivation() const;
};
