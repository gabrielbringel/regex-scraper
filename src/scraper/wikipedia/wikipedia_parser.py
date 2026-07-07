"""
wikipedia_parser.py
Autores: Miguel Mochizuki Silva e Gabriel Bringel Gonçalves
Descrição: Parser específico para Wikipedia
"""
import re
from urllib.parse import unquote
from typing import List, Tuple, Optional
from ..base.html_parser import HTMLParser
from ...utils.constants import (
    REGEX_TOC, REGEX_ITEM_TOC, REGEX_IMG_SRC, REGEX_EXTENSAO_IMG,
    REGEX_PX, REGEX_WIKI_PREFIX, REGEX_WIKI_INTERNO, REGEX_EXTENSOES,
    REGEX_CONTEUDO, REGEX_CATLINKS
)
from ...utils.validators import limpar_texto

class WikipediaParser(HTMLParser):
    """Parser especializado para conteúdo da Wikipedia"""

    @staticmethod
    def extrair_conteudo_principal(html: str) -> Optional[str]:
        """Extrai o conteúdo principal do artigo"""
        match = REGEX_CONTEUDO.search(html)
        if not match:
            return None

        conteudo = match.group(1)
        corte = REGEX_CATLINKS.search(conteudo)
        if corte:
            conteudo = conteudo[:corte.start()]
        return conteudo

    @staticmethod
    def extrair_indice(html: str) -> List[Tuple[str, str]]:
        """Extrai os itens do índice"""
        toc_match = REGEX_TOC.search(html)
        if not toc_match:
            return []

        toc_html = toc_match.group(1)
        itens = REGEX_ITEM_TOC.findall(toc_html)
        resultados = []

        for num_html, texto_html in itens:
            num = limpar_texto(num_html) if num_html else None
            texto = limpar_texto(texto_html)

            if num and texto.startswith(num):
                texto = texto[len(num):].strip()

            resultados.append((num, texto))

        return resultados

    @staticmethod
    def extrair_imagens(html: str) -> List[str]:
        """Extrai nomes de arquivos de imagem"""
        srcs = REGEX_IMG_SRC.findall(html)
        imagens = []

        for src in srcs:
            if src:
                nome_arquivo = HTMLParser.extrair_nome_arquivo_url(src)
                nome_arquivo = REGEX_PX.sub('', nome_arquivo)

                if REGEX_EXTENSAO_IMG.search(nome_arquivo):
                    if nome_arquivo not in imagens:
                        imagens.append(nome_arquivo)

        return imagens

    @staticmethod
    def extrair_links_wikipedia(html: str) -> List[Tuple[str, str, str]]:
        """Extrai links para outros artigos da Wikipedia"""
        conteudo = WikipediaParser.extrair_conteudo_principal(html)
        if not conteudo:
            return []

        def filtro_wikipedia(href: str) -> bool:
            return (href.startswith('/wiki/') or
                   href.startswith('//pt.wikipedia.org/wiki/'))

        links = HTMLParser.extrair_links(conteudo, filtro_wikipedia)
        resultados = []

        for texto, href in links:
            if ':' in href or '#' in href:
                continue
            if REGEX_EXTENSOES.search(href):
                continue

            # Processa o título
            if REGEX_WIKI_INTERNO.match(href):
                titulo = unquote(href.replace('/wiki/', ''))
            else:
                titulo = unquote(REGEX_WIKI_PREFIX.sub('', href))
            titulo = titulo.replace('_', ' ')

            # Monta URL completa
            if href.startswith('//'):
                url_completa = f"https:{href}"
            else:
                url_completa = f"https://pt.wikipedia.org{href}"

            resultados.append((texto, url_completa, titulo))

        return HTMLParser.remover_duplicatas(resultados)

    @staticmethod
    def extrair_links_externos(html: str) -> List[Tuple[str, str]]:
        """Extrai links externos (não Wikipedia)"""
        conteudo = WikipediaParser.extrair_conteudo_principal(html)
        if not conteudo:
            return []

        def filtro_externo(href: str) -> bool:
            return (href.startswith('http') and 'wikipedia.org' not in href)

        links = HTMLParser.extrair_links(conteudo, filtro_externo)
        links_validos = []

        for texto, href in links:
            if texto and len(texto) > 1 and not texto.startswith('['):
                links_validos.append((texto, href))

        return HTMLParser.remover_duplicatas(links_validos)
