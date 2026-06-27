#include "../include/ModuloTransformacoes.h"
#include <GL/freeglut.h>
#include "imgui.h"

std::string ModuloTransformacoes::ObterNome() const {
    return "Transformações Geométricas";
}

void ModuloTransformacoes::RenderizarUI() {
    ImGui::Text("Valores das Transformações:");
    ImGui::Spacing();
    
    // --- CONTROLES COM BOTOES DE RESET INDIVIDUAIS ---

    // 1. Translação
    ImGui::Text("Translação (X,Y,Z)"); 
    ImGui::SameLine();
    if (ImGui::Button("Reset##T")) { 
        translacao[0] = 0.0f; translacao[1] = 0.0f; translacao[2] = 0.0f; 
    }
    ImGui::DragFloat3("##Trans", translacao, 0.02f, -5.0f, 5.0f, "%.2f");

    ImGui::Spacing();

    // 2. Rotação
    ImGui::Text("Rotação (Graus)"); 
    ImGui::SameLine();
    if (ImGui::Button("Reset##R")) { 
        rotacao[0] = 0.0f; rotacao[1] = 0.0f; rotacao[2] = 0.0f; 
    }
    ImGui::DragFloat3("##Rot", rotacao, 0.5f, -360.0f, 360.0f, "%.1f");

    ImGui::Spacing();

    // 3. Escala (ATENÇÃO: Escala reseta para 1.0, não para 0.0!)
    ImGui::Text("Escala (X,Y,Z)"); 
    ImGui::SameLine();
    if (ImGui::Button("Reset##E")) { 
        escala[0] = 1.0f; escala[1] = 1.0f; escala[2] = 1.0f; 
    }
    ImGui::DragFloat3("##Esc", escala, 0.02f, 0.1f, 5.0f, "%.2f");

    ImGui::Separator();
    
    // --- CONTROLE DIDÁTICO DA ORDEM ---
    ImGui::Text("Ordem de Aplicação no Código OpenGL:");
    ImGui::RadioButton("1º Translada, 2º Rotaciona (Gira no próprio eixo)", &ordemTransformacao, 0);
    ImGui::RadioButton("1º Rotaciona, 2º Translada (Orbita a origem)", &ordemTransformacao, 1);
    
    ImGui::Spacing();
    ImGui::Checkbox("Mostrar cubo original na origem (0,0,0)", &mostrarCuboReferencia);

    ImGui::Separator();
    
    // --- CÓDIGO GERADO DINAMICAMENTE ---
    ImGui::Text("Código OpenGL Gerado:");
    ImGui::BeginChild("CodigoArea", ImVec2(0, 0), true);

    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glPushMatrix();");

    if (ordemTransformacao == 0) {
        ImGui::Text("  glTranslatef(%.2f, %.2f, %.2f);", translacao[0], translacao[1], translacao[2]);
        ImGui::Text("  glRotatef(%.1f, 1, 0, 0);", rotacao[0]);
        ImGui::Text("  glRotatef(%.1f, 0, 1, 0);", rotacao[1]);
        ImGui::Text("  glRotatef(%.1f, 0, 0, 1);", rotacao[2]);
    } else {
        ImGui::Text("  glRotatef(%.1f, 1, 0, 0);", rotacao[0]);
        ImGui::Text("  glRotatef(%.1f, 0, 1, 0);", rotacao[1]);
        ImGui::Text("  glRotatef(%.1f, 0, 0, 1);", rotacao[2]);
        ImGui::Text("  glTranslatef(%.2f, %.2f, %.2f);", translacao[0], translacao[1], translacao[2]);
    }
    
    ImGui::Text("  glScalef(%.2f, %.2f, %.2f);", escala[0], escala[1], escala[2]);
    ImGui::Text("\n  glutWireCube(1.0);");
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glPopMatrix();");

    ImGui::EndChild();
}

void ModuloTransformacoes::RenderizarCena3D() {
    // 1. Desenha um cubo fantasma na origem para referência visual
    if (mostrarCuboReferencia) {
        glDisable(GL_LIGHTING);
        glColor3f(0.4f, 0.4f, 0.4f); // Cinza
        
        // Ativa um padrão de linha pontilhada no OpenGL
        glLineStipple(1, 0x00FF);    
        glEnable(GL_LINE_STIPPLE);
        
        glutWireCube(1.0);
        
        glDisable(GL_LINE_STIPPLE);
        glColor3f(1.0f, 1.0f, 1.0f); // Volta pro branco
    }

    // 2. Aplica as transformações no cubo principal
    glPushMatrix();
        
        if (ordemTransformacao == 0) {
            // Translada e depois rotaciona o sistema de coordenadas local
            glTranslatef(translacao[0], translacao[1], translacao[2]);
            glRotatef(rotacao[0], 1.0f, 0.0f, 0.0f);
            glRotatef(rotacao[1], 0.0f, 1.0f, 0.0f);
            glRotatef(rotacao[2], 0.0f, 0.0f, 1.0f);
        } else {
            // Rotaciona o sistema global e empurra o objeto pra frente nesse sistema girado
            glRotatef(rotacao[0], 1.0f, 0.0f, 0.0f);
            glRotatef(rotacao[1], 0.0f, 1.0f, 0.0f);
            glRotatef(rotacao[2], 0.0f, 0.0f, 1.0f);
            glTranslatef(translacao[0], translacao[1], translacao[2]);
        }
        
        // A escala sempre fica por último (mais próxima do desenho do vértice) 
        // para não deformar o sistema de eixos de rotação.
        glScalef(escala[0], escala[1], escala[2]);

        glLineWidth(2.0f); // Deixa a linha mais grossa para destacar
        glutWireCube(1.0);
        glLineWidth(1.0f);
        
    glPopMatrix();
}