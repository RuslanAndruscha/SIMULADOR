#ifndef MODULO_CURVAS_H
#define MODULO_CURVAS_H

#include "ModuloDidatico.h"

class ModuloCurvas : public ModuloDidatico {
private:
    // 4 Pontos de controle para uma curva Cúbica (X, Y, Z)
    float pontosControle[4][3] = {
        {-2.0f, -1.0f, 0.0f},
        {-1.0f,  2.0f, 0.0f},
        { 1.0f, -2.0f, 0.0f},
        { 2.0f,  1.0f, 0.0f}
    };
    int resolucaoCurva = 30;

public:
    void RenderizarUI() override;
    void RenderizarCena3D() override;
    std::string ObterNome() const override;
};

#endif