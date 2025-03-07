TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += bib
INCLUDEPATH += gui_glut
INCLUDEPATH += trab1

LIBS += -lGL -lGLU -lglut -l3ds -lSDL_image -lIL

SOURCES += main.cpp \
    bib/Camera.cpp \
    bib/CameraDistante.cpp \
    bib/CameraJogo.cpp \
    bib/Desenha.cpp \
    bib/gerenciadorcameras.cpp \
    bib/model3ds.cpp \
    bib/Vetor3D.cpp \
    gui_glut/extra.cpp \
    gui_glut/gui.cpp \
    gui_glut/OpenTextures.cpp \
    objetos/arvorecomplexa.cpp \
    objetos/arvoresimples.cpp \
    objetos/carro3d.cpp \
    bib/gerenciadorarquivo.cpp \
    objetos/gerenciadorarvores.cpp \
    objetos/pista.cpp \
    objetos/placaanuncio.cpp \
    bib/teclado.cpp

HEADERS += \
    bib/Camera.h \
    bib/CameraDistante.h \
    bib/CameraJogo.h \
    bib/Desenha.h \
    bib/gerenciadorcameras.h \
    bib/model3ds.h \
    bib/Vetor3D.h \
    gui_glut/extra.h \
    gui_glut/gui.h \
    gui_glut/OpenTextures.h \
    objetos/arvore.h \
    objetos/arvorecomplexa.h \
    objetos/arvoresimples.h \
    objetos/carro3d.h \
    bib/gerenciadorarquivo.h \
    objetos/gerenciadorarvores.h \
    objetos/grama.h \
    objetos/objeto.h \
    objetos/pista.h \
    objetos/placaanuncio.h \
    bib/teclado.h
