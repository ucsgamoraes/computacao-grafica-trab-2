# Planejamento — Xadrez 3D em C++ e OpenGL

Construir um tabuleiro 8×8 com 32 peças geométricas reconhecíveis, texturas, múltiplos shaders, câmera navegável e seleção/movimentação pelo mouse. A entrega deve abrir, compilar e executar pelo projeto nativo `.cbp` do Code::Blocks. Não haverá regras de xadrez.

O desenvolvimento dos requisitos obrigatórios foi autorizado após a revisão do planejamento. Preserve a execução incremental e consulte as referências de cada tarefa antes de modificá-la. A funcionalidade opcional permanece separada.

## Estado da implementação

T01 a T09 possuem implementação no projeto `Xadrez3D.cbp`. Debug e Release compilam diretamente com g++, usando as configurações declaradas no projeto. Passaram 236 verificações OpenGL e testes de interface nas duas compilações: seleção, destino externo, cancelamento, movimentos após redimensionar, câmera e Escape. As primitivas e a cena foram inspecionadas visualmente. A documentação e os recursos da T10 estão preparados; falta a validação pela IDE.

**Aceite pela IDE pendente:** Code::Blocks está instalado. Os targets Debug e Release estão configurados como aplicativos gráficos para executar diretamente, sem exigir `xterm`. Falta confirmar a execução pela IDE com essa configuração. As checkboxes permanecem abertas porque os critérios incluem essa validação. Isso não significa que as funcionalidades estejam sem implementação. Consulte [README.md](README.md) para compilar, executar e reproduzir os testes auxiliares.

**Escopo implementado:** casas coloridas nos vértices, base texturizada, seis tipos de peças, posição inicial, câmera, seleção e movimento para qualquer casa. A seleção testa caixas locais de cada parte da peça. Não há regras de xadrez nem bloqueio de ocupação. O01 permanece sem implementação.

## Orientações obrigatórias aos agentes

1. Leia a tarefa, suas dependências e o critério de conclusão antes de editar.
2. **Abra os PDFs e os exemplos originais indicados nas referências antes de implementar. Examine também o código presente nas imagens dos slides.** As imagens abaixo facilitam a consulta, mas não substituem os materiais completos.
3. Reutilize e adapte as soluções do professor quando forem adequadas. Preserve a organização simples, a nomenclatura compreensível e o nível de abstração dos exemplos.
4. Consulte o [índice de referências e cuidados](REFERENCIAS.md). Não copie erros conhecidos nem configurações específicas da máquina do professor.
5. Não altere `referencias/`. Adapte cópias no projeto do trabalho. Não acrescente bibliotecas ou funcionalidades sem necessidade técnica ligada à tarefa.
6. Mantenha cada incremento executável. Atualize o `.cbp` quando incluir arquivos e valide pelo Code::Blocks antes de marcar a tarefa.
7. Registre arquivos adaptados, validação realizada e limitações encontradas. Se não puder executar a IDE, registre a pendência e mantenha a tarefa aberta.
8. Marque uma checkbox somente após cumprir todo o critério de conclusão. Não antecipe tarefas futuras.

## Base analisada e decisões iniciais

