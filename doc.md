# Documentação de Código: Dino-Run IA / `main.cpp`

## 1. Visão Geral
Imagine que você tem um amigo que nunca jogou o famoso "Jogo do Dinossauro" do Google Chrome (aquele que aparece quando a internet cai) e você precisa ensiná-lo a jogar do zero. Este código faz exatamente isso, mas o "amigo" em questão é uma Inteligência Artificial construída inteiramente do zero.

Neste projeto, não estamos usando bibliotecas gigantes e "mágicas" como TensorFlow ou PyTorch. Estamos sujando as mãos e implementando uma **Rede Neural Perceptron Multicamadas (MLP)** de forma nativa em C++. A escolha de fazer isso "na unha" tem um propósito educacional claro: **desmistificar a caixa-preta da Inteligência Artificial**. 

De forma macro, o arquivo `main.cpp` atua como o grande **"Maestro" (ou Controlador)** da nossa aplicação. O problema central que ele resolve é **orquestrar o treinamento em tempo real da IA enquanto o jogo roda**, garantindo que a renderização visual e os cálculos matemáticos complexos aconteçam em harmonia, a 60 quadros por segundo. 

Para isso, este arquivo une três pilares fundamentais:
1. **O Motor Gráfico e Multimídia (Raylib):** Responsável por desenhar os cactos, o dinossauro, o chão em movimento e, brilhantemente, o "Dashboard Neural", que desenha os neurônios e conexões da IA na tela em tempo real.
2. **A Lógica e Física do Jogo (`GameWorld`):** As regras imutáveis do universo do jogo. A gravidade, a velocidade com que os obstáculos se aproximam e o sistema de colisão. A IA não pode "trapacear" essas regras; ela deve se adaptar a elas.
3. **O Cérebro Computacional (`MLP`):** O modelo matemático que tenta encontrar padrões. Ele coleta informações do ambiente (onde estão os cactos?), decide uma ação (pular, abaixar ou correr) e aprende com seus próprios erros quando morre no jogo.

A união destes pilares cria um ambiente de **Aprendizado por Reforço (Reinforcement Learning)** prático. A IA começa o jogo de forma caótica, pulando e se abaixando aleatoriamente, mas, a cada "Game Over", o maestro entra em ação: ele pausa o jogo momentaneamente, analisa os últimos passos, aplica uma penalidade pelo erro e ajusta as conexões neurais. O ciclo então se repete (nova geração), e o dinossauro acorda um pouco mais sábio do que na vida anterior.

## 2. Arquitetura e Fluxo de Dados

A arquitetura deste projeto simula perfeitamente um **Arco Reflexo Biológico**, onde o sistema recebe um estímulo, processa a informação e executa uma resposta motora. Em termos de Engenharia de Software, organizamos isso como um fluxo contínuo de dados (Pipeline), executado dezenas de vezes por segundo.

O fluxo de dados se divide em quatro estágios cruciais:

1. **A Percepção de Estado (Os "Olhos" do Dino):** 
   A cada quadro (frame), o motor do jogo congela o tempo por um milissegundo e extrai exatamente 4 variáveis críticas. Por que apenas 4? Na Inteligência Artificial, chamamos isso de "Espaço de Estados" (State Space). O objetivo é passar apenas o estritamente necessário para tomar uma decisão, evitando sobrecarregar a rede com "ruído" inútil (como a posição das nuvens, que são meramente estéticas).
   - **Distância até o próximo obstáculo:** O quão perto o perigo está?
   - **Velocidade do jogo:** O quão rápido preciso reagir? (Lembre-se: o jogo acelera com o tempo).
   - **Altura do obstáculo:** É um cacto baixo ou um pterodáctilo alto?
   - **Largura do obstáculo:** É apenas um cacto ou três juntos? Preciso de um pulo longo?

