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
#include <vector>
#include <algorithm>

#ifdef _WIN32
	#include <cstdlib>
	static inline void CLEAR_SCREEN() { int drop = system("cls"); (void)drop; }
#else
	#include <cstdlib>
	static inline void CLEAR_SCREEN() { int drop = system("clear"); (void)drop; }
#endif

#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_MAGENTA "\033[35m"

/**
 * Decodifica uma string HTML, substituindo entidades HTML por seus caracteres correspondentes.
 *
 * Parâmetros:
 * const std::string& html: A string HTML a ser decodificada.
 *
 * Retorna std::string: A string decodificada.
 */
std::string decode_html(const std::string& html) {
	std::string decoded;
	decoded.reserve(html.size());

	for (size_t i = 0; i < html.size(); ++i) {
		if (html[i] == '&') {
			if (html.compare(i, 5, "&amp;") == 0) {
				decoded += '&';
				i += 4;
			} else if (html.compare(i, 4, "&lt;") == 0) {
				decoded += '<';
				i += 3;
			} else if (html.compare(i, 4, "&gt;") == 0) {
				decoded += '>';
				i += 3;
			} else if (html.compare(i, 6, "&quot;") == 0) {
				decoded += '"';
				i += 5;
			} else if (html.compare(i, 6, "&apos;") == 0) {
				decoded += '\'';
				i += 5;
			} else {
				decoded += '&';
			}
		} else {
			decoded += html[i];
		}
	}

	return decoded;
}

/**
 * Exibe um cabeçalho estilizado para uma janela.
 *
 * Parâmetros:
 * const std::string& title: O título do cabeçalho.
 */
void display_header(const std::string& title) {
	std::cout << COLOR_CYAN << "==============================" << COLOR_RESET << '\n';
	std::cout << COLOR_BOLD << "       " << title << "          " << COLOR_RESET << '\n';
	std::cout << COLOR_CYAN << "==============================" << COLOR_RESET << "\n\n";
}

/**
 * Exibe uma mensagem formatada com cor opcional.
 *
 * Parâmetros:
 * const std::string& msg: A mensagem a ser exibida.
 * const std::string& color: O código de cor ANSI (opcional).
 */
void display_msg(const std::string& msg, const std::string& color) {
	if (!color.empty())
		std::cout << color;
	std::cout << msg << COLOR_RESET << '\n';
}

/**
 * Exibe uma mensagem de erro.
 *
 * Parâmetros:
 * const std::string& msg: A mensagem de erro.
 */
void display_error(const std::string& msg) {
	display_msg("[ERRO] " + msg, COLOR_RED);
}

/**
 * Exibe uma mensagem de sucesso.
 *
 * Parâmetros:
 * const std::string& msg: A mensagem de sucesso.
 */
void display_success(const std::string& msg) {
	display_msg("[OK] " + msg, COLOR_GREEN);
}

/**
 * Limpa o buffer de entrada.
 */
void clear_input() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * Obtém uma escolha numérica do usuário dentro de um intervalo.
 *
 * Parâmetros:
 * int min: O valor mínimo permitido.
 * int max: O valor máximo permitido.
 *
 * Retorna int: A escolha do usuário. Retorna -1 se a entrada for inválida.
 */
int get_choice(int min, int max) {
	std::string input;
	std::getline(std::cin, input);

	if (input.empty() || input.length() > 1)
		return -1;

	int choice = input[0] - '0';
	return (choice >= min && choice <= max) ? choice : -1;
}

/**
 * Aguarda o usuário pressionar Enter para continuar.
 */
void wait_enter() {
	std::cout << "\nPressione Enter para continuar...";
	std::cin.get();
}

/**
 * Remove espaços em branco das extremidades de uma string.
 *
 * Parâmetros:
 * const std::string& str: A string a ser processada.
 *
 * Retorna std::string: A string sem espaços nas extremidades.
 */
std::string trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\n\r");
	if (start == std::string::npos)
		return "";

	size_t end = str.find_last_not_of(" \t\n\r");
	return str.substr(start, end - start + 1);
}

/**
 * Exibe o título de um artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML do artigo.
 */
void show_title(const std::string& html_content) {
	std::string title = regex_core::extract_wikipedia_title(html_content);
	std::string decoded = decode_html(title);

	if (!decoded.empty()) {
		CLEAR_SCREEN();
		display_header("Título do Artigo");
		std::cout << COLOR_BOLD << decoded << COLOR_RESET << "\n";
		std::cout << COLOR_CYAN << "==============================" << COLOR_RESET << "\n";
	} else {
		display_error("Falha ao extrair o título do artigo.");
	}

	wait_enter();
}

/**
 * Exibe o sumário (TOC) de um artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML do artigo.
 */
