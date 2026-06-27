#ifndef MODULO_PROJECAO_H
#define MODULO_PROJECAO_H

#include "ModuloDidatico.h"

class ModuloProjecao : public ModuloDidatico {
private:
    int tipoProjecao = 1; // 0 = glOrtho, 1 = glFrustum, 2 = gluPerspective

    // --- Parâmetros de Projeção ---
    float orthoLimites[6] = {-4.0f, 4.0f, -3.0f, 3.0f, 1.0f, 50.0f};
    float frustumLimites[6] = {-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 50.0f};
    float perspFOV = 60.0f;
    float perspAspect = 1.33f;
    float perspZ[2] = {1.0f, 15.0f};

    // --- NOVO: Correção de Distorção ---
    bool aspectAutomatico = true;

    // --- Parâmetros da Câmera (gluLookAt) ---
    float camEye[3] = {0.0f, 2.0f, 3.0f};    
    float camCenter[3] = {0.0f, 0.0f, -6.0f}; 
    float camUp[3] = {0.0f, 1.0f, 0.0f};      

    // --- Controles de Câmera e Mini-Mapa ---
    int objetoFoco = 0; // 0=Livre, 1=Vermelho, 2=Verde, 3=Azul
    bool mostrarMiniMapa = true;

    void DesenharCenaExemplo();
    void DesenharModeloCamera(); 

public:
    void RenderizarUI() override;
    void RenderizarCena3D() override;
    std::string ObterNome() const override;
};

#endif