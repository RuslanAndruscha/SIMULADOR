#include "../include/ModuloIluminacao.h"
#include <GL/freeglut.h>
#include "imgui.h"

std::string ModuloIluminacao::ObterNome() const {
    return "Iluminação e Materiais";
}

void ModuloIluminacao::RenderizarUI() {
    ImGui::Checkbox("Habilitar GL_LIGHTING (Global)", &iluminacaoGlobal);
    ImGui::Separator();

    ImGui::Text("Objeto Modelo:");
    ImGui::RadioButton("Esfera", &objetoSelecionado, 0); ImGui::SameLine();
    ImGui::RadioButton("Cubo", &objetoSelecionado, 1); ImGui::SameLine();
    ImGui::RadioButton("Bule", &objetoSelecionado, 2);

    ImGui::Separator();
    ImGui::Spacing();

    // ==========================================
    // SISTEMA DE ABAS COM BOTÕES DE RESET
    // ==========================================
    if (ImGui::BeginTabBar("AbasIluminacao")) {
        
        // --- ABA LUZ 1 ---
        if (ImGui::BeginTabItem("Luz 1 (GL_LIGHT0)")) {
            ImGui::Checkbox("Ligar Luz Principal", &luz0Ativa);
            ImGui::SameLine();
            if (ImGui::Button("Reset##L1")) {
                posLuz0[0] = 3.0f;  posLuz0[1] = 4.0f;  posLuz0[2] = 5.0f;
                ambLuz0[0] = 0.10f; ambLuz0[1] = 0.00f; ambLuz0[2] = 0.05f;
                difLuz0[0] = 1.00f; difLuz0[1] = 0.00f; difLuz0[2] = 0.20f;
                espLuz0[0] = 1.00f; espLuz0[1] = 0.30f; espLuz0[2] = 0.30f;
            }

            if (luz0Ativa) {
                ImGui::DragFloat3("Posição (X,Y,Z)##L0", posLuz0, 0.05f, -10.0f, 10.0f, "%.2f");
                ImGui::Spacing();
                ImGui::ColorEdit3("Cor Ambiente##L0", ambLuz0);
                ImGui::ColorEdit3("Cor Difusa##L0", difLuz0);
                ImGui::ColorEdit3("Cor Especular##L0", espLuz0);
            }
            ImGui::EndTabItem();
        }

        // --- ABA LUZ 2 ---
        if (ImGui::BeginTabItem("Luz 2 (GL_LIGHT1)")) {
            ImGui::Checkbox("Ligar Luz Secundária", &luz1Ativa);
            ImGui::SameLine();
            if (ImGui::Button("Reset##L2")) {
                posLuz1[0] = -4.0f; posLuz1[1] = -2.0f; posLuz1[2] = -3.0f;
                ambLuz1[0] = 0.00f; ambLuz1[1] = 0.05f; ambLuz1[2] = 0.10f;
                difLuz1[0] = 0.00f; difLuz1[1] = 0.70f; difLuz1[2] = 1.00f;
                espLuz1[0] = 0.30f; espLuz1[1] = 0.80f; espLuz1[2] = 1.00f;
            }

            if (luz1Ativa) {
                ImGui::DragFloat3("Posição (X,Y,Z)##L1", posLuz1, 0.05f, -10.0f, 10.0f, "%.2f");
                ImGui::Spacing();
                ImGui::ColorEdit3("Cor Ambiente##L1", ambLuz1);
                ImGui::ColorEdit3("Cor Difusa##L1", difLuz1);
                ImGui::ColorEdit3("Cor Especular##L1", espLuz1);
            }
            ImGui::EndTabItem();
        }

        // --- ABA MATERIAL ---
        if (ImGui::BeginTabItem("Material (glMaterial)")) {
            ImGui::Text("Propriedades Base");
            ImGui::SameLine();
            if (ImGui::Button("Reset##Mat")) {
                matAmb[0] = 0.05f; matAmb[1] = 0.05f; matAmb[2] = 0.05f;
                matDif[0] = 0.10f; matDif[1] = 0.10f; matDif[2] = 0.12f;
                matEsp[0] = 0.90f; matEsp[1] = 0.90f; matEsp[2] = 0.95f;
                matBrilho = 90.0f;
            }
            ImGui::Spacing();

            ImGui::ColorEdit3("Reflexão Ambiente", matAmb);
            ImGui::ColorEdit3("Reflexão Difusa", matDif);
            ImGui::ColorEdit3("Reflexão Especular", matEsp);
            ImGui::SliderFloat("Intensidade (Shininess)", &matBrilho, 0.0f, 128.0f, "%.0f");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Separator();
    
    // ==========================================
    // CÓDIGO OPENGL GERADO DINAMICAMENTE
    // ==========================================
    ImGui::Text("Código OpenGL Gerado:");
    ImGui::BeginChild("CodigoArea", ImVec2(0, 0), true);
    
    if (iluminacaoGlobal) {
        ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnable(GL_LIGHTING);");
    } else {
        ImGui::TextColored(ImVec4(0.8f, 0.3f, 0.3f, 1.0f), "glDisable(GL_LIGHTING);");
    }
    
    ImGui::Spacing();

    if (luz0Ativa) {
        ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnable(GL_LIGHT0);");
        ImGui::Text("GLfloat pos0[] = {%.1ff, %.1ff, %.1ff, 1.0f};", posLuz0[0], posLuz0[1], posLuz0[2]);
        ImGui::Text("glLightfv(GL_LIGHT0, GL_POSITION, pos0);");
        
        // Componente Ambiente Adicionada
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "GLfloat amb0[] = {%.2ff, %.2ff, %.2ff, 1.0f};", ambLuz0[0], ambLuz0[1], ambLuz0[2]);
        ImGui::Text("glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);");
        
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "GLfloat dif0[] = {%.2ff, %.2ff, %.2ff, 1.0f};", difLuz0[0], difLuz0[1], difLuz0[2]);
        ImGui::Text("glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);");
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "GLfloat esp0[] = {%.2ff, %.2ff, %.2ff, 1.0f};", espLuz0[0], espLuz0[1], espLuz0[2]);
        ImGui::Text("glLightfv(GL_LIGHT0, GL_SPECULAR, esp0);");
    } else {
        ImGui::TextColored(ImVec4(0.8f, 0.3f, 0.3f, 1.0f), "glDisable(GL_LIGHT0);");
    }

    ImGui::Spacing();

    if (luz1Ativa) {
        ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnable(GL_LIGHT1);");
        ImGui::Text("GLfloat pos1[] = {%.1ff, %.1ff, %.1ff, 1.0f};", posLuz1[0], posLuz1[1], posLuz1[2]);
        ImGui::Text("glLightfv(GL_LIGHT1, GL_POSITION, pos1);");
        
        // Componente Ambiente Adicionada
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "GLfloat amb1[] = {%.2ff, %.2ff, %.2ff, 1.0f};", ambLuz1[0], ambLuz1[1], ambLuz1[2]);
        ImGui::Text("glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);");
        
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "GLfloat dif1[] = {%.2ff, %.2ff, %.2ff, 1.0f};", difLuz1[0], difLuz1[1], difLuz1[2]);
        ImGui::Text("glLightfv(GL_LIGHT1, GL_DIFFUSE, dif1);");
        
        // Componente Especular Adicionada
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "GLfloat esp1[] = {%.2ff, %.2ff, %.2ff, 1.0f};", espLuz1[0], espLuz1[1], espLuz1[2]);
        ImGui::Text("glLightfv(GL_LIGHT1, GL_SPECULAR, esp1);");
    }

    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "// Aplicando Material ao Objeto:");
    
    // Componente Ambiente do Material Adicionada
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "GLfloat matAmb[] = {%.2ff, %.2ff, %.2ff, 1.0f};", matAmb[0], matAmb[1], matAmb[2]);
    ImGui::Text("glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);");
    
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "GLfloat matDif[] = {%.2ff, %.2ff, %.2ff, 1.0f};", matDif[0], matDif[1], matDif[2]);
    ImGui::Text("glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);");
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "GLfloat matEsp[] = {%.2ff, %.2ff, %.2ff, 1.0f};", matEsp[0], matEsp[1], matEsp[2]);
    ImGui::Text("glMaterialfv(GL_FRONT, GL_SPECULAR, matEsp);");
    ImGui::Text("glMaterialf(GL_FRONT, GL_SHININESS, %.1ff);", matBrilho);

    ImGui::EndChild();
}

