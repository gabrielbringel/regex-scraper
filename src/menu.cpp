/**
 * menu.cpp
 * Autor: Miguel Mochizuki Silva
 * Descrição: Implementação de funções para exibição de menus e interação com o usuário.
 */
#include "menu.hpp"
#include "fetch_url.hpp"
#include "regex_core.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <iomanip>

/**
 * Exibe o menu principal do programa.
 */
void display_menu(void) {
	std::cout << "==============================" << std::endl;
	std::cout << "       Regex Scraper          " << std::endl;
	std::cout << "==============================\n\n" << std::endl;
}

/**
 * Obtém a escolha do usuário no menu principal.
 *
 * Retorna int: A escolha do usuário como um número inteiro. Retorna -1 se a entrada for inválida.
 */
int get_user_choice(void) {
	std::string input;
	std::cout << "Escolha uma opção:" << std::endl;
	std::cout << "1. Buscar URL" << std::endl;
	std::cout << "2. Sair" << std::endl;
	std::cout << "Opção: ";

	std::getline(std::cin, input);

	if (input.empty() || input.length() > 1) {
		return -1;
	}

	if (input[0] < '1' || input[0] > '2') {
		return -1;
	}

	return input[0] - '0';
}

/**
 * Loop principal do programa, que exibe o menu e processa a escolha do usuário.
 */
void main_loop(void) {
	while (true) {
		display_menu();
		int choice = get_user_choice();

		switch (choice) {
			case 1: {
				std::string url;
				std::cout << "Digite a URL: ";
				std::getline(std::cin, url);

				// Remover espaços em branco do início e fim
				size_t start = url.find_first_not_of(" \t\n\r");
				if (start != std::string::npos) {
					url = url.substr(start);
				}

				size_t end = url.find_last_not_of(" \t\n\r");
				if (end != std::string::npos) {
					url = url.substr(0, end + 1);
				}

				if (regex_core::is_wikipedia_url(url)) {
					std::string html_content = fetch_url::fetch_url(url);
					if (!html_content.empty()) {
						std::cout << "Conteúdo HTML obtido com sucesso!" << std::endl;
						article_loop(html_content);
					} else {
						std::cout << "Falha ao obter o conteúdo HTML da URL." << std::endl;
					}
				} else {
					std::cout << "A URL fornecida não é uma URL válida da Wikipedia." << std::endl;
				}
				break;
			}
			case 2:
				std::cout << "Saindo do programa..." << std::endl;
				return;
			default:
				std::cout << "Opção inválida. Tente novamente." << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}

/**
 * Exibe o menu de opções para um dado artigo da Wikipedia.
 */
void display_article_menu(void) {
	std::cout << "\n\n==============================" << std::endl;
	std::cout << "       Artigo da Wikipedia     " << std::endl;
	std::cout << "==============================\n\n" << std::endl;
}

/**
 * Obtém a escolha do usuário no menu de opções do artigo.
 *
 * Retorna int: A escolha do usuário como um número inteiro. Retorna -1 se a entrada for inválida.
 */
int get_article_choice(void) {
	std::string input;
	std::cout << "Escolha uma opção:" << std::endl;
	std::cout << "1. Extrair título do artigo" << std::endl;
	std::cout << "2. Extrair sumário (TOC) do artigo" << std::endl;
	std::cout << "3. Voltar ao menu principal" << std::endl;
	std::cout << "Opção: ";

	std::getline(std::cin, input);

	if (input.empty() || input.length() > 1) {
		return -1;
	}

	if (input[0] < '1' || input[0] > '3') {
		return -1;
	}

	return input[0] - '0';
}

/**
 * Loop para processar o conteúdo HTML de um artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML do artigo da Wikipedia.
 */
void article_loop(const std::string& html_content) {
	while (true) {
		display_article_menu();
		int choice = get_article_choice();

		switch (choice) {
			case 1: {
				std::string title = regex_core::extract_wikipedia_title(html_content);
				display_article_title(title);
				break;
			}
			case 2: {
				std::vector<std::pair<std::string, std::string>> toc_items =
					regex_core::extract_wikipedia_toc(html_content);
				display_toc(toc_items);
				break;
			}
			case 3:
				return;
			default:
				std::cout << "Opção inválida. Tente novamente." << std::endl;
		}
	}
}

/**
 * Exibe o título do artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& title: O título do artigo.
 */
void display_article_title(const std::string& title) {
	if (!title.empty()) {
		std::cout << "\n==============================" << std::endl;
		std::cout << "         Título do Artigo      " << std::endl;
		std::cout << "==============================" << std::endl;
		std::cout << title << std::endl;
		std::cout << "==============================\n" << std::endl;
	} else {
		std::cout << "Falha ao extrair o título do artigo." << std::endl;
	}
}

/**
 * Exibe o sumário (TOC) do artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::vector<std::pair<std::string, std::string>>& toc_items: Um vetor de pares contendo os itens do sumário,
 * onde cada par consiste em um número de seção e o texto do título.
 */
void display_toc(const std::vector<std::pair<std::string, std::string>>& toc_items) {
	if (toc_items.empty()) {
		std::cout << "Nenhum item de sumário encontrado." << std::endl;
		return;
	}

	std::cout << "\n==============================" << std::endl;
	std::cout << "       Sumário do Artigo       " << std::endl;
	std::cout << "==============================" << std::endl;

	// Encontrar o tamanho máximo do número para alinhamento
	size_t max_number_width = 0;
	for (const auto& item : toc_items) {
		max_number_width = std::max(max_number_width, item.first.length());
	}

	// Exibir cada item com formatação consistente
	for (const auto& item : toc_items) {
		// Alinhar o número à direita e o título à esquerda
		std::cout << std::right << std::setw(max_number_width + 2)
				  << item.first << "  "
				  << std::left << item.second << std::endl;
	}

	std::cout << "==============================\n" << std::endl;
}