2. **O Processamento (O "Cérebro" - Forward Pass):** 
   Esses 4 números flutuantes (floats) formam um vetor que é injetado na camada de entrada da Rede Neural (`MLP`). A partir daí, ocorre a propagação para frente (Forward Pass): os valores são multiplicados por "pesos" (weights) e somados, passando pelas camadas ocultas. Pense nisso como a água fluindo por canos de diferentes espessuras. No final, a "pressão" da água acende uma das três luzes de saída (os neurônios finais): **Pular, Abaixar ou Continuar Correndo**.

3. **A Execução e o Arquivamento (O "Corpo" e a "Memória"):** 
   O dinossauro executa a ação que obteve a maior probabilidade matemática no estágio anterior. Independentemente do resultado imediato, essa fração de segundo é arquivada:
   - A entrada (o que a IA viu) é salva no vetor `X`.
   - A previsão (o que a IA decidiu) é salva no vetor `Y`.
   Isso cria um diário contínuo, um conjunto de dados vivo (*Dataset*) que cresce enquanto o jogo roda.

4. **A Retroalimentação e Punição (A "Lição" - Backpropagation):** 
   Se o dinossauro desviar do obstáculo, o ciclo recomeça silenciosamente. Mas, se ele colidir (Game Over), a sirene toca! O fluxo normal é interrompido. O algoritmo vai até o "diário", pega os últimos quadros gravados e sobrescreve a ação que falhou com o que **deveria ter sido feito** (ex: "Aqui, em vez de correr, o certo era pular"). 
   
   Com essa "correção" em mãos (o erro calculado), o código aciona a função de Treino (`mlp->train`). A rede neural então ajusta os "canos" (pesos) matematicamente para que, da próxima vez que vir uma situação parecida, a água flua para o lado certo. Finalmente, o jogo reseta e a **próxima geração** de IA nasce, ligeiramente menos propensa a repetir aquele erro estúpido.

## 3. Mergulho Profundo no Código

### O Gerenciador de Estado Global: `AppState`
Em vez de espalhar dezenas de variáveis globais pelo arquivo (uma péssima prática arquitetural em C++), o desenvolvedor agrupou todo o ecossistema do jogo em uma única estrutura `struct`.

```cpp
struct AppState {
  GameWorld game; // Instância com as regras e física do jogo
  MLP mlp;        // O cérebro da IA (inicializado com as camadas)
  vector<vector<double>> X; // Memória: O que a IA viu (Entradas) ao longo do tempo
  vector<vector<double>> Y; // Memória: O que a IA decidiu (Saídas)
  int generation = 1;
  int speed_multiplier = 1;
  // ... (texturas e sprites de renderização omitidos)
};
```
**Análise Pedagógica:** Isso é análogo a ter uma "Mochila de Acampamento". Em vez de carregar a barraca, o saco de dormir e a panela soltos nas mãos, você coloca tudo na mochila (`AppState`) e carrega apenas ela. 
* **Vantagem em Memória:** Ao passar um único ponteiro de `AppState` para a função de renderização, economizamos um custo altíssimo de cópia de memória, um conceito vital quando lidamos com 60 quadros por segundo. Observe que as matrizes `X` e `Y` formam o nosso *Dataset* ao vivo, que crescerá conforme o jogo roda.

### O "Coração Pulsante": `updateDrawFrame`
Esta é a função mais crítica do arquivo, executada em loop constante. Ela intercala predição, ação e correção. Vamos quebrar a desconstrução lógica dela:

**Fase A: Observação e Predição**
```cpp
vector<double> x = game->getInputs();
X.push_back(x); // Arquiva o que foi visto

vector<double> action_pred = mlp->predict(x); 
```
Neste bloco, o jogo consulta o ambiente (`getInputs`). O vetor `action_pred` devolvido pelo método `predict` contém valores de *probabilidade*. Por exemplo: `[0.1, 0.8, 0.1]` (10% de chance de ser melhor abaixar, 80% correr, 10% pular). O código subsequentemente procura o maior valor (`max_idx`) e o converte na ação final que o motor gráfico entende (-1, 0 ou 1).

