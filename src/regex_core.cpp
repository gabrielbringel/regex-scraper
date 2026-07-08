/**
 * regex_core.cpp
 * Autor: Miguel Mochizuki Silva
 * Descrição: Implementação de funções para manipulação de expressões regulares.
 */
#include "regex_core.hpp"
#include <iostream>
#include <sstream>

/**
 * Valida se a URL é da Wikipedia.
 *
 * Parâmetros:
 * const std::string& url: A URL a ser validada.
 *
 * Retorna bool: true se a URL for da Wikipedia, false caso contrário.
 */
bool regex_core::is_wikipedia_url(const std::string& url) {
	return std::regex_match(url, WIKIPEDIA_URL_REGEX);
}

/**
 * Extrai o título da página da Wikipedia a partir do conteúdo HTML.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML da página.
 *
 * Retorna std::string: O título da página da Wikipedia, ou uma string vazia se não for encontrado.
 */
std::string regex_core::extract_wikipedia_title(const std::string& html_content) {
	std::smatch match;
	if (std::regex_search(html_content, match, WIKIPEDIA_TITLE_REGEX)) {
		return match[1].str();
	}
	return "";
}

/**
 * Extrai o conteúdo do sumário (TOC) da página da Wikipedia a partir do conteúdo HTML.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML da página.
 *
 * Retorna std::vector<std::pair<std::string, std::string>>: Um vetor de pares contendo o número do item e o título do item do sumário, ou um vetor vazio se não for encontrado.
 */
std::vector<std::pair<std::string, std::string>> regex_core::extract_wikipedia_toc(const std::string& html_content) {
	std::vector<std::pair<std::string, std::string>> toc_items;

	// Criar um iterador para percorrer todas as ocorrências
	std::sregex_iterator iter(html_content.begin(), html_content.end(), WIKIPEDIA_TOC_REGEX);
	std::sregex_iterator end;

	// Iterar sobre todas as correspondências encontradas
	while (iter != end) {
		std::smatch match = *iter;

		// match[1] contém o número do item do sumário
		// match[2] contém o título do item do sumário
		std::string number = match[1].str();
		std::string title = match[2].str();

		// Limpar espaços em branco extras
		number = std::regex_replace(number, std::regex(R"(^\s+|\s+$)"), "");
		title = std::regex_replace(title, std::regex(R"(^\s+|\s+$)"), "");

		// Se o número estiver vazio, usar um contador
		if (number.empty()) {
			number = std::to_string(toc_items.size() + 1);
		}

		toc_items.push_back(std::make_pair(number, title));
		++iter;
	}

	return toc_items;
}
