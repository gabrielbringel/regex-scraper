import requests
import re
from urllib.parse import unquote

# ==============================
# CONSTANTES
# ==============================


URL_BASE_WIKIPEDIA = r"^https?://(pt\.wikipedia\.org)/wiki/.*$"

HEADERS = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
    'Accept-Language': 'pt-BR,pt;q=0.9,en;q=0.8',
    'Accept-Encoding': 'gzip, deflate, br',
    'Connection': 'keep-alive',
    'Upgrade-Insecure-Requests': '1',
}


REGEX_URL = re.compile(URL_BASE_WIKIPEDIA)

REGEX_TOC = re.compile(r'<div\b[^>]*\bid="vector-toc"[^>]*>(.*)', re.S)
REGEX_ITEM_TOC = re.compile(
    r'<a\b[^>]*\bclass="[^"]*vector-toc-link[^"]*"[^>]*>\s*'
    r'<div\b[^>]*\bclass="[^"]*vector-toc-text[^"]*"[^>]*>'
    r'(?:<span\b[^>]*\bclass="[^"]*vector-toc-numb[^"]*"[^>]*>(.*?)</span>)?'
    r'(.*?)</div>',
    re.S
)

REGEX_IMG_SRC = re.compile(r'<img\b[^>]*?\bsrc\s*=\s*"([^"]*)"', re.I)
REGEX_EXTENSAO_IMG = re.compile(r'\.(jpg|jpeg|png|gif|svg|webp|ico)$', re.I)

REGEX_CONTEUDO = re.compile(r'\bid="mw-content-text"(.*)', re.S)
REGEX_CATLINKS = re.compile(r'\bid="catlinks"')
REGEX_LINKS = re.compile(r'<a\b[^>]*?\bhref\s*=\s*"([^"]*)"[^>]*>(.*?)</a>', re.I | re.S)
REGEX_ARQUIVOS = re.compile(r'\.(jpg|png|gif|svg|pdf|ico|webp)', re.I)
REGEX_TAGS = re.compile(r'<[^>]+>')
REGEX_ESPACOS = re.compile(r'\s+')
REGEX_EXTENSOES = re.compile(r'\.(jpg|png|gif|svg|webp|ico)')
REGEX_PX = re.compile(r'^\d+px-')
REGEX_WIKI_PREFIX = re.compile(r'^//pt\.wikipedia\.org/wiki/')
REGEX_WIKI_INTERNO = re.compile(r'^/wiki/')

# ==============================
# Funções
# ==============================

def parse_url(url: str) -> bool:
    return bool(REGEX_URL.match(url))

def obter_conteudo_html(url: str) -> str:
    try:
        print("Acessando a página...")
        response = requests.get(url, headers=HEADERS, timeout=10)
        response.raise_for_status()

        if 'text/html' in response.headers.get('Content-Type', ''):
            return response.text
        else:
            print("A resposta não é HTML.")
            return None

    except requests.exceptions.RequestException as e:
        print(f"Erro ao acessar a página: {e}")
        return None

def limpar_texto(texto: str) -> str:
    """Remove tags HTML e normaliza espaços."""
    texto = REGEX_TAGS.sub(' ', texto)
    texto = REGEX_ESPACOS.sub(' ', texto)
    return texto.strip()

def extrair_conteudo(html: str) -> str:
    """Extrai o conteúdo principal do artigo."""
    match = REGEX_CONTEUDO.search(html)
    if not match:
        return None

    conteudo = match.group(1)
    corte = REGEX_CATLINKS.search(conteudo)
    if corte:
        conteudo = conteudo[:corte.start()]
    return conteudo

def listar_indice(html: str):
    print("\n=== ÍNDICE DO ARTIGO ===\n")

    toc_match = REGEX_TOC.search(html)
    if not toc_match:
        print("Nenhum índice encontrado para este artigo.")
        return

    toc_html = toc_match.group(1)
    itens = REGEX_ITEM_TOC.findall(toc_html)

    if not itens:
        print("Nenhum item de índice encontrado.")
        return

    for num_html, texto_html in itens:
        num = limpar_texto(num_html) if num_html else None
        texto = limpar_texto(texto_html)

        if num and texto.startswith(num):
            texto = texto[len(num):].strip()

        if num:
            print(f"{num} - {texto}")
        else:
            print(f"- {texto}")

def listar_imagens(html: str):
    print("\n=== IMAGENS DO ARTIGO ===\n")

    srcs = REGEX_IMG_SRC.findall(html)
    imagens_encontradas = []

    for src in srcs:
        if src:
            nome_arquivo = src.split('/')[-1]
            nome_arquivo = nome_arquivo.split('?')[0]
            nome_arquivo = REGEX_PX.sub('', nome_arquivo)

            if REGEX_EXTENSAO_IMG.search(nome_arquivo):
                if nome_arquivo not in imagens_encontradas:
                    imagens_encontradas.append(nome_arquivo)

    if imagens_encontradas:
        for i, img in enumerate(imagens_encontradas, 1):
            print(f"{i}. {img}")
    else:
        print("Nenhuma imagem encontrada no artigo.")

