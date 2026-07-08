# Estágio 1: Build
FROM alpine:latest AS builder

# Instala dependências de build
RUN apk add --no-cache \
    g++ \
    cmake \
    make \
    curl-dev \
    openssl-dev \
    build-base

# Define o diretório de trabalho
WORKDIR /app

# Copia os arquivos do projeto
COPY . .

# Cria um diretório de build e compila
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make

# Estágio 2: Runtime (opcional, para imagem menor)
FROM alpine:latest

# Instala dependências de runtime
RUN apk add --no-cache \
    libstdc++ \
    curl-dev \
    openssl-dev

# Define o diretório de trabalho
WORKDIR /app

# Copia o binário compilado do estágio de build
COPY --from=builder /app/build/regex_scraper /app/regex_scraper

# Torna o binário executável
RUN chmod +x /app/regex_scraper

# Define o comando padrão
CMD ["./regex_scraper"]
