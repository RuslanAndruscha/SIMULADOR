#include "../include/ModuloObjetos.h"
#include <GL/freeglut.h>
#include "imgui.h"
#include <cmath>

#define PI 3.14159265f

std::string ModuloObjetos::ObterNome() const {
    return "Modelos 2D e 3D";
}

void ModuloObjetos::RenderizarUI() {
    ImGui::Text("Dimensão do Objeto:");
    ImGui::RadioButton("2D (Plano XY)", &dimensaoSelecionada, 0); ImGui::SameLine();
    ImGui::RadioButton("3D (Volume)", &dimensaoSelecionada, 1);
    
    ImGui::Separator();
    ImGui::Spacing();

    // ==========================================
    // CONTROLES ESPECÍFICOS POR DIMENSÃO
    // ==========================================
    if (dimensaoSelecionada == 0) {
        ImGui::Text("Forma Geométrica 2D:");
        ImGui::Combo("##Combo2D", &objeto2D, nomes2D, 3);
        ImGui::Spacing();
        
        ImGui::SliderFloat("Tamanho (Raio/Lado)", &tamanhoBase, 0.5f, 5.0f, "%.1f");
        if (objeto2D == 2) { // Só mostra o controlo de resolução se for o Círculo
            ImGui::SliderInt("Qtd. de Vértices", &resolucao, 3, 64);
        }
    } else {
        ImGui::Text("Modelos Nativos do FreeGLUT (3D):");
        ImGui::Combo("##Combo3D", &objeto3D, nomes3D, 5); // Voltou para 5 itens
        ImGui::Spacing();

        ImGui::SliderFloat("Tamanho/Raio Base", &tamanhoBase, 0.5f, 3.0f, "%.1f");
        
        // Esfera, Cone e Toroide dependem de Fatias e Pilhas (resolução)
        if (objeto3D == 1 || objeto3D == 2 || objeto3D == 3) {
            ImGui::SliderInt("Fatias/Pilhas (Resolução)", &resolucao, 4, 64);
        }
    }

    ImGui::Separator();
    
    // ==========================================
    // ESTILO DE RENDERIZAÇÃO
    // ==========================================
    ImGui::Text("Estilo de Renderização:");
    ImGui::Checkbox("Modo Wireframe (Aramado)", &modoWireframe);
    
    // A luz não faz sentido para polígonos 2D puros da nossa implementação
    if (dimensaoSelecionada == 1 && !modoWireframe) {
        ImGui::Checkbox("Aplicar Luz Básica", &aplicarLuz);
    }

    ImGui::Separator();

    // ==========================================
    // CÓDIGO OPENGL GERADO DINAMICAMENTE
    // ==========================================
    ImGui::Text("Código OpenGL Gerado:");
    ImGui::BeginChild("CodigoAreaObjetos", ImVec2(0, 0), true);

    if (modoWireframe && dimensaoSelecionada == 0) {
        ImGui::TextColored(ImVec4(0.8f, 0.3f, 0.8f, 1.0f), "glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);");
    }

    if (dimensaoSelecionada == 0) {
        // Código para 2D
        if (objeto2D == 0) {
            ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glBegin(GL_TRIANGLES);");
            ImGui::Text("  glVertex2f(0.0f, %.1ff);", tamanhoBase);
            ImGui::Text("  glVertex2f(-%.1ff, -%.1ff);", tamanhoBase, tamanhoBase);
            ImGui::Text("  glVertex2f(%.1ff, -%.1ff);", tamanhoBase, tamanhoBase);
            ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnd();");
        } else if (objeto2D == 1) {
            ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glBegin(GL_QUADS);");
            ImGui::Text("  glVertex2f(-%.1ff, %.1ff);", tamanhoBase, tamanhoBase);
            ImGui::Text("  glVertex2f(%.1ff, %.1ff);", tamanhoBase, tamanhoBase);
            ImGui::Text("  glVertex2f(%.1ff, -%.1ff);", tamanhoBase, tamanhoBase);
            ImGui::Text("  glVertex2f(-%.1ff, -%.1ff);", tamanhoBase, tamanhoBase);
            ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnd();");
        } else {
            ImGui::Text("// Usando trigonometria (sin/cos)");
            ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glBegin(GL_POLYGON);");
            ImGui::Text("  for(int i=0; i<%d; i++) { ... }", resolucao);
            ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnd();");
        }
    } else {
        // Código para 3D (FreeGLUT)
        const char* estado = modoWireframe ? "Wire" : "Solid";
        
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "// Função nativa do FreeGLUT:");
        
        switch (objeto3D) {
            case 0: ImGui::Text("glut%sCube(%.1f);", estado, tamanhoBase); break;
            case 1: ImGui::Text("glut%sSphere(%.1f, %d, %d);", estado, tamanhoBase, resolucao, resolucao); break;
            case 2: ImGui::Text("glut%sCone(%.1f, %.1f, %d, %d);", estado, tamanhoBase, tamanhoBase * 1.5f, resolucao, resolucao); break;
            case 3: ImGui::Text("glut%sTorus(%.1f, %.1f, %d, %d);", estado, tamanhoBase * 0.4f, tamanhoBase, resolucao, resolucao); break;
            case 4: ImGui::Text("glut%sTeapot(%.1f);", estado, tamanhoBase); break;
        }
    }

    if (modoWireframe && dimensaoSelecionada == 0) {
        ImGui::TextColored(ImVec4(0.8f, 0.3f, 0.8f, 1.0f), "glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);");
    }

    ImGui::EndChild();
}

