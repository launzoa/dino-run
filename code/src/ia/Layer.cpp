#include "headers/Layer.h"

using namespace std;

// Construtor da Camada
// Recruta uma equipe de 'num_neurons', todos preparados para receber 'input_size' informações.
Layer::Layer(int input_size, int num_neurons) {
  for (int i = 0; i < num_neurons; ++i) {
    this->neurons.push_back(Neuron(input_size));
  }

  this->x = vector<double>(input_size);
  this->y = vector<double>(num_neurons);
}

Layer::~Layer() {};

// Propagação (Raciocínio)
// Imagine uma linha de montagem: a entrada 'x' chega, cada neurônio analisa a peça
// e carimba sua ativação. O vetor 'y' é a bandeja final com os carimbos de todos.
vector<double> Layer::forward(const vector<double> &x) {
  this->x = x; // Arquivamos a entrada atual para usá-la mais tarde na fase de treino

  for (int i = 0; i < this->neurons.size(); ++i) {
    this->neurons[i].soma(x);              // Cada neurônio faz sua soma ponderada
    this->y[i] = this->neurons[i].sigmoid(); // E aplica a função de disparo
  }
  return this->y; // O pulso total desta camada, pronto para a próxima!
}

// Retropropagação (A "Bronca" Coletiva)
// Esta função repassa o erro de trás para frente. É como um gerente recebendo 
// uma bronca (err) do cliente e distribuindo para seus funcionários (neurônios),
// antes de repassar a bronca acumulada para os fornecedores de peças (camada anterior).
vector<double> Layer::backward(const vector<double> &err, double lr) {
  vector<double> err_back(this->x.size(), 0.0);

  // Para cada neurônio da equipe:
  for (int i = 0; i < this->neurons.size(); ++i) {
    // 1. Calcula a sua culpa específica
    this->neurons[i].calculate_delta(err[i]);
    
    // 2. Aprende a lição (Muda seus próprios pesos usando a entrada original guardada 'x')
    this->neurons[i].update_weights(this->x, lr);

    // 3. Empacota a culpa que deve ser repassada para a camada anterior
    vector<double> delta_err = this->neurons[i].delta_error();
    for (int j = 0; j < delta_err.size(); ++j) {
      err_back[j] += delta_err[j];
    }
  }

  // Devolve o erro cumulativo para continuar descendo a cadeia de comando
  return err_back;
}

const std::vector<Neuron>& Layer::getNeurons() const {
  return this->neurons;
}