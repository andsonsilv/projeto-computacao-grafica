#include "bib/teclado.h"
#include <iostream>
#include <GL/glut.h>
#include <gui.h>
#include "objetos/arvoresimples.h"
#include "objetos/arvorecomplexa.h"

using namespace std;

void Teclado::ativarTeclado(unsigned char tecla, int x, int y,
                            std::vector<std::unique_ptr<Objeto>>& objetos,
                            int& objetoSelecionado,
                            GerenciadorCameras& gerenciadorCameras, bool& mostrarTodosEixos) {
    GUI::keyInit(tecla, x, y);

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

        // Translação
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

        // Apaga o objeto selecionado do vetor
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

    case 'L': // Pressionar 'L' alterna a visibilidade da luz
        GUI::ocultarLuz = !GUI::ocultarLuz;
        cout << "Luz " << (GUI::ocultarLuz ? "ocultada" : "visível") << endl;
        break;

    default:
        break;
    }
}