void ModuloIluminacao::RenderizarCena3D() {
    // 1. CHAVE GERAL DE ILUMINAÇÃO
    if (iluminacaoGlobal) glEnable(GL_LIGHTING);
    else glDisable(GL_LIGHTING);

    // 2. CONFIGURA LUZ 0
    if (luz0Ativa) {
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, posLuz0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambLuz0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, difLuz0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, espLuz0);
    } else {
        glDisable(GL_LIGHT0);
    }

    // 3. CONFIGURA LUZ 1
    if (luz1Ativa) {
        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_POSITION, posLuz1);
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambLuz1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, difLuz1);
        glLightfv(GL_LIGHT1, GL_SPECULAR, espLuz1);
    } else {
        glDisable(GL_LIGHT1);
    }

    // ==========================================
    // 4. INDICADORES VISUAIS DAS LUZES 
    // ==========================================
    glDisable(GL_LIGHTING); 
    
    if (luz0Ativa && iluminacaoGlobal) {
        glPushMatrix();
        glTranslatef(posLuz0[0], posLuz0[1], posLuz0[2]);
        glColor3fv(difLuz0); 
        glutSolidSphere(0.15, 16, 16);
        glPopMatrix();
    }
    
    if (luz1Ativa && iluminacaoGlobal) {
        glPushMatrix();
        glTranslatef(posLuz1[0], posLuz1[1], posLuz1[2]);
        glColor3fv(difLuz1);
        glutSolidSphere(0.15, 16, 16);
        glPopMatrix();
    }
    
    if (iluminacaoGlobal) glEnable(GL_LIGHTING);

    // ==========================================
    // 5. APLICAÇÃO DO MATERIAL E DESENHO
    // ==========================================
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matEsp);
    glMaterialf(GL_FRONT, GL_SHININESS, matBrilho);

    if (objetoSelecionado == 0) {
        glutSolidSphere(1.2, 64, 64);
    } else if (objetoSelecionado == 1) {
        glPushMatrix();
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
        glutSolidCube(1.5);
        glPopMatrix();
    } else {
        glutSolidTeapot(1.2);
    }
}