**Fase B: Avaliação de Risco e Punição**
```cpp
game->update(deltaTime, action); // Roda a física por um frame

if (game->getGameOverState()) {
  vector<double> action_correct = {0, 0, 1}; // Padrão: "Bateu? Devia ter pulado!"
  if (game->getObstacles().front().getWidth() == PTERODACTYL_WIDTH) {
    action_correct = {1, 0, 0}; // Ajuste de regra de negócio: Se o inimigo for largo, é um pássaro, logo abaixe!
  }
```
**O Porquê:** A rede neural não tem a mínima ideia do que é um cacto ou um pássaro. Ela descobre pela dor da colisão! A punição retroativa injeta a regra de negócio central: se morremos agora, a ação correta para a situação imediatamente anterior deveria ter sido diferente da que foi tomada.

### A Magia da Retenção: Experience Replay
Se ensinarmos um aluno apenas corrigindo rigorosamente o último erro isolado que ele cometeu, ele esquecerá as lições antigas devido ao superajuste (Overfitting). Em IA, chamamos isso de **Esquecimento Catastrófico**. O código resolve isso maravilhosamente:

```cpp
  // 1. Resgata e "corrige" os últimos 15 frames que levaram à morte
  for (int j = 1; j <= back_frames; ++j) {
    Y[Y.size() - j] = action_correct;
    batch_X.push_back(X[X.size() - j]);
    batch_Y.push_back(action_correct);
  }

  // 2. Preenche o restante do lote (Batch) com memórias antigas aleatórias
  for (int i = back_frames; i < batch_size; ++i) {
    int rand_idx = GetRandomValue(0, X.size() - 1);
    batch_X.push_back(X[rand_idx]);
    batch_Y.push_back(Y[rand_idx]);
  }

  mlp->train(batch_X, batch_Y, EPOCHS); 
```
**Análise Pedagógica:** Criamos um "lote de estudos" (Batch) de tamanho 256. Colocamos as questões que o dinossauro *acabou de errar* (os últimos quadros pré-colisão) misturadas com várias questões antigas sorteadas do histórico de acertos que ele já tinha vivenciado. Ao chamar `mlp->train` com este lote variado, a rede aprende a pular o novo cacto **sem desaprender** que deve continuar correndo na pista reta!

### O Revelador de Segredos: `drawNeuralNetwork`
Geralmente, Redes Neurais são tratadas como "Caixas Pretas". Entra um dado, sai uma ação, e o que ocorre no meio é invisível. Este código quebra esse tabu visualmente com a **Raylib**:

* **Espessura da Linha:** Representa a magnitude de uma conexão sináptica (quanto maior o peso matemático `weight`, mais grossa a linha).
* **Cor da Linha:** Linhas Verdes transmitem valores positivos (estimulam a ação seguinte). Linhas Vermelhas transmitem valores negativos (inibem a ação).
* **Brilho do Neurônio (Glow):** Quando a ativação de um neurônio ultrapassa `0.5`, o código pinta gradientes luminosos em volta do nó simulando pulsos elétricos: `DrawCircleGradient(..., Fade(YELLOW, activation))`. O desenvolvedor/jogador finalmente **enxerga** o cérebro pensando frame a frame!

## 4. Padrões e Aprendizados

Para fechar nosso estudo, vamos compilar as lições de Engenharia de Software e Inteligência Artificial que este código nos ensina de forma prática:

### 1. Separação de Preocupações (Game Loop Pattern)
O código aplica rigorosamente o padrão **Update/Draw**. Toda a matemática brutal da rede neural, a movimentação da gravidade e as colisões acontecem dentro de `updateDrawFrame` antes de qualquer pixel ser desenhado. 
* **O que o aluno aprende:** Nunca misture regras de negócio (ou treinamento de IA) com comandos de renderização (`DrawTexture`). Essa separação permite que a IA "pense" milhares de vezes mais rápido nos bastidores se desligarmos a tela, o que é crucial em treinamentos reais.

