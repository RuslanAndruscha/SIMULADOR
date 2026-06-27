# ==============================================================================
# MAKEFILE - PROJETO IMGUI + FREEGLUT (WINDOWS / MINGW)
# ==============================================================================

# Nome do executável final (com a extensão .exe para Windows)
EXE = simulador.exe

# Compilador de C++
CXX = g++

# Diretórios de Include (Onde o compilador vai procurar os arquivos .h)
INCLUDES = -Iinclude -Ivendor/imgui -Ivendor/imgui/backends -Ivendor/freeglut/include

# Flags base de compilação (-Wall para avisos, -g para debug)
CXXFLAGS = $(INCLUDES) -Wall -g

LIBS = vendor/freeglut/lib/x64/libfreeglut.a -lglu32 -lopengl32

# ------------------------------------------------------------------------------
# ARQUIVOS DO PROJETO
# ------------------------------------------------------------------------------

# Lista de todos os arquivos de código-fonte (.cpp) que precisam ser compilados
SOURCES = src/main.cpp \
          src/ModuloPrimitivas.cpp \
          src/ModuloTransformacoes.cpp \
          src/ModuloProjecao.cpp \
          src/ModuloObjetos.cpp \
          src/ModuloIluminacao.cpp \
          src/ModuloCurvas.cpp \
          src/ModuloTexturas.cpp \
          vendor/imgui/imgui.cpp \
          vendor/imgui/imgui_demo.cpp \
          vendor/imgui/imgui_draw.cpp \
          vendor/imgui/imgui_tables.cpp \
          vendor/imgui/imgui_widgets.cpp \
          vendor/imgui/backends/imgui_impl_glut.cpp \
          vendor/imgui/backends/imgui_impl_opengl2.cpp

# Transforma a lista de .cpp em arquivos objeto .o correspondentes
OBJS = $(SOURCES:.cpp=.o)

# ------------------------------------------------------------------------------
# REGRAS DE COMPILAÇÃO
# ------------------------------------------------------------------------------

# Regra executada ao digitar 'make'
all: $(EXE)

# Regra de Linkagem: Junta os .o e gera o executável final
$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

# Regra de Compilação: Compila cada .cpp individualmente em um .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Regra de Limpeza: Apaga o executável e os arquivos gerados
clean:
	-del /Q /F $(EXE) 2>nul
	-del /Q /F $(subst /,\,$(OBJS)) 2>nul