#ifndef MODULO_TEXTURAS_H
#define MODULO_TEXTURAS_H

#include "ModuloDidatico.h"
#include <GL/freeglut.h>

class ModuloTexturas : public ModuloDidatico {
private:
    GLuint idTexturaXadrez = 0;
    GLuint idTexturaGrade = 0;
    bool texturasInicializadas = false;

    // Parâmetros de controle da UI
    int tipoObjeto = 1;         // 0 = Plano, 1 = Cubo
    int tipoTextura = 0;        // 0 = Xadrez Clássico 8x8, 1 = Bordas Neon
    int filtroMinMag = 0;       // 0 = GL_NEAREST, 1 = GL_LINEAR
    int modoWrap = 0;           // 0 = GL_REPEAT, 1 = GL_CLAMP_TO_EDGE
    int modoEnv = 1;            // 0 = GL_MODULATE, 1 = GL_REPLACE (Padrão Cohen)

    float repeticaoU = 1.0f;
    float repeticaoV = 1.0f;
    float corObjeto[3] = { 1.0f, 1.0f, 1.0f }; // Para testar o GL_MODULATE

    void InicializarTexturas();
    void GerarTexturaXadrez();
    void GerarTexturaGradeNeon();
    void DesenharCuboTexturizado();

public:
    void RenderizarUI() override;
    void RenderizarCena3D() override;
    std::string ObterNome() const override;
};

#endif