void show_toc(const std::string& html_content) {
	auto toc_items = regex_core::extract_wikipedia_toc(html_content);

	// Decodifica os títulos
	for (auto& item : toc_items)
		item.second = decode_html(item.second);

	if (!toc_items.empty()) {
		CLEAR_SCREEN();
		display_header("Sumário do Artigo");

		// Calcula largura máxima para alinhamento
		size_t max_width = 0;
		for (const auto& item : toc_items)
			max_width = std::max(max_width, item.first.length());

		// Exibe itens formatados
		for (const auto& item : toc_items) {
			std::cout << std::right << std::setw(max_width + 2)
					  << item.first << "  "
					  << std::left << item.second << '\n';
		}

		std::cout << COLOR_CYAN << "==============================" << COLOR_RESET << "\n";
	} else {
		display_error("Nenhum item de sumário encontrado.");
	}

	wait_enter();
}

/**
 * Exibe as imagens de um artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML do artigo.
 */
void show_images(const std::string& html_content) {
	auto image_items = regex_core::extract_wikipedia_images(html_content);

	// Decodifica os textos alternativos
	for (auto& item : image_items)
		item.second = decode_html(item.second);

	if (!image_items.empty()) {
		CLEAR_SCREEN();
		display_header("Imagens do Artigo");

		// Limitar a exibição para não sobrecarregar
		size_t max_display = std::min(image_items.size(), size_t(20));

		std::cout << COLOR_YELLOW << "Total de imagens encontradas: " << image_items.size() << COLOR_RESET << "\n\n";

		// Calcula largura máxima para alinhamento
		size_t max_width = 0;
		for (size_t i = 0; i < max_display; ++i)
			max_width = std::max(max_width, std::to_string(i + 1).length());

		// Exibe itens formatados
		for (size_t i = 0; i < max_display; ++i) {
			const auto& item = image_items[i];
			std::cout << std::right << std::setw(max_width + 2)
					  << (i + 1) << ". "
					  << std::left << COLOR_CYAN << "URL: " << COLOR_RESET
					  << item.first << "\n";
			if (!item.second.empty()) {
				std::cout << std::setw(max_width + 4) << " "
						  << COLOR_MAGENTA << "Alt: " << COLOR_RESET
						  << item.second << "\n";
			}
			std::cout << "\n";
		}

		if (image_items.size() > max_display) {
			std::cout << COLOR_YELLOW << "... e mais "
					  << (image_items.size() - max_display)
					  << " imagens não exibidas." << COLOR_RESET << "\n";
		}

		std::cout << COLOR_CYAN << "==============================" << COLOR_RESET << "\n";
	} else {
		display_error("Nenhuma imagem encontrada no artigo.");
	}

	wait_enter();
}

/**
 * Exibe e processa o menu do artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML do artigo.
 *
 * Retorna bool: true se deve continuar no menu do artigo, false se deve voltar.
 */
bool article_menu(const std::string& html_content) {
	if (html_content.empty()) {
		display_error("Nenhum conteúdo de artigo disponível.");
		wait_enter();
		return false;
	}

	CLEAR_SCREEN();
	display_header("Artigo da Wikipedia");

	std::cout << COLOR_YELLOW << "Escolha uma opção:" << COLOR_RESET << '\n';
	std::cout << "  1. Extrair título do artigo\n";
	std::cout << "  2. Extrair sumário (TOC) do artigo\n";
	std::cout << "  3. Listar imagens do artigo\n";
	std::cout << "  4. Voltar ao menu principal\n";
	std::cout << "Opção: ";

	switch (get_choice(1, 4)) {
		case 1: show_title(html_content); return true;
		case 2: show_toc(html_content); return true;
		case 3: show_images(html_content); return true;
		case 4: return false;
		default:
			display_error("Opção inválida. Tente novamente.");
			clear_input();
			return true;
	}
}

/**
 * Processa a entrada da URL pelo usuário e gerencia o fluxo do artigo.
 */
void handle_url_input() {
	std::cout << "Digite a URL: ";
	std::string url;
	std::getline(std::cin, url);

	url = trim(url);

	if (regex_core::is_wikipedia_url(url)) {
		std::string content = fetch_url::fetch_url(url);
		if (!content.empty()) {
			display_success("Conteúdo HTML obtido com sucesso!");

			// Loop do menu do artigo
			while (article_menu(content)) {
				// Continua no menu do artigo
			}
		} else {
			display_error("Falha ao obter o conteúdo HTML da URL.");
		}
	} else {
		display_error("A URL fornecida não é uma URL válida da Wikipedia.");
	}

	wait_enter();
}

/**
 * Exibe e processa o menu principal do programa.
 *
 * Retorna bool: true se deve continuar no menu principal, false se deve sair.
 */
bool main_menu() {
	CLEAR_SCREEN();
	display_header("Regex Scraper");

	std::cout << COLOR_YELLOW << "Escolha uma opção:" << COLOR_RESET << '\n';
	std::cout << "  1. Buscar URL\n";
	std::cout << "  2. Sair\n";
	std::cout << "Opção: ";

	switch (get_choice(1, 2)) {
		case 1:
			handle_url_input();
			return true;
		case 2:
			display_msg("Saindo do programa...");
			return false;
		default:
			display_error("Opção inválida. Tente novamente.");
			clear_input();
			return true;
	}
}

/**
 * Loop principal do programa.
 */
void main_loop() {
	while (main_menu()) {
		// Continua no menu principal
	}
}
