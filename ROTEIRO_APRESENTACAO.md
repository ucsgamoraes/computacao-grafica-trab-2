# Xadrez 3D — roteiro de apresentação e revisão

Este guia cruza as cinco apresentações em `referencias/aulas/` com o código atual do projeto. As páginas citadas são páginas físicas do PDF, contando a capa. As falas são sugestões para ensaio; adapte a pessoa verbal à participação real de cada integrante.

## 1. Ideia central que você precisa defender

O trabalho é uma aplicação interativa de Computação Gráfica: um tabuleiro de xadrez 3D, com peças construídas por composição geométrica, duas formas de renderização, câmera navegável e interação pelo mouse.

O objetivo é demonstrar modelagem, transformações, projeção, shaders, textura e conversão entre coordenadas. Não há regras de xadrez, turnos ou captura. Como no opcional do enunciado, uma peça não pode ir para uma casa ocupada. A movimentação ocorre entre centros de casas, por dois cliques, sem arrastar e sem animação de percurso.

O encadeamento que organiza toda a explicação é:

1. Gerar geometria no espaço local.
2. Compor e posicionar objetos usando matrizes.
3. Observar a cena com uma câmera em perspectiva.
4. Desenhar triângulos com shaders e teste de profundidade.
5. Converter o clique em um raio para selecionar e mover peças.

## 2. Roteiro falado — cerca de 10 a 12 minutos, incluindo demonstração

### 0:00–0:45 — Objetivo e resultado

**Mostre:** a cena na posição inicial.

**Fala sugerida:**

> “O trabalho representa um tabuleiro de xadrez em um ambiente tridimensional. A cena tem 64 casas e 32 peças na posição inicial convencional. As peças são construídas com formas geométricas básicas. Podemos navegar pela cena, selecionar uma peça e movê-la para outra casa. O foco é aplicar os conceitos de Computação Gráfica estudados nas aulas.”

Acrescente que as regras do xadrez não fazem parte do escopo. Isso explica por que movimentos diferentes das regras são permitidos.

### 0:45–1:30 — Tecnologias e organização

**Mostre:** `Xadrez3D.cbp` e a estrutura de arquivos, se houver tempo.

**Fala sugerida:**

> “A aplicação usa C++14 e OpenGL 3.3 Core. GLFW cria a janela, o contexto e recebe as entradas. GLEW disponibiliza as funções do OpenGL. GLM fornece vetores e matrizes. A biblioteca stb_image lê a imagem utilizada como textura. A organização parte do exemplo GLFW_Cubo_OO do professor, principalmente das transformações dos objetos e da composição em partes.”

> “Application cuida da inicialização. Mesh administra a geometria e os buffers. Peca reúne as formas de cada peça. Tabuleiro mantém as casas e suas coordenadas. Camera controla o ponto de vista. Interacao trata os raios e as interseções.”

**Referências:** Introdução OpenGL, pp. 21–23 e 37; OpenGL e Objetos Tridimensionais, pp. 3–8; exemplo `GLFW_Cubo_OO`.

### 1:30–2:45 — Como a geometria vira imagem

**Mostre:** `src/Mesh.cpp` e `shaders/cena.vert`.

**Fala sugerida:**

> “Os objetos são representados por malhas de triângulos. Cada vértice contém posição, cor e coordenadas de textura. O VBO armazena esses dados. O EBO armazena os índices que formam os triângulos. O VAO registra como os atributos serão lidos. O desenho usa glDrawElements.”

> “O vertex shader transforma a posição de cada vértice. Depois, o OpenGL monta e rasteriza os triângulos, produzindo fragmentos. As cores e coordenadas de textura são interpoladas. O fragment shader calcula a cor de cada fragmento. Os testes de profundidade determinam quais superfícies ficam visíveis.”

Explique o layout somente se necessário: posição com três floats, cor com três e UV com dois; oito floats por vértice.

**Referências:** shaders-pt1, pp. 12–16, 23–32 e 34–48; shaders-pt2, pp. 20–36 e 52–57.

### 2:45–4:00 — Tabuleiro, coordenadas e matrizes

**Mostre:** `Tabuleiro::centro`, `Tabuleiro::casa` e a linha de `gl_Position`.

**Fala sugerida:**

