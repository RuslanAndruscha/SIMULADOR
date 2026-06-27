#include "../include/ModuloProjecao.h"
#include <GL/freeglut.h>
#include "imgui.h"
#include <cmath>

#define PI 3.14159265f

std::string ModuloProjecao::ObterNome() const {
    return "Projeções e Câmera";
}

void ModuloProjecao::RenderizarUI() {
    ImGui::Text("Pipeline de Visualização OpenGL");
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::BeginTabBar("AbasProjecaoCamera")) {
        
        // ==========================================
        // ABA 1: CÂMERA (GL_MODELVIEW)
        // ==========================================
        if (ImGui::BeginTabItem("1. Câmera (Posição)")) {
            ImGui::TextWrapped("gluLookAt: Define a posição física da câmera no mundo e para onde ela está apontando.");
            ImGui::Spacing();

            ImGui::Checkbox("Mostrar Visão Externa (God View)", &mostrarMiniMapa);
            ImGui::Separator();

            ImGui::Text("Alvo Automático (Target Lock):");
            ImGui::RadioButton("Livre", &objetoFoco, 0); ImGui::SameLine();
            ImGui::RadioButton("Vermelho", &objetoFoco, 1); ImGui::SameLine();
            ImGui::RadioButton("Verde", &objetoFoco, 2); ImGui::SameLine();
            ImGui::RadioButton("Azul", &objetoFoco, 3);
            
            if (objetoFoco == 1) { camCenter[0] = -2.5f; camCenter[1] = 0.5f; camCenter[2] = -2.0f; }
            if (objetoFoco == 2) { camCenter[0] =  0.0f; camCenter[1] = 0.5f; camCenter[2] = -6.0f; }
            if (objetoFoco == 3) { camCenter[0] =  2.5f; camCenter[1] = 0.5f; camCenter[2] = -10.0f; }

            ImGui::Spacing();
            ImGui::Text("Coordenadas da Câmera:");
            if (ImGui::Button("Resetar Câmera##Cam", ImVec2(-1, 0))) {
                camEye[0] = 0.0f; camEye[1] = 2.0f; camEye[2] = 3.0f;
                camCenter[0] = 0.0f; camCenter[1] = 0.0f; camCenter[2] = -6.0f;
                camUp[0] = 0.0f; camUp[1] = 1.0f; camUp[2] = 0.0f;
                objetoFoco = 0; 
            }

            ImGui::DragFloat3("Olho (Eye)##Cam", camEye, 0.1f, -20.0f, 20.0f, "%.1f");
            
            if (objetoFoco != 0) ImGui::BeginDisabled();
            ImGui::DragFloat3("Alvo (Center)##Cam", camCenter, 0.1f, -20.0f, 20.0f, "%.1f");
            if (objetoFoco != 0) ImGui::EndDisabled();
            
            ImGui::DragFloat3("Cima (Up)##Cam", camUp, 0.1f, -1.0f, 1.0f, "%.1f");

            ImGui::Separator();
            ImGui::Text("Código OpenGL Gerado:");
            ImGui::BeginChild("CodigoAreaCam", ImVec2(0, 0), true);
            ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glMatrixMode(GL_MODELVIEW);");
            ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glLoadIdentity();");
            ImGui::Text("gluLookAt(%.1f, %.1f, %.1f,  // Eye", camEye[0], camEye[1], camEye[2]);
            ImGui::Text("          %.1f, %.1f, %.1f,  // Center", camCenter[0], camCenter[1], camCenter[2]);
            ImGui::Text("          %.1f, %.1f, %.1f); // Up", camUp[0], camUp[1], camUp[2]);
            ImGui::EndChild();

            ImGui::EndTabItem();
        }

        // ==========================================
        // ABA 2: PROJEÇÃO (GL_PROJECTION)
        // ==========================================
        if (ImGui::BeginTabItem("2. Projeção (Lentes)")) {
            ImGui::Text("Tipo de Lente Óptica:");
            ImGui::RadioButton("glOrtho", &tipoProjecao, 0); ImGui::SameLine();
            ImGui::RadioButton("glFrustum", &tipoProjecao, 1); ImGui::SameLine();
            ImGui::RadioButton("gluPerspective", &tipoProjecao, 2);
            ImGui::Spacing();

            // Toggle para corrigir a distorção da interface dinamicamente
            ImGui::Checkbox("Aspect Ratio Automático (Evitar Distorção)", &aspectAutomatico);
            ImGui::Separator();

            if (tipoProjecao == 0) {
                ImGui::TextWrapped("PROJEÇÃO ORTOGONAL: Raios paralelos. Objetos mantêm o mesmo tamanho independentemente da distância da câmera.");
                if (ImGui::Button("Resetar Volume##Ortho", ImVec2(-1, 0))) {
                    orthoLimites[0] = -4.0f; orthoLimites[1] = 4.0f;
                    orthoLimites[2] = -3.0f; orthoLimites[3] = 3.0f;
                    orthoLimites[4] = 1.0f;  orthoLimites[5] = 50.0f;
                }
                
                if (aspectAutomatico) ImGui::BeginDisabled();
                ImGui::DragFloat2("Left / Right##O", &orthoLimites[0], 0.1f, -10.0f, 10.0f);
                if (aspectAutomatico) ImGui::EndDisabled();
                
                ImGui::DragFloat2("Bottom / Top##O", &orthoLimites[2], 0.1f, -10.0f, 10.0f);
                ImGui::DragFloat2("Near / Far##O", &orthoLimites[4], 0.1f, 0.1f, 20.0f);
                
                ImGui::Separator();
                ImGui::Text("Código OpenGL Gerado:");
                ImGui::BeginChild("CodigoAreaProj", ImVec2(0, 0), true);
                if (aspectAutomatico) ImGui::Text("// Os valores Left e Right estão sendo calculados \n// pela proporção da janela em tempo real!\n");
                ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glOrtho(L, R, %.2f, %.2f, %.2f, %.2f);", 
                            orthoLimites[2], orthoLimites[3], orthoLimites[4], orthoLimites[5]);
                ImGui::EndChild();
            } 
            else if (tipoProjecao == 1) {
                ImGui::TextWrapped("PROJEÇÃO PERSPECTIVA (Nativa): Define o tronco de pirâmide. Permite criar câmeras assimétricas.");
                if (ImGui::Button("Resetar Tronco##Frustum", ImVec2(-1, 0))) {
                    frustumLimites[0] = -1.0f; frustumLimites[1] = 1.0f;
                    frustumLimites[2] = -1.0f; frustumLimites[3] = 1.0f;
                    frustumLimites[4] = 1.0f;  frustumLimites[5] = 50.0f;
                }

                if (aspectAutomatico) ImGui::BeginDisabled();
                ImGui::DragFloat2("Left / Right##F", &frustumLimites[0], 0.05f, -5.0f, 5.0f);
                if (aspectAutomatico) ImGui::EndDisabled();

                ImGui::DragFloat2("Bottom / Top##F", &frustumLimites[2], 0.05f, -5.0f, 5.0f);
                ImGui::DragFloat2("Near / Far##F", &frustumLimites[4], 0.1f, 0.1f, 20.0f);
                
                ImGui::Separator();
                ImGui::Text("Código OpenGL Gerado:");
                ImGui::BeginChild("CodigoAreaProj", ImVec2(0, 0), true);
                if (aspectAutomatico) ImGui::Text("// Os valores Left e Right estão sendo calculados \n// pela proporção da janela em tempo real!\n");
                ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glFrustum(L, R, %.2f, %.2f, %.2f, %.2f);", 
                            frustumLimites[2], frustumLimites[3], frustumLimites[4], frustumLimites[5]);
                ImGui::EndChild();
            }
            else {
                ImGui::TextWrapped("PROJEÇÃO PERSPECTIVA (GLU): Função facilitadora que usa o FOV e a proporção da tela.");
                if (ImGui::Button("Resetar Lente##Persp", ImVec2(-1, 0))) {
                    perspFOV = 60.0f; perspAspect = 1.33f;
                    perspZ[0] = 1.0f; perspZ[1] = 50.0f;
                }
                ImGui::SliderFloat("Campo Visão (FOV)", &perspFOV, 10.0f, 150.0f, "%.1f graus");
                
                if (aspectAutomatico) ImGui::BeginDisabled();
                ImGui::SliderFloat("Razão Aspecto", &perspAspect, 0.1f, 3.0f, "%.2f");
                if (aspectAutomatico) ImGui::EndDisabled();

                ImGui::DragFloat2("Near / Far##P", perspZ, 0.1f, 0.1f, 50.0f);
                
                ImGui::Separator();
                ImGui::Text("Código OpenGL Gerado:");
                ImGui::BeginChild("CodigoAreaProj", ImVec2(0, 0), true);
                if (aspectAutomatico) ImGui::Text("// O Aspect Ratio está seguindo a janela dinamicamente\n");
                ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "gluPerspective(%.1f, AspectRatio, %.2f, %.2f);", perspFOV, perspZ[0], perspZ[1]);
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void ModuloProjecao::DesenharCenaExemplo() {
    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    for(int i = -12; i <= 12; i++) {
        glVertex3f((float)i, 0.0f, -12.0f); glVertex3f((float)i, 0.0f, 12.0f);
        glVertex3f(-12.0f, 0.0f, (float)i); glVertex3f(12.0f, 0.0f, (float)i);
    }
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    GLfloat posLuz[] = { 5.0f, 10.0f, 5.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, posLuz);

    glColor3f(1.0f, 0.3f, 0.3f); 
    glPushMatrix(); glTranslatef(-2.5f, 0.5f, -2.0f); glutSolidCube(1.0); glPopMatrix();

    glColor3f(0.3f, 1.0f, 0.3f); 
    glPushMatrix(); glTranslatef(0.0f, 0.5f, -6.0f); glutSolidCube(1.0); glPopMatrix();

    glColor3f(0.3f, 0.3f, 1.0f); 
    glPushMatrix(); glTranslatef(2.5f, 0.5f, -10.0f); glutSolidCube(1.0); glPopMatrix();
    
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void ModuloProjecao::DesenharModeloCamera() {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    glColor3f(0.8f, 0.8f, 0.8f);
    glutSolidCube(0.6);

    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.3f);
    glutSolidCone(0.25, 0.4, 16, 16);
    glPopMatrix();

    glColor3f(0.15f, 0.15f, 0.15f);
    glPushMatrix();
    glTranslatef(0.0f, 0.35f, -0.15f);
    glutSolidTorus(0.05, 0.2, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, 0.35f, 0.15f);
    glutSolidTorus(0.05, 0.2, 10, 10);
    glPopMatrix();

    glDisable(GL_LIGHTING);
}

void ModuloProjecao::RenderizarCena3D() {
    GLint viewportOrig[4];
    glGetIntegerv(GL_VIEWPORT, viewportOrig);

    // ==========================================
    // CÁLCULO RESPONSIVO DA LENTE DA CÂMERA
    // ==========================================
    // Descobre a proporção física exata (Largura / Altura) da nossa área de renderização 3D
    float telaAspect = (float)viewportOrig[2] / (float)viewportOrig[3];

    // Cópias temporárias dos limites matemáticos que usaremos no OpenGL
    float outLeft = orthoLimites[0], outRight = orthoLimites[1];
    float outFLeft = frustumLimites[0], outFRight = frustumLimites[1];
    float outPerspAspect = perspAspect;

    // Se estiver automático, substituímos a largura manual pela física para garantir pixels quadrados
    if (aspectAutomatico) {
        float orthoHeight = orthoLimites[3] - orthoLimites[2]; // Top - Bottom
        float orthoWidth = orthoHeight * telaAspect;
        outLeft = -(orthoWidth / 2.0f); outRight = (orthoWidth / 2.0f);

        float frustumHeight = frustumLimites[3] - frustumLimites[2];
        float frustumWidth = frustumHeight * telaAspect;
        outFLeft = -(frustumWidth / 2.0f); outFRight = (frustumWidth / 2.0f);

        outPerspAspect = telaAspect;
    }

    // ==========================================
    // PASSO 1: CENA PRINCIPAL (VISÃO DA LENTE)
    // ==========================================
    glViewport(viewportOrig[0], viewportOrig[1], viewportOrig[2], viewportOrig[3]);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    
    if (tipoProjecao == 0) glOrtho(outLeft, outRight, orthoLimites[2], orthoLimites[3], orthoLimites[4], orthoLimites[5]);
    else if (tipoProjecao == 1) glFrustum(outFLeft, outFRight, frustumLimites[2], frustumLimites[3], frustumLimites[4], frustumLimites[5]);
    else gluPerspective(perspFOV, outPerspAspect, perspZ[0], perspZ[1]);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();
    gluLookAt(camEye[0], camEye[1], camEye[2], camCenter[0], camCenter[1], camCenter[2], camUp[0], camUp[1], camUp[2]);

    DesenharCenaExemplo();

    glMatrixMode(GL_PROJECTION); glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW); glPopMatrix(); 

    // ==========================================
    // PASSO 2: MINI-MAPA (GOD VIEW)
    // ==========================================
    if (mostrarMiniMapa) {
        int miniW = 380; int miniH = 380; int margem = 20;
        int miniX = viewportOrig[0] + viewportOrig[2] - miniW - margem; 
        int miniY = viewportOrig[1] + margem;

        glEnable(GL_SCISSOR_TEST);
        glScissor(miniX, miniY, miniW, miniH);
        
        GLfloat corAntiga[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE, corAntiga);
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glClearColor(corAntiga[0], corAntiga[1], corAntiga[2], corAntiga[3]);
        
        glDisable(GL_SCISSOR_TEST);
        glViewport(miniX, miniY, miniW, miniH);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix(); glLoadIdentity();
        gluPerspective(70.0, 1.0, 0.1, 100.0); 

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); glLoadIdentity();
        
        gluLookAt(18.0, 15.0, 10.0, 0.0, 0.0, -6.0, 0.0, 1.0, 0.0);

        DesenharCenaExemplo();

        glDisable(GL_LIGHTING); 
        glColor3f(1.0f, 1.0f, 0.0f); 
        glLineStipple(1, 0x0F0F);
        glEnable(GL_LINE_STIPPLE);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
            glVertex3fv(camEye);
            glVertex3fv(camCenter);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        glLineWidth(1.0f);

        glColor3f(1.0f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(camCenter[0], camCenter[1], camCenter[2]);
        glutWireSphere(0.3, 10, 10);
        glPopMatrix();

        float dx = camCenter[0] - camEye[0];
        float dy = camCenter[1] - camEye[1];
        float dz = camCenter[2] - camEye[2];
        
        float yaw = atan2(dx, dz) * (180.0f / PI);
        float distXZ = sqrt(dx * dx + dz * dz);
        float pitch = atan2(dy, distXZ) * (180.0f / PI);

        glPushMatrix();
            glTranslatef(camEye[0], camEye[1], camEye[2]);
            glRotatef(yaw, 0.0f, 1.0f, 0.0f);
            glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
            DesenharModeloCamera();
        glPopMatrix();

        glMatrixMode(GL_PROJECTION); glPopMatrix(); 
        glMatrixMode(GL_MODELVIEW); glPopMatrix(); 

        glViewport(viewportOrig[0], viewportOrig[1], viewportOrig[2], viewportOrig[3]);
    }
}