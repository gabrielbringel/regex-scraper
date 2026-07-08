/**
 * menu.hpp
 * Autor: Miguel Mochizuki Silva
 * Descrição: Protótipos de funções para exibição de menus e interação com o usuário.
 */
#ifndef MENU_HPP
#define MENU_HPP

#include <string>
#include <vector>
#include <utility>

/**
 * Exibe o menu principal do programa.
 */
void display_menu(void);

/**
 * Obtém a escolha do usuário no menu principal.
 *
 * Retorna int: A escolha do usuário como um número inteiro. Retorna -1 se a entrada for inválida.
 */
int get_user_choice(void);

/**
 * Loop principal do programa, que exibe o menu e processa a escolha do usuário.
 */
void main_loop(void);

/**
 * Exibe o menu de opções para um dado artigo da Wikipedia.
 */
void display_article_menu(void);

/**
 * Obtém a escolha do usuário no menu de opções do artigo.
 *
 * Retorna int: A escolha do usuário como um número inteiro. Retorna -1 se a entrada for inválida.
 */
int get_article_choice(void);

/**
 * Loop para processar o conteúdo HTML de um artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML do artigo da Wikipedia.
 */
void article_loop(const std::string& html_content);

/**
 * Exibe o título do artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& title: O título do artigo.
 */
void display_article_title(const std::string& title);

/**
 * Exibe o sumário (TOC) do artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::vector<std::pair<std::string, std::string>>& toc_items: Um vetor de pares contendo os itens do sumário,
 * onde cada par consiste em um número de seção e o texto do título.
 */
void display_toc(const std::vector<std::pair<std::string, std::string>>& toc_items);

#endif
