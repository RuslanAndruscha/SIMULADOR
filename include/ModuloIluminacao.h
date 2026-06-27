#ifndef MODULO_ILUMINACAO_H
#define MODULO_ILUMINACAO_H

#include "ModuloDidatico.h"

class ModuloIluminacao : public ModuloDidatico {
private:
    bool iluminacaoGlobal = true;
    int objetoSelecionado = 0; // 0 = Esfera, 1 = Cubo, 2 = Bule

    // --- LUZ 0 (Vermelho Neon) ---
    bool luz0Ativa = true;
    float posLuz0[4] = { 3.0f, 4.0f, 5.0f, 1.0f }; 
    float ambLuz0[4] = { 0.10f, 0.00f, 0.05f, 1.0f }; 
    float difLuz0[4] = { 1.00f, 0.00f, 0.20f, 1.0f }; // Vermelho forte
    float espLuz0[4] = { 1.00f, 0.30f, 0.30f, 1.0f }; 

    // --- LUZ 1 (Ciano / Azul Neon) ---
    bool luz1Ativa = true; 
    float posLuz1[4] = { -4.0f, -2.0f, -3.0f, 1.0f };
    float ambLuz1[4] = { 0.00f, 0.05f, 0.10f, 1.0f };
    float difLuz1[4] = { 0.00f, 0.70f, 1.00f, 1.0f }; // Azul/Ciano vibrante
    float espLuz1[4] = { 0.30f, 0.80f, 1.00f, 1.0f }; 

    // --- MATERIAL DO OBJETO (Metal Cyberpunk Escuro) ---
    float matAmb[4] = { 0.05f, 0.05f, 0.05f, 1.0f }; 
    float matDif[4] = { 0.10f, 0.10f, 0.12f, 1.0f }; // Base quase preta/chumbo
    float matEsp[4] = { 0.90f, 0.90f, 0.95f, 1.0f }; // Reflexo especular altíssimo
    float matBrilho = 90.0f; // Superfície bem polida/molhada

public:
    void RenderizarUI() override;
    void RenderizarCena3D() override;
    std::string ObterNome() const override;
};

#endif