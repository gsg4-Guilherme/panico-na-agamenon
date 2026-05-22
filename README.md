# Pânico na Agamenon

Esqueleto em C com Raylib para o jogo “Pânico na Agamenon”.

O objetivo é ter uma base compilável, comentada e separada por responsabilidades, sem entregar o jogo completo pronto. A ideia é que cada pessoa do grupo consiga entender onde mexer e continuar a implementação com o próprio estilo.

## Ideia do jogo

O jogador controla um carro na Avenida Agamenon Magalhães e precisa desviar de veículos que descem pela tela. A pontuação cresce pelo tempo de sobrevivência. Depois, o grupo pode completar dificuldade progressiva, chuva, engarrafamento, telas finais e recordes.

## Divisão por devs

- Dev 1, engine e núcleo: `src/main.c`, `src/engine.c`, `include/engine.h`, `include/config.h`.
- Dev 2, jogabilidade: `src/jogador.c`, `src/obstaculos.c`, `src/pista.c` e seus headers.
- Dev 3, interface e dados: `src/interface.c`, `src/pontuacao.c`, `src/ranking.c` e seus headers.

## Estrutura

```text
assets/
  veiculos/   Sprites do jogador, carros e onibus.
  cenario/    Sprites da pista e elementos do ambiente.
  powerups/   Sprites dos power-ups coletaveis.
  interface/  Sprites de HUD, menu e botões.
build/         Arquivos gerados pela compilação, ignorados pelo Git.
data/          Arquivos de dados locais, como ranking.
include/       Headers públicos dos módulos.
src/           Implementações em C.
Makefile       Comandos de compilação, execução e limpeza.
```

## Sprites esperados

```text
assets/veiculos/
  carro_jogador.png
  carro_verde.png
  carro_branco.png
  carro_vermelho.png
  onibus_vermelho.png
  onibus_amarelo.png

assets/powerups/
  escudo.png
  baixa_velocidade.png
  buzina.png
  dobro_de_pontos.png
  limpa_faixa.png

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

## Controles atuais

- `ENTER`: iniciar modo 1 jogador ou reiniciar.
- `ESPAÇO`: iniciar modo 2 jogadores.
- `ESC`: voltar ao menu ou fechar no menu.
- Modo 1 jogador: `Seta esquerda` e `Seta direita` mudam de faixa; `Seta cima` usa power-up.
- Modo 2 jogadores, jogador esquerdo: `A` e `D` mudam de faixa; `W` usa power-up.
- Modo 2 jogadores, jogador direito: `Seta esquerda` e `Seta direita` mudam de faixa; `Seta cima` usa power-up.
- Os obstáculos aparecem automaticamente em faixas aleatórias.

## Git e GitHub

Este repositório foi pensado para commits por etapa:

```bash
git log --oneline --decorate --all
```

Para enviar ao GitHub depois de instalar o `gh`:

```powershell
gh auth login
gh repo create jogopif --source . --private --push
git push --all origin
```

Se o professor exigir repositório público, troque `--private` por `--public`.

## Referências

- Raylib: https://www.raylib.com/
- Raylib no Windows: https://github-wiki-see.page/m/raysan5/raylib/wiki/Working-on-Windows
- Raylib no Linux: https://github-wiki-see.page/m/raysan5/raylib/wiki/Working-on-GNU-Linux
- w64devkit: https://github.com/skeeto/w64devkit/releases