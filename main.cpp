#include <iostream>
#include <vector>
#include <memory>
#include <objetos/arvoresimples.h>
#include <objetos/arvorecomplexa.h>
#include <GL/glut.h>
#include <gui.h>
#include <objetos/gerenciadorarquivo.h>
#include "bib/Camera.h"
#include "bib/CameraDistante.h"
#include "bib/CameraJogo.h"
#include "bib/gerenciadorcameras.h"
#include "bib/model3ds.h"

using namespace std;

Model3DS carro = Model3DS("../3ds/cartest.3DS");

float posicaoCarroZ = -10.0; // Começa no início da pista
float velocidadeCarro = 0.05; // Velocidade de deslocamento


Vetor3D posicaoLuz = Vetor3D(1.0, 1.5, 1.0);

// Vetor dinâmico de objetos
vector<unique_ptr<Objeto>> objetos;

// Índice do objeto atualmente selecionado
int objetoSelecionado = -1;

// Instância do Gerenciador de Câmeras
GerenciadorCameras gerenciadorCameras;

// Controle global da exibição dos eixos para todos os objetos
bool mostrarTodosEixos = false;

// Índice da configuração de câmera atual
int indiceCameraAtual = 0;



void desenhar() {
    GUI::displayInit();

    Camera* camera = gerenciadorCameras.getCamera();
    if (camera) {
        camera->posicionar();
    }

    GUI::setLight(0, posicaoLuz.x, posicaoLuz.y, posicaoLuz.z, true, false);

    // grama
    glColor3f(0.2, 0.35, 0.15);
    GUI::drawFloor(20, 20, 1, 1, 1, 1);

    // pista
    glEnable(GL_TEXTURE_2D);
    GUI::selecionaTextura(0);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0, 0);  glVertex3f(1, 0.002, -10);
    glTexCoord2f(1, 0);  glVertex3f(-1, 0.002, -10);
    glTexCoord2f(0, 1);  glVertex3f(-1, 0.002, 10);
    glTexCoord2f(1, 1);  glVertex3f(1, 0.002, 10);
    glEnd();
    glDisable(GL_TEXTURE_2D);


    GUI::draw3ds(carro, 0, 0.1, posicaoCarroZ, 0, 0, 0, 1, 1, 1);


    // Atualiza a posição do carro
    posicaoCarroZ += velocidadeCarro;

    // Se chegar ao fim da pista, reinicia no começo
    if (posicaoCarroZ > 10.0) {
        posicaoCarroZ = -10.0;
    }



    for (size_t i = 0; i < objetos.size(); i++) {
        auto& obj = objetos[i];
        glPushMatrix();

        if ((int)i == objetoSelecionado) {
            obj->rotacaoX += glutGUI::dax * 5.0;
            obj->rotacaoY += glutGUI::day * 5.0;
            obj->translacaoX += glutGUI::drx * 0.5;
            obj->translacaoY += glutGUI::dry * 0.5;
        }

        // Aplicar transformações ao objeto
        glTranslatef(obj->translacaoX, obj->translacaoY, obj->translacaoZ);
        glRotatef(obj->rotacaoX, 1, 0, 0);
        glRotatef(obj->rotacaoY, 0, 1, 0);
        glRotatef(obj->rotacaoZ, 0, 0, 1);
        glScalef(obj->escalaX, obj->escalaY, obj->escalaZ);

        // Respeitar a exibição global dos eixos
        if (mostrarTodosEixos) {
            obj->setMostrarEixos(true);
        }

        obj->desenhar();
        glPopMatrix();
    }

    if (objetoSelecionado >= 0) {
        auto& obj = objetos[objetoSelecionado];
        float scaleFactor = 1.0 + glutGUI::dsx;
        obj->escalaX *= scaleFactor;
        obj->escalaY *= scaleFactor;
        obj->escalaZ *= scaleFactor;
    }

    GUI::displayEnd();
}

