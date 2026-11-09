# Xadrez 3D

Trabalho de Computação Gráfica em C++14 e OpenGL 3.3 Core. A cena contém um tabuleiro 8×8, 32 peças compostas por formas geométricas, base e peças texturizadas, dois programas de shader, câmera navegável e movimentação de peças pelo mouse.

## Executar pelo script

```bash
./jogar.sh
```

O script compila a versão Release na primeira execução e quando os fontes ou cabeçalhos mudam. Nas demais execuções, abre o executável existente. Ele ajusta o diretório de execução para encontrar shaders e texturas, mesmo quando chamado de outra pasta pelo caminho completo. Uma falha de compilação impede a abertura de um executável antigo.

## Abrir pelo Code::Blocks

Abra `Xadrez3D.cbp`, escolha o target do seu sistema na caixa **Build target** e use **Build and run (F9)**:

| Sistema | Targets |
| --- | --- |
| Linux | **Linux Debug** ou **Linux Release** |
| Windows | **Windows Debug** ou **Windows Release** |

Cada target só compila no sistema indicado. Em todos eles, o programa roda a partir da raiz do projeto, onde estão `shaders/` e `res/images/`. Como no exemplo do professor, o programa abre junto um console com mensagens: versão do OpenGL, peça selecionada e movimentos.

### Linux

O projeto usa GLFW e GLEW instalados no sistema, como o exemplo `GLFW_Cubo_OO` do professor.

```bash
sudo apt install build-essential libglfw3-dev libglew-dev xterm          # Debian/Ubuntu
sudo pacman -S --needed base-devel mesa glfw-x11 glew xterm codeblocks   # Arch
```

O `xterm` é o terminal que o Code::Blocks usa para mostrar o console do programa no Linux.

Em sessões Wayland, o aplicativo seleciona X11/XWayland quando essa opção está disponível no GLFW. Isso é necessário porque a versão do GLEW instalada no ambiente usa GLX. O compositor precisa oferecer XWayland.

### Windows

Não é preciso instalar nenhuma biblioteca: tudo vem na pasta `libs/`. Use o Code::Blocks com MinGW de 64 bits (o instalador `codeblocks-...-mingw-setup.exe`).

- `libs/glfw/`: GLFW 3.4, versão pré-compilada para MinGW 64 bits. O programa usa a `glfw3.dll` da raiz do projeto.
- `libs/glew/`: GLEW 2.2.0 em código-fonte. O `glew.c` é compilado junto com o projeto (só nos targets Windows), com `GLEW_STATIC`.
- `libs/glm/`: GLM 1.0.1, só cabeçalhos. Também é usada no Linux, então lá não é preciso instalar a GLM.

As bibliotecas do compilador (`libstdc++` e `libgcc`) vão embutidas no executável. Por isso o `.exe` só precisa da `glfw3.dll` para rodar. Se for executá-lo fora do Code::Blocks, rode a partir da raiz do projeto, para ele achar os shaders, a textura e a DLL.

As licenças das bibliotecas estão nas respectivas pastas em `libs/`.

### Em todos os sistemas

Se o projeto já estava aberto antes de atualizar o `.cbp`, feche e reabra o projeto para carregar a configuração nova.

## Controles

| Entrada | Ação |
| --- | --- |
| W / S | Andar para frente / para trás |
| A / D | Andar lateralmente |
| Setas | Orientar a câmera |
| Q / Z | Aumentar / diminuir a altura |
| Clique esquerdo na peça | Selecionar e destacar |
| Segundo clique esquerdo no tabuleiro | Mover para o centro da casa indicada |
| Clique direito | Cancelar a seleção |
| Escape | Fechar a janela |

Não há turnos, regras de jogada ou captura. Qualquer peça pode ir para qualquer casa vazia. O clique no destino usa a superfície do tabuleiro. Um destino fora do tabuleiro ou uma casa ocupada mantém a seleção para uma nova tentativa; a casa ocupada é avisada no console e no título da janela. Essa é a restrição opcional do enunciado: duas peças nunca ocupam a mesma casa.

## Estrutura e referências

