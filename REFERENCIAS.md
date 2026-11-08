# Referências para os agentes implementadores

Índice dos materiais realmente disponíveis e de sua aplicação em [TASKS.md](TASKS.md). **Consulte o PDF e o código original antes de implementar.** Não use somente resumos ou imagens extraídas. Outros materiais locais podem ser consultados quando contribuírem para a tarefa.

Os números abaixo são páginas físicas do PDF, contadas a partir de 1, incluindo a capa. Os rodapés visíveis conferidos coincidem com essa numeração. As nove imagens em `plan/imagens/` são páginas completas renderizadas dos PDFs e foram inspecionadas visualmente; não são novas implementações.

## Apresentações

### R1

[Introdução OpenGL.pdf](referencias/aulas/Introdu%C3%A7%C3%A3o%20OpenGL.pdf) — 40 páginas.

- **Necessário:** 21–23, papel das bibliotecas GLEW/GLFW e estrutura de inicialização.
- **Útil:** 27–29, código de janela, contexto, loop e eventos; 37, distinção entre OpenGL antiga e moderna.
- **Dispensável para implementar:** histórico, marcas e exemplos comerciais; `glBegin`/`glVertex` como base final, pois o trabalho adotará shaders e buffers.
- **Limitação:** 39 remete a `Setup OpenGL.pdf`, não fornecido. O exemplo legado dos slides não deve ser combinado com contexto Core 3.3.

### R2

[shaders-pt1.pdf](referencias/aulas/shaders-pt1.pdf) — 67 páginas.

- **Necessário, conceitos:** 12–16 e 23–32, pipeline, vertex/fragment shader e profundidade.
- **Necessário, implementação:** 34–48, envio de vértices, atributos, compilação, link e uso de shaders.
- **Útil:** 50–60, tipos GLSL e comunicação de dados; 61–62, exemplos pequenos de shaders.
- **Dispensável:** 17–22, tesselação e geometry shader; não são requisitos do tabuleiro.
- **Adaptação importante:** aparecem fontes GLSL `#version 320 es`, inadequadas como base direta para o contexto desktop 3.3 escolhido. Preferir os shaders `330 core` do exemplo OO. No slide 61, a entrada de posição aparece como `out`; usar uma entrada `in`, como os exemplos 3D.

### R3

[OpenGL e Objetos Tridimensionais.pdf](referencias/aulas/OpenGL%20e%20Objetos%0ATridimensionais.pdf) — 78 páginas.

O nome físico possui uma quebra de linha entre `Objetos` e `Tridimensionais`. O link usa `%0A` para preservar o caminho, sem renomear o original.

- **Necessário, conceitos:** 3–8, GLM; 10–30, espaços local/mundial/visualização/recorte e viewport; 39–51, perspectiva e composição das matrizes.
- **Necessário, implementação:** 54–59, identidade, rotação, view e perspectiva; 60–62, uniforms e MVP; 65–67, cubo triangulado; 68–72, problema de oclusão e teste de profundidade.
- **Útil:** 78, exercício de adaptar a geometria do cubo; não contém implementação de cilindro ou esfera.
- **Dispensável para esta entrega:** implementação de câmera ortográfica, 32–37. A navegação será em perspectiva.
- **Imagens nas tasks:** [p. 61](plan/imagens/objetos-3d-p61-uniforms.png), [p. 62](plan/imagens/objetos-3d-p62-vertex-shader.png) e [p. 72](plan/imagens/objetos-3d-p72-profundidade.png), incorporadas em T02.
- **Adaptação importante:** proporção `800/600` é ilustrativa; obter o aspecto do framebuffer atual. Locais de uniforms devem aceitar `-1`, portanto usar `GLint`.

### R4

[shaders-pt2.pdf](referencias/aulas/shaders-pt2.pdf) — 78 páginas.

- **Necessário, implementação:** 3–7, shaders em arquivos; 20–23, perfil/VAO/VBO/layout; 32–36, índices e desenho; 41–45, localização e envio de uniforms.
- **Necessário, textura:** 47–51, conceitos de UV; 52–57, layout posição/cor/UV e shaders; 58–63, wrapping, filtros, mipmaps, stb_image e canais; 64–65, carga e aplicação.
- **Útil:** 9–14, diagnóstico de erros; 25–31, motivação dos índices; 38–40, conceito de uniform; 67, correspondência com os exemplos fornecidos.
- **Dispensável:** 69–74 e exercícios 76/78 de exploração de efeitos externos. ShaderToy não é necessário para o xadrez.
- **Imagens nas tasks:** [p. 21](plan/imagens/shaders-pt2-p21-vao.png), em T02; [p. 23](plan/imagens/shaders-pt2-p23-layout.png), [p. 54](plan/imagens/shaders-pt2-p54-textura-shaders.png) e [p. 64](plan/imagens/shaders-pt2-p64-carga-textura.png), em T06.
- **Adaptação importante:** o layout da p. 23 possui apenas posição 2D; não copiar seu stride no cubo 3D. O vertex shader da p. 54 precisa receber MVP, como R3 p. 62. `glCall` é uma macro dos exemplos, não uma função nativa do OpenGL.