void teclado(unsigned char tecla, int x, int y) {
    GUI::keyInit(tecla, x, y);

    // Objeto ativo
    Objeto* obj = (objetoSelecionado >= 0) ? objetos[objetoSelecionado].get() : nullptr;
    Camera* cam = gerenciadorCameras.getCamera();

    switch (tecla) {
    // Incluir nova ArvoreSimples
    case '1': {
        auto novaArvore = make_unique<ArvoreSimples>(
            0.0f, 0.0f, 0.0f, 1.2f, 0.2f, 0.5f, 0.3f,
            0.55f, 0.27f, 0.07f, 0.0f, 0.8f, 0.0f
            );
        novaArvore->setSelecionado(true);
        novaArvore->setMostrarEixos(false);

        if (objetoSelecionado >= 0) {
            objetos[objetoSelecionado]->setSelecionado(false);
        }

        objetos.push_back(move(novaArvore));
        objetoSelecionado = objetos.size() - 1;

        cout << "Nova ArvoreSimples adicionada." << endl;
        break;
    }

        // Incluir nova ArvoreComplexa
    case '2': {
        auto novaArvore = make_unique<ArvoreComplexa>(
            0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.8f, 0.35f,
            0.6f, 0.3f, 0.1f, 0.0f, 0.7f, 0.0f
            );
        novaArvore->setSelecionado(true);
        novaArvore->setMostrarEixos(false);

        if (objetoSelecionado >= 0) {
            objetos[objetoSelecionado]->setSelecionado(false);
        }

        objetos.push_back(move(novaArvore));
        objetoSelecionado = objetos.size() - 1;

        cout << "Nova ArvoreComplexa adicionada." << endl;
        break;
    }

        // Alternar para o próximo objeto
    case 'n':
        if (!objetos.empty()) {
            objetos[objetoSelecionado]->setSelecionado(false);
            objetoSelecionado = (objetoSelecionado + 1) % objetos.size();
            objetos[objetoSelecionado]->setSelecionado(true);
            cout << "Selecionado próximo objeto." << endl;
        }
        break;

        // Alternar para o objeto anterior
    case 'b':
        if (!objetos.empty()) {
            objetos[objetoSelecionado]->setSelecionado(false);
            objetoSelecionado = (objetoSelecionado - 1 + objetos.size()) % objetos.size();
            objetos[objetoSelecionado]->setSelecionado(true);
            cout << "Selecionado objeto anterior." << endl;
        }
        break;

        // Exibição de eixos
    case 'R': // Alternar exibição dos eixos do objeto selecionado
        if (obj) {
            obj->setMostrarEixos(!obj->mostrarEixos);
            cout << "Eixos do objeto " << objetoSelecionado << " "
                 << (obj->mostrarEixos ? "MOSTRADOS" : "OCULTOS") << "." << endl;
        }
        break;

    case 'T': // Ativar/Desativar exibição dos eixos de todos os objetos
        mostrarTodosEixos = !mostrarTodosEixos;
        if (!mostrarTodosEixos) {
            for (auto& obj : objetos) {
                obj->setMostrarEixos(false);
            }
        }
        cout << "Exibição dos eixos globais: "
             << (mostrarTodosEixos ? "ATIVADA" : "DESATIVADA") << endl;
        break;

        // Movimentação
    case 'w': if (obj) obj->mover(0, 0.1, 0); break;
    case 's': if (obj) obj->mover(0, -0.1, 0); break;
    case 'a': if (obj) obj->mover(-0.1, 0, 0); break;
    case 'd': if (obj) obj->mover(0.1, 0, 0); break;
    case 'r': if (obj) obj->mover(0, 0, 0.1); break;
    case 'f': if (obj) obj->mover(0, 0, -0.1); break;

        // Rotação
    case 'i': if (obj) obj->rotacionar(5, 0, 0); break;
    case 'k': if (obj) obj->rotacionar(-5, 0, 0); break;
    case 'j': if (obj) obj->rotacionar(0, 5, 0); break;
    case 'l': if (obj) obj->rotacionar(0, -5, 0); break;
    case 'u': if (obj) obj->rotacionar(0, 0, 5); break;
    case 'h': if (obj) obj->rotacionar(0, 0, -5); break;

        // Escala
    case '+': if (obj) obj->escalar(1.1, 1.1, 1.1); break;
    case '-': if (obj) obj->escalar(0.9, 0.9, 0.9); break;

    case '#':
        if (!objetos.empty() && objetoSelecionado >= 0) {
            cout << "Removendo objeto selecionado: " << objetoSelecionado << endl;
            objetos.erase(objetos.begin() + objetoSelecionado);

            if (objetos.empty()) {
                objetoSelecionado = -1;
            } else {
                objetoSelecionado = objetoSelecionado % objetos.size();
                objetos[objetoSelecionado]->setSelecionado(true);
            }
        } else {
            cout << "Nenhum objeto para remover!" << endl;
        }
        break;

        // Apagar o último objeto do vetor
    case '@':
        if (!objetos.empty()) {
            cout << "Removendo o último objeto." << endl;
            objetos.pop_back();

            // Ajustar o índice da seleção após a remoção
            if (objetos.empty()) {
                objetoSelecionado = -1;
            } else if (objetoSelecionado >= (int)objetos.size()) {
                objetoSelecionado = objetos.size() - 1;
                objetos[objetoSelecionado]->setSelecionado(true);
            }
        } else {
            cout << "Nenhum objeto no vetor para remover!" << endl;
        }
        break;

        // Salvar e carregar objetos
    case '$':
        GerenciadorArquivo::salvarObjetos(objetos);
        break;
    case '%':
        GerenciadorArquivo::carregarObjetos(objetos, objetoSelecionado);
        break;

        // Mover Câmeras
    case 'W': if (cam) cam->moverFrente(); break;
    case 'S': if (cam) cam->moverTras(); break;
    case 'A': if (cam) cam->moverEsquerda(); break;
    case 'D': if (cam) cam->moverDireita(); break;
    case 'Q': if (cam) cam->subir(); break;
    case 'E': if (cam) cam->descer(); break;


        // Alterar Câmeras Distante e Jogo
    case '3': gerenciadorCameras.alternarCamera(0); break;
    case '4': gerenciadorCameras.alternarCamera(1); break;
    case '5': gerenciadorCameras.alternarCamera(2); break;
    case '6': gerenciadorCameras.alternarCamera(3); break;
    case '7': gerenciadorCameras.alternarCamera(4); break;
    case '8': gerenciadorCameras.alternarCamera(5); break;


    default:
        break;
    }
}

int main() {
    cout << "Trabalho 1 - Andson" << endl;
    GUI gui = GUI(800, 600, desenhar, teclado);
    return 0;
}
