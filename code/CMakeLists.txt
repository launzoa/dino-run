cmake_minimum_required(VERSION 3.14)
project(TRex_Perceptron CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Busca automaticamente todos os arquivos .cpp dentro da pasta src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Integração automática da biblioteca Raylib usando FetchContent.
# Isso garante que a biblioteca será baixada e compilada sem você precisar instalar nada manualmente no Linux.
if (EMSCRIPTEN)
    set(PLATFORM "Web" CACHE STRING "Web platform" FORCE)
endif()

include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
)
FetchContent_MakeAvailable(raylib)

# Cria o executável do jogo
add_executable(dino_run ${SOURCES})

# Adiciona os diretórios de include para que o compilador ache as headers (headers/TRex.h, etc)
target_include_directories(dino_run PRIVATE src/core/headers src/ia/headers)

# Linca o jogo com a biblioteca Raylib
target_link_libraries(dino_run PRIVATE raylib)

# Configurações específicas para WebAssembly (Emscripten)
if (EMSCRIPTEN)
    set_target_properties(dino_run PROPERTIES SUFFIX ".html")
    target_link_options(dino_run PRIVATE "-sUSE_GLFW=3")
    target_link_options(dino_run PRIVATE --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets)
endif()
