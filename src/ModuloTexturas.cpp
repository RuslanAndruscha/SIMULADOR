#include "../include/ModuloTexturas.h"
#include "imgui.h"

std::string ModuloTexturas::ObterNome() const {
    return "Mapeamento de Textura";
}

void ModuloTexturas::GerarTexturaXadrez() {
    const int largura = 64;
    const int altura = 64;
    GLubyte imagem[altura][largura][3];

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            int c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
            imagem[i][j][0] = (GLubyte)c;
            imagem[i][j][1] = (GLubyte)c;
            imagem[i][j][2] = (GLubyte)c;
        }
    }

    glGenTextures(1, &idTextura);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, largura, altura, 0, GL_RGB, GL_UNSIGNED_BYTE, imagem);
    
    texturaGerada = true;
}

void ModuloTexturas::RenderizarUI() {
    ImGui::Text("Parâmetros da Textura:");
    ImGui::SliderFloat("Repetição (Escala UV)", &repeticaoTextura, 0.5f, 5.0f);
    
    ImGui::Separator();
    ImGui::Text("Código OpenGL Gerado:");
    ImGui::BeginChild("CodigoArea", ImVec2(0, 0), true);
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glEnable(GL_TEXTURE_2D);");
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.8f, 1.0f), "glBindTexture(GL_TEXTURE_2D, idTextura);");
    
    ImGui::Text("\nglBegin(GL_QUADS);");
    ImGui::Text("  glTexCoord2f(0.0, 0.0);"); 
    ImGui::Text("  glVertex3f(-1.5, -1.5, 0.0);");
    
    ImGui::Text("\n  glTexCoord2f(%.1f, 0.0);", repeticaoTextura);
    ImGui::Text("  glVertex3f( 1.5, -1.5, 0.0);");
    
    ImGui::Text("\n  glTexCoord2f(%.1f, %.1f);", repeticaoTextura, repeticaoTextura);
    ImGui::Text("  glVertex3f( 1.5,  1.5, 0.0);");
    
    ImGui::Text("\n  glTexCoord2f(0.0, %.1f);", repeticaoTextura);
    ImGui::Text("  glVertex3f(-1.5,  1.5, 0.0);");
    ImGui::Text("glEnd();");
    ImGui::EndChild();
}

void ModuloTexturas::RenderizarCena3D() {
    if (!texturaGerada) {
        GerarTexturaXadrez();
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);                         glVertex3f(-1.5f, -1.5f, 0.0f);
        glTexCoord2f(repeticaoTextura, 0.0f);             glVertex3f( 1.5f, -1.5f, 0.0f);
        glTexCoord2f(repeticaoTextura, repeticaoTextura); glVertex3f( 1.5f,  1.5f, 0.0f);
        glTexCoord2f(0.0f, repeticaoTextura);             glVertex3f(-1.5f,  1.5f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}