# ==============================
# Makefile para regex-scraper
# ==============================

# Variáveis
PROJECT_NAME := regex_scraper
DOCKER_IMAGE := $(PROJECT_NAME)
DOCKER_CONTAINER := $(PROJECT_NAME)-container
BUILD_DIR := build
BINARY := $(BUILD_DIR)/$(PROJECT_NAME)

# Cores para output
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[0;33m
BLUE := \033[0;34m
NC := \033[0m # No Color

# ==============================
# Build targets
# ==============================

.PHONY: build-local
build-local: ## Compila o projeto localmente
	@printf "$(BLUE)Compilando projeto localmente...$(NC)\n"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && make
	@printf "$(GREEN)Build concluído!$(NC)\n"
	@printf "$(GREEN)Binário: $(BINARY)$(NC)\n"

.PHONY: build-docker
build-docker: ## Compila o projeto usando Docker
	@printf "$(BLUE)Construindo imagem Docker...$(NC)\n"
	@docker build -t $(DOCKER_IMAGE) .
	@printf "$(GREEN)Imagem Docker construída com sucesso!$(NC)\n"
	@printf "$(GREEN)Imagem: $(DOCKER_IMAGE)$(NC)\n"

# ==============================
# Run targets
# ==============================

.PHONY: run-local
run-local: build-local ## Executa o projeto localmente
	@printf "$(BLUE)Executando $(PROJECT_NAME) localmente...$(NC)\n"
	@./$(BINARY) $(ARGS)

.PHONY: run-docker
run-docker: ## Executa o projeto usando Docker (usa imagem existente ou constrói se não existir)
	@printf "$(BLUE)Executando $(PROJECT_NAME) no Docker...$(NC)\n"
	@if docker image inspect $(DOCKER_IMAGE) >/dev/null 2>&1; then \
		printf "$(GREEN)Usando imagem existente: $(DOCKER_IMAGE)$(NC)\n"; \
	else \
		printf "$(YELLOW)Imagem não encontrada. Construindo...$(NC)\n"; \
		docker build -t $(DOCKER_IMAGE) .; \
		printf "$(GREEN)Imagem construída com sucesso!$(NC)\n"; \
	fi
	@docker run --rm -it \
		--name $(DOCKER_CONTAINER) \
		$(DOCKER_IMAGE) $(ARGS)

# ==============================
# Clean targets
# ==============================

.PHONY: clean-local
clean-local: ## Limpa arquivos de build local
	@printf "$(YELLOW)Limpando arquivos de build local...$(NC)\n"
	@rm -rf $(BUILD_DIR)
	@printf "$(GREEN)Limpeza local concluída!$(NC)\n"

.PHONY: clean-docker
clean-docker: ## Limpa imagens e containers Docker
	@printf "$(YELLOW)Removendo container Docker...$(NC)\n"
	@docker rm -f $(DOCKER_CONTAINER) 2>/dev/null || true
	@printf "$(YELLOW)Removendo imagem Docker...$(NC)\n"
	@docker rmi -f $(DOCKER_IMAGE) 2>/dev/null || true
	@printf "$(GREEN)Limpeza Docker concluída!$(NC)\n"

# ==============================
# Rebuild targets
# ==============================

.PHONY: rebuild-local
rebuild-local: clean-local build-local ## Recompila o projeto localmente

.PHONY: rebuild-docker
rebuild-docker: clean-docker build-docker ## Recompila o projeto usando Docker

# ==============================
# Help
# ==============================

.PHONY: help
help: ## Mostra esta ajuda
	@printf "$(BLUE)Uso: make [target] [ARGS='argumentos']$(NC)\n"
	@printf "\n"
	@printf "$(BLUE)Targets disponíveis:$(NC)\n"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
		awk -v green="$(GREEN)" -v nc="$(NC)" \
		'BEGIN {FS = ":.*?## "}; {printf "  %s%-22s%s %s\n", green, $$1, nc, $$2}'

# ==============================
# Development targets (extras)
# ==============================

.PHONY: shell-docker
shell-docker: ## Abre um shell no container Docker
	@printf "$(BLUE)Abrindo shell no container Docker...$(NC)\n"
	@docker run --rm -it \
		--name $(DOCKER_CONTAINER)-shell \
		$(DOCKER_IMAGE) /bin/sh

.PHONY: test-local
test-local: build-local ## Executa os testes localmente (se existirem)
	@printf "$(BLUE)Executando testes localmente...$(NC)\n"
	@cd $(BUILD_DIR) && ctest --output-on-failure || true

.PHONY: logs-docker
logs-docker: ## Mostra logs do container Docker
	@docker logs $(DOCKER_CONTAINER)

# ==============================
# Default target
# ==============================

.DEFAULT_GOAL := help
