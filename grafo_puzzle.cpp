//
// Created by Daniel on 05/08/2024.
//

#include "grafo_puzzle.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace gp {
    ////-------------------------------------GRAFO COM STRUCT NÓS E PONTEIROS---------------------------////

    // Construtor do No
    No::No(const std::vector<int>& estado, int g, int h, No* pai)
        : estado(estado), g(g), h(h), f(g + h), pai(pai) { }

    // Destrutor do No
    No::~No() {
        for (No* filho : filhos) {
            //std::cout<<"a ";
            delete filho; // Deletar os filhos, deve chamar o destrutor recursivamente
        }
    }

    // Implementação do construtor da classe grafoPP
    grafoPP::grafoPP(No* RAIZ) : raiz(RAIZ) {}

    // Implementação da função referenciaRaiz
    No* grafoPP::referenciaRaiz() {
        return raiz;
    }

    void grafoPP::AddFilho(No* pai, No* filho) {
        if (pai != nullptr && filho != nullptr) {
            pai->filhos.push_back(filho); // Adiciona o filho ao vetor de filhos do pai
            filho->pai = pai; // Define o pai do filho
        }
    }

    // reconstroi o grafo com nova raiz No
    void grafoPP::grafoPP::reconstruir(No* NOVA_RAIZ) {
        if (raiz != nullptr) {
            delete raiz; // Deletar a raiz antiga, o destrutor irá lidar com a destruição dos filhos
        }
        raiz = NOVA_RAIZ;
    }

    void grafoPP::deletaGrafo() {
        reconstruir(nullptr);
    }

    void grafoPP::deletaNo(No* no) {
        if (no == nullptr) return;
        // Se o nó tem um pai, remover o nó da lista de filhos do pai
        if (no->pai != nullptr) {
            auto& filhosDoPai = no->pai->filhos; // Tenho que pegar o endereço do vector(referencia) pra modificar diretamente
            auto it = std::find(filhosDoPai.begin(), filhosDoPai.end(), no);
            if (it != filhosDoPai.end()) {
                filhosDoPai.erase(it); // Remover o nó da lista de filhos do pai
            }
        }
        // Deletar o nó
        delete no;
    }

    void grafoPP::deletaFilhosDoNo(No* no) {
        if (no == nullptr) return;
        // Iterar sobre todos os filhos do nó e deletar cada um deles
        for (No* filho : no->filhos) {
            delete filho; // O destrutor do nó será chamado, que por sua vez irá deletar os filhos do filho
        }
        // Limpar o vetor de filhos do nó após a exclusão
        no->filhos.clear();
    }


    // Destrutor da classe
    grafoPP::~grafoPP() {
        delete raiz; // O destrutor de grafoPP irá chamar o destrutor do nó, que lida com todos os filhos
    }

    ////-------------------------------------------------------------------------------------------------////
}
// namespace gp