### 2. A Desmistificação do Aprendizado por Reforço
Muitos cursos começam ensinando IA importando bibliotecas pesadas como TensorFlow ou PyTorch, que escondem toda a matemática. 
* **O que o aluno aprende:** O *Experience Replay* implementado aqui mostra, de forma crua, que uma IA aprende construindo um histórico (o Dataset `X` e `Y`), limitando seu tamanho máximo para economizar RAM (`10.000` registros) e sorteando amostras desse histórico (`mini-batches` de 256 instâncias) para atualizar seus pesos gradativamente. O aprendizado é estatística contínua, não mágica instantânea.

### 3. Injeção de Dependências por Ponteiros no C++
Em C++, a passagem de matrizes gigantes por valor copia os dados na memória, matando a performance. O código resolve isso usando ponteiros: `(GameWorld *game, MLP *mlp)`.
* **O que o aluno aprende:** Funções que atuam como orquestradoras não devem "possuir" os dados, mas apenas receber as chaves de acesso a eles (ponteiros). Isso permite que a IA modifique diretamente o "Cérebro" original na memória principal sem sobrecarga computacional.

### 4. Flexibilidade de Arquitetura (Web vs Desktop)
Na função principal `main()`, o uso das diretivas do pré-processador `#if defined(PLATFORM_WEB)` encapsula lógicas de loop diferentes.
* **O que o aluno aprende:** Projetos modernos de alto desempenho em C++ não estão mais presos a executáveis `.exe` do Windows. Com tecnologias como WebAssembly (através do Emscripten), podemos pegar um código com IA e física, compilá-lo, e fazê-lo rodar nativamente dentro da aba de qualquer navegador web, provando que um bom código é inerentemente portável.

## 5. Evolução Cognitiva: Dominando o "Fast Fall"

### 5.1. O Objetivo
Ensinar a IA do jogo Dino Run a dominar a mecânica de **"Fast Fall"** (Abaixar enquanto está no ar). Essa mecânica é crítica para que o dinossauro consiga aterrissar mais rápido após um pulo e esteja preparado para o próximo obstáculo, especialmente quando a velocidade do jogo atinge níveis altíssimos. Adicionalmente, queremos que a IA consiga identificar e reagir corretamente a inimigos voadores (Pterodáctilos) em diferentes alturas.

### 5.2. A Primeira Decisão: Arquitetura de Inputs (Consciência Corporal e Visão)
A primeira grande mudança arquitetural é a reestruturação da "visão" da nossa Rede Neural. Para tomar decisões complexas, a IA precisa de informações melhores do ambiente e de si mesma.

Em vez de criar booleanos simples ("Estou no ar?" ou "É um Pássaro?"), optamos por entregar variáveis contínuas baseadas na física.

**A Nova Matriz de Inputs:**
1. **Tempo de Colisão (Distância / Velocidade):** Em vez de usar apenas a distância em pixels, calcular o *tempo* para o impacto normaliza a dificuldade gerada pela aceleração progressiva do jogo. 100 pixels no nível 1 é muito diferente de 100 pixels no nível 10.
2. **Largura do Obstáculo:** Necessário para saber quanto tempo o Dino precisa ficar no ar.
3. **Altura (Y) do Obstáculo:** Vital. É isso que diz se o Pterodáctilo voa alto (pode passar correndo/agachando), no meio (deve agachar) ou baixo (deve pular). Cactos sempre terão Y fixo no chão.
4. **Velocidade Y do T-Rex (Consciência Corporal):** Indica a inércia atual. O Dino está subindo no pulo (Y Vel positivo) ou já começou a cair (Y Vel negativo)?
5. **Altura Y do T-Rex (Consciência Corporal):** Indica o quão longe do chão o Dino está.