> “O tabuleiro usa o plano XZ, com Y indicando a altura. A superfície das casas está em Y igual a zero. Cada casa tem lado um. Dois laços geram as oito linhas e oito colunas. A paridade da soma da linha com a coluna alterna as cores.”

> “O tabuleiro fica centralizado na origem. O centro de uma casa é calculado por 3,5 menos a coluna e linha menos 3,5. A coluna entra invertida para que, vista do lado das brancas, a coluna A fique à esquerda, como num tabuleiro real. Essa mesma conversão posiciona as peças e define seus destinos. Assim, o desenho e a interação seguem uma única convenção.”

> “No shader, usamos projection vezes view vezes model vezes o vértice. Model posiciona o objeto no mundo. View expressa a cena em relação à câmera. Projection define a perspectiva. A multiplicação atua da direita para a esquerda.”

**Referências:** OpenGL e Objetos Tridimensionais, pp. 10–30, 39–51 e 54–62. A divisão em casas é uma aplicação desses conceitos ao trabalho.

### 4:00–5:15 — Modelagem e composição das peças

**Mostre:** um cavalo, um rei e uma dama. Se necessário, abra `Peca::init` e `Peca::desenhar`.

**Fala sugerida:**

> “As peças são compostas por cubos, cilindros, esferas e prismas triangulares. O peão tem cabeça esférica. A torre possui ameias. O bispo tem uma mitra com entalhe. A dama tem uma coroa. O rei tem uma cruz. O cavalo combina pescoço inclinado, cabeça, focinho, orelhas e olhos.”

> “Cada parte possui sua transformação local. A transformação da peça funciona como transformação pai. Multiplicamos a matriz da peça pela matriz de cada parte. Por isso, mudar a posição da peça desloca todas as partes juntas, mantendo o formato.”

> “Essa composição segue a ideia de Banrisul no exemplo do professor. A geração de cilindros, esferas e do prisma triangular complementa a base fornecida.”

**Referências:** exemplo `GLFW_Cubo_OO`, especialmente `Banrisul`; OpenGL e Objetos Tridimensionais, pp. 20–23, 54–62 e 65–67; shaders-pt2, pp. 25–36.

### 5:15–6:15 — Shaders, cor e textura

**Mostre:** a base e as peças texturizadas, os enfeites em cor sólida e os três arquivos em `shaders/`.

**Fala sugerida:**

> “A cena usa dois programas de shader. Eles compartilham o vertex shader cena.vert. O programa com textura.frag desenha a base e o corpo das peças com a imagem de madeira. O programa com cor.frag desenha as casas e os enfeites das peças: cabeça do peão, ameias da torre, orelhas e olhos do cavalo, bolinha do bispo, coroa da dama e cruz do rei. Portanto, são dois programas, formados a partir de três arquivos.”

> “Cada parte de uma peça guarda o campo texturizada. Em Peca::desenhar, cada parte ativa o programa correspondente antes de ser desenhada. Assim, toda peça é composta por formas básicas e desenhada com os dois shaders.”

> “O textura.frag multiplica a cor da imagem pela cor do vértice. Por isso a mesma imagem fica clara nas peças brancas e escura nas pretas.”

> “As coordenadas UV relacionam a imagem à superfície. A imagem é carregada com stb_image e enviada ao OpenGL. O fragment shader consulta a textura usando essas coordenadas. O programa também gera mipmaps para a amostragem quando a textura aparece reduzida.”

> “A seleção muda um uniform booleano, presente nos dois fragment shaders. Eles misturam a cor da peça com uma cor alaranjada. A sensação de volume vem de variações de cor nos vértices; o projeto não calcula iluminação dinâmica.”

**Referências:** shaders-pt2, pp. 38–65.

### 6:15–7:15 — Câmera e profundidade

**Faça:** caminhe com W/A/S/D, olhe com as setas e ajuste a altura com Q/Z.

**Fala sugerida:**

> “A câmera usa posição, direção frontal e vetor para cima. A função lookAt gera a matriz de visualização. As setas alteram yaw e pitch, que controlam a orientação horizontal e vertical. O pitch fica limitado entre menos 89 e mais 89 graus para evitar uma direção paralela ao vetor para cima.”

