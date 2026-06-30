#include "../include/ModuloTexturas.h"
#include "imgui.h"

// ==========================================
// FIX PARA CABEÇALHOS ANTIGOS DO MINGW/WINDOWS
// ==========================================
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

std::string ModuloTexturas::ObterNome() const {
    return "Mapeamento de Textura";
}

void ModuloTexturas::InicializarTexturas() {
    if (texturasInicializadas) return;

    GerarTexturaXadrez();
    GerarTexturaGradeNeon();

    texturasInicializadas = true;
}

// Algoritmo clássico 8x8 do livro (i + j) % 2
void ModuloTexturas::GerarTexturaXadrez() {
    const int tam = 8;
    GLubyte imagem[tam][tam][3];

    for (int i = 0; i < tam; ++i) {
        for (int j = 0; j < tam; ++j) {
            if ((i + j) % 2 == 0) {
                // Branco
                imagem[i][j][0] = 255;
                imagem[i][j][1] = 255;
                imagem[i][j][2] = 255;
            } else {
                // Preto
                imagem[i][j][0] = 0;
                imagem[i][j][1] = 0;
                imagem[i][j][2] = 0;
            }
        }
    }

    glGenTextures(1, &idTexturaXadrez);
    glBindTexture(GL_TEXTURE_2D, idTexturaXadrez);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tam, tam, 0, GL_RGB, GL_UNSIGNED_BYTE, imagem);
}

void ModuloTexturas::GerarTexturaGradeNeon() {
    const int tam = 8;
    GLubyte imagem[tam][tam][3];

    for (int i = 0; i < tam; ++i) {
        for (int j = 0; j < tam; ++j) {
            if (i == 0 || i == tam - 1 || j == 0 || j == tam - 1) {
                imagem[i][j][0] = 229; // Neon
                imagem[i][j][1] = 61;
                imagem[i][j][2] = 173;
            } else {
                imagem[i][j][0] = 29;  // Fundo
                imagem[i][j][1] = 30;
                imagem[i][j][2] = 44;
            }
        }
    }

    glGenTextures(1, &idTexturaGrade);
    glBindTexture(GL_TEXTURE_2D, idTexturaGrade);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tam, tam, 0, GL_RGB, GL_UNSIGNED_BYTE, imagem);
}

void ModuloTexturas::DesenharCuboTexturizado() {
    glBegin(GL_QUADS);
        // Face Frontal
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);          glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(repeticaoU, 0.0f);    glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(repeticaoU, repeticaoV); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, repeticaoV);    glVertex3f(-1.0f,  1.0f,  1.0f);

        // Face Traseira
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(repeticaoU, 0.0f);    glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(repeticaoU, repeticaoV); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, repeticaoV);    glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);          glVertex3f( 1.0f, -1.0f, -1.0f);

        // Face Superior
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);          glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, repeticaoV);    glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(repeticaoU, repeticaoV); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(repeticaoU, 0.0f);    glVertex3f( 1.0f,  1.0f, -1.0f);

        // Face Inferior
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(repeticaoU, 0.0f);    glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);          glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, repeticaoV);    glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(repeticaoU, repeticaoV); glVertex3f(-1.0f, -1.0f,  1.0f);

        // Face Direita
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(repeticaoU, 0.0f);    glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(repeticaoU, repeticaoV); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, repeticaoV);    glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f);          glVertex3f( 1.0f, -1.0f,  1.0f);

        // Face Esquerda
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);          glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(repeticaoU, 0.0f);    glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(repeticaoU, repeticaoV); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, repeticaoV);    glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
}

