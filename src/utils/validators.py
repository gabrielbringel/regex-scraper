"""
validators.py
Autores: Miguel Mochizuki Silva e Gabriel Bringel Gonçalves
Descrição: Funções de validação
"""
from .constants import REGEX_URL

def validar_url_wikipedia(url: str) -> bool:
    """Valida se a URL é da Wikipedia em português"""
    return bool(REGEX_URL.match(url))

def limpar_texto(texto: str) -> str:
    """Remove tags HTML e normaliza espaços."""
    from .constants import REGEX_TAGS, REGEX_ESPACOS
    texto = REGEX_TAGS.sub(' ', texto)
    texto = REGEX_ESPACOS.sub(' ', texto)
    return texto.strip()
