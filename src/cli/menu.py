"""
menu.py
Autores: Miguel Mochizuki Silva e Gabriel Bringel Gonçalves
Descrição: Interface de linha de comando
"""
from typing import Optional
from ..scraper.wikipedia.wikipedia_scraper import WikipediaScraper

class MenuCLI:
    """Menu interativo para o scraper"""

    def __init__(self):
        self.scraper = WikipediaScraper()
        self.running = True

    def exibir_cabecalho(self):
        """Exibe cabeçalho do programa"""
        print("\n" + "="*50)
        print("WEB SCRAPER - WIKIPÉDIA")
        print("="*50)

    def exibir_menu_principal(self):
        """Exibe menu principal"""
        print("\n1. Listar tópicos do índice")
        print("2. Listar nomes de arquivos de imagens")
        print("3. Listar links para outros artigos da Wikipédia")
        print("4. Listar links externos (extra)")
        print("5. Sair")
        print("="*50)

    def exibir_indice(self):
        """Exibe o índice do artigo"""
        itens = self.scraper.listar_indice()

        print("\n=== ÍNDICE DO ARTIGO ===\n")

        if not itens:
            print("Nenhum índice encontrado para este artigo.")
            return

        for num, texto in itens:
            if num:
                print(f"{num} - {texto}")
            else:
                print(f"- {texto}")

    def exibir_imagens(self):
        """Exibe as imagens do artigo"""
        imagens = self.scraper.listar_imagens()

        print("\n=== IMAGENS DO ARTIGO ===\n")

        if imagens:
            for i, img in enumerate(imagens, 1):
                print(f"{i}. {img}")
        else:
            print("Nenhuma imagem encontrada no artigo.")

    def exibir_links_wikipedia(self):
        """Exibe links para outros artigos da Wikipedia"""
        links = self.scraper.listar_links_wikipedia()

        print("\n=== LINKS PARA ARTIGOS DA WIKIPÉDIA ===\n")

        if links:
            limite = min(30, len(links))
            print(f"Mostrando os primeiros {limite} de {len(links)} links encontrados:\n")

            for i, (texto, url, titulo) in enumerate(links[:limite], 1):
                print(f"{i}. {texto}")
                print(f"   Título: {titulo}")
                print(f"   Link: {url}\n")

            if len(links) > 30:
                print(f"... e mais {len(links) - 30} links.")
        else:
            print("Nenhum link para outros artigos da Wikipédia encontrado.")

    def exibir_links_externos(self):
        """Exibe links externos"""
        links = self.scraper.listar_links_externos()

        print("\n=== LINKS EXTERNOS (NÃO-WIKIPÉDIA) ===\n")

        if links:
            limite = min(20, len(links))
            for i, (texto, href) in enumerate(links[:limite], 1):
                print(f"{i}. {texto}")
                print(f"   Link: {href}\n")

            if len(links) > 20:
                print(f"... e mais {len(links) - 20} links externos.")
        else:
            print("Nenhum link externo encontrado.")

    def processar_opcao(self, opcao: int) -> bool:
        """Processa a opção escolhida pelo usuário"""
        if opcao == 1:
            self.exibir_indice()
        elif opcao == 2:
            self.exibir_imagens()
        elif opcao == 3:
            self.exibir_links_wikipedia()
        elif opcao == 4:
            self.exibir_links_externos()
        elif opcao == 5:
            return True  # Sair do menu atual
        else:
            print("Opção inválida!")

        if opcao != 5:
            input("\nPressione Enter para continuar...")
        return False

    def executar(self):
        """Executa o menu principal"""
        print("Bem-vindo ao Web Scraper da Wikipédia!")

        while self.running:
            self.exibir_cabecalho()

            url = input("\nDigite a URL do artigo da Wikipédia (ou 'sair' para encerrar): ").strip()

            if url.lower() == 'sair':
                print("Encerrando programa...")
                self.running = False
                break

            if not self.scraper.carregar_url(url):
                print("\nURL inválida ou não foi possível acessar a página.")
                print("Certifique-se de usar o domínio pt.wikipedia.org")
                print("Exemplo: https://pt.wikipedia.org/wiki/Python")
                continue

            print("Página acessada com sucesso!")

            while True:
                self.exibir_menu_principal()

                try:
                    opcao = int(input("\nEscolha uma opção (1-5): "))

                    if self.processar_opcao(opcao):
                        break  # Volta para seleção de URL

                except ValueError:
                    print("Por favor, digite um número válido.")
                except Exception as e:
                    print(f"Ocorreu um erro: {e}")
