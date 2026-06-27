# 🖥️ Simulador Gráfico OpenGL Interativo

Um ambiente de simulação gráfica 3D desenvolvido em **C++** com **OpenGL Clássico** e **ImGui**. 

Este projeto foi construído como uma ferramenta didática interativa para explorar e demonstrar, em tempo real, os conceitos matemáticos e visuais do pipeline gráfico, sendo ideal para estudantes e professores de Computação Gráfica.

---

## ✨ Módulos e Funcionalidades

O simulador é dividido em módulos de aprendizado acessíveis por um menu lateral responsivo. Cada módulo altera a cena 3D e exibe dinamicamente o código fonte em C gerado por trás daquela configuração:

* **📐 Modelos 2D e 3D:** Construção de primitivas matemáticas (usando trigonometria), pirâmides vértice a vértice e a renderização de malhas complexas nativas da biblioteca FreeGLUT (como o famoso *Utah Teapot*).
* **🔄 Transformações Geométricas:** Demonstração visual do uso das matrizes de Translação, Rotação e Escala (`glTranslate`, `glRotate`, `glScale`) e a importância da ordem de empilhamento das matrizes (`glPushMatrix` / `glPopMatrix`).
* **🎥 Projeções e Câmera (Com God View):** Exploração das diferenças entre projeção Ortogonal e Perspectiva (Lentes). Conta com um inovador modo **"God View" (Visão Externa)** num mini-mapa dinâmico que ilustra o vetor e a carcaça física da câmera se movendo pelo cenário ao alterar o `gluLookAt`, além de um sistema de "Target Lock" (foco automático).
* **💡 Iluminação e Materiais:** Laboratório visual do modelo de reflexão de Phong. Permite instanciar múltiplas luzes (como configurações *Neon/Cyberpunk*) e editar em tempo real as componentes de cor Ambiente, Difusa e Especular do objeto e das luzes (`glMaterial` e `glLight`).

---

## 🚀 Como Baixar e Executar (Modo Rápido)

Como o executável já está incluído no repositório, rodar o projeto no Windows é imediato:

1. No topo desta página, clique no botão verde **`<> Code`** e selecione **`Download ZIP`**.
2. Extraia a pasta no seu computador.
3. Abra a pasta extraída e dê um duplo clique no arquivo **`simulador.exe`**.
4. Pronto! O simulador abrirá imediatamente.

> **⚠️ Nota de compatibilidade:** O executável foi gerado para arquiteturas Windows (64-bits). Certifique-se de manter a estrutura de pastas intacta, pois o executável pode depender de arquivos de textura ou bibliotecas `.dll` contidas no diretório.

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