### R5

[Câmera Virtual.pdf](referencias/aulas/C%C3%A2mera%20Virtual.pdf) — 55 páginas.

- **Necessário, conceitos:** 14–26, vetores da câmera e `glm::lookAt`; 34–43, yaw/pitch e vetor de direção.
- **Necessário, implementação:** 31–33, `cameraPos`/`cameraFront`/`cameraUp`, `lookAt` e W/A/S/D; 45–47, setas e `viraCamera`; 48–49, Q/Z para altura.
- **Útil:** 50, resumo dos controles; 55, atividade para integrar os controles no exemplo do cubo.
- **Dispensável como controle final:** 29, órbita automática. Sozinha não atende à navegação livre pedida.
- **Imagens nas tasks:** [p. 33](plan/imagens/camera-p33-movimento.png) e [p. 47](plan/imagens/camera-p47-orientacao.png), incorporadas em T07.
- **Correções observadas:** p. 18 apresenta um parêntese excedente em `glm::normalize`; p. 33 mistura `cameraSpeed` e `camSpeed`. A p. 47 já limita pitch a ±89°. Acrescentar tempo entre quadros; projetar a direção em XZ para caminhar sem alterar altura.

## Projetos e arquivos de código

### E1

[GLFW_Cubo_OO](referencias/codigos/GLFW_Cubo_OO/) — base principal de organização, composição e entrega.

- [`GLFW_Cubo_OO.cbp`](referencias/codigos/GLFW_Cubo_OO/GLFW_Cubo_OO.cbp): targets Debug/Release, lista de unidades e configuração Linux de GLFW, GL e GLEW.
- [`main.cpp`](referencias/codigos/GLFW_Cubo_OO/main.cpp): criação de `Application`, shaders, texturas, objetos e loop com desenho. A demonstração contém rotações animadas que não devem ser herdadas pelo tabuleiro.
- [`src/Application.cpp`](referencias/codigos/GLFW_Cubo_OO/src/Application.cpp): contexto 3.3 Core, GLEW, depth test e callback de framebuffer. `run()` apenas processa troca de buffers/eventos; o desenho efetivo do exemplo está no loop de `main.cpp`.
- [`include/Object.h`](referencias/codigos/GLFW_Cubo_OO/include/Object.h) e [`src/Object.cpp`](referencias/codigos/GLFW_Cubo_OO/src/Object.cpp): posição, rotação, escala, ângulo e `draw` virtual. A implementação base de desenho é vazia.
- [`src/Cube.cpp`](referencias/codigos/GLFW_Cubo_OO/src/Cube.cpp): cubo com 24 vértices, 36 índices, VAO/VBO/EBO e transformações locais. Usa posição + UV, stride de cinco `float`.
- [`include/Banrisul.h`](referencias/codigos/GLFW_Cubo_OO/include/Banrisul.h) e [`src/Banrisul.cpp`](referencias/codigos/GLFW_Cubo_OO/src/Banrisul.cpp): `vector<unique_ptr<Object>> parts`, criação de cubos em `init()` e passagem da matriz pai em `draw()`. Modelo direto para compor as peças; requer C++14 por `make_unique`.
- [`include/Shader.h`](referencias/codigos/GLFW_Cubo_OO/include/Shader.h): classe implementada no próprio header, com carga de arquivos, compilação/link e setters de uniforms. Não é necessário copiar implementações alternativas de `Shader` dos outros projetos.
- [`src/Texture.cpp`](referencias/codigos/GLFW_Cubo_OO/src/Texture.cpp) e [`src/stb_image.cpp`](referencias/codigos/GLFW_Cubo_OO/src/stb_image.cpp): carga de imagem, upload, filtros, bind e unidade única de implementação stb_image.
- [`vertex.glsl`](referencias/codigos/GLFW_Cubo_OO/vertex.glsl) e [`fragment.glsl`](referencias/codigos/GLFW_Cubo_OO/fragment.glsl): GLSL 330, MVP e mistura de duas texturas em **um** programa. Duas imagens não significam dois programas de shader.
- [`src/Prisma.cpp`](referencias/codigos/GLFW_Cubo_OO/src/Prisma.cpp): estrutura semelhante a `Cube`, mas os vértices/índices também descrevem um cubo. Não tratar como prisma triangular pronto.
- [`src/Mesh.cpp`](referencias/codigos/GLFW_Cubo_OO/src/Mesh.cpp): exemplo de malha não indexada com posição/UV. Útil como consulta, sem obrigação de usá-lo junto de `Cube`.

**Cuidados ao adaptar:** o `.cbp` contém `/home/krohn/...`, bibliotecas por caminho absoluto e configuração de link concentrada em Debug. Corrigir os dois targets na cópia. `Cube` e `Prisma` repetem campos de transformação já existentes em `Object`; manter uma única fonte de estado. `Banrisul::draw()` aplica posição/escala, mas não sua própria rotação/ângulo; completar se a orientação da peça depender deles. Revisar liberação dos buffers/programas e sua vida útil em relação ao contexto. Não fazer uma refatoração geral do exemplo.

