#!/bin/bash

# Cria o diretório de build web se não existir
mkdir -p build_web
cd build_web

# Carrega as variaveis de ambiente do Emscripten
source ~/dev/emsdk/emsdk_env.sh

# Configura o CMake usando a toolchain do Emscripten
echo "Configurando o CMake para WebAssembly..."
emcmake cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5

# Compila o projeto
echo "Compilando para WebAssembly..."
emmake make -j4

# Se a compilação foi bem sucedida, roda o servidor HTTP Python local
if [ $? -eq 0 ]; then
    echo ""
    echo "================================================="
    echo "Compilação concluída com sucesso!"
    echo "Servidor rodando em http://localhost:8080"
    echo "Pressione Ctrl+C para parar o servidor."
    echo "================================================="
    python3 -m http.server 8080
else
    echo "Erro na compilação!"
fi
