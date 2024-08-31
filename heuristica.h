//
// Created by Daniel on 02/08/2024.
//

#ifndef HEURISTICA_H
#define HEURISTICA_H
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;
namespace heu {



    //interface heuristica
    class heuristica{
        protected:
        bool preparado = false;

        public:
        virtual int calcularH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim) = 0;
        virtual void prepararH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim) = 0;
        virtual ~heuristica() {}  // Destruidor virtual para permitir a destruição correta das classes derivadas
    };


    //uma heuristica que implenta a interface - distancia de manhatam
    class heuristica_manhatanDistance : public heuristica{
    public:
        int calcularH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim)override;
        void prepararH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim)override;
    };


    //uma heuristica que implenta a interface - distancia de manhatam com pré calculo
    class heuristica_manhatanDistancePC : public heuristica{
    private:
        std::vector<std::vector<int>> MatrizPC; // matriz de pre computagem
    public:
        int calcularH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim)override;
        void prepararH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim)override;
    };


    //uma heuristica que implenta a interface - conflito linear , ela sozinha é horrivel
    class heuristica_conflitoLinear : public heuristica{
    public:
        int calcularH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim)override;
        void prepararH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim)override;
    };


    //uma heuristica que implenta a interface - conflito linear + manhatan distance
    class heuristica_CL_MD : public heuristica{
        private:
        std::vector<std::vector<int>> MatrizPC;

    public:
        int calcularH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim)override;
        void prepararH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim)override;
    };

    //Um classe que implenta a heuristica mas da um resultado como 0 sempre
    class heuristica_Tudo0 : public heuristica{
    public:
        int calcularH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim)override;
        void prepararH(const std::vector<int>& atual, const std::vector<int>& destino, const int dim)override;
    };

}
#endif //HEURISTICA_H
