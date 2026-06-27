#ifndef MODULO_DIDATICO_H
#define MODULO_DIDATICO_H

#include <string>

class ModuloDidatico {
public:
    virtual ~ModuloDidatico() = default;

    // Função para desenhar os botões e textos didáticos no ImGui
    virtual void RenderizarUI() = 0;

    // Função que conterá as chamadas OpenGL (glBegin, glutSolidTeapot, etc)
    virtual void RenderizarCena3D() = 0;

    // Retorna o nome que vai aparecer na aba/menu
    virtual std::string ObterNome() const = 0;
};

#endif