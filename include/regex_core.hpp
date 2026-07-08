/**
 * regex_core.hpp
 * Autor: Miguel Mochizuki Silva
 * Descrição: Protótipos de funções para manipulação de expressões regulares.
 */
#include <regex>
#include <string>
#include <utility>
#include <vector>

namespace regex_core {

	const std::regex WIKIPEDIA_URL_REGEX(R"(https?:\/\/pt\.wikipedia\.org\/wiki\/[^\s]+)");

	const std::regex WIKIPEDIA_TITLE_REGEX(
		std::string("<title>([^<]*?)(?:") +
		"\xE2\x80\x93" +
		"\\s*Wikipédia[^<]*|" +
		"\\s*-\\s*Wikipédia[^<]*|" +
		")<\\/title>"
	);

	const std::regex WIKIPEDIA_TOC_REGEX(R"(<div[^>]*class="[^"]*vector-toc-text[^"]*"[^>]*>[\s\S]*?(?:<a[^>]*>)?\s*<span[^>]*class="[^"]*vector-toc-numb[^"]*"[^>]*>([^<]*)</span>\s*<span>([^<]+)</span>)");

	/**
	 * Valida se a URL é da Wikipedia.
	 *
	 * Parâmetros:
	 * const std::string& url: A URL a ser validada.
	 *
	 * Retorna bool: true se a URL for da Wikipedia, false caso contrário.
	 */
	bool is_wikipedia_url(const std::string& url);

	/**
	 * Extrai o título da página da Wikipedia a partir do conteúdo HTML.
	 *
	 * Parâmetros:
	 * const std::string& html_content: O conteúdo HTML da página.
	 *
	 * Retorna std::string: O título da página da Wikipedia, ou uma string vazia se não for encontrado.
	 */
	std::string extract_wikipedia_title(const std::string& html_content);

	/**
	 * Extrai o conteúdo do sumário (TOC) da página da Wikipedia a partir do conteúdo HTML.
	 *
	 * Parâmetros:
	 * const std::string& html_content: O conteúdo HTML da página.
	 *
	 * Retorna std::vector<std::pair<std::string, std::string>>: Um vetor de pares contendo o número do item e o título do item do sumário, ou um vetor vazio se não for encontrado.
	 */
	std::vector<std::pair<std::string, std::string>> extract_wikipedia_toc(const std::string& html_content);

}
