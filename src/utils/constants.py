"""
constants.py
Autores: Miguel Mochizuki Silva e Gabriel Bringel Gonçalves
Descrição: Constantes compartilhadas do sistema
"""
import re

# URLs e Headers
URL_BASE_WIKIPEDIA = r"^https?://(pt\.wikipedia\.org)/wiki/.*$"

HEADERS = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
    'Accept-Language': 'pt-BR,pt;q=0.9,en;q=0.8',
    'Accept-Encoding': 'gzip, deflate, br',
    'Connection': 'keep-alive',
    'Upgrade-Insecure-Requests': '1',
}

# Expressões regulares principais
REGEX_URL = re.compile(URL_BASE_WIKIPEDIA)

# Expressões para o índice (TOC - Table of Contents)
REGEX_TOC = re.compile(r'<div\b[^>]*\bid="vector-toc"[^>]*>(.*)', re.S)
REGEX_ITEM_TOC = re.compile(
    r'<a\b[^>]*\bclass="[^"]*vector-toc-link[^"]*"[^>]*>\s*'
    r'<div\b[^>]*\bclass="[^"]*vector-toc-text[^"]*"[^>]*>'
    r'(?:<span\b[^>]*\bclass="[^"]*vector-toc-numb[^"]*"[^>]*>(.*?)</span>)?'
    r'(.*?)</div>',
    re.S
)

# Expressões para conteúdo
REGEX_CONTEUDO = re.compile(r'\bid="mw-content-text"(.*)', re.S)
REGEX_CATLINKS = re.compile(r'\bid="catlinks"')

# Expressões para imagens
REGEX_IMG_SRC = re.compile(r'<img\b[^>]*?\bsrc\s*=\s*"([^"]*)"', re.I)
REGEX_EXTENSAO_IMG = re.compile(r'\.(jpg|jpeg|png|gif|svg|webp|ico)$', re.I)
REGEX_PX = re.compile(r'^\d+px-')

# Expressões para links
REGEX_LINKS = re.compile(r'<a\b[^>]*?\bhref\s*=\s*"([^"]*)"[^>]*>(.*?)</a>', re.I | re.S)
REGEX_EXTENSOES = re.compile(r'\.(jpg|png|gif|svg|webp|ico)')
REGEX_WIKI_PREFIX = re.compile(r'^//pt\.wikipedia\.org/wiki/')
REGEX_WIKI_INTERNO = re.compile(r'^/wiki/')
REGEX_ARQUIVOS = re.compile(r'\.(jpg|png|gif|svg|pdf|ico|webp)', re.I)

# Expressões para texto
REGEX_TAGS = re.compile(r'<[^>]+>')
REGEX_ESPACOS = re.compile(r'\s+')

# Expressões para processamento de URLs de imagens
REGEX_EXTENSOES_IMG = re.compile(r'\.(jpg|jpeg|png|gif|svg|webp|ico|pdf)$', re.I)
