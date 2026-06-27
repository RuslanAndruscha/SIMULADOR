#ifndef MODULO_TRANSFORMACOES_H
#define MODULO_TRANSFORMACOES_H

#include "ModuloDidatico.h"

class ModuloTransformacoes : public ModuloDidatico {
private:
    float translacao[3] = {0.0f, 0.0f, 0.0f};
    float rotacao[3] = {0.0f, 0.0f, 0.0f};
    float escala[3] = {1.0f, 1.0f, 1.0f};

    // Novas variáveis de controle
    int ordemTransformacao = 0; // 0 = Translada->Rotaciona, 1 = Rotaciona->Translada
    bool mostrarCuboReferencia = true;

public:
    void RenderizarUI() override;
    void RenderizarCena3D() override;
    std::string ObterNome() const override;
};

#endif