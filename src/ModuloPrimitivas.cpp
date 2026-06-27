#include "../include/ModuloPrimitivas.h"
#include <GL/freeglut.h>
#include "imgui.h"
#include <cmath>

#define PI 3.14159265f

ModuloPrimitivas::ModuloPrimitivas() {
    // Inicializa todos os vértices com a cor BRANCA padrão
    for(int i = 0; i < MAX_VERTICES; i++) {
        cores[i][0] = 1.0f; // R
        cores[i][1] = 1.0f; // G
        cores[i][2] = 1.0f; // B
    }
    GerarPoligonoRegular();
}

std::string ModuloPrimitivas::ObterNome() const {
    return "Primitivas Gráficas";
}

void ModuloPrimitivas::GerarPoligonoRegular() {
    for (int i = 0; i < qtdVertices; i++) {
        float angulo = (2.0f * PI * i) / qtdVertices;
        vertices[i][0] = raioPoligono * cos(angulo);
        vertices[i][1] = raioPoligono * sin(angulo);
        vertices[i][2] = 0.0f; 
    }
}

void ModuloPrimitivas::DesenharTexto(float x, float y, float z, const char* texto) {
    glDisable(GL_DEPTH_TEST);
    float offsets[8][2] = {{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};

    glColor3f(0.0f, 0.0f, 0.0f); 
    for (int i = 0; i < 8; i++) {
        glRasterPos3f(x + 0.1f, y + 0.1f, z);
        glBitmap(0, 0, 0, 0, offsets[i][0], offsets[i][1], NULL);
        for (const char* c = texto; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glColor3f(1.0f, 1.0f, 1.0f); 
    glRasterPos3f(x + 0.1f, y + 0.1f, z);
    for (const char* c = texto; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glEnable(GL_DEPTH_TEST);
}

void ModuloPrimitivas::RenderizarUI() {
    // --- 1. CONFIGURAÇÕES GERAIS ---
    ImGui::Text("Tipo de Primitiva:");
    if (ImGui::BeginCombo("##combo", nomesPrimitivas[primitivaSelecionada])) {
        for (int i = 0; i < 10; i++) {
            bool is_selected = (primitivaSelecionada == i);
            if (ImGui::Selectable(nomesPrimitivas[i], is_selected)) primitivaSelecionada = i;
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    
    // Controles de Estilo (Aplicável a linhas e pontos)
    ImGui::SliderFloat("Espessura / Tamanho", &espessuraLinha, 1.0f, 10.0f, "%.1f px");
    ImGui::Combo("Estilo da Linha", &estiloLinha, nomesEstilos, 4);
    
    ImGui::Checkbox("Mostrar numeração dos vértices", &mostrarIndices);
    ImGui::Separator();
    
    // --- 2. GERADOR MATEMÁTICO ---
    ImGui::Text("Gerador de Polígonos Regulares:");
    ImGui::SliderInt("Qtd de Vértices", &qtdVertices, 3, MAX_VERTICES);
    ImGui::SliderFloat("Raio", &raioPoligono, 0.5f, 5.0f);
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
    if (ImGui::Button("Gerar Forma no Centro", ImVec2(-1, 0))) {
        GerarPoligonoRegular();
    }
    ImGui::PopStyleColor(2);
    ImGui::Separator();
    
    // --- 3. EDIÇÃO MANUAL (POSIÇÃO E COR) ---
    ImGui::Text("Edição Manual de Vértices:");
    ImGui::BeginChild("ListaVertices", ImVec2(0, 150), true);
    for (int i = 0; i < qtdVertices; i++) {
        ImGui::PushID(i); // Garante que cada linha tenha IDs únicos no ImGui
        
        // Botão de cor compacto
        ImGui::ColorEdit3("##cor", cores[i], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        ImGui::SameLine();
        
        char label[32];
        sprintf(label, "V%d", i);
        ImGui::DragFloat3(label, vertices[i], 0.01f, -5.0f, 5.0f, "%.2f");
        
        ImGui::PopID();
    }
    ImGui::EndChild();
    ImGui::Separator();
    
    // --- 4. CÓDIGO GERADO ---
    ImGui::Text("Código OpenGL Gerado:");
    ImGui::BeginChild("CodigoArea", ImVec2(0, 0), true);
    
    if (espessuraLinha > 1.0f) {
        ImGui::Text("glLineWidth(%.1f);", espessuraLinha);
        ImGui::Text("glPointSize(%.1f);", espessuraLinha);
    }
    
    if (estiloLinha != 0) {
        ImGui::TextColored(ImVec4(0.8f, 0.3f, 0.8f, 1.0f), "glEnable(GL_LINE_STIPPLE);");
        ImGui::Text("glLineStipple(1, 0x%04X);", padroesLinha[estiloLinha]);
    }

    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "\nglBegin(%s);", nomesPrimitivas[primitivaSelecionada]);
    for (int i = 0; i < qtdVertices; i++) {
        // Mostra a mudança de cor antes da posição
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "  glColor3f(%.2f, %.2f, %.2f);", cores[i][0], cores[i][1], cores[i][2]);
        ImGui::Text("  glVertex3f(%.2f, %.2f, %.2f);", vertices[i][0], vertices[i][1], vertices[i][2]);
    }
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnd();");
    
    if (estiloLinha != 0) {
        ImGui::TextColored(ImVec4(0.8f, 0.3f, 0.8f, 1.0f), "glDisable(GL_LINE_STIPPLE);");
    }
    
    ImGui::EndChild();
}

void ModuloPrimitivas::RenderizarCena3D() {
    // 1. Aplica as propriedades globais de estilo
    glLineWidth(espessuraLinha);
    glPointSize(espessuraLinha); // Aplica também ao GL_POINTS

    if (estiloLinha != 0) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, padroesLinha[estiloLinha]);
    }

    // 2. Desenha a Primitiva Principal com Cores Individuais
    glBegin(tiposGL[primitivaSelecionada]);
    for (int i = 0; i < qtdVertices; i++) {
        // No OpenGL Clássico, a cor é um estado. Devemos chamá-la ANTES do vértice
        glColor3fv(cores[i]); 
        glVertex3fv(vertices[i]);
    }
    glEnd();

    // Limpa os estilos para não afetar as linhas auxiliares
    glDisable(GL_LINE_STIPPLE);
    glLineWidth(1.0f);
    glPointSize(1.0f);

    // 3. Desenha pontos e índices auxiliares
    if (mostrarIndices) {
        glDisable(GL_LIGHTING); 
        for (int i = 0; i < qtdVertices; i++) {
            glColor3f(0.5f, 0.5f, 0.5f); // Pontinho auxiliar cinza
            glPointSize(4.0f);
            glBegin(GL_POINTS);
                glVertex3fv(vertices[i]);
            glEnd();

            char id[4];
            sprintf(id, "%d", i);
            DesenharTexto(vertices[i][0], vertices[i][1], vertices[i][2], id);
        }
    }
}