> “W, A, S e D deslocam a câmera no plano horizontal. Q e Z alteram a altura. O movimento considera o tempo entre quadros, reduzindo sua dependência da taxa de renderização.”

> “A projeção é em perspectiva, com abertura vertical de 45 graus. Ao redimensionar, atualizamos o viewport e a proporção. O teste de profundidade faz as superfícies próximas ocultarem as distantes.”

**Referências:** Câmera Virtual, pp. 14–26, 31–43 e 45–50; OpenGL e Objetos Tridimensionais, pp. 44–51 e 68–72.

### 7:15–9:15 — Seleção e movimento pelo mouse

**Faça:** selecione um peão, mostre o destaque e clique em uma casa vazia. Depois, demonstre cancelamento com o botão direito.

**Fala sugerida:**

> “Um clique fornece uma posição bidimensional. Essa posição não define sozinha um ponto no espaço: ela define uma direção de observação. Por isso, convertemos o cursor em um raio tridimensional usando as mesmas matrizes da renderização.”

> “Primeiro, convertemos o cursor para coordenadas normalizadas. Aplicamos a inversa de projection vezes view a um ponto do plano próximo e outro do plano distante. A diferença entre esses pontos define a direção do raio.”

> “Para selecionar, testamos esse raio contra caixas que envolvem as partes das peças. O raio é transformado para o espaço local de cada parte. Escolhemos a interseção mais próxima, considerando também a oclusão pelo tabuleiro.”

> “No segundo clique, intersectamos o raio com o plano Y igual a zero. Convertemos o ponto encontrado para linha e coluna. A peça passa para o centro da casa. Um clique fora mantém a seleção. O botão direito cancela.”

Finalize essa parte com:

> “A técnica de seleção por raio é uma extensão do trabalho. Os materiais fornecem os fundamentos de coordenadas, matrizes e entrada do mouse, mas não uma seleção 3D pronta.”

### 9:15–10:15 — Demonstração complementar

1. Redimensione a janela e mostre que o tabuleiro preserva as proporções.
2. Selecione novamente uma peça já movida.
3. Se houver tempo, mova uma torre para uma casa fora de sua regra convencional e explique o movimento livre.
4. Selecione uma peça e clique numa casa ocupada: o título da janela avisa que a casa está ocupada e a seleção continua. Depois clique numa casa vazia.

**Fala sugerida:**

> “A seleção continua funcionando depois de mover a câmera, redimensionar a janela ou deslocar uma peça, porque utiliza as transformações atuais. O movimento é livre entre casas. Não há validação das regras, turnos ou captura.”

### 10:15–11:00 — Encerramento

**Fala sugerida:**

> “O trabalho integra os conceitos das aulas em uma cena interativa: malhas trianguladas, composição geométrica, transformações, projeção em perspectiva, shaders, textura, câmera e teste de profundidade. A interação acrescenta o caminho inverso: transformar um clique na tela em uma ação sobre um objeto no mundo tridimensional.”

Se o tempo for de cinco minutos, priorize objetivo, composição e MVP, dois programas de shader, câmera e demonstração da seleção. Reserve os detalhes dos buffers e das interseções para perguntas.

## 3. Fundamentos para responder sem decorar o código

### 3.1 Bibliotecas e inicialização

- **OpenGL:** API gráfica utilizada para configurar o pipeline e emitir comandos de renderização. Não cria a janela por conta própria.
- **GLFW:** cria a janela e o contexto OpenGL; fornece eventos e consulta de teclado e mouse.
- **GLEW:** carrega os pontos de entrada das funções OpenGL disponíveis no ambiente. Sua inicialização ocorre depois de tornar o contexto corrente.
- **GLM:** realiza a matemática na CPU, com tipos e operações semelhantes aos usados em GLSL.
- **stb_image:** decodifica a imagem em pixels na CPU. `glTexImage2D` envia esses pixels ao OpenGL.
- **GLSL:** linguagem dos shaders executados na GPU. O projeto usa `#version 330 core`.
- **C++14:** permite, entre outros recursos utilizados, `std::make_unique`.

O perfil Core usa shaders e buffers. O código legado com `glBegin` e `glVertex` mostrado na introdução não é a abordagem deste projeto.

### 3.2 Geometria, atributos e índices

