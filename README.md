# regex-scraper

Web scraper para artigos da Wikipedia escrito em Python, extraíndo índice, imagens e links usando **expressões regulares**.

## Contexto Acadêmico

Este projeto foi desenvolvido como parte da disciplina de **Linguagens Formais e Autômatos**, com o objetivo de demonstrar a aplicação prática de expressões regulares em um problema real de extração de dados.

A escolha deliberada por **usar exclusivamente expressões regulares** (em vez de bibliotecas especializadas como BeautifulSoup) tem como propósito:
- Demonstrar o poder e a versatilidade das regex
- Explorar os limites do que é possível com linguagens regulares
- Entender quando e por que usar (ou não usar) regex para parsing de HTML

## Autores

- Miguel Mochizuki Silva
- Gabriel Bringel Gonçalves

## Estrutura do Projeto

```
.
├── main.py                             # Ponto de entrada do programa
├── pyproject.toml                      # Configurações do projeto e dependências
├── README.md                           # Documentação
├── src
│   ├── cli
│   │   └── menu.py                     # Interface de linha de comando
│   ├── scraper
│   │   ├── base
│   │   │   ├── base_scraper.py         # Classe abstrata base para scrapers
│   │   │   └── html_parser.py          # Parser HTML com funcionalidades comuns
│   │   └── wikipedia
│   │       ├── wikipedia_parser.py     # Parser específico para Wikipedia
│   │       └── wikipedia_scraper.py    # Implementação do scraper para Wikipedia
│   └── utils
│       ├── constants.py                # Constantes e expressões regulares
│       └── validators.py               # Funções de validação
└── uv.lock                             # Lock file do gerenciador de pacotes uv
```

## Funcionalidades

O scraper permite extrair as seguintes informações de artigos da Wikipédia em português:

- **Índice do artigo**: Lista todos os tópicos e sub-tópicos do artigo
- **Imagens**: Extrai os nomes dos arquivos de imagem do artigo
- **Links internos**: Links para outros artigos da Wikipédia, com título e URL completa
- **Links externos**: Links para sites externos (não pertencentes à Wikipédia)

## Como Usar

### Pré-requisitos

- Python 3.8 ou superior
- Gerenciador de pacotes `uv` (recomendado) ou `pip`

### Instalação

1. Clone o repositório:
```bash
git clone https://github.com/gabrielbringel/regex-scraper
cd regex-scraper
```

2. Instale as dependências com uv (recomendado):
```bash
uv sync
```

Ou com pip:
```bash
pip install -r requirements.txt
```

### Executando o Programa

1. Execute o arquivo principal:
```bash
uv run main.py # Com uv
python main.py # Com pip
```

2. Digite a URL de um artigo da Wikipédia em português:
```
Digite a URL do artigo da Wikipédia (ou 'sair' para encerrar): https://pt.wikipedia.org/wiki/Python
```

3. Escolha uma opção do menu:
```
1. Listar tópicos do índice
2. Listar nomes de arquivos de imagens
3. Listar links para outros artigos da Wikipédia
4. Listar links externos (extra)
5. Sair
```

## Arquitetura

O projeto segue uma arquitetura modular e orientada a objetos:

- **BaseScraper (ABC)**: Define o contrato para todos os scrapers
- **WikipediaScraper**: Implementação concreta para a Wikipédia
- **HTMLParser**: Utilitários para parsing de HTML
- **WikipediaParser**: Parsing especializado para conteúdo da Wikipédia
- **MenuCLI**: Interface de usuário para interação via linha de comando

### Fluxo de Dados

1. Usuário fornece URL via CLI
2. `WikipediaScraper` valida e carrega a página
3. `WikipediaParser` processa o HTML
4. Expressões regulares extraem o conteúdo desejado
5. Resultados são exibidos no menu interativo

## Expressões Regulares: Escolhas e Desafios

### Por que Regex para Parsing de HTML?

Este projeto utiliza **exclusivamente expressões regulares** para extrair dados do HTML, uma decisão que vai contra a sabedoria convencional (que recomenda parsers como BeautifulSoup ou lxml). Esta escolha foi intencional e educacional:

**Vantagens das Regex neste contexto:**
- **Precisão**: Podemos definir padrões exatos para os elementos que nos interessam
- **Performance**: Para padrões bem definidos, regex é extremamente rápido
- **Transparência**: As regras de extração ficam explícitas no código
- **Educacional**: Excelente para demonstrar o poder das linguagens regulares

**Desafios encontrados:**
- HTML não é uma linguagem regular (é livre de contexto)
- A Wikipedia frequentemente atualiza sua estrutura HTML
- Lidar com variações nos atributos e espaçamentos

### Expressões Regulares Implementadas

O projeto utiliza as seguintes expressões regulares principais:

#### Validação de URL
```regex
^https?://(pt\.wikipedia\.org)/wiki/.*$
```
- Valida URLs da Wikipedia em português
- Aceita HTTP e HTTPS
- Garante o domínio correto

