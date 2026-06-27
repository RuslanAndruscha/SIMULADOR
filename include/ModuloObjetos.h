#ifndef MODULO_OBJETOS_H
#define MODULO_OBJETOS_H

#include "ModuloDidatico.h"

class ModuloObjetos : public ModuloDidatico {
private:
    int dimensaoSelecionada = 1; // 0 = 2D, 1 = 3D
    
    int objeto2D = 0; // 0 = Triângulo, 1 = Quadrado, 2 = Círculo
    int objeto3D = 4; // 0 = Cubo, 1 = Esfera, 2 = Cone, 3 = Toroide, 4 = Bule

    bool modoWireframe = false;
    bool aplicarLuz = true; // Essencial para ver o volume dos sólidos 3D

    // Parâmetros de controle geométrico
    float tamanhoBase = 1.0f;
    int resolucao = 16; // Controla as 'fatias' da esfera, toroide e círculo

    // Listas para o ImGui
    const char* nomes2D[3] = { "Triângulo", "Quadrado", "Círculo (Polígono)" };
    const char* nomes3D[5] = { "Cubo", "Esfera", "Cone", "Toroide (Donut)", "Bule (Utah Teapot)" };

public:
    void RenderizarUI() override;
    void RenderizarCena3D() override;
    std::string ObterNome() const override;
};

#endif