def listar_links_wikipedia(html: str):
    conteudo = extrair_conteudo(html)
    if not conteudo:
        print("Conteúdo do artigo não encontrado.")
        return

    print("\n=== LINKS PARA ARTIGOS DA WIKIPÉDIA ===\n")

    links = REGEX_LINKS.findall(conteudo)
    links_validos = []

    for href, conteudo_link in links:
        if href.startswith('/wiki/') or href.startswith('//pt.wikipedia.org/wiki/'):
            if ':' not in href and '#' not in href:
                if not REGEX_EXTENSOES.search(href):
                    texto = limpar_texto(conteudo_link)
                    if texto and len(texto) > 1:
                        if REGEX_WIKI_INTERNO.match(href):
                            titulo = unquote(href.replace('/wiki/', ''))
                        else:
                            titulo = unquote(REGEX_WIKI_PREFIX.sub('', href))
                        titulo = titulo.replace('_', ' ')

                        if href.startswith('//'):
                            url_completa = f"https:{href}"
                        else:
                            url_completa = f"https://pt.wikipedia.org{href}"

                        links_validos.append((texto, url_completa, titulo))

    vistos = set()
    links_unicos = []
    for texto, url, titulo in links_validos:
        if url not in vistos:
            vistos.add(url)
            links_unicos.append((texto, url, titulo))

    if links_unicos:
        limite = min(30, len(links_unicos))
        print(f"Mostrando os primeiros {limite} de {len(links_unicos)} links encontrados:\n")

        for i, (texto, url, titulo) in enumerate(links_unicos[:limite], 1):
            print(f"{i}. {texto}")
            print(f"   Título: {titulo}")
            print(f"   Link: {url}\n")

        if len(links_unicos) > 30:
            print(f"... e mais {len(links_unicos) - 30} links.")
    else:
        print("Nenhum link para outros artigos da Wikipédia encontrado.")

def listar_links_personalizados(html: str):
    conteudo = extrair_conteudo(html)
    if not conteudo:
        print("Conteúdo do artigo não encontrado.")
        return

    print("\n=== LINKS EXTERNOS (NÃO-WIKIPÉDIA) ===\n")

    links = REGEX_LINKS.findall(conteudo)
    links_externos = []

    for href, conteudo_link in links:
        if href.startswith('http') and 'wikipedia.org' not in href:
            texto = limpar_texto(conteudo_link)
            if texto and len(texto) > 1 and not texto.startswith('['):
                links_externos.append((texto, href))

    vistos = set()
    links_unicos = []
    for texto, href in links_externos:
        if href not in vistos:
            vistos.add(href)
            links_unicos.append((texto, href))

    if links_unicos:
        limite = min(20, len(links_unicos))
        for i, (texto, href) in enumerate(links_unicos[:limite], 1):
            print(f"{i}. {texto}")
            print(f"   Link: {href}\n")

        if len(links_unicos) > 20:
            print(f"... e mais {len(links_unicos) - 20} links externos.")
    else:
        print("Nenhum link externo encontrado.")

def exibir_menu():
    print("\n" + "="*50)
    print("WEB SCRAPER - WIKIPÉDIA")
    print("="*50)
    print("1. Listar tópicos do índice")
    print("2. Listar nomes de arquivos de imagens")
    print("3. Listar links para outros artigos da Wikipédia")
    print("4. Listar links externos (extra)")
    print("5. Sair")
    print("="*50)

def processar_html(html: str, opcao: int):
    if opcao == 1:
        listar_indice(html)
    elif opcao == 2:
        listar_imagens(html)
    elif opcao == 3:
        listar_links_wikipedia(html)
    elif opcao == 4:
        listar_links_personalizados(html)
    else:
        print("Opção inválida!")

# ==============================
# MAIN
# ==============================

def main():
    print("Bem-vindo ao Web Scraper da Wikipédia!")
    print("="*50)

    while True:
        url = input("\nDigite a URL do artigo da Wikipédia (ou 'sair' para encerrar): ").strip()

        if url.lower() == 'sair':
            print("Encerrando programa...")
            break

        if not parse_url(url):
            print("URL inválida! Certifique-se de usar o domínio pt.wikipedia.org")
            print("Exemplo: https://pt.wikipedia.org/wiki/Python")
            continue

        html = obter_conteudo_html(url)

        if not html:
            print("\nNão foi possível acessar a página. Tente novamente ou verifique a URL.")
            continue

        print("Página acessada com sucesso!")

        while True:
            exibir_menu()

            try:
                opcao = int(input("\nEscolha uma opção (1-5): "))

                if opcao == 5:
                    print("Voltando para seleção de URL...")
                    break

                if 1 <= opcao <= 4:
                    processar_html(html, opcao)
                    input("\nPressione Enter para continuar...")
                else:
                    print("Opção inválida! Digite um número entre 1 e 5.")

            except ValueError:
                print("Por favor, digite um número válido.")
            except Exception as e:
                print(f"Ocorreu um erro: {e}")

if __name__ == "__main__":
    main()