#### Extração do Índice (TOC)
```regex
<div\b[^>]*\bid="vector-toc"[^>]*>(.*)
```
- Busca o container do índice
- Usa `[^>]*` para lidar com atributos variáveis
- Captura todo o conteúdo do TOC

#### Parsing de Itens do Índice
```regex
    <a\b[^>]*\bclass="[^"]*vector-toc-link[^"]*"[^>]*>\s*
    <div\b[^>]*\bclass="[^"]*vector-toc-text[^"]*"[^>]*>
    (?:<span\b[^>]*\bclass="[^"]*vector-toc-numb[^"]*"[^>]*>(.*?)</span>)?
    (.*?)</div>
```
- Extrai número e texto de cada item do índice
- Usa lookaheads para lidar com elementos opcionais
- Gerencia múltiplos níveis de indentação

#### Extração de Imagens
```regex
<img\b[^>]*?\bsrc\s*=\s*"([^"]*)"
```
- Busca tags `<img>` com atributo `src`
- Captura a URL da imagem
- Lida com diferentes posições do atributo src

### Desafios Encontrados com Regex para HTML

1. **Estrutura Aninhada**: HTML tem elementos aninhados, enquanto regex é linear
2. **Variações de Sintaxe**: Espaços, aspas simples/duplas, atributos em ordem diferente
3. **Caracteres Especiais**: URLs e textos com caracteres que precisam ser escapados
4. **Manutenção**: Regex são difíceis de manter comparadas a parsers estruturados

## Tratamento de Erros e Limitações

### Tratamento Implementado

- **Validação de URL**: Verifica se a URL é válida e pertence à Wikipédia em português
- **Timeout**: Limite de 10 segundos para requisições HTTP
- **Erros de Rede**: Captura exceções de conexão e timeout
- **Fallbacks**: Mensagens amigáveis quando não há conteúdo para extrair
- **Limpeza de Texto**: Remove tags HTML e normaliza espaços em branco

### Limitações

#### Limitações Teóricas das Regex para HTML

HTML é uma **linguagem livre de contexto**, enquanto expressões regulares reconhecem **linguagens regulares** (um subconjunto próprio). Isso significa que:

- **Tags aninhadas**: Regex não pode contar níveis de aninhamento corretamente
  ```
  <div><div><div>texto</div></div></div>
  ```
  - Regex pode encontrar o primeiro `</div>` e parar, perdendo estrutura

- **Atributos opcionais**: Variações na estrutura HTML são difíceis de capturar
  ```html
  <img src="img.jpg">
  <img class="thumb" src="img.jpg">
  <img src="img.jpg" class="thumb">
  ```

- **Comentários e CDATA**: Podem quebrar padrões de regex

#### Limitações Práticas

- **Parsing baseado em Regex**: O scraper é dependente da estrutura atual do HTML da Wikipédia. Mudanças no layout podem quebrar a extração.
- **HTML Malformado**: Não há tratamento específico para HTML com erros de sintaxe
- **Encoding**: Usa o encoding padrão do `requests` sem detecção adicional
- **Estrutura Específica**: Desenvolvido apenas para a versão desktop da Wikipédia em português
- **Performance**: Para páginas muito grandes, regex pode ser menos eficiente que parsers especializados

### Lições Aprendidas

Este projeto demonstra que:

1. **Regex é poderoso** para extração de padrões bem definidos
2. A **manutenção** de regex para HTML é trabalhosa
3. Em produção, **parsers especializados** (BeautifulSoup, lxml) são mais robustos
4. Regex é excelente como **primeira camada** de extração, combinada com outros métodos

### Recomendações de Uso

Para melhores resultados:
- Use URLs da versão desktop da Wikipédia em português
- Certifique-se de que a página existe e está acessível
- Esteja ciente que mudanças na estrutura HTML da Wikipédia podem afetar o funcionamento
- Para projetos reais, considere usar parsers HTML especializados

## Dependências

- **requests**: Para requisições HTTP
- **urllib3**: Para manipulação de URLs
- **re**: (biblioteca padrão) Para expressões regulares
- **abc**: (biblioteca padrão) Para classes abstratas

## Contribuição

1. Faça um fork do projeto
2. Crie uma branch para sua feature (`git checkout -b feature/nova-feature`)
3. Commit suas mudanças (`git commit -am 'Adiciona nova feature'`)
4. Push para a branch (`git push origin feature/nova-feature`)
5. Crie um Pull Request

## Licença

O projeto está sob a [licença MIT](./LICENSE)

## Melhorias Futuras

- [ ] Suporte para outras wikis (inglês, espanhol, etc.)
- [ ] Exportação de dados em JSON/CSV
- [ ] Download automático de imagens
- [ ] Suporte a páginas de discussão
- [ ] Busca por palavras-chave no conteúdo
- [ ] Interface web (FastAPI/Flask)
- [ ] Testes automatizados com pytest
- [ ] Detecção automática de encoding com `charset-normalizer`
- [ ] Estratégias de fallback para mudanças na estrutura HTML