void ModuloObjetos::RenderizarCena3D() {
    // Cores base para os objetos
    glColor3f(0.3f, 0.8f, 0.8f); // Ciano amigável

    // ==========================================
    // RENDERIZAÇÃO 2D
    // ==========================================
    if (dimensaoSelecionada == 0) {
        glDisable(GL_LIGHTING); // 2D não precisa de luz

        if (modoWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (objeto2D == 0) { // Triângulo
            glBegin(GL_TRIANGLES);
                glVertex2f(0.0f, tamanhoBase);
                glVertex2f(-tamanhoBase, -tamanhoBase);
                glVertex2f(tamanhoBase, -tamanhoBase);
            glEnd();
        } 
        else if (objeto2D == 1) { // Quadrado
            glBegin(GL_QUADS);
                glVertex2f(-tamanhoBase, tamanhoBase);
                glVertex2f(tamanhoBase, tamanhoBase);
                glVertex2f(tamanhoBase, -tamanhoBase);
                glVertex2f(-tamanhoBase, -tamanhoBase);
            glEnd();
        } 
        else if (objeto2D == 2) { // Círculo (Polígono)
            glBegin(GL_POLYGON);
            for(int i = 0; i < resolucao; i++) {
                float angulo = (2.0f * PI * i) / resolucao;
                glVertex2f(tamanhoBase * cos(angulo), tamanhoBase * sin(angulo));
            }
            glEnd();
        }

        // Reseta o PolygonMode para não quebrar outros módulos
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } 
    // ==========================================
    // RENDERIZAÇÃO 3D (FREEGLUT)
    // ==========================================
    else {
        // Configuração rápida de luz para dar volume aos sólidos
        if (aplicarLuz && !modoWireframe) {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_COLOR_MATERIAL);
            
            GLfloat posLuz[] = { 5.0f, 5.0f, 5.0f, 0.0f };
            glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
        } else {
            glDisable(GL_LIGHTING);
        }

        // Desenha usando as funções da biblioteca
        switch (objeto3D) {
            case 0: // Cubo
                if (modoWireframe) glutWireCube(tamanhoBase);
                else glutSolidCube(tamanhoBase);
                break;
            case 1: // Esfera
                if (modoWireframe) glutWireSphere(tamanhoBase, resolucao, resolucao);
                else glutSolidSphere(tamanhoBase, resolucao, resolucao);
                break;
            case 2: // Cone
                glPushMatrix();
                glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
                if (modoWireframe) glutWireCone(tamanhoBase, tamanhoBase * 1.5f, resolucao, resolucao);
                else glutSolidCone(tamanhoBase, tamanhoBase * 1.5f, resolucao, resolucao);
                glPopMatrix();
                break;
            case 3: // Toroide
                if (modoWireframe) glutWireTorus(tamanhoBase * 0.4f, tamanhoBase, resolucao, resolucao);
                else glutSolidTorus(tamanhoBase * 0.4f, tamanhoBase, resolucao, resolucao);
                break;
            case 4: // Bule
                glFrontFace(GL_CW); 
                if (modoWireframe) glutWireTeapot(tamanhoBase);
                else glutSolidTeapot(tamanhoBase);
                glFrontFace(GL_CCW); 
                break;
        }

        // Limpa a luz e a cor para não afetar o resto
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
    }
    
    glColor3f(1.0f, 1.0f, 1.0f); // Retorna a cor branca padrão
}