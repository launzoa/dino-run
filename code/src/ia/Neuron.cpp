#include "headers/Neuron.h"
#include <cmath>
#include <random>

using namespace std;

// Construtor: Um neurônio nasce sem saber de nada.
// Atribuímos pesos (w) e viés (b) totalmente aleatórios (distribuição entre -1 e 1).
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

// A Sinapse Matemática (Soma Ponderada)
// Analogia: "Eu devo pular? Depende... o cacto está perto (x0) e meu peso para isso (w0) é forte?"
void Neuron::soma(const vector<double> &x) {
  this->u = this->b; // Inicia com o Viés (Bias)

  // Multiplica cada entrada pela sua respectiva importância (Peso) e acumula
  for (int i = 0; i < x.size(); ++i) {
    this->u += this->w[i] * x[i];
  }
}

// A Função de Ativação (Sigmoid)
// Transforma qualquer número livre em uma probabilidade estritamente entre 0.0 e 1.0.
double Neuron::sigmoid() { return 1.0 / (1.0 + exp(-this->u)); }

// Derivada da Sigmoid
// Em cálculo diferencial, a derivada mostra a inclinação. Precisamos dela
// para saber o quão rápido devemos alterar os pesos na descida do gradiente.
double Neuron::sigmoid_derivative() {
  return this->sigmoid() * (1.0 - this->sigmoid());
}

// Calcula o Delta (A parcela de culpa deste neurônio pelo erro)
// Erro total * Derivada local = "Minha contribuição para termos batido no cacto"
void Neuron::calculate_delta(double err) {
  this->delta = err * this->sigmoid_derivative();
}

// Repassa o cálculo da culpa para a camada anterior (Essência do Backpropagation)
vector<double> Neuron::delta_error() {
  vector<double> delta_err;
  for (int i = 0; i < w.size(); ++i) {
    delta_err.push_back(this->delta * this->w[i]);
  }
  return delta_err;
}

// O Aprendizado Efetivo (Descida do Gradiente / Gradient Descent)
// Muda as sinapses (Pesos) na direção contrária do erro.
// 'lr' é o Learning Rate (Taxa de Aprendizado) - o tamanho do passo que damos a cada treino.
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