- Na análise inicial, o repositório continha o prompt, cinco apresentações e seis projetos de exemplo, além de `shaders-pt1.cpp`, sem aplicativo próprio.
- A base principal será `GLFW_Cubo_OO`: `Application` inicializa a janela; `Object` fornece transformações e `draw`; `Cube` cria a malha; `Banrisul` compõe objetos por `parts`. `Shader` e `Texture` carregam recursos.
- Usar **GLFW + GLEW + GLM + stb_image**, encontrados nos exemplos. GLFW gerencia janela/entrada; GLEW carrega funções; GLM fornece matemática; stb_image carrega imagens. GLAD, engines e bibliotecas de física não são necessários.
- Adotar OpenGL **3.3 Core / GLSL 330**, como `GLFW_Cubo_OO`. Não copiar o contexto 4.0 dos exemplos de efeitos nem os shaders `320 es` da primeira aula.
- Reutilizar `Shader.h`, `Texture`, `Cube` e a composição de `Banrisul`, com ajustes pontuais. Não criar uma engine, grafo de cena genérico ou seis hierarquias independentes de peças.
- Usar `projection * view * model`, teste de profundidade e transformações locais relativas à peça. O tabuleiro permanece estático; a navegação modifica a câmera.
- Adotar posição 3D, cor RGB e UV no vértice, seguindo `GLFW_Textures`: atributos 0/1/2 e stride de oito `float`. Ao adaptar o cubo OO, ajustar também seus dados e atributos; ele originalmente usa cinco `float` e UV no atributo 1.
- Desenhar as casas com **cores nos vértices**. Aplicar uma textura real à base do tabuleiro, reaproveitando inicialmente `pedra-28.jpg`. Isso atende ao uso de textura sem exigir um seletor de modos.
- Planejar dois programas de shader efetivamente usados na cena: cor para casas/peças e textura para a base. Podem compartilhar o vertex shader. A quantidade exata exigida pelo professor ainda precisa ser confirmada.
- Propor seleção por raio e volumes simples das peças, seguida de interseção raio/plano para o destino. **Essa técnica não foi encontrada implementada nos materiais.** T08 deverá provar que funciona antes da integração completa.

### Convenções compartilhadas

Fixar estas convenções em T02 para permitir trabalho independente:

- Mundo com Y para cima; superfície das casas em `y = 0`; base abaixo desse plano.
- Casas de lado 1; tabuleiro ocupa `[-4, 4)` em X e Z. Coluna `c` e linha `r` variam de 0 a 7.
- Centro da casa: `(c - 3.5, 0, r - 3.5)`. Conversão inversa usa `floor(x + 4)` e `floor(z + 4)`, após verificar os limites.
- Coluna 0 corresponde a A; linha 0 corresponde à primeira fileira das brancas. A1 é escura. Brancas iniciam nas linhas 0/1; pretas nas linhas 7/6.
- Cada peça possui identidade estável, tipo, lado, linha/coluna e transformação principal. As partes usam coordenadas locais; a base da peça repousa em `y = 0`.
- Câmera, desenho e seleção usam as mesmas matrizes e dimensões atuais do viewport. Não manter uma segunda câmera exclusiva para o mouse.

### Organização sugerida

```text
Xadrez3D.cbp
main.cpp
include/                    # Application, Object, Cube, Shader, Texture
                            # Tabuleiro, Peca, Camera e primitivas necessárias
src/                        # Implementações correspondentes e stb_image.cpp
shaders/                    # cena.vert, cor.frag, textura.frag
res/images/                 # Texturas efetivamente utilizadas
bin/Debug/ e bin/Release/    # Saídas de compilação
obj/                        # Objetos de compilação
```

Nomes são uma sugestão; mantenha a organização já estabelecida quando a tarefa for executada. `Peca` pode guardar o tipo e uma lista de partes, como `Banrisul`. Funções de seleção podem ficar junto da interação em `main.cpp`; extraia um arquivo apenas se houver benefício concreto. `Mesh` existe no exemplo, mas não é obrigatório introduzir outra camada sobre as primitivas.

## Etapa 1 — Ambiente e renderização

### T01 — Preparar o projeto Code::Blocks

- [ ] **T01 concluída**

**Objetivo:** criar o `.cbp` do trabalho e obter uma janela OpenGL executada pela IDE.

**Atividades:**

