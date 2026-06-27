#include <iostream>
#include <vector>

// Bibliotecas do OpenGL e FreeGLUT
#include <GL/freeglut.h>

// Bibliotecas do ImGui
#include "imgui.h"
#include "backends/imgui_impl_glut.h"
#include "backends/imgui_impl_opengl2.h"

// Inclusão dos módulos didáticos
#include "ModuloDidatico.h"
#include "ModuloPrimitivas.h"
#include "ModuloTransformacoes.h"
#include "ModuloProjecao.h"
#include "ModuloIluminacao.h"
#include "ModuloCurvas.h"
#include "ModuloTexturas.h"
#include "ModuloObjetos.h"


// --- Variáveis Globais de Estado ---
std::vector<ModuloDidatico*> modulos;
int moduloAtivo = 0;

// --- Configuração da Câmera Básica do Simulador ---
float cameraZ = 10.0f;
float rotX = 0.0f; // Rotação no eixo X (Cima/Baixo)
float rotY = 0.0f; // Rotação no eixo Y (Esquerda/Direita)
int lastMouseX = 0;
int lastMouseY = 0;
bool isDragging = false;

bool mostrarEixosGlobais = true;

// === FUNÇÃO DE DESENHO ===
void DesenharReferenciaEspacial() {
    // Desliga a iluminação temporariamente para que os eixos tenham cores puras
    glDisable(GL_LIGHTING); 
    
    // Desenha uma grade (Grid) no chão (Plano XZ) para dar noção de profundidade e piso
    glColor3f(0.3f, 0.3f, 0.3f); // Cinza escuro
    glBegin(GL_LINES);
    for(int i = -5; i <= 5; i++) {
        if (i == 0) continue; // Pula o centro para não desenhar por cima dos eixos principais
        glVertex3f((float)i, 0.0f, -5.0f); glVertex3f((float)i, 0.0f,  5.0f); // Linhas no eixo Z
        glVertex3f(-5.0f, 0.0f, (float)i); glVertex3f( 5.0f, 0.0f, (float)i); // Linhas no eixo X
    }
    glEnd();

    // Desenha os Eixos Principais (Padrão da Indústria: RGB = XYZ)
    glLineWidth(2.0f); // Deixa as linhas principais mais grossas
    glBegin(GL_LINES);
        // Eixo X (Vermelho - Direita)
        glColor3f(1.0f, 0.2f, 0.2f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(5.0f, 0.0f, 0.0f);
        // Eixo Y (Verde - Cima)
        glColor3f(0.2f, 1.0f, 0.2f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 5.0f, 0.0f);
        // Eixo Z (Azul - Frente)
        glColor3f(0.2f, 0.5f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 5.0f);
    glEnd();
    glLineWidth(1.0f); // Retorna a espessura ao normal

    glColor3f(1.0f, 1.0f, 1.0f); // Reseta a cor para branco
}

// --- Funções de Inicialização e Limpeza ---
void InicializarModulos() {
    // Instancia os módulos dinamicamente.
    modulos.push_back(new ModuloPrimitivas());
    modulos.push_back(new ModuloObjetos());
    modulos.push_back(new ModuloTransformacoes());
    modulos.push_back(new ModuloProjecao());
    modulos.push_back(new ModuloIluminacao());
    modulos.push_back(new ModuloCurvas());
    modulos.push_back(new ModuloTexturas());
}

void LimparMemoria() {
    for (ModuloDidatico* modulo : modulos) {
        delete modulo;
    }
    modulos.clear();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}

// --- Callbacks do FreeGLUT ---

void ReshapeCallback(int w, int h) {
    // Repassa o reshape para o ImGui
    ImGui_ImplGLUT_ReshapeFunc(w, h);

    // Configura a Viewport para ocupar a janela inteira
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    // Configura a matriz de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    
    // Retorna para a matriz de modelagem
    glMatrixMode(GL_MODELVIEW);
}

void DisplayCallback() {
    // 1. INICIAR NOVO FRAME DO IMGUI
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::NewFrame();

    // Pega as dimensões atuais da janela do Windows gerenciada pelo GLUT
    int winWidth = glutGet(GLUT_WINDOW_WIDTH);
    int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

    // =================================================================
    // 2. CONSTRUÇÃO DA INTERFACE DIDÁTICA (PAINEL LATERAL ESQUERDO)
    // =================================================================
    
    // Força a janela a iniciar colada no canto superior esquerdo
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    
    // Define restrições de largura (mínimo de 250px, máximo da tela menos 100px para o 3D)
    // e trava a altura para sempre ocupar 100% da janela
    ImGui::SetNextWindowSizeConstraints(ImVec2(250, winHeight), ImVec2(winWidth - 100, winHeight));
    
    // Define o tamanho inicial padrão apenas para a primeira inicialização
    ImGui::SetNextWindowSize(ImVec2(450, winHeight), ImGuiCond_FirstUseEver);
    
    // Remove barras de título e movimentação, mantendo livre apenas o redimensionamento de borda
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Painel Didático", nullptr, flags);
    
    ImGui::Text("Simulador OpenGL");
    ImGui::Separator();
    ImGui::Spacing();

    // --- CONTROLES GLOBAIS DA CÂMERA ---
    // Verifica qual é o módulo ativo no momento
    bool moduloControlaPropriaCamera = false;
    if (!modulos.empty() && modulos[moduloAtivo]->ObterNome() == "Projeções e Câmera") {
        moduloControlaPropriaCamera = true;
    }

    // Só desenha os controles globais se o módulo NÃO for o de Projeção
    if (!moduloControlaPropriaCamera) {
        // --- CONTROLES GLOBAIS DA CÂMERA ---
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.0f));
        
        if (ImGui::Button("Reset Câmera (Rotação e Zoom)", ImVec2(-1, 0))) {
            rotX = 0.0f;
            rotY = 0.0f;
            cameraZ = 10.0f; 
        }
        ImGui::PopStyleColor(2); 
        
        ImGui::SliderFloat("Zoom (Distância Z)", &cameraZ, 0.5f, 20.0f);

        ImGui::Checkbox("Mostrar Grade e Eixos XYZ", &mostrarEixosGlobais);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    // --- NAVEGAÇÃO ENTRE OS MÓDULOS DIDÁTICOS ---
    if (ImGui::BeginTabBar("AbasModulos")) {
        for (size_t i = 0; i < modulos.size(); ++i) {
            if (ImGui::BeginTabItem(modulos[i]->ObterNome().c_str())) {
                moduloAtivo = i;
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }

    // Renderiza os controles específicos (e a caixa de código) do módulo ativo
    if (!modulos.empty()) {
        modulos[moduloAtivo]->RenderizarUI();
    }
    
    // CAPTURA ESSENCIAL: Descobre qual é a largura atual do menu após o usuário arrastar a borda
    float menuWidthDinamico = ImGui::GetWindowWidth();
    
    ImGui::End();
    ImGui::Render();

    // =================================================================
    // 3. RENDERIZAÇÃO DO OPENGL (CENA 3D NA DIREITA RESIDUAL)
    // =================================================================
    
    // Calcula quanta largura sobrou para a viewport 3D
    int cenaWidth = winWidth - (int)menuWidthDinamico;
    if (cenaWidth < 1) cenaWidth = 1; // Proteção contra divisão por zero

    // Modifica a área de atuação do OpenGL para começar exatamente onde o menu termina
    glViewport((int)menuWidthDinamico, 0, cenaWidth, winHeight);

    // Limpa a tela com o tom escuro de azul de fundo
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Atualiza a Projeção da cena baseado no aspecto dinâmico do retângulo restante
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)cenaWidth / (float)winHeight;
    gluPerspective(45.0, aspect, 0.1, 100.0);
    
    // Configura a Câmera Base e aplica as transformações orbitais do mouse
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 1º: Posiciona a lente com base no valor de Zoom (cameraZ)
    gluLookAt(0.0, 0.0, cameraZ,  
              0.0, 0.0, 0.0,      
              0.0, 1.0, 0.0);     

    // 2º: Gira o ambiente 3D de acordo com o clique-e-arraste capturado nas callbacks do mouse
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // === NOVA CHAMADA DE REFERÊNCIA ESPACIAL ===
    // Só desenhamos se o checkbox estiver marcado E se não estivermos no módulo de Projeção
    if (mostrarEixosGlobais && !moduloControlaPropriaCamera) {
        DesenharReferenciaEspacial();
    }

    // Desenha o conteúdo tridimensional do módulo ativo
    if (!modulos.empty()) {
        modulos[moduloAtivo]->RenderizarCena3D();
    }

    // =================================================================
    // 4. DESENHAR INTERFACE IMGUI POR CIMA E EXIBIR FRAME
    // =================================================================
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    
    // Troca os buffers do Double Buffer
    glutSwapBuffers();
    
    // Solicita uma nova atualização imediatamente (essencial para manter as animações e inputs fluidos)
    glutPostRedisplay();
}

// --- Tratamento de Input ---
// Interceptamos o input para garantir que, se o usuário estiver clicando no ImGui,
// a cena 3D (como rotação de câmera manual) não receba esse clique.

void MouseCallback(int button, int state, int x, int y) {
    ImGui_ImplGLUT_MouseFunc(button, state, x, y);
    
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return; // Se clicou no menu ImGui, ignora a cena 3D

    // Lógica da Câmera Orbital
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

void MotionCallback(int x, int y) {
    ImGui_ImplGLUT_MotionFunc(x, y);
    
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;

    // Se o módulo atual for o de Projeção, ignora a rotação global do mouse
    if (!modulos.empty() && modulos[moduloAtivo]->ObterNome() == "Projeções e Câmera") {
        return;
    }

    if (isDragging) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        rotY += deltaX * 0.5f; 
        rotX += deltaY * 0.5f;

        lastMouseX = x;
        lastMouseY = y;
    }
}
void MouseWheelCallback(int wheel, int direction, int x, int y) {
    
    // Em algumas implementações do ImGui + GLUT, precisamos repassar o evento
    // Mas o mais importante é checar se o usuário não está scrollando o menu do ImGui
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;

    // Bloqueia o scroll de afetar o zoom global na aba de Projeção
    if (!modulos.empty() && modulos[moduloAtivo]->ObterNome() == "Projeções e Câmera") {
        return;
    }

    // direction > 0 significa que rolou para frente (Zoom In)
    // direction < 0 significa que rolou para trás (Zoom Out)
    if (direction > 0) {
        cameraZ -= 0.5f; 
    } else {
        cameraZ += 0.5f; 
    }

    // Travas de segurança para a câmera não atravessar os objetos (Z negativo) 
    // ou não ir para o infinito e sumir com a cena
    if (cameraZ < 0.5f) cameraZ = 0.5f;
    if (cameraZ > 20.0f) cameraZ = 20.0f;
}

void PassiveMotionCallback(int x, int y) {
    ImGui_ImplGLUT_MotionFunc(x, y);
}

void KeyboardCallback(unsigned char key, int x, int y) {
    ImGui_ImplGLUT_KeyboardFunc(key, x, y);
    
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard) return;

    // Lógica de atalhos de teclado (ex: 'ESC' para sair)
    if (key == 27) { 
        LimparMemoria();
        exit(0);
    }
}

