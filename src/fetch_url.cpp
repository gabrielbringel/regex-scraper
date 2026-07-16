/**
 * fetch_url.cpp
 * Autores: Gabriel Bringel Gonçalves e Miguel Mochizuki Silva
 * Descrição: Implementação de fetch_url, que obtém o conteúdo HTML de uma URL e o retorna como uma string.
 */
#include "fetch_url.hpp"
#include <curl/curl.h>

/**
 * Callback para escrever os dados recebidos pelo cURL em uma string.
 *
 * Parâmetros:
 * void* contents: Ponteiro para os dados recebidos.
 * size_t size: Tamanho de cada elemento de dados.
 * size_t nmemb: Número de elementos de dados.
 * std::string* userp: Ponteiro para a string onde os dados serão armazenados
 *
 * Retorna size_t: O número total de bytes escritos na string.
 */
size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* userp) {
	size_t total_size = size * nmemb;
	userp->append(static_cast<char*>(contents), total_size);
	return total_size;
}

/**
 * Obtém o conteúdo HTML de uma URL e o retorna como uma string.
 *
 * Parâmetros:
 * const std::string& url: A URL da qual o conteúdo HTML será obtido.
 *
 * Retorna std::string: O conteúdo HTML da URL especificada.
 */
std::string fetch_url::fetch_url(const std::string& url) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		return "";
	}

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, fetch_url::USER_AGENT.c_str());

	std::string response_string;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

	if (curl_easy_perform(curl) != CURLE_OK) {
		curl_easy_cleanup(curl);
		return "";
	}
	curl_easy_cleanup(curl);
	return response_string;
}