- Identificar sistema operacional, compilador e bibliotecas disponíveis no ambiente do grupo. Registrar como reproduzir a configuração.
- Verificar o funcionamento de uma cópia de `GLFW_Cubo_OO`; preservar o original. Usar seu `.cbp`, `main.cpp` e `Application` como base do trabalho.
- Configurar Debug e Release com includes e bibliotecas necessários. Substituir caminhos pessoais e vinculações incompatíveis por configurações adequadas ao ambiente; não copiar entradas duplicadas dos projetos Windows.
- Usar C++14 ou superior se preservar `std::make_unique`, presente em `Banrisul`.
- Definir o diretório de execução como a raiz do projeto, garantindo caminhos relativos para `shaders/` e `res/images/`. Compilar stb_image em uma única unidade.
- Manter um único loop principal; não combinar `Application::run()` com outro loop em `main.cpp`.

**Dependências:** nenhuma. **Atenção:** bloqueia todas as demais tarefas.

**Critério de conclusão:** abrir `Xadrez3D.cbp`, compilar Debug e Release e executar pela IDE. A janela informa a versão OpenGL, processa eventos e fecha corretamente; não há caminhos da máquina do professor no projeto adaptado.

**Referências:** [R1](REFERENCIAS.md#r1), slides 21–23 e 27–29; [R3](REFERENCIAS.md#r3), slides 3–8 sobre GLM; [E1](REFERENCIAS.md#e1), `GLFW_Cubo_OO.cbp`, `main.cpp` e `src/Application.cpp`. Adaptar diretamente essa base, sem criar outro sistema de build obrigatório.

### T02 — Renderizar um cubo com transformações e profundidade

- [ ] **T02 concluída**

**Objetivo:** estabelecer a renderização comum e as convenções utilizadas pelos demais agentes.

**Atividades:**

- Adaptar `Cube`, `Object` e `Shader` para desenhar um cubo colorido com VAO/VBO/EBO, mantendo a abordagem do exemplo OO.
- Fixar o layout de posição/cor/UV indicado acima. Criar o vertex shader com MVP e o fragment shader de cor em arquivos separados.
- Inicializar matrizes com identidade, aplicar translação/rotação/escala e enviar uniforms ao programa ativo. Não acumular transformações entre quadros ou entre objetos independentes.
- Ativar `GL_DEPTH_TEST` e limpar cor e profundidade a cada quadro. Atualizar `glViewport` e o aspecto da projeção no redimensionamento; ignorar renderização com framebuffer de altura zero.
- Registrar as convenções de coordenadas e a assinatura de desenho antes de dividir tarefas. Manter uma única posição/rotação/escala por objeto, evitando o sombreamento de campos encontrado em `Cube`.
- Garantir mensagens de falha de leitura, compilação e link de shader; liberar recursos OpenGL antes de destruir o contexto.

**Dependências:** T01.

**Critério de conclusão:** cubo sólido, colorido e posicionado em perspectiva, sem faces traseiras sobrepostas às dianteiras. Redimensionar a janela preserva proporções; shaders são encontrados pela IDE e falhas são identificáveis no console.

**Referências:** [R2](REFERENCIAS.md#r2), slides 34–48; [R3](REFERENCIAS.md#r3), conceitos 10–30 e 39–51, implementações 54–62 e 65–72; [R4](REFERENCIAS.md#r4), 16–23 e 32–45. Adaptar [E1](REFERENCIAS.md#e1), `src/Cube.cpp`, `include/Shader.h` e `vertex.glsl`; usar [E4](REFERENCIAS.md#e4), `main.cpp`, para posição/cor/UV.

**Código nos slides — conferir antes de adaptar:** R4 p. 21 mostra a criação do VAO. R3 pp. 61–62 mostram uniforms e MVP; p. 72 mostra profundidade. Em R3 p. 61, usar `GLint` para localizações, que podem retornar `-1`.

![R4, slide 21: criar e vincular o VAO antes dos atributos](plan/imagens/shaders-pt2-p21-vao.png)

![R3, slide 61: localizar uniforms e enviar matrizes](plan/imagens/objetos-3d-p61-uniforms.png)

![R3, slide 62: transformação projection * view * model no vertex shader](plan/imagens/objetos-3d-p62-vertex-shader.png)

![R3, slide 72: teste e limpeza do buffer de profundidade](plan/imagens/objetos-3d-p72-profundidade.png)

## Etapa 2 — Tabuleiro e peças

### T03 — Construir o tabuleiro e as conversões de coordenadas

- [ ] **T03 concluída**

**Objetivo:** desenhar as 64 casas sobre uma base com espessura e fornecer suas posições às peças e à interação.

**Atividades:**

- Criar `Tabuleiro` com laços de linha/coluna e alternância de cores por `(r + c) % 2`.
- Reutilizar cubos achatados para as casas e um cubo maior para a base, sem superfícies coincidentes que causem cintilação.
- Usar as convenções compartilhadas; centralizar as conversões casa/mundo em funções reutilizadas pelo posicionamento e pelo mouse.
- Preparar a base para receber a textura em T06. Não incluir regras de ocupação ou movimento nesta tarefa.

**Dependências:** T02.

**Critério de conclusão:** 8 linhas, 8 colunas, 32 casas claras e 32 escuras; A1 escura e espessura visível. Conferir os centros das quatro casas de canto e rejeitar coordenadas fora de `[-4, 4)`.

**Referências:** [R3](REFERENCIAS.md#r3), 20–23 e 54–62; [E1](REFERENCIAS.md#e1), `src/Cube.cpp` e `src/Banrisul.cpp`, para repetição e transformação de objetos; [E4](REFERENCIAS.md#e4), `main.cpp` e `vertex.glsl`, para cores por vértice. A organização do tabuleiro é uma adaptação do trabalho, não um exemplo pronto das aulas.

### T04 — Preparar as primitivas necessárias às peças

- [ ] **T04 concluída**

**Objetivo:** fornecer cubo, cilindro, esfera e prisma triangular simples, compatíveis com o mesmo desenho e transformações.

**Atividades:**

- Reutilizar `Cube`. Criar cilindro com tampas e esfera com triangulação simples, sem bibliotecas adicionais.
- Usar a estrutura de `Cube` como referência para buffers, índices e UV. Limitar a resolução das formas ao necessário para reconhecimento visual.
- Construir um prisma realmente triangular. **`Prisma.cpp` do professor repete a geometria de um cubo**, portanto não atende sozinho a esse objetivo.
- Preservar a transformação do objeto pai em `draw`. Criar buffers na inicialização e cuidar da liberação; não gerar malhas a cada quadro.

**Dependências:** T02. Pode ocorrer em paralelo com T03 e T07.

**Critério de conclusão:** apresentar as quatro primitivas lado a lado na cena, com volume, tampas e transformações independentes. Todas aceitam o mesmo shader e uma transformação pai.

**Referências:** [E1](REFERENCIAS.md#e1), `include/Object.h`, `src/Cube.cpp`, `src/Prisma.cpp` e `src/Banrisul.cpp`; [R3](REFERENCIAS.md#r3), 65–67 e atividade 78, sobre alterar a geometria; [R4](REFERENCIAS.md#r4), 25–36, sobre índices. **Não foram encontrados exemplos prontos de esfera/cilindro.** A geração dessas malhas é uma extensão necessária, proporcional ao trabalho.

### T05 — Compor os seis tipos e posicionar as 32 peças

- [ ] **T05 concluída**

**Objetivo:** formar peças reconhecíveis por composição geométrica e montar a posição inicial convencional.

**Atividades:**

- Adaptar `Banrisul::init()` e `Banrisul::draw()` para uma `Peca` com tipo, lado e partes. Não copiar a rotação automática da demonstração.
- Usar uma base comum e silhuetas diferentes: peão com cabeça esférica; torre com ameias; bispo com cabeça alongada e entalhe sugerido; dama com coroa; rei com cruz; cavalo com pescoço inclinado, cabeça, focinho e orelhas.
- Compor o cavalo com cubos/prismas, garantindo perfil semelhante a um cavalo. Um bloco genérico não atende ao requisito.
- Distinguir os lados por cores claras/escuras legíveis. Ajustar largura e altura à escala das casas e orientar os cavalos dos lados opostos para o centro.
- Montar 16 peças por lado: oito peões, duas torres, dois cavalos, dois bispos, uma dama e um rei. Na linha principal, usar torre/cavalo/bispo/dama/rei/bispo/cavalo/torre, de A a H.
- Usar os centros fornecidos por `Tabuleiro`; dama branca em D1 e preta em D8, rei em E1/E8. Guardar limites geométricos simples de cada peça para T08.
- Manter identidade e transformação principal por peça, de modo que mover uma instância leve todas as partes.

**Dependências:** T03 e T04. **Atenção:** reconhecimento do cavalo e transformação das partes exigem revisão visual.

**Critério de conclusão:** cena com 32 peças, todas apoiadas nas casas corretas. Os seis tipos são distinguíveis em vista oblíqua, os lados são identificáveis e o cavalo tem perfil reconhecível. Alterar a posição de uma peça move apenas aquela instância inteira.

**Referências:** [E1](REFERENCIAS.md#e1), `include/Banrisul.h`, `src/Banrisul.cpp` e `src/Cube.cpp`, para `parts` e transformação pai/filho; [R3](REFERENCIAS.md#r3), 20–23 e 54–62. As aulas fornecem composição e transformações, mas não modelos de peças de xadrez.

## Etapa 3 — Aparência e navegação

### T06 — Aplicar textura e usar múltiplos programas de shader

- [ ] **T06 concluída**

**Objetivo:** integrar textura real à base e manter os programas de cor e textura ativos em desenhos distintos da mesma cena.

**Atividades:**

- Adaptar `Texture` do exemplo OO e copiar somente a imagem utilizada para `res/images/`.
- Manter o programa de cor da T02 e criar o programa de textura, compartilhando o vertex shader com MVP, cor e UV quando conveniente.
- Desenhar casas e peças pelo programa de cor; desenhar a base pelo programa de textura. Atualizar `view`, `projection`, `model` e uniforms no programa correspondente.
- Conferir UV, unidade da textura, sampler, canais RGB/RGBA, alinhamento de linhas e filtros/mipmaps. Não presumir canais apenas pela extensão do arquivo.
- Manter o contraste entre casas e lados. Não adicionar iluminação avançada, sombras ou efeitos de ShaderToy.

**Dependências:** T03. Pode avançar em paralelo com T04/T05/T07; a integração com todas as peças é conferida em T10.

**Critério de conclusão:** a base exibe uma imagem carregada do disco; casas e peças mantêm suas cores. Dois programas vinculados distintos são realmente usados no mesmo quadro, sem erros de uniforms, atributos ou caminhos na execução pela IDE.

**Referências:** [R4](REFERENCIAS.md#r4), conceitos 47–51, implementações 52–57 e 64–65, cuidados 58–63; [E1](REFERENCIAS.md#e1), `src/Texture.cpp`, `include/Shader.h` e `fragment.glsl`; [E4](REFERENCIAS.md#e4), `main.cpp` e shaders. Adaptar a carga de textura; a divisão em dois programas é uma decisão deste projeto.

**Código nos slides — conferir antes de adaptar:** R4 p. 54 usa posição/cor/UV, mas ainda não aplica MVP; combinar com R3 p. 62. O stride 2D da p. 23 é apenas didático: neste projeto serão oito `float`. A p. 64 mostra carga e liberação da imagem; escolher o formato pelos canais retornados.

![R4, slide 23: conceito de stride; adaptar ao layout 3D de posição, cor e UV](plan/imagens/shaders-pt2-p23-layout.png)

![R4, slide 54: comunicação de cor e UV entre vertex e fragment shader](plan/imagens/shaders-pt2-p54-textura-shaders.png)

![R4, slide 64: stb_image, upload da textura, mipmaps e liberação](plan/imagens/shaders-pt2-p64-carga-textura.png)

### T07 — Implementar a câmera navegável

- [ ] **T07 concluída**

**Objetivo:** permitir caminhar pela cena e orientar a visão, mantendo o mouse livre para selecionar peças.

**Atividades:**

- Adaptar `cameraPos`, `cameraFront`, `cameraUp`, `glm::lookAt` e `viraCamera` dos slides.
- Usar W/A/S/D para deslocamento no plano XZ, setas para yaw/pitch e Q/Z para altura. Separar direção de caminhada da inclinação do olhar, mantendo altura ao andar.
- Inicializar uma visão oblíqua que mostre o tabuleiro inteiro. Limitar pitch a `[-89°, 89°]` e normalizar vetores.
- Adaptar velocidades ao tempo entre quadros com `glfwGetTime()`. A velocidade fixa por quadro dos slides varia entre computadores.
- Atualizar uma única matriz `view` e disponibilizá-la à renderização e à interação. Não capturar o cursor para girar a câmera.

**Dependências:** T02. Pode ser validada inicialmente com o cubo, em paralelo com T03/T04.

**Critério de conclusão:** andar em frente/trás e lateralmente, olhar para todos os lados e variar a altura. Andar olhando para cima não provoca voo; não há inversão ao limitar pitch. O cursor continua disponível, e a câmera funciona com janela redimensionada.

**Referências:** [R5](REFERENCIAS.md#r5), conceitos 14–26 e 34–43, implementações 31–33 e 45–49; [E2](REFERENCIAS.md#e2), `main.cpp`, para integrar a matriz de visualização. Os controles completos estão nos slides; não existe uma classe `Camera` pronta no exemplo fornecido.

**Código nos slides — conferir antes de adaptar:** na p. 33, corrigir `camSpeed` para o mesmo nome de `cameraSpeed`. A p. 47 fornece yaw/pitch e o limite de inclinação; adaptar o incremento ao tempo entre quadros.

![R5, slide 33: entrada W/A/S/D e vetores de deslocamento](plan/imagens/camera-p33-movimento.png)

![R5, slide 47: viraCamera, yaw/pitch e normalização de cameraFront](plan/imagens/camera-p47-orientacao.png)

## Etapa 4 — Interação

### T08 — Selecionar uma peça com o mouse

- [ ] **T08 concluída**

**Objetivo:** converter um clique na janela em uma peça selecionada na cena 3D.

**Atividades:**

- Adaptar os callbacks de cursor e botão de `GLFW_Shaders6` somente para obter posição e evento de clique. Processar uma seleção por pressão, sem repetir a ação a cada quadro.
- Provar primeiro a seleção de um cubo conhecido, usando a câmera e as matrizes reais. Depois integrar as peças.
- Converter cursor de coordenadas da janela para framebuffer/viewport, incluindo escala de pixels e inversão de Y. Obter um raio mundial pelos pontos near/far transformados pela inversa de `projection * view`, com divisão por W.
- Intersectar o raio com caixas delimitadoras simples das peças e escolher a interseção positiva mais próxima. Ajustar os limites às silhuetas para evitar que uma caixa excessiva selecione a peça vizinha.
- Respeitar a oclusão pelo tabuleiro: não selecionar peças através da base quando a câmera estiver abaixo dela.
- Guardar o identificador da peça e destacá-la por cor/uniform; clique sem acerto limpa a seleção nesta etapa. O botão direito cancela a seleção.
- Usar os limites e posições atuais, não as coordenadas iniciais. Não adicionar renderização por IDs, framebuffer extra ou uma biblioteca de picking sem necessidade comprovada.

**Dependências:** T03, T05 e T07. **Dificuldade alta:** principal risco técnico; os materiais não fornecem picking 3D completo.

**Critério de conclusão:** selecionar individualmente peças de ambos os lados e tipos, inclusive após mover a câmera e redimensionar a janela. Na sobreposição visual, escolher a peça mais próxima; clicar fora das peças não seleciona uma peça aleatória. Registrar os resultados do protótipo e da cena completa.

**Referências:** [E5](REFERENCIAS.md#e5), `main.cpp`, linhas 82–110 e registro dos callbacks em 153–162; [R3](REFERENCIAS.md#r3), 13–30 e 50–62, para espaços/MVP/viewport; [R5](REFERENCIAS.md#r5), 26 e 31, para a câmera. **Essas referências cobrem entrada e transformações, não o algoritmo de raio/interseção.** Implementar a extensão com GLM e funções locais pequenas; registrar a derivação usada e não atribuí-la aos slides.

### T09 — Mover a peça selecionada para outra casa

- [ ] **T09 concluída**

**Objetivo:** escolher um destino pelo mouse e mover a peça inteira sem validar jogadas de xadrez.

**Atividades:**

- Integrar um fluxo de dois cliques: primeiro selecionar a peça, depois indicar a casa de destino. Botão direito cancela.
- Com seleção ativa, interpretar o segundo clique como destino, inclusive se a casa estiver ocupada. Sem seleção, interpretar o clique como tentativa de selecionar uma peça.
- Intersectar o raio com `y = 0`. Rejeitar raio paralelo, interseção atrás da origem e ponto fora do tabuleiro, preservando a seleção para nova tentativa.
- Converter o ponto para linha/coluna e mover ao centro da casa. Atualizar a transformação principal, o estado e os limites usados na seleção; limpar a seleção após um movimento válido.
- Permitir qualquer casa, qualquer distância e ambos os lados, sem turnos, captura, promoção, xeque ou validação por tipo.
- Sem O01, permitir coexistência na casa ocupada: não remover nem capturar a outra peça. A restrição de ocupação permanece opcional.

**Dependências:** T08. Reutiliza as conversões de T03.

**Critério de conclusão:** mover um peão várias casas, uma torre na diagonal e uma peça preta sem alternância de turno. Mover a mesma peça novamente após mudar a câmera; todas as partes e o limite de seleção acompanham a posição. Destino externo não altera o estado. Na versão obrigatória, uma casa ocupada não bloqueia o deslocamento.

**Referências:** [E1](REFERENCIAS.md#e1), `src/Banrisul.cpp`, para mover uma composição pela transformação pai; [R3](REFERENCIAS.md#r3), 20–23 e 50–62; [E5](REFERENCIAS.md#e5), callbacks citados em T08. Interseção raio/plano e conversão para casas são extensões do projeto; não há implementação pronta nos slides.

## Etapa 5 — Integração e entrega

### T10 — Validar os requisitos e preparar a apresentação

- [ ] **T10 concluída**

**Objetivo:** comprovar a entrega completa pelo Code::Blocks, com arquivos e recursos necessários.

**Atividades:**

- Abrir o `.cbp` em uma cópia limpa do projeto no ambiente do grupo; recompilar Debug e Release e executar pela IDE.
- Conferir 64 casas, espessura, 32 peças na posição inicial, seis tipos reconhecíveis, lados distintos e cavalo semelhante a um cavalo.
- Demonstrar textura real e dois programas de shader desenhando a cena; caminhar, orientar e elevar/abaixar a câmera.
- Selecionar e mover peças de ambos os lados após navegar e redimensionar. Conferir cliques nas bordas, destinos externos e casas ocupadas conforme a opção escolhida.
- Conferir arquivos listados no `.cbp`, diretório de execução, ausência de caminhos pessoais e recursos presentes. Não depender de executáveis já compilados ou da pasta `referencias/` para executar.
- Preparar instruções curtas de abertura, dependências, compilação e controles. Registrar evidências visuais e problemas restantes, sem adicionar novas funcionalidades.

**Dependências:** T01 a T09; O01 somente se solicitada e incorporada.

**Critério de conclusão:** todas as verificações acima reproduzidas na IDE e registradas. A pasta entregue contém fontes, `.cbp`, shaders e imagens necessários; não há requisito obrigatório pendente nem erro conhecido que impeça a demonstração.

**Referências:** [prompt do trabalho](plan/prompt.md), [E1](REFERENCIAS.md#e1) como modelo de entrega e referências específicas de cada tarefa. Não afirmar execução pelo Code::Blocks sem realizá-la.

## Opcional — Executar somente depois dos obrigatórios

### O01 — Impedir sobreposição de peças

- [ ] **O01 concluída, se solicitada**

**Objetivo:** impedir que duas peças terminem na mesma casa, sem introduzir regras de xadrez.

**Atividades:** consultar o estado das peças antes do movimento; rejeitar destino ocupado por outra identidade; preservar seleção e posição quando rejeitado. A própria casa continua válida. Usar o vetor de peças existente, sem criar sistema de física ou captura.

**Dependências:** T09 e conclusão dos requisitos obrigatórios; repetir em T10 a verificação de movimentação afetada.

**Critério de conclusão:** destino vazio aceita movimento; destino ocupado rejeita sem alterar nenhuma peça; selecionar e mover novamente continua funcionando.

**Referências:** [prompt](plan/prompt.md), que define a restrição como opcional. Não foi encontrado exemplo específico dessa regra nos materiais.

## Sequência e divisão possível entre até três agentes

T01 e T02 são a base comum e devem terminar primeiro. Depois, três frentes podem avançar: T03/T06 (tabuleiro e aparência), T04/T05 (primitivas e peças, integrando T03) e T07 (câmera). T08 depende da integração dessas geometrias com a câmera; T09 vem depois. T10 encerra os obrigatórios.

Não dividir o mesmo arquivo de inicialização entre agentes simultaneamente. Combinar previamente o layout dos vértices, a assinatura de `draw`, as coordenadas e o estado das peças. Um agente integra as alterações de `main.cpp` e do `.cbp`; os demais trabalham nos componentes da tarefa. Essa é uma possibilidade de divisão futura, sem atribuição de responsáveis ou execução nesta etapa.

## Limitações e decisões a confirmar

- **Enunciado original:** não foi encontrado outro documento de requisitos além de `plan/prompt.md`. Confirmar com o grupo se existe rubrica adicional.
- **Múltiplos shaders:** adota-se provisoriamente dois programas distintos. Confirmar quantidade e efeitos exigidos antes de T06; vertex + fragment de um único programa não será considerado prova suficiente neste plano.
- **Movimentação livre:** interpreta-se como qualquer casa do tabuleiro, com encaixe ao centro, sem regras de jogada. Confirmar antes de T09 se o professor exige arraste contínuo ou posição livre dentro da casa.
- **Ambiente de entrega:** bibliotecas verificadas no Arch Linux: GLFW 3.5.1, GLEW 2.3.1, GLM 1.0.3 e Mesa 26.2.6; compilador GCC 16.2.1. Execução gráfica verificada com OpenGL 3.3/NVIDIA. O projeto seleciona X11/XWayland para compatibilidade do GLEW com GLX. Falta validar a IDE Code::Blocks.
- **Material ausente:** `Introdução OpenGL.pdf`, slide 39, cita `Setup OpenGL.pdf`, que não está em `referencias/`. Usar os `.cbp` existentes e registrar a configuração efetivamente validada.
- **Referências insuficientes:** não há código pronto para picking 3D, interseção raio/plano, cilindro/esfera, malhas de xadrez ou gerenciamento de ocupação. T04/T05/T08/T09 identificam explicitamente as adaptações necessárias.
- **Validação realizada:** materiais e imagens consultados; cópia do exemplo OO compilada e executada fora de `referencias/`, com ajuste X11; projeto próprio compilado em Debug/Release; primitivas e cena inspecionadas; testes de integração OpenGL executados. A validação pela IDE continua pendente. Evidências em [plan/evidencias/](plan/evidencias/).
