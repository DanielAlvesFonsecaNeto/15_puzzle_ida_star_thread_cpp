//
// Created by Daniel on 05/08/2024.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>


namespace gp {
    using namespace std;

////-------------------------------------GRAFO COM STRUCT NÓS E PONTEIROS---------------------------////

    struct No {
        std::vector<int> estado;
        int g, h, f; 
        No* pai;
        std::vector<No*> filhos;

        No(const std::vector<int>& estado, int g, int h, No* pai = nullptr);//construtor

        ~No(); // Destrutor
    };


    class grafoPP {
    private:
        No* raiz;

    public:
        grafoPP(No* RAIZ);

        No* referenciaRaiz();

        void AddFilho(No* pai, No* filho);

        void reconstruir(No* NOVA_RAIZ);

        void deletaGrafo();

        void deletaNo(No* no);

        void deletaFilhosDoNo(No* no);

        ~grafoPP();

    };

////-------------------------------------------------------------------------------------------------////


} // namespace gp

#endif // GRAPH_H
