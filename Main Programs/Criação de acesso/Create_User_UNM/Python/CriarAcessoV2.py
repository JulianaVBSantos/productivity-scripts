# está com auto clique, precisa corrigir!

import time
import pyautogui
import keyboard
import csv
import threading

DELAY = 0.5  # Delay entre ações (segundos)
MAX_USUARIOS = 100
MAX_CLICKS = 20

parar = False
cliques = []
totalCliques = 0


class Usuario:
    def __init__(self, nome, email, login, senha, descricao):
        self.nome = nome
        self.email = email
        self.login = login
        self.senha = senha
        self.descricao = descricao


def escutar_tecla():
    global parar
    while True:
        if keyboard.is_pressed('F12'):
            parar = True
            print("\nAutomação interrompida pelo usuário!")
            break
        time.sleep(0.1)


def registrar_clique(x, y, tempo):
    global totalCliques
    if totalCliques < MAX_CLICKS:
        cliques.append({'x': x, 'y': y, 'tempo': tempo})
        totalCliques += 1
        print(f"Clique registrado: X={x}, Y={y}, Tempo={tempo} ms")


def gravar_cliques():
    global totalCliques
    print("Realize o procedimento manualmente. Pressione ESC ao terminar.")
    totalCliques = 0
    tempo_inicial = int(time.time() * 1000)

    while True:
        if pyautogui.mouseDown():
            x, y = pyautogui.position()
            tempo_atual = int(time.time() * 1000)
            registrar_clique(x, y, tempo_atual - tempo_inicial)
            tempo_inicial = tempo_atual
            time.sleep(0.3)
        if keyboard.is_pressed('ESC'):
            print("Gravação finalizada. Aguardando 5 segundos antes de iniciar automação...")
            time.sleep(5)  # Aguarda 5 segundos antes de iniciar a automação
            print("Iniciando automação.")
            break
        time.sleep(0.05)


def click_mouse(x, y):
    if parar:
        return
    pyautogui.click(x, y)
    time.sleep(DELAY)


def digitar_texto(texto):
    for char in texto:
        if parar:
            return

        if char == '@':
            pyautogui.write('@')
        else:
            pyautogui.write(char)
        time.sleep(0.05)


def press_tab():
    if parar:
        return
    pyautogui.press('tab')
    time.sleep(DELAY)


def press_backspace():
    if parar:
        return
    pyautogui.press('backspace')
    time.sleep(DELAY)


def ler_usuarios(arquivo):
    usuarios = []
    try:
        with open(arquivo, newline='', encoding='utf-8') as csvfile:
            reader = csv.reader(csvfile)
            for row in reader:
                if len(row) == 5:
                    usuarios.append(Usuario(row[0], row[1], row[2], row[3], row[4]))
    except FileNotFoundError:
        print("Erro ao abrir o arquivo CSV!")
    return usuarios


def preencher_campos(usuario):
    click_mouse(cliques[0]['x'], cliques[0]['y'])
    digitar_texto(usuario.login)
    press_tab()

    digitar_texto(usuario.nome)
    press_tab()

    digitar_texto("NULL")
    press_tab()

    digitar_texto(usuario.email)
    press_tab()

    digitar_texto(usuario.descricao)

    if totalCliques > 4:
        click_mouse(cliques[4]['x'], cliques[4]['y'])
    else:
        pyautogui.moveTo(100, 300)
        click_mouse(100, 300)

    digitar_texto(usuario.senha)
    press_tab()

    digitar_texto(usuario.senha)
    press_tab()


def main():
    usuarios = ler_usuarios('usuarios.csv')
    if not usuarios:
        return

    threading.Thread(target=escutar_tecla, daemon=True).start()
    gravar_cliques()

    print("Arquivos localizados! Iniciando automação.")

    for usuario in usuarios:
        if parar:
            break
        print(f"Processando usuário: {usuario.nome}")

        preencher_campos(usuario)

        for i in range(1, min(7, totalCliques)):
            click_mouse(cliques[i]['x'], cliques[i]['y'])

        print(f"Usuário {usuario.nome} processado.")
        time.sleep(2)

    print("Último usuário processado. Finalizando automação.")


if __name__ == '__main__':
    main()
