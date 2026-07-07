"""
wikipedia_scraper.py
Autores: Miguel Mochizuki Silva e Gabriel Bringel Gonçalves
Descrição: Implementação concreta do scraper para Wikipedia
"""
from typing import List, Optional, Tuple
from ..base.base_scraper import BaseScraper
from .wikipedia_parser import WikipediaParser
from ...utils.constants import HEADERS
from ...utils.validators import validar_url_wikipedia

class WikipediaScraper(BaseScraper):
    """Scraper especializado para Wikipedia em português"""

    def __init__(self):
        super().__init__(headers=HEADERS, timeout=10)
        self.parser = WikipediaParser()

    def validar_url(self, url: str) -> bool:
        """Valida se a URL é da Wikipedia em português"""
        return validar_url_wikipedia(url)

    def extrair_conteudo(self) -> Optional[str]:
        """Extrai o conteúdo principal do artigo"""
        if not self.html_content:
            return None
        return self.parser.extrair_conteudo_principal(self.html_content)

    def listar_indice(self) -> List[Tuple[str, str]]:
        """Lista os itens do índice"""
        if not self.html_content:
            return []
        return self.parser.extrair_indice(self.html_content)

    def listar_imagens(self) -> List[str]:
        """Lista as imagens encontradas"""
        if not self.html_content:
            return []
        return self.parser.extrair_imagens(self.html_content)

    def listar_links_internos(self) -> List[Tuple[str, str, str]]:
        """Lista links para outros artigos da Wikipedia"""
        if not self.html_content:
            return []
        return self.parser.extrair_links_wikipedia(self.html_content)

    def listar_links_externos(self) -> List[Tuple[str, str]]:
        """Lista links externos"""
        if not self.html_content:
            return []
        return self.parser.extrair_links_externos(self.html_content)

    def listar_links_wikipedia(self) -> List[Tuple[str, str, str]]:
        """Método específico para Wikipedia (alias para listar_links_internos)"""
        return self.listar_links_internos()