// --- Função Principal ---

int main(int argc, char** argv) {
    // Inicialização do FreeGLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Simulador OpenGL");

    // Habilita teste de profundidade para o 3D
    glEnable(GL_DEPTH_TEST);

    // Inicialização do Contexto ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // Tema escuro (opcional)

    //Config. da fonte
    ImFont* fonteProjetor = io.Fonts->AddFontFromFileTTF("fonts/Sono-Regular.ttf", 20.0f);
    
    if (fonteProjetor == nullptr) {
        // Se a fonte não for encontrada na pasta, avisa no terminal para facilitar o debug
        std::cerr << "ERRO: Arquivo de fonte não encontrado em fonts/Sono-Regular.ttf" << std::endl;
        io.FontGlobalScale = 1.0f; // Fallback
    }

    // Configuração dos Backends do ImGui
    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL2_Init();

    // Instala as funções de callback customizadas
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MotionCallback);
    glutPassiveMotionFunc(PassiveMotionCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutMouseWheelFunc(MouseWheelCallback);

    // Prepara os módulos didáticos
    InicializarModulos();

    // Loop principal
    glutMainLoop();

    // Código inatingível na maioria das implementações do GLUT (use FreeGLUT e glutSetOption para retornar daqui se necessário)
    LimparMemoria();
    return 0;
}