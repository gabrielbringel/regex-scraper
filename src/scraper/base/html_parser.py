"""
html_parser.py
Autores: Miguel Mochizuki Silva e Gabriel Bringel Gonçalves
Descrição: Parser base com funcionalidades comuns
"""
import re
from typing import List, Tuple, Callable, Optional
from urllib.parse import unquote
from ...utils.validators import limpar_texto
from ...utils.constants import REGEX_LINKS, REGEX_TAGS, REGEX_ESPACOS

class HTMLParser:
    """Parser base com métodos utilitários para processar HTML"""

    @staticmethod
    def limpar_texto(texto: str) -> str:
        return limpar_texto(texto)

    @staticmethod
    def extrair_links(html: str, filtro: Optional[Callable]) -> List[Tuple[str, str]]:
        """Extrai links do HTML com opção de filtro"""
        links = REGEX_LINKS.findall(html)
        resultados = []

        for href, texto in links:
            if href and texto:
                texto_limpo = limpar_texto(texto)
                if texto_limpo and len(texto_limpo) > 1:
                    if filtro is None or filtro(href):
                        resultados.append((texto_limpo, href))

        return resultados

    @staticmethod
    def remover_duplicatas(lista: List[Tuple]) -> List[Tuple]:
        """Remove itens duplicados baseado no segundo elemento (URL)"""
        vistos = set()
        unicos = []
        for item in lista:
            if item[1] not in vistos:
                vistos.add(item[1])
                unicos.append(item)
        return unicos

    @staticmethod
    def extrair_nome_arquivo_url(url: str) -> str:
        """Extrai o nome do arquivo de uma URL"""
        nome = url.split('/')[-1]
        nome = nome.split('?')[0]
        return nome