void ModuloTexturas::RenderizarUI() {
    // Arrays de texto para os Menus Dropdown (Combobox)
    const char* nomesObj[] = { "Plano 2D", "Cubo 3D" };
    const char* nomesTex[] = { "Xadrez Clássico", "Bordas Neon" };
    const char* nomesEnv[] = { "GL_MODULATE (Luz/Cor)", "GL_REPLACE (Substituição Pura)" };
    const char* nomesFiltro[] = { "GL_NEAREST (Pixelado)", "GL_LINEAR (Suavizado)" };
    const char* nomesWrap[] = { "GL_REPEAT (Repetir)", "GL_CLAMP_TO_EDGE (Travar)" };

    // ==========================================
    // PAINEL SUPERIOR (Mais compacto)
    // ==========================================
    ImGui::Text("Configurações do Material:");
    ImGui::PushItemWidth(200); // Limita a largura das caixas de seleção
    
    ImGui::Combo("Geometria Alvo", &tipoObjeto, nomesObj, 2);
    ImGui::Combo("Textura de Origem", &tipoTextura, nomesTex, 2);
    ImGui::Combo("Modo de Aplicação", &modoEnv, nomesEnv, 2);

    if (modoEnv == 0) {
        ImGui::ColorEdit3("Cor Base do Vértice", corObjeto);
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Parâmetros do OpenGL (glTexParameteri):");
    ImGui::Combo("Filtro (Min/Mag)", &filtroMinMag, nomesFiltro, 2);
    ImGui::Combo("Envelopamento", &modoWrap, nomesWrap, 2);
    ImGui::PopItemWidth(); // Restaura a largura normal

    ImGui::Spacing();

    // Coloca os dois Sliders lado a lado para economizar espaço
    ImGui::PushItemWidth(110);
    ImGui::SliderFloat("Escala U (X)", &repeticaoU, 0.1f, 5.0f, "%.1f");
    ImGui::SameLine(0, 20); // Espaçamento de 20 pixels entre os dois
    ImGui::SliderFloat("Escala V (Y)", &repeticaoV, 0.1f, 5.0f, "%.1f");
    ImGui::PopItemWidth();
    
    ImGui::Spacing();

    if (ImGui::Button("Resetar Mapeamento", ImVec2(-1, 0))) {
        tipoObjeto = 1; tipoTextura = 0; filtroMinMag = 0; modoWrap = 0; modoEnv = 1;
        repeticaoU = 1.0f; repeticaoV = 1.0f;
        corObjeto[0] = corObjeto[1] = corObjeto[2] = 1.0f;
    }

    ImGui::Separator();
    
    // ==========================================
    // CÓDIGO OPENGL GERADO DINAMICAMENTE
    // ==========================================
    ImGui::Text("Código OpenGL Gerado:");
    
    // Agora o BeginChild terá um espaço GIGANTE para respirar!
    ImGui::BeginChild("CodigoAreaTextura", ImVec2(0, 0), true);
    
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "// 1. Geração (CPU -> GPU):");
    ImGui::Text("GLubyte textura[8][8][3];");
    if (tipoTextura == 0) {
        // Usamos TextUnformatted aqui para evitar que o "% 2" quebre o formatador do ImGui
        ImGui::TextUnformatted("if ((i + j) % 2 == 0) { /* Branco */ }\nelse { /* Preto */ }");
    } else {
        ImGui::TextUnformatted("if (bordas) { /* Rosa Neon */ }\nelse { /* Escuro */ }");
    }
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, \n             0, GL_RGB, GL_UNSIGNED_BYTE, textura);");
    
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "// 2. Configuração (Render Loop):");
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnable(GL_TEXTURE_2D);");
    ImGui::Text("glBindTexture(GL_TEXTURE_2D, idTextura);");
    ImGui::Text("glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, %s);", (filtroMinMag == 0) ? "GL_NEAREST" : "GL_LINEAR");
    ImGui::Text("glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, %s);", (modoWrap == 0) ? "GL_REPEAT" : "GL_CLAMP_TO_EDGE");
    
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, %s);", 
                 (modoEnv == 0) ? "GL_MODULATE" : "GL_REPLACE");
    
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "// 3. Desenho e Coordenadas UV:");
    if (modoEnv == 0) {
        ImGui::Text("glColor3f(%.1ff, %.1ff, %.1ff); // Tinta os fragmentos", corObjeto[0], corObjeto[1], corObjeto[2]);
    }
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glBegin(GL_QUADS);");
    if (tipoObjeto == 0) {
        ImGui::Text("  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5f, -1.5f, 0.0f);");
        ImGui::Text("  glTexCoord2f(%.2ff, 0.0f); glVertex3f(1.5f, -1.5f, 0.0f);", repeticaoU);
        ImGui::Text("  glTexCoord2f(%.2ff, %.2ff); glVertex3f(1.5f, 1.5f, 0.0f);", repeticaoU, repeticaoV);
        ImGui::Text("  glTexCoord2f(0.0f, %.2ff); glVertex3f(-1.5f, 1.5f, 0.0f);", repeticaoV);
    } else {
        ImGui::Text("  // Coordenadas frontais (repete para 6 faces)");
        ImGui::Text("  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);");
        ImGui::Text("  glTexCoord2f(%.2ff, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);", repeticaoU);
        ImGui::Text("  glTexCoord2f(%.2ff, %.2ff); glVertex3f(1.0f, 1.0f, 1.0f);", repeticaoU, repeticaoV);
        ImGui::Text("  glTexCoord2f(0.0f, %.2ff); glVertex3f(-1.0f, 1.0f, 1.0f);", repeticaoV);
    }
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnd();");

    ImGui::EndChild();
}

void ModuloTexturas::RenderizarCena3D() {
    InicializarTexturas();

    // Se o modo for MODULATE, vamos ligar uma fonte de luz para mostrar a textura reagindo às sombras
    if (modoEnv == 0) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        
        GLfloat posLuz[] = { 0.0f, 0.0f, 5.0f, 1.0f };
        GLfloat luzAmb[] = { 0.3f, 0.3f, 0.3f, 1.0f };
        GLfloat luzDif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        
        glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
        glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmb);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDif);
        
        // Altera a cor de pintura do material
        glColor3f(corObjeto[0], corObjeto[1], corObjeto[2]);
    } else {
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
        glColor3f(1.0f, 1.0f, 1.0f); // Reseta para branco puro
    }

    glEnable(GL_TEXTURE_2D);
    
    GLuint texturaAtiva = (tipoTextura == 0) ? idTexturaXadrez : idTexturaGrade;
    glBindTexture(GL_TEXTURE_2D, texturaAtiva);

    GLint filtroGL = (filtroMinMag == 0) ? GL_NEAREST : GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtroGL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtroGL);

    GLint wrapGL = (modoWrap == 0) ? GL_REPEAT : GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapGL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapGL);

    // Configuração do ambiente baseada na UI
    GLint envModeGL = (modoEnv == 0) ? GL_MODULATE : GL_REPLACE;
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, envModeGL);

    if (tipoObjeto == 0) {
        // Plano Horizontal Simples
        glNormal3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);          glVertex3f(-1.5f, -1.5f, 0.0f);
            glTexCoord2f(repeticaoU, 0.0f);    glVertex3f( 1.5f, -1.5f, 0.0f);
            glTexCoord2f(repeticaoU, repeticaoV); glVertex3f( 1.5f,  1.5f, 0.0f);
            glTexCoord2f(0.0f, repeticaoV);    glVertex3f(-1.5f,  1.5f, 0.0f);
        glEnd();
    } 
    else {
        // Cubo 3D Mapeado Manualmente com Normais para iluminação funcionar no MODULATE
        DesenharCuboTexturizado();
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    
    // Reseta o ambiente de textura para o padrão seguro do pipeline fixo
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}