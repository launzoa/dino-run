#include "headers/MLP.h"

using namespace std;

// O Arquiteto da Rede
// Ele lê a topologia {4, 16, 3} e constrói as pontes (Layers) entre eles.
// Teremos uma Layer que recebe 4 e sai 16, e outra que recebe 16 e sai 3.
MLP::MLP(const vector<int> &topology, double lr) {
  this->lr = lr;

  for (int i = 0; i < topology.size() - 1; ++i) {
    this->layers.push_back(Layer(topology[i], topology[i + 1]));
  }
}

MLP::~MLP() {}

// A Fase de Treinamento Intensivo (O verdadeiro Backpropagation)
// X = Histórico do que a IA viu; y = Histórico do que ela devia ter feito.
void MLP::train(const vector<vector<double>> &X,
                const vector<vector<double>> &y, int num_epochs) {

  // Epócas: Quantas vezes vamos repassar todo o material de estudo
  for (int epoch = 0; epoch < num_epochs; ++epoch) {
    // Para cada memória do nosso histórico (batch):
    for (int i = 0; i < X.size(); ++i) {
      vector<double> output = X[i];
      
      // 1. FORWARD: Pede para a rede prever com os pesos atuais
      for (Layer &layer : this->layers) {
        output = layer.forward(output);
      }

      // 2. CALCULAR O ERRO BRUTO (Resposta Certa - Resposta da Rede)
      vector<double> err_back;
      for (int j = 0; j < y[i].size(); ++j) {
        err_back.push_back(y[i][j] - output[j]);
      }

      // 3. BACKWARD: Envia a bronca de trás para frente!
      // Começa da última camada (Saída) e vai retrocedendo até a primeira (Oculta)
      for (int k = this->layers.size() - 1; k >= 0; --k) {
        err_back = this->layers[k].backward(err_back, this->lr);
      }
    }
  }
}

// O Raciocínio (Predição)
// Usado enquanto o jogo roda vivo para tomar a decisão do próximo pulo.
vector<double> MLP::predict(const vector<double> &x) {
  vector<double> y = x;
  // Repassa a informação sequencialmente por todas as camadas
  for (Layer &layer : this->layers) {
    y = layer.forward(y);
  }

  return y; // Retorna a bandeja final (ex: as probabilidades de [Abaixar, Correr, Pular])
}

const std::vector<Layer>& MLP::getLayers() const {
  return this->layers;
}