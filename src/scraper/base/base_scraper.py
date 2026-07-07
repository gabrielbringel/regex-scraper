"""
base_scraper.py
Autores: Miguel Mochizuki Silva e Gabriel Bringel Gonçalves
Descrição: Classe abstrata base para scrapers
"""
from abc import ABC, abstractmethod
import requests
from typing import Optional

class BaseScraper(ABC):
    """Classe abstrata que define o contrato para scrapers"""

    def __init__(self, headers: Optional[dict] = None, timeout: int = 10):
        self.headers = headers or {}
        self.timeout = timeout
        self.html_content = None
        self.url = None

    @abstractmethod
    def validar_url(self, url: str) -> bool:
        """Valida se a URL é suportada pelo scraper"""
        pass

    @abstractmethod
    def extrair_conteudo(self) -> Optional[str]:
        """Extrai o conteúdo principal da página"""
        pass

    @abstractmethod
    def listar_indice(self) -> list:
        """Lista os itens do índice/tabela de conteúdos"""
        pass

    @abstractmethod
    def listar_imagens(self) -> list:
        """Lista as imagens encontradas"""
        pass

    @abstractmethod
    def listar_links_internos(self) -> list:
        """Lista links internos do site"""
        pass

    @abstractmethod
    def listar_links_externos(self) -> list:
        """Lista links externos"""
        pass

    def carregar_url(self, url: str) -> bool:
        """Carrega o conteúdo HTML da URL"""
        if not self.validar_url(url):
            return False

        try:
            response = requests.get(url, headers=self.headers, timeout=self.timeout)
            response.raise_for_status()

            if 'text/html' in response.headers.get('Content-Type', ''):
                self.html_content = response.text
                self.url = url
                return True
            return False

        except requests.exceptions.RequestException as e:
            print(f"Erro ao acessar a página: {e}")
            return False

    def get_html(self) -> Optional[str]:
        """Retorna o conteúdo HTML carregado"""
        return self.html_content