Uma malha descreve uma superfície por vértices e triângulos. Um vértice inclui todos os seus atributos, não somente a posição.

O layout do trabalho é:

| Atributo | Localização | Componentes | Deslocamento |
| --- | --- | --- | --- |
| Posição | 0 | x, y, z | 0 |
| Cor | 1 | r, g, b | `3 * sizeof(float)` |
| UV | 2 | u, v | `6 * sizeof(float)` |

O stride é `8 * sizeof(float)`: a distância em bytes entre o início de dois vértices consecutivos.

O VBO contém os atributos. O EBO, também chamado IBO nos slides, contém índices. O VAO registra a configuração dos atributos, suas associações aos buffers e a associação do EBO. O VAO não é o vetor de posições.

O cubo do projeto possui 24 registros de vértice e 36 índices, formando 12 triângulos. Existem oito posições de canto distintas, mas faces diferentes precisam de registros próprios para suas cores e UV. Índices reutilizam vértices cujos atributos são compatíveis.

O cilindro usa 24 segmentos, com laterais e tampas trianguladas. A esfera usa 12 divisões latitudinais e 24 longitudinais. O prisma é um triângulo extrudado, com duas faces triangulares e três retangulares subdivididas.

As malhas são criadas na inicialização. O movimento altera matrizes, sem reconstruir os vértices a cada quadro. A lógica de buffers é reutilizada pela classe `Mesh`; o código não usa instancing nem um único VBO compartilhado entre todas as instâncias.

### 3.3 Pipeline e comunicação com os shaders

1. A CPU prepara os dados e envia comandos.
2. O vertex shader processa os atributos dos vértices e produz posições em espaço de recorte.
3. O OpenGL monta e recorta as primitivas.
4. A divisão por W produz coordenadas normalizadas; o viewport relaciona essas coordenadas à área de desenho.
5. A rasterização gera fragmentos e interpola atributos.
6. O fragment shader calcula cores.
7. Operações como o teste de profundidade controlam a escrita nos buffers.

Um fragmento é um candidato a contribuir para um pixel. Ele pode ser rejeitado. Não diga que todo fragmento necessariamente aparece na imagem final.

No vertex shader, `in` recebe os atributos; `out` fornece dados para a etapa seguinte. No fragment shader, `in` recebe dados interpolados e `out` fornece a cor. A interpolação entre vértices ocorre durante a rasterização.

Um `uniform` é um valor enviado pela CPU, comum às invocações de uma chamada de desenho. Ele pode mudar entre desenhos e entre quadros. No projeto, são uniforms as matrizes, `selected` e `texture1`.

Os shaders são lidos de arquivos, compilados por etapa e ligados em um programa. `glUseProgram` ativa o programa. Os setters da classe `Shader` usam funções `glUniform...` para atualizá-lo. Falhas de leitura, compilação e link geram mensagens.

### 3.4 Espaços e transformações

- **Local:** coordenadas em relação à origem da malha.
- **Mundial:** coordenadas comuns à cena.
- **Visualização:** coordenadas em relação à câmera.
- **Recorte:** coordenadas homogêneas produzidas pela projeção.
- **NDC:** coordenadas normalizadas após a divisão de X, Y e Z por W.
- **Tela:** coordenadas relacionadas ao viewport.

A linha central do shader é:

```glsl
gl_Position = projection * view * model * vec4(aPos, 1.0);
```

`gl_Position` contém coordenadas de recorte, antes da divisão por W. A faixa canônica de recorte é de `-w` a `w`; após a divisão, a região correspondente fica entre `-1` e `1` em NDC.

O modelo do objeto tem a forma `M = T * R * S`. Aplicado a um vetor coluna, primeiro escala, depois rotaciona e depois translada. Trocar a ordem pode alterar o resultado: rotação e translação, em geral, não comutam.

Na composição, `M_final = M_peca * M_parte`. As partes mantêm suas posições locais quando a peça se desloca. A matriz identidade representa a ausência de transformação adicional.

Matrizes 4×4 e coordenadas homogêneas permitem representar translações junto de rotações e escalas. Um ponto usa W igual a 1; uma direção usa W igual a 0, pois não deve receber translação.

### 3.5 Tabuleiro e posição inicial

O tabuleiro tem 64 cubos achatados e uma base maior. As casas têm espessura de 0,12 e ocupam de Y igual a −0,12 até zero. A base fica abaixo, sem ocupar o mesmo plano das casas.

Para linha `r` e coluna `c`, ambas entre 0 e 7:

```text
centro = (3.5 - c, 0, r - 3.5)
coluna = floor(4 - x)
linha = floor(z + 4)
```

A coluna usa `3.5 - c`, e não `c - 3.5`, porque a câmera começa do lado das brancas olhando para +Z. Desse ponto de vista, +X aparece à esquerda da tela. Com `3.5 - c`, a coluna A (c = 0) fica em X = 3.5, à esquerda de quem joga de brancas, como num tabuleiro real: dama à esquerda do rei e casa clara no canto direito.

Antes da conversão inversa, X deve estar em `(-4, 4]` e Z em `[-4, 4)`. Os limites que gerariam índice 8 ficam de fora. A1 é escura. A coluna 0 representa A; a linha 0 representa a fileira 1.

Exemplo: E2 tem linha 1 e coluna 4, com centro `(-0.5, 0, -2.5)`. E4 tem linha 3 e coluna 4, com centro `(-0.5, 0, -0.5)`.

Cada lado tem oito peões, duas torres, dois cavalos, dois bispos, uma dama e um rei. As peças principais ocupam A–H na ordem torre, cavalo, bispo, dama, rei, bispo, cavalo, torre. As brancas começam nas linhas internas 0 e 1; as pretas, em 7 e 6. As damas ficam em D1/D8 e os reis em E1/E8.

As peças pretas recebem rotação de 180° em torno de Y. Isso orienta os cavalos dos dois lados para o centro.

### 3.6 Perspectiva, câmera e tempo

A projeção usa `glm::perspective`, com FOV vertical de 45°, aspecto igual à largura dividida pela altura do framebuffer e planos próximo e distante de 0,1 e 100 unidades. O volume de visão é o frustum. A perspectiva faz objetos distantes parecerem menores. Uma projeção ortográfica não produziria essa redução com a distância.

A câmera usa:

```cpp
return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
```

O segundo parâmetro é um ponto alvo. Por isso, somamos posição e direção frontal. A matriz de visão transforma o mundo para o referencial da câmera.

Yaw controla a orientação horizontal e pitch, a vertical. Não existe controle de roll. Com os ângulos em radianos:

```text
front.x = cos(yaw) * cos(pitch)
front.y = sin(pitch)
front.z = sin(yaw) * cos(pitch)
```

Para caminhar, a direção frontal é projetada em XZ e normalizada. O vetor lateral vem do produto vetorial entre essa direção e o vetor para cima. Normalizar produz comprimento 1; não significa colocar todos os componentes entre 0 e 1.

O deslocamento horizontal usa direção normalizada, velocidade de 4 unidades por segundo e tempo entre quadros. A normalização evita acelerar ao combinar W com D, por exemplo. O código limita o tempo considerado a 0,1 segundo para reduzir saltos após pausas. Q/Z controlam a altura separadamente. Não há colisão que impeça a câmera de atravessar objetos.

### 3.7 Textura e cor

UV são coordenadas da imagem associadas aos vértices. Elas são interpoladas para consultar a textura na superfície dos triângulos.

No trabalho, `madeira.jpg` é aplicada à base e ao corpo das peças. A imagem guarda só os veios da madeira em tons de cinza claro; o tom vem da cor do vértice, creme nas peças brancas, marrom escuro nas pretas e marrom médio na base. A textura é vinculada à unidade 0. O uniform `sampler2D texture1` recebe o número dessa unidade, não o identificador do objeto de textura.

O código configura `GL_REPEAT`, magnificação linear e minificação com `GL_LINEAR_MIPMAP_LINEAR`. Os mipmaps são versões progressivamente menores da imagem. A filtragem usa essas versões para melhorar a amostragem quando a textura aparece pequena na tela. O alinhamento de upload é ajustado para 1 e depois restaurado.

O fragment shader texturizado multiplica a cor lida da imagem pela cor interpolada do vértice. O programa de cor utiliza a cor interpolada do vértice. Nos dois programas, quando `selected` está ativo, a cor é misturada com laranja usando fator 0,55.

As variações de cor por face e pela superfície são definidas ao gerar os vértices. Não há fonte de luz dinâmica, sombras projetadas ou cálculo de iluminação por fragmento.

### 3.8 Visibilidade e loop principal

O teste de profundidade é ativado com `glEnable(GL_DEPTH_TEST)`. A cada quadro, o programa limpa os buffers de cor e profundidade:

```cpp
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

O depth buffer guarda a profundidade usada para comparar fragmentos. Com a configuração padrão, fragmentos mais próximos podem substituir os mais distantes. Sem esse teste, superfícies desenhadas depois podem cobrir superfícies que deveriam estar à frente.

O loop processa eventos, atualiza a câmera, consulta o framebuffer, limpa os buffers, calcula as matrizes, desenha a base, desenha casas e peças e troca os buffers com `glfwSwapBuffers`. A troca apresenta o quadro preparado no buffer de trás.

Redimensionar atualiza `glViewport` e o aspecto da projeção. Um framebuffer com dimensão zero não é renderizado. A mesma câmera e a mesma projeção são utilizadas na seleção.

### 3.9 Seleção por raio, passo a passo

O método é ray casting para interação. A imagem é produzida por rasterização; o projeto não usa ray tracing para renderizar.

1. Ler a posição do cursor na janela.
2. Considerar a relação entre tamanho da janela e framebuffer, inclusive em HiDPI.
3. Converter para NDC: `x_ndc = 2*x/largura - 1` e `y_ndc = 1 - 2*y/altura`, após a conversão para pixels do framebuffer.
4. Construir pontos com Z em NDC igual a −1 e +1, correspondentes aos planos próximo e distante.
5. Aplicar a inversa de `projection * view` a esses pontos homogêneos.
6. Dividir cada ponto pelo seu W.
7. Usar o ponto próximo como origem e normalizar a diferença entre os pontos como direção.

O raio é `R(t) = O + tD`, com `t >= 0`. Neste código, sua origem está no plano próximo, não exatamente na posição da câmera.

Para cada parte, aplicamos a inversa de sua matriz de modelo ao raio. A origem usa W igual a 1; a direção usa W igual a 0. Nesse espaço, testamos uma caixa alinhada aos eixos, calculada a partir dos limites dos vértices.

O teste usa intervalos por eixo, também conhecido como método das slabs. Cada eixo restringe o intervalo de valores de `t` em que o raio está dentro da caixa. Há interseção quando os intervalos têm uma região comum à frente da origem. Raios paralelos a um eixo exigem tratamento próprio.

A direção local não é normalizada novamente. Assim, mesmo com escalas diferentes, o parâmetro `t` continua comparável ao do raio mundial. Selecionamos a parte com menor distância positiva e identificamos sua peça. Casas e base também participam como volumes de oclusão.

A seleção usa caixas por parte, não triângulos exatos. Por isso, uma pequena região vazia próxima de uma forma curva pode integrar a área clicável.

### 3.10 Interseção com o plano e movimento

O destino utiliza o plano Y igual a zero. Substituindo esse plano em `R(t)`, temos:

```text
t = -O.y / D.y
ponto = O + t * D
```

Se `D.y` estiver próximo de zero, o raio é paralelo e não define destino. Se `t < 0`, a interseção está atrás da origem. Se o ponto ficar fora dos limites do tabuleiro, o destino é rejeitado.

Um destino válido é convertido em linha/coluna. `Peca::mover` atualiza esses índices e a posição principal. A geometria local não muda; a seleção futura usa a nova transformação. A identidade da peça permanece a mesma.

O estado da interação guarda uma peça selecionada ou −1. O primeiro clique tenta selecionar. O segundo tenta mover. Destino externo preserva a seleção; destino válido encerra a seleção. Clique direito cancela.

## 4. Perguntas prováveis e respostas curtas

**Qual é a principal aplicação dos slides neste trabalho?**  
As matrizes e o pipeline permitem montar objetos locais, posicioná-los no mundo e projetar a cena. A câmera e as texturas completam a visualização.

**Onde a orientação a objetos contribui?**  
`Object` concentra transformações. `Mesh` trata buffers e desenho. As primitivas especializam a geometria. `Peca` compõe várias partes, seguindo a ideia de `Banrisul`.

**Como a peça inteira se move sem desmontar?**  
Cada parte recebe a matriz da peça como transformação pai. Mudar a posição da peça altera todas as partes de forma consistente.

**Por que a ordem das matrizes importa?**  
As operações não comutam. No projeto, `T * R * S` aplica escala, rotação e translação ao vértice, nessa ordem.

**Por que usar duas matrizes, model e view, se ambas movem coordenadas?**  
Model posiciona cada objeto. View representa o referencial do observador e é compartilhada pelos objetos do quadro.

**Por que a projeção é em perspectiva?**  
Ela representa a redução aparente dos objetos com a distância e combina com a navegação pelo ambiente.

**São dois shaders ou dois programas?**  
São dois programas. Cada programa combina um vertex shader e um fragment shader. O arquivo do vertex shader é compartilhado.

**O que muda quando uma peça é selecionada?**  
O uniform `selected` ativa a mistura com uma cor alaranjada no fragment shader. A geometria permanece igual.

**Por que há volume visual se não existe iluminação dinâmica?**  
Há geometria tridimensional, perspectiva, oclusão e variações de cor definidas nos vértices. Essas variações simulam diferenças de claridade.

**O teste de profundidade também seleciona a peça?**  
Não. Ele controla a renderização. A seleção faz testes geométricos na CPU e considera a oclusão com volumes do tabuleiro.

**Como um clique 2D identifica um objeto 3D?**  
O clique define um raio. A inversa das matrizes de projeção e visão permite reconstruí-lo no mundo. Interseções indicam o objeto atingido.

**Por que não basta converter X e Y do mouse diretamente em linha e coluna?**  
A perspectiva e a câmera alteram a relação entre tela e tabuleiro. O raio considera essas transformações e distingue peças pela profundidade.

**Por que usar caixas em vez dos triângulos da peça?**  
As caixas simplificam os testes e atendem à interação do trabalho. A contrapartida é uma seleção aproximada nas bordas curvas.

**Como a seleção continua correta depois de mover uma peça?**  
O teste transforma o raio usando a matriz atual da peça e de suas partes. Não conserva um volume fixo na posição antiga.

**Por que multiplicar a velocidade pelo tempo entre quadros?**  
Isso expressa a velocidade em unidades por segundo, reduzindo a dependência da quantidade de quadros renderizados.

**Por que limitar pitch?**  
Para evitar olhar exatamente na direção do vetor para cima ou na direção oposta, o que degeneraria a base da câmera.

**O que acontece em uma casa ocupada?**  
O movimento é recusado e a seleção continua. A função `casaOcupada`, em `main.cpp`, percorre as 32 peças e compara linha e coluna com o destino. O aviso aparece no console e no título da janela. Não há captura: a peça que já está na casa permanece nela.

**O movimento é contínuo?**  
Não. O segundo clique atualiza diretamente a posição para o centro da casa escolhida. Não há arraste nem animação do percurso.

**O que foi reutilizado e o que foi acrescentado?**  
A inicialização, os shaders, a textura, o cubo e a composição seguem os exemplos do professor. Tabuleiro, peças, formas adicionais e seleção por raio adaptam e estendem essa base.

**Como o projeto foi validado?**  
Há testes de coordenadas, raios, HiDPI, distribuição das peças, seleção após movimento, oclusão e renderização. O README registra 236 verificações anteriores. Este roteiro não representa uma nova execução dos testes. A validação final pela IDE ainda consta como pendente no README.

## 5. Onde mostrar o código

| Assunto | Arquivo ou função |
| --- | --- |
| Janela, contexto Core e depth test | `src/Application.cpp`, `Application::init` |
| Ordem de eventos e renderização | `main.cpp`, loop principal |
| Dois programas e callback do mouse | `main.cpp`, construção dos shaders e `mouse_button_callback` |
| Layout de atributos e desenho indexado | `src/Mesh.cpp`, construtor e `draw` |
| Translação, rotação e escala | `src/Object.cpp`, `modelMatrix` |
| Geração das formas | `src/Cube.cpp` e `src/Primitivas.cpp` |
| Composição, posição inicial e movimento | `src/Peca.cpp`, `init`, `draw`, `criarPecas` e `mover` |
| Casas e conversão de coordenadas | `src/Tabuleiro.cpp`, `centro` e `casa` |
| lookAt, yaw/pitch e deltaTime | `src/Camera.cpp` |
| Construção do raio e interseções | `src/Interacao.cpp` |
| MVP e passagem de atributos | `shaders/cena.vert` |
| Cor e destaque da seleção | `shaders/cor.frag` |
| Amostragem da textura e destaque nas partes texturizadas | `shaders/textura.frag` |
| Escolha do shader de cada parte da peça | `Peca::desenhar` em `src/Peca.cpp` |
| Leitura, upload e filtros da imagem | `src/Texture.cpp` |

## 6. Slides para revisar hoje

Não é necessário estudar histórico do OpenGL, tesselação, geometry shaders ou efeitos do ShaderToy para explicar esta implementação.

| Material | Páginas prioritárias | O que revisar |
| --- | --- | --- |
| [Introdução OpenGL](referencias/aulas/Introdu%C3%A7%C3%A3o%20OpenGL.pdf) | 21–23; 27–29; 37 | Bibliotecas, contexto, loop e diferença entre legado e moderno |
| [shaders-pt1](referencias/aulas/shaders-pt1.pdf) | 12–16; 23–32; 34–48 | Pipeline, vertex/fragment shader, buffers, compilação e uso |
| [OpenGL e Objetos Tridimensionais](referencias/aulas/OpenGL%20e%20Objetos%0ATridimensionais.pdf) | 10–30; 39–51; 54–62; 65–72 | Espaços, MVP, perspectiva, uniforms, cubo e profundidade |
| [shaders-pt2](referencias/aulas/shaders-pt2.pdf) | 20–36; 38–45; 47–65 | VAO/VBO/EBO, layout, uniforms, UV, textura e mipmaps |
| [Câmera Virtual](referencias/aulas/C%C3%A2mera%20Virtual.pdf) | 14–26; 31–43; 45–50 | Base da câmera, lookAt, movimento, yaw/pitch e altura |

O PDF de objetos tridimensionais possui uma quebra de linha no nome físico; o link preserva esse nome. O índice detalhado dos exemplos e das adaptações está em [REFERENCIAS.md](REFERENCIAS.md).

Alguns slides simplificam os conceitos ou usam outra versão de GLSL. Para a apresentação, use os termos do código atual: `in`/`out`, GLSL 330 Core, saída em espaço de recorte, interpolação na rasterização e sampler associado a uma unidade de textura.

## 7. Ensaio antes da apresentação

1. Abra `Xadrez3D.cbp` e confira a compilação e execução pela IDE no computador da apresentação. Essa verificação ainda consta como pendente na documentação.
2. Confira que `shaders/` e `res/images/` estão disponíveis a partir do diretório de execução. O script auxiliar é `./jogar.sh`.
3. Deixe a aplicação na posição inicial. Reiniciar restaura a cena; o programa não possui comando de desfazer ou reset.
4. Ensaie uma sequência curta: visão geral, câmera, seleção de um peão, destino vazio, cancelamento e redimensionamento.
5. Mantenha abertas apenas as funções que pretende explicar. Comece pelo resultado visual; abra o código para justificar uma técnica.
6. Revise as imagens em `plan/evidencias/` como apoio caso a demonstração ao vivo falhe.
7. Não atribua a seleção por raio a um slide específico: ela é uma extensão baseada nos fundamentos das aulas.
8. Não afirme iluminação dinâmica, validação de regras, captura, arraste ou animação de movimento.

### Dez pontos para lembrar sem consulta

1. O trabalho demonstra uma cena 3D interativa de xadrez.
2. As peças são composições de malhas trianguladas.
3. `model` posiciona, `view` muda o referencial e `projection` projeta.
4. A transformação final usa `projection * view * model`.
5. A matriz da peça transforma todas as suas partes.
6. VBO guarda atributos, EBO guarda índices e VAO guarda a configuração de leitura.
7. Dois programas compartilham o vertex shader: cor para casas/peças e textura para a base.
8. `lookAt`, yaw/pitch e tempo entre quadros controlam a câmera.
9. O clique gera um raio; a seleção usa caixas e o destino usa o plano do tabuleiro.
10. O movimento é livre entre casas, sem regras do xadrez.