**Por que essa decisão foi tomada?**
Dar à IA a `Velocidade Y` e a `Altura Y` permite que a rede neural faça relações matemáticas de parábola. Ela entende que *se* a altura está diminuindo (Velocidade Y negativa) e a altura atual é 'H', logo ela chegará no chão. A rede descobre a relação de causa e efeito sem precisarmos codificar regras literais nela.

### 5.3. A Segunda Decisão: Arquitetura de Supervisão (A Regra de Ouro)
Como a IA aprende por retropropagação (treinamento retroativo), ela precisa de um "Professor" (o motor do jogo) que diga a ela o que ela *deveria* ter feito ao bater em um obstáculo.

A Regra de Ouro anterior era simples e cega: *"Se bateu, a ação correta era pular"*. Isso causava mortes repetidas, pois pássaros começaram a nascer em três alturas dinâmicas (Altos, Médios e Baixos).

**A Nova Regra de Ouro (Física de Hitboxes):**
O Professor, diferente da IA, tem acesso onisciente a todas as variáveis e hitboxes do mapa. Quando a colisão ocorre, o Professor faz três checagens cruciais e matemáticas:
- **Passo 1 (O Fast Fall):** O Dino estava com a Velocidade Y positiva (ou seja, ele já estava *caindo* de um pulo) e bateu?
  - *Lógica:* Se ele estava caindo em cima de um obstáculo, é porque o pulo demorou demais para terminar. Se ele tivesse forçado a queda antes, ele estaria a salvo.
  - *Lição ensinada:* "Ação Correta = Abaixar (Fast Fall)".
- **Passo 2 (Aves Médias vs Cactos):** Se não for o caso do Passo 1, o código compara a base (Pé) do obstáculo com a base do T-Rex. O "pé" do obstáculo é mais alto que o "pé" do Dino (`obstacle.Y + Height < trex.Y + Height`)?
  - *Lógica:* Se o obstáculo não toca o chão, ele é voador e tem potencial de bater na cabeça.
  - *Lição ensinada:* "Ação Correta = Abaixar".
- **Passo 3 (Obstáculos de Chão):** O obstáculo toca o chão (ex: Cactos ou Pterodáctilos rasantes)?
  - *Lógica:* Obstáculos no chão trituram o Dino se ele não pular.
  - *Lição ensinada:* "Ação Correta = Pular".

Dessa forma, a regra de correção deixou de ser codificada pelo "nome" do inimigo e passou a ser uma regra puramente universal baseada em Geometria.

### 5.4. A Terceira Decisão: Hyperparameter Tuning e Deep Learning
A inclusão de Pterodáctilos em três alturas diferentes somada à necessidade de escolher entre correr, pular ou usar o Fast Fall aumentou absurdamente a complexidade do "Espaço de Estados" (todas as combinações possíveis que a IA pode encontrar).

A rede neural original tinha apenas 1 camada oculta com 8 neurônios. Essa arquitetura era rasa demais ("Shallow") e atingiu seu **limite de capacidade representacional**. Em termos práticos: a rede não conseguia decorar todas as regras ao mesmo tempo sem esquecer as antigas.

Para dar à IA a inteligência necessária para vencer o jogo complexo, aplicamos as seguintes evoluções:
1. **Rede Profunda (Deep Learning):** A topologia foi alterada de `[5, 8, 3]` para `[5, 16, 16, 3]`. Agora a rede possui *duas* camadas ocultas. Isso permite a abstração de características: a primeira camada entende dados crus ("obstáculo está a 50px de distância") e a segunda deduz conceitos complexos ("o obstáculo está perto E é baixo, logo é um risco iminente de tropeço").
2. **Redução da Ansiedade (Learning Rate):** A taxa de aprendizado caiu de `0.1` para `0.02`. Um passo matemático menor e mais prudente evita a oscilação violenta de pesos (Esquecimento Catastrófico) durante os acertos do Experience Replay, construindo um aprendizado sólido.
