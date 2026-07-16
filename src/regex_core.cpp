/**
 * regex_core.cpp
 * Autores: Gabriel Bringel Gonçalves e Miguel Mochizuki Silva
 * Descrição: Implementação de funções para manipulação de expressões regulares.
 */
#include "regex_core.hpp"
#include <iostream>
#include <sstream>
#include <set>

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

/**
 * Extrai as imagens da página da Wikipedia a partir do conteúdo HTML.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML da página.
 *
 * Retorna std::vector<std::pair<std::string, std::string>>: Um vetor de pares contendo a URL da imagem e o texto alternativo (alt), ou um vetor vazio se não for encontrado.
 */
std::vector<std::pair<std::string, std::string>> regex_core::extract_wikipedia_images(const std::string& html_content) {
	std::vector<std::pair<std::string, std::string>> image_items;

	// Criar um iterador para percorrer todas as ocorrências
	std::sregex_iterator iter(html_content.begin(), html_content.end(), WIKIPEDIA_IMAGE_REGEX);
	std::sregex_iterator end;

	// Iterar sobre todas as correspondências encontradas
	while (iter != end) {
		std::smatch match = *iter;

		// match[1] contém a URL da imagem
		// match[2] contém o texto alternativo (alt) da imagem (opcional)
		std::string src = match[1].str();
		std::string alt = match[2].str();

		// Limpar espaços em branco extras
		src = std::regex_replace(src, std::regex(R"(^\s+|\s+$)"), "");
		alt = std::regex_replace(alt, std::regex(R"(^\s+|\s+$)"), "");

		// Corrigir URLs relativas para URLs absolutas da Wikipedia
		if (src.find("//") == 0) {
			src = "https:" + src;
		} else if (src.find("/") == 0) {
			src = "https://pt.wikipedia.org" + src;
		}

		image_items.push_back(std::make_pair(src, alt));
		++iter;
	}

	return image_items;
}

/**
 * Extrai os links para outros artigos da Wikipedia a partir do conteúdo HTML.
 *
 * Parâmetros:
 * const std::string& html_content: O conteúdo HTML da página.
 *
 * Retorna std::vector<std::pair<std::string, std::string>>: Um vetor de pares contendo o caminho do link e o texto do link, ou um vetor vazio se não for encontrado.
 */
std::vector<std::pair<std::string, std::string>> regex_core::extract_wikipedia_links(const std::string& html_content) {
	std::vector<std::pair<std::string, std::string>> link_items;
	std::set<std::string> unique_links; // Para evitar duplicatas

	// Criar um iterador para percorrer todas as ocorrências
	std::sregex_iterator iter(html_content.begin(), html_content.end(), WIKIPEDIA_LINK_REGEX);
	std::sregex_iterator end;

	// Iterar sobre todas as correspondências encontradas
	while (iter != end) {
		std::smatch match = *iter;

		// match[1] contém o caminho do link (/wiki/...)
		// match[2] contém o texto do link
		std::string href = match[1].str();
		std::string text = match[2].str();

		// Limpar espaços em branco extras
		href = std::regex_replace(href, std::regex(R"(^\s+|\s+$)"), "");
		text = std::regex_replace(text, std::regex(R"(^\s+|\s+$)"), "");

		// Filtrar links que não são artigos (ex: /wiki/Ficheiro:, /wiki/Categoria:, etc.)
		if (href.find("/wiki/Ficheiro:") == 0 ||
		    href.find("/wiki/Categoria:") == 0 ||
		    href.find("/wiki/Ajuda:") == 0 ||
		    href.find("/wiki/Portal:") == 0 ||
		    href.find("/wiki/Predefinição:") == 0 ||
		    href.find("/wiki/Wikipédia:") == 0 ||
		    href.find("/wiki/Especial:") == 0 ||
		    href.find("/wiki/Utilizador:") == 0 ||
		    href.find("/wiki/Discussão:") == 0 ||
		    href.find("#") != std::string::npos ||
		    href.find("File:") != std::string::npos ||
		    href.find("Ficheiro:") != std::string::npos) {
			++iter;
			continue;
		}

		// Verificar se o link já foi adicionado
		if (unique_links.insert(href).second) {
			link_items.push_back(std::make_pair(href, text));
		}

		++iter;
	}

	return link_items;
}