### E2

[GLFW_Coordenadas_3D_Cubo](referencias/codigos/GLFW_Coordenadas_3D_Cubo/) — cubo 3D, MVP, textura e perspectiva em um programa procedural.

Consultar [`main.cpp`](referencias/codigos/GLFW_Coordenadas_3D_Cubo/main.cpp), especialmente layout em 114–117, matrizes em 203–218 e desenho em 222. O teste de profundidade está **comentado** na linha 53; ativá-lo na adaptação. Não confundir o exemplo sem índices (`glDrawArrays`) com o cubo OO indexado (`glDrawElements`). O projeto não contém os controles completos da aula de câmera.

### E3

[GLFW_Coordenadas_3D_Quadrado](referencias/codigos/GLFW_Coordenadas_3D_Quadrado/) — plano texturizado, rotação e MVP.

[`main.cpp`](referencias/codigos/GLFW_Coordenadas_3D_Quadrado/main.cpp), 175–192, mostra transformações e desenho indexado. Útil para entender o plano das casas; o tabuleiro pode usar cubos achatados da base OO. Não importar outro fluxo de renderização apenas para usar esse exemplo.

### E4

[GLFW_Textures](referencias/codigos/GLFW_Textures/) — referência de posição/cor/UV e textura.

[`main.cpp`](referencias/codigos/GLFW_Textures/main.cpp), 103–109, define os três atributos com stride de oito `float`; 138–150 mostra alinhamento, carga e upload. [`vertex.glsl`](referencias/codigos/GLFW_Textures/vertex.glsl) e [`fragment.glsl`](referencias/codigos/GLFW_Textures/fragment.glsl) mostram o fluxo de cor/UV. A classe `Shader` também aparece no header; não compilar junto um `src/Shader.cpp` redundante de outro exemplo.

### E5

[GLFW_Shaders6](referencias/codigos/GLFW_Shaders6/) — aproveitar somente entrada do mouse e, se necessário, diagnóstico de shaders.

[`main.cpp`](referencias/codigos/GLFW_Shaders6/main.cpp), 82–110, contém posição do cursor e callbacks dos botões; 153–162 registra callbacks. O exemplo envia coordenadas 2D a `iMouse` para efeitos: **não seleciona objetos 3D**. Não copiar a inversão com altura fixa ou chamadas de uniform sem verificar programa ativo. A coleção de efeitos em `shaders/` é dispensável.

### E6

[GLFW_Shaders8](referencias/codigos/GLFW_Shaders8/) e [`shaders-pt1.cpp`](referencias/codigos/shaders-pt1.cpp) — apoio para entender o pipeline.

[`GLFW_Shaders8/opengl_utils.cpp`](referencias/codigos/GLFW_Shaders8/opengl_utils.cpp) oferece `parseShader`, compilação e macros de diagnóstico; `main.cpp` demonstra VAO/IBO/uniforms e posição do cursor. Usar apenas como consulta se `Shader.h` da base OO já atender. O exemplo inclui `opengl_utils.cpp` em `main.cpp` e também o lista no `.cbp`; não transportar esse arranjo sem revisar a compilação. A versão inicial `shaders-pt1.cpp` usa GLSL ES e não é a base final.

## O que não foi encontrado

Não há implementação pronta de seleção 3D por clique, interseção de raio com plano/volumes, cilindro/esfera, modelos de xadrez ou restrição de ocupação. As referências oferecem os fundamentos e partes reutilizáveis; essas lacunas estão identificadas nas tasks. Não atribuir essas extensões a um slide inexistente.

Na etapa de planejamento, os materiais foram analisados seletivamente, sem execução. Durante o desenvolvimento, uma cópia do exemplo OO foi compilada e executada em `/tmp`, após selecionar X11 para compatibilidade com GLEW/GLX. Os originais foram preservados. As bibliotecas e o aplicativo próprio foram verificados; permanece pendente a execução pela IDE Code::Blocks.

## Adaptações presentes no aplicativo

- `Application`, `Object`, `Cube` e `Mesh`: inicialização, transformações, dados do cubo e buffers de E1, com propriedade dos recursos e layout posição/cor/UV de E4.
- `include/Shader.h`: cópia de E1, com liberação do programa, estado de validade e interrupção após falha de leitura/compilação/link.
- `Texture` e `stb_image`: carga de E1, com tratamento dos canais e alinhamento. `pedra-28.jpg` também foi copiada de E1.
- `Peca`: composição por `parts` e matriz pai de `Banrisul`, E1. As formas e proporções dos seis tipos foram construídas para este trabalho.
- `Camera`: variáveis, `lookAt` e `viraCamera` de R5, com deslocamento em XZ e velocidade por tempo.
- `main.cpp`: integração dos eventos do mouse a partir dos callbacks de E5; fluxo de renderização e shaders de E1/R3/R4.
- `Primitivas.cpp` e `Interacao.cpp`: extensões para geração de cilindro/esfera/prisma e interseções 3D. Não há implementação equivalente pronta nos slides fornecidos.