`Application` cuida da janela e do contexto. `Object` concentra as transformações; `Mesh` cuida dos buffers compartilhados pelas implementações de cubo, cilindro, esfera e prisma triangular. `Peca` compõe essas formas pelo mesmo princípio de `Banrisul` em `GLFW_Cubo_OO`. `Tabuleiro`, `Camera` e as funções de `Interacao` mantêm as coordenadas consistentes entre desenho e mouse.

A base e o corpo das peças usam `textura.frag`; as casas e os enfeites das peças (cabeça do peão, ameias, orelhas e olhos do cavalo, bolinha do bispo, coroa da dama e cruz do rei) usam `cor.frag`. Cada parte da peça guarda o campo `texturizada`, e `Peca::desenhar` ativa o programa correspondente antes de desenhá-la. Ambos usam `cena.vert`, com `projection * view * model` e layout posição/cor/UV. O teste de profundidade permanece ativo. Não há iluminação dinâmica; as variações de cor são definidas nos vértices para evidenciar o volume.

A textura `res/images/madeira.jpg` guarda só os veios da madeira, em cinza claro. Como `textura.frag` multiplica a imagem pela cor do vértice, a mesma imagem fica creme nas peças brancas, marrom escuro nas pretas e marrom médio na base. A imagem foi derivada de [Wood049](https://ambientcg.com/view?id=Wood049), do ambientCG (licença CC0), convertida para tons de cinza com ImageMagick.

O planejamento e a origem das adaptações estão em [TASKS.md](TASKS.md) e [REFERENCIAS.md](REFERENCIAS.md). Os arquivos originais de `referencias/` foram preservados. A seleção usa volumes locais por parte da peça; regiões vazias próximas às bordas de formas curvas podem integrar esses volumes simplificados.

## Validação fora da IDE

A compilação direta serve como verificação auxiliar. O `.cbp` continua sendo o formato de entrega.

```bash
mkdir -p bin/Debug bin/Release
g++ -std=c++14 -Wall -Wextra -Wpedantic -g -Iinclude main.cpp src/*.cpp -o bin/Debug/Xadrez3D -lglfw -lGLEW -lGL
g++ -std=c++14 -Wall -Wextra -Wpedantic -O2 -DNDEBUG -Iinclude main.cpp src/*.cpp -o bin/Release/Xadrez3D -lglfw -lGLEW -lGL
./bin/Debug/Xadrez3D
```

Execute a partir da raiz. Recursos ausentes são informados no console e encerram a inicialização. A cópia local de stb_image recebeu uma conversão explícita de tamanho e a remoção de duas variáveis redundantes para compilar sem avisos com o GCC atual.

Os testes precisam de uma sessão gráfica com OpenGL:

```bash
mkdir -p /tmp/xadrez-desenvolvimento
g++ -std=c++14 -Wall -Wextra -Wpedantic -g -Iinclude tests/validacao.cpp src/*.cpp -o /tmp/xadrez-desenvolvimento/validacao -lglfw -lGLEW -lGL
/tmp/xadrez-desenvolvimento/validacao
```

O teste não integra o executável do `.cbp`, pois possui seu próprio `main`. Ele verifica conversão de casas, raios, escala de framebuffer, distribuição inicial, identidade das peças, seleção após movimento, oclusão pelo tabuleiro, ausência de regras de jogada e renderização dos dois programas. Também grava uma imagem PPM das primitivas em `/tmp/xadrez-desenvolvimento/`.

## Estado da entrega

Debug e Release foram compilados com g++ usando os fontes, opções e bibliotecas declarados no `.cbp`. Passaram 236 verificações em OpenGL 3.3, com driver NVIDIA. Nas duas compilações, a automação da janela confirmou seleção, destino externo, cancelamento, movimento E2/E4/H4 após redimensionar, controles de câmera e fechamento por Escape. A falha por shader ausente também foi conferida. As evidências visuais estão em [plan/evidencias/](plan/evidencias/).

A IDE Code::Blocks está instalada. A validação final de execução pela IDE permanece pendente. As checkboxes de aceite permanecem abertas em `TASKS.md` até essa verificação.
