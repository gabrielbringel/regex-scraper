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
 * Decodifica uma string HTML, substituindo entidades HTML por seus caracteres correspondentes.
 *
 * Parâmetros:
 * const std::string& html: A string HTML a ser decodificada.
 *
 * Retorna std::string: A string decodificada.
 */
std::string decode_html(const std::string& html);

/**
 * Exibe um cabeçalho estilizado para uma janela.
 *
 * Parâmetros:
 * const std::string& title: O título do cabeçalho.
 */
void display_header(const std::string& title);

/**
 * Exibe uma mensagem formatada com cor opcional.
 *
 * Parâmetros:
 * const std::string& msg: A mensagem a ser exibida.
 * const std::string& color: O código de cor ANSI (opcional).
 */
void display_msg(const std::string& msg, const std::string& color = "");

/**
 * Exibe uma mensagem de erro.
 *
 * Parâmetros:
 * const std::string& msg: A mensagem de erro.
 */
void display_error(const std::string& msg);

/**
 * Exibe uma mensagem de sucesso.
 *
 * Parâmetros:
 * const std::string& msg: A mensagem de sucesso.
 */
void display_success(const std::string& msg);

/**
 * Limpa o buffer de entrada.
 */
void clear_input(void);

/**
 * Obtém uma escolha numérica do usuário dentro de um intervalo.
 *
 * Parâmetros:
 * int min: O valor mínimo permitido.
 * int max: O valor máximo permitido.
 *
 * Retorna int: A escolha do usuário. Retorna -1 se a entrada for inválida.
 */
int get_choice(int min, int max);

/**
 * Aguarda o usuário pressionar Enter para continuar.
 */
void wait_enter(void);

/**
 * Remove espaços em branco das extremidades de uma string.
 *
 * Parâmetros:
 * const std::string& str: A string a ser processada.
 *
 * Retorna std::string: A string sem espaços nas extremidades.
 */
std::string trim(const std::string& str);

/**
 * Exibe o título de um artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML do artigo.
 */
void show_title(const std::string& html_content);

/**
 * Exibe o sumário (TOC) de um artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML do artigo.
 */
void show_toc(const std::string& html_content);

/**
 * Exibe e processa o menu do artigo da Wikipedia.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML do artigo.
 *
 * Retorna bool: true se deve continuar no menu do artigo, false se deve voltar.
 */
bool article_menu(const std::string& html_content);

/**
 * Processa a entrada da URL pelo usuário e gerencia o fluxo do artigo.
 */
void handle_url_input(void);

/**
 * Exibe e processa o menu principal do programa.
 *
 * Retorna bool: true se deve continuar no menu principal, false se deve sair.
 */
bool main_menu(void);

/**
 * Loop principal do programa.
 */
void main_loop(void);

#endif
