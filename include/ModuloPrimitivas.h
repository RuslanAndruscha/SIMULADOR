#ifndef MODULO_PRIMITIVAS_H
#define MODULO_PRIMITIVAS_H

#include "ModuloDidatico.h"

class ModuloPrimitivas : public ModuloDidatico {
private:
    int primitivaSelecionada = 0;
    
    const char* nomesPrimitivas[10] = {
        "GL_POINTS", "GL_LINES", "GL_LINE_STRIP", "GL_LINE_LOOP",
        "GL_TRIANGLES", "GL_TRIANGLE_STRIP", "GL_TRIANGLE_FAN",
        "GL_QUADS", "GL_QUAD_STRIP", "GL_POLYGON"
    };

    unsigned int tiposGL[10] = {
        0x0000, 0x0001, 0x0003, 0x0002, 
        0x0004, 0x0005, 0x0006,          
        0x0007, 0x0008, 0x0009           
    };

    // --- Geometria e Cores ---
    static const int MAX_VERTICES = 32;
    int qtdVertices = 8;
    float vertices[MAX_VERTICES][3];
    float cores[MAX_VERTICES][3]; // NOVO: Array para guardar o RGB de cada vértice
    
    // --- Estilo da Primitiva ---
    float espessuraLinha = 2.0f;
    int estiloLinha = 0; 
    const char* nomesEstilos[4] = {"Sólida", "Tracejada", "Pontilhada", "Traço-Ponto"};
    unsigned short padroesLinha[4] = {0xFFFF, 0x00FF, 0x0101, 0x1C47}; // Padrões de 16-bits para o Stipple

    float raioPoligono = 1.5f;
    bool mostrarIndices = true;

    void DesenharTexto(float x, float y, float z, const char* texto);

public:
    ModuloPrimitivas(); 
    
    void GerarPoligonoRegular(); 
    void RenderizarUI() override;
    void RenderizarCena3D() override;
    std::string ObterNome() const override;
};

#endif