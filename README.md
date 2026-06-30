# 🖥️ Simulador Gráfico OpenGL Interativo

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl)
![ImGui](https://img.shields.io/badge/Dear_ImGui-0A0A0A?style=for-the-badge&logo=c%2B%2B)

## 📖 Descrição do Software
Um laboratório virtual interativo de Computação Gráfica construído em **C++** com **OpenGL Clássico (Pipeline Fixo)** e interface gráfica guiada pelo **Dear ImGui**. O simulador consolida em um único ambiente as principais etapas do pipeline de renderização 3D clássico, englobando desde a construção de primitivas matemáticas até a aplicação de texturas e a física da iluminação de Phong.

## 🎯 Objetivo Principal
Eliminar a lacuna entre a abstração matemática e o resultado visual. Desenvolvido com um rigoroso foco acadêmico e didático, o software transforma horas de testes "às cegas" em uma experimentação dinâmica: cada interação na interface gráfica atualiza a cena 3D em tempo real e exibe, simultaneamente, o **código C++ / OpenGL** exato que está sendo executado nos bastidores. 

É uma ferramenta de apoio desenhada para ajudar alunos e professores a compreenderem na prática o comportamento de matrizes, vetores e coordenadas no espaço tridimensional.

---

## ✨ Módulos de Exploração Didática

O ambiente está dividido em módulos focados nos principais tópicos da renderização 3D clássica:

### 📐 1. Modelos 2D e 3D
Laboratório de primitivas geométricas e topologia.
* Construção de polígonos 2D baseados em trigonometria e matemática pura.
* Renderização de sólidos platônicos e malhas complexas nativas do **FreeGLUT** (como o icônico *Utah Teapot*).
* Estudo da diferença de renderização entre vértices preenchidos (`GL_FILL`) e o modo aramado/wireframe (`GL_LINE`).

### 🔄 2. Transformações Geométricas
Entendimento prático das matrizes de manipulação espacial.
* Controles independentes para Translação, Rotação e Escala ao longo dos eixos X, Y e Z.
* Demonstração da importância crítica do empilhamento de matrizes (`glPushMatrix` e `glPopMatrix`) e da ordem das operações geométricas.

### 🎥 3. Projeções e Câmera (Sistema God View)
A mecânica de lentes e da matriz `GL_MODELVIEW`.
* Comparação interativa entre o Tronco de Pirâmide (Projeção Perspectiva) e a Projeção Ortogonal.
* **Aspect Ratio Responsivo:** O sistema ajusta a matriz de projeção dinamicamente ao redimensionar a janela, evitando distorções visuais.
* **Inovador Modo "God View":** Um mini-mapa picture-in-picture recortado via `glScissor` que exibe a cena por uma câmera isométrica externa. Ele permite visualizar a carcaça física da câmera e o seu vetor de direção (`gluLookAt`) movendo-se pelo espaço enquanto foca em alvos pré-definidos (Target Lock).

### 💡 4. Iluminação e Materiais (Modelo de Phong)
O comportamento físico da luz nos polígonos.
* Controle individual das componentes **Ambiente, Difusa e Especular** da iluminação.
* Suporte a múltiplas fontes de luz (`GL_LIGHT0`, `GL_LIGHT1`), com indicadores visuais de suas posições (debug esferas coloridas).
* Edição do material do objeto e reflexividade (*Shininess*), mostrando como a luz afeta objetos com sombreamento facetado (`GL_FLAT`) vs. suavizado (`GL_SMOOTH`).

### 🏁 5. Mapeamento de Texturas Bidimensionais
Como os arrays de memória revestem a geometria 3D.
* **Texturas Procedurais:** Geração programática de matrizes 8x8 na CPU (Xadrez Clássico e Grade Cyberpunk Neon) enviadas para a GPU (`glTexImage2D`).
* **Filtros (Min/Mag):** Demonstração da diferença entre interpolação Bilinear Suave (`GL_LINEAR`) e o efeito pixelado Retro (`GL_NEAREST`).
* **Envelopamento (UV Wrap):** O que ocorre ao extrapolar as coordenadas UV (`GL_REPEAT` vs `GL_CLAMP_TO_EDGE`).
* **Modos de Ambiente:** Comparação entre substituir a cor do polígono pela textura (`GL_REPLACE`) ou multiplicar a textura pela iluminação dinâmica da cena (`GL_MODULATE`).

---

## 🚀 Uso Rápido (Apenas Windows)

Como este projeto possui foco educacional, o executável já está incluído para facilitar a distribuição e testes imediatos em salas de aula:

1. No topo da página do GitHub, clique em **`<> Code`** e selecione **`Download ZIP`**.
2. Extraia o arquivo no seu computador.
3. Dê um duplo clique em **`simulador.exe`**.
4. A aplicação abrirá instantaneamente, sem necessidade de instalar dependências.

> **Importante:** Não mova o arquivo `.exe` para fora da pasta do projeto. Ele precisa estar junto à estrutura de diretórios para localizar suas dependências eventuais.

---

## 🛠️ Como Compilar o Projeto (Para Desenvolvedores)

Se você deseja modificar o código fonte e recompilar o projeto na sua máquina, siga os passos abaixo:

### Pré-requisitos
* **MinGW (GCC/G++)** configurado nas variáveis de ambiente do Windows.
* Bibliotecas incluídas na pasta `vendor`: **FreeGLUT** e **ImGui**.

### Compilando via Terminal
1. Abra o terminal (Prompt de Comando ou PowerShell) na raiz do projeto.
2. Execute o comando do Makefile:

       mingw32-make

3. O compilador gerará os arquivos objeto (`.o`) e atualizará o binário final `simulador.exe`. Caso queira limpar os binários antigos antes de compilar novamente, rode:

       mingw32-make clean

---

## 🧰 Tecnologias Utilizadas

* **Linguagem:** C++
* **API Gráfica:** OpenGL Clássico (Pipeline fixo)
* **Gerenciamento de Janelas/Inputs:** FreeGLUT
* **Interface de Usuário (GUI):** Dear ImGui
* **Build System:** GNU Make (Makefile)

---

Desenvolvido para transformar a abstração matemática da computação gráfica em experimentação visual em tempo real. 🚀