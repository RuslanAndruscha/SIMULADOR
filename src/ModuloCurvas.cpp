#include "../include/ModuloCurvas.h"
#include <GL/freeglut.h>
#include "imgui.h"

std::string ModuloCurvas::ObterNome() const {
    return "Curvas de Bézier";
}

void ModuloCurvas::RenderizarUI() {
    ImGui::Text("Pontos de Controle (Arraste para alterar):");
    ImGui::SliderFloat2("Ponto 0", pontosControle[0], -3.0f, 3.0f);
    ImGui::SliderFloat2("Ponto 1", pontosControle[1], -3.0f, 3.0f);
    ImGui::SliderFloat2("Ponto 2", pontosControle[2], -3.0f, 3.0f);
    ImGui::SliderFloat2("Ponto 3", pontosControle[3], -3.0f, 3.0f);
    
    ImGui::Separator();
    ImGui::SliderInt("Resolução (Segmentos)", &resolucaoCurva, 5, 50);

    ImGui::Separator();
    ImGui::Text("Código OpenGL Gerado:");
    ImGui::BeginChild("CodigoArea", ImVec2(0, 0), true);
    ImGui::Text("// 1. Passa o array de pontos para o avaliador");
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &pontos[0][0]);");
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnable(GL_MAP1_VERTEX_3);");
    
    ImGui::Text("\n// 2. Avalia a curva em malha (Grid)");
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glMapGrid1f(%d, 0.0, 1.0);", resolucaoCurva);
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEvalMesh1(GL_LINE, 0, %d);", resolucaoCurva);
    ImGui::EndChild();
}

void ModuloCurvas::RenderizarCena3D() {
    // Desenha a linha conectando os pontos de controle (Polígono de Controle)
    glColor3f(0.4f, 0.4f, 0.4f); // Cinza
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 4; i++) {
        glVertex3fv(pontosControle[i]);
    }
    glEnd();

    // Desenha os pontos de controle como quadrados vermelhos
    glColor3f(1.0f, 0.2f, 0.2f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++) {
        glVertex3fv(pontosControle[i]);
    }
    glEnd();

    // Configura e desenha a Curva de Bézier
    glColor3f(1.0f, 1.0f, 1.0f); // Branco
    glLineWidth(2.0f);
    
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &pontosControle[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    
    glMapGrid1f(resolucaoCurva, 0.0, 1.0);
    glEvalMesh1(GL_LINE, 0, resolucaoCurva);
    
    // Limpeza
    glDisable(GL_MAP1_VERTEX_3);
    glLineWidth(1.0f);
    glPointSize(1.0f);
}