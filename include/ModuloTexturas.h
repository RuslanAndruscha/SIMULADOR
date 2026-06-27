#ifndef MODULO_TEXTURAS_H
#define MODULO_TEXTURAS_H

#include "ModuloDidatico.h"
#include <GL/freeglut.h>

class ModuloTexturas : public ModuloDidatico {
private:
    bool texturaGerada = false;
    GLuint idTextura;
    float repeticaoTextura = 1.0f; 

    void GerarTexturaXadrez();

public:
    void RenderizarUI() override;
    void RenderizarCena3D() override;
    std::string ObterNome() const override;
};

#endif