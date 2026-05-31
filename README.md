# Pânico na Agamenon

Jogo 2D top-down de trânsito feito em C com Raylib.

O jogador controla um carro na Avenida Agamenon Magalhães e precisa desviar dos veículos que descem pela tela. A pontuação cresce com o tempo de sobrevivência, e o melhor recorde é salvo em arquivo.

## Funcionalidades

- Jogador único controlado por setas.
- Pista com cinco faixas e cenário animado.
- Carros e ônibus aleatórios como obstáculos.
- Chuva e engarrafamento entram em ciclos conforme o tempo de jogo.
- Pontuação por sobrevivência e ranking persistido em `data/ranking.txt`.
- Menu, partida e tela de fim de jogo.

## Estrutura

```text
assets/
  veiculos/   Sprites do jogador, carros e onibus.
  cenario/    Sprite da pista.
build/         Arquivos gerados pela compilação, ignorados pelo Git.
data/          Arquivo do ranking.
include/       Headers públicos dos módulos.
src/           Implementações em C.
Makefile       Comandos de compilação, execução e limpeza.
```

## Sprites usados

```text
assets/veiculos/
  carro_jogador.png
  carro_verde.png
  carro_branco.png
  carro_vermelho.png
  onibus_vermelho.png
  onibus_amarelo.png

assets/cenario/
  tela.png
```

## Windows com PowerShell, sem WSL

Use esta opção principalmente no Windows.

1. Instale Git e GitHub CLI:

```powershell
winget install --id Git.Git -e
winget install --id GitHub.cli -e
```

2. Instale um compilador MinGW-w64 moderno. A opção simples para Raylib é o `w64devkit`.

- Baixe em: https://github.com/skeeto/w64devkit/releases
- Extraia em `C:\w64devkit`.
- No PowerShell, coloque o kit no `PATH` da sessão:

```powershell
$env:Path = "C:\w64devkit\bin;$env:Path"
gcc --version
make --version
```

Se aparecer `MinGW.org GCC-6.3.0`, ainda é o compilador antigo. Ajuste o `PATH` para o `C:\w64devkit\bin` aparecer antes dele.

3. Instale ou extraia a Raylib.

- Baixe a Raylib em: https://github.com/raysan5/raylib/releases
- Deixe os diretórios `include` e `lib` em um caminho sem espaços, por exemplo `C:\raylib\raylib`.
- Informe esse caminho ao Makefile:

```powershell
$env:RAYLIB_PATH = "C:/raylib/raylib"
```

4. Compile e rode:

```powershell
make
make run
```

Se preferir passar o caminho diretamente:

```powershell
make RAYLIB_PATH=C:/raylib/raylib
```

5. Limpe os arquivos gerados:

```powershell
make clean
```

## Linux ou WSL

No Ubuntu ou WSL com Ubuntu:

```bash
sudo apt update
sudo apt install build-essential git make
```

Instale a Raylib pela forma disponível na sua distribuição ou compile pela documentação oficial. Depois, rode:

```bash
make
make run
```

O Makefile usa, no Linux, as flags comuns:

```bash
-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Controles

- `ENTER`: iniciar o jogo no menu ou reiniciar no game over.
- `ESC`: voltar ao menu durante a partida, ou fechar a janela no menu.
- `Seta esquerda` e `Seta direita`: trocar de faixa.

## Referências

- Raylib: https://www.raylib.com/
- Raylib no Windows: https://github-wiki-see.page/m/raysan5/raylib/wiki/Working-on-Windows
- Raylib no Linux: https://github-wiki-see.page/m/raysan5/raylib/wiki/Working-on-GNU-Linux
- w64devkit: https://github.com/skeeto/w64devkit/releases

## Vídeo do Jogo
- https://youtu.be/okifUNw7-I4
