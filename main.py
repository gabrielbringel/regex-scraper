"""
main.py
Autores: Miguel Mochizuki Silva e Gabriel Bringel Gonçalves
Descrição: Interface principal do scraper
"""
from src.cli.menu import MenuCLI

def main():
    """Função principal do programa"""
    menu = MenuCLI()
    menu.executar()

if __name__ == "__main__":
    main()
