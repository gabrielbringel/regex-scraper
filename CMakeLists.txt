cmake_minimum_required(VERSION 3.10)
project(regex_scraper VERSION 1.0)

# Configuração C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar libcurl
find_package(CURL REQUIRED)

# Incluir diretórios de cabeçalho
include_directories(
    ${CURL_INCLUDE_DIRS}
    include
)

# Criar executável
add_executable(regex_scraper
    src/main.cpp
    src/fetch_url.cpp
    src/regex_core.cpp
    src/menu.cpp
)

# Linkar bibliotecas
target_link_libraries(regex_scraper
    ${CURL_LIBRARIES}
)

# Configurações de compilação
target_compile_options(regex_scraper PRIVATE
    -Wall -Wextra -Wpedantic
)

# Configurações específicas de build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(regex_scraper PRIVATE -g -O0)
else()
    target_compile_options(regex_scraper PRIVATE -O2)
endif()

# Mensagem de sucesso
message(STATUS "Projeto ${PROJECT_NAME} configurado com sucesso!")
