/**
 * fetch_url.hpp
 * Autores: Gabriel Bringel Gonçalves e Miguel Mochizuki Silva
 * Descrição: Protótipo de fetch_url, que obtém o conteúdo HTML de uma URL e o retorna como uma string.
 */
#include <string>

namespace fetch_url {

	const std::string USER_AGENT = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3";

	/**
	 * Obtém o conteúdo HTML de uma URL e o retorna como uma string.
	 *
	 * Parâmetros:
	 * const std::string& url: A URL da qual o conteúdo HTML será obtido.
	 *
	 * Retorna std::string: O conteúdo HTML da URL especificada.
	 */
	std::string fetch_url(const std::string& url);

}
