//
// Created by Daniel on 06/08/2024.
//

#ifndef ESTRUTURAS_E_UTEIS_H
#define ESTRUTURAS_E_UTEIS_H
#include <vector>
#include <iostream>
#include "grafo_puzzle.h"
#include <cmath>
#include <stack>
#include <iomanip>
#include <chrono>
#include <string>


namespace eeu {

    ////------------------------------------STRUCUT SOLUÇÃO-------------------------------////
    // estrtura que armazena a solução final de um algoritimo de menor caminho como a* por exemplo
    struct solucao {
        long int qtdNosVisitados = 0;
        int qtdMovimentos, aThreadQueEcontrou = 0;
        std::vector<std::vector<int>> caminhoSolucao;
        bool foiInterrompido = false;

        // Construtor padrão
        solucao();

        // Construtor com parâmetros
        solucao(int qtdMovimentos, int qtdNosVisitados, std::vector<std::vector<int>> caminho );

        // Método para imprimir a solução
        void printaSol() const;
        
        // Reconstroi o caminho "caminhoSolucao" a partir do Nó final, tbm define o valor de "qtdMovimentos"
        void reconstruir_caminho_com_grafoPP(gp::No* UltimoEstado);
    };

    solucao instanciarS();

    ////------------------------------------[[[]]]-------------------------------////




    ////------------------------------------FUNÇÕES PARA COMPARAÇÕES COM NO DO GRAFO PP-------------------------------////
    // struct com uma sobrecarga para ser usada no container set com grafoPP comparando os estados
    // vai ser usado na lista fechada
    struct comparaEstados_GP_PP {
        bool operator()(const gp::No* a,  const gp::No* b) const {
            return a->estado < b->estado;
        }
    };


    //usar isso com container de fila
    // using MinHeap = std::priority_queue<No*, std::vector<No*>, ComparePesoAcumulado_Fila>;
    struct ComparePesoAcumulado_Fila {
        bool operator()(const gp::No* lhs, const gp::No* rhs) const {
            return lhs->f > rhs->f;
        }
    };

    //usar isso com container de fila mas para ordenar de forma a prioridade ser os maiores
    // using MaxHeap = std::priority_queue<No*, std::vector<No*>, ComparePesoAcumulado_Fila_Inverso>;
    // vou usar isso no IDA para jogar os filhos do nó na pilha de forma que os menores fiquem no topo
    struct ComparePesoAcumulado_Fila_Inverso {
        bool operator()(const gp::No* lhs, const gp::No* rhs) const {
            return lhs->f < rhs->f;
        }
    };

    ////------------------------------------[[[]]]-------------------------------////




    ////------------------------------------GERAR MATRIZ MOVIMENTOS POSSIVEIS-------------------------------////
    /// ////------------------------------------PRINTAR MATRIZ VEC VEC INT-------------------------------////
    /// /// ////------------------------------------PRINTAR VETOR VEC INT-------------------------------////
    /// /// /// ////------------------------------------Calcular DIM-------------------------------------////
    ///  /// /// /// /// ////--------------------------ENCONTRA O INDICE DO 0 NO VETOR------////
    ///  /// /// /// /// ////---------------------------------MOVIMENTAR O ZERO------------////
    ///   /// /// /// /// /// /// ////------------------------------Printar vec vec int como matriz------------////
     /// /// // // // // ///////------------------------------saida vetor de matriz para HTML------------////

       // função que gera a matriz de filhos movimentos para o espaço vazio dado um indice
    std::vector<std::vector<int>>gerarMatrizMovimentosPossiveis(int dim);

        // função para printar vector de vector de int MATRIZ
    void printarVecVecInt(const std::vector<std::vector<int>>& matriz);

        // função para printar vector  int Linha
    void printarVecInt(const std::vector<int>& vetor);

        // função pra calcular dimensão passando um vetor com n elementos , dim = raiz de n
    int calcular_dim(const std::vector<int>& entrada);

        // função para encontrar o indice que esta 0 de um estado de puzzle
    int encontra_indice_do_ZERO(const std::vector<int>& estado);

        // gera um vetor com o movimento que o espaço vazio deve fazer, troca de lugar o zero com o destino
    std::vector<int> fazer_movimento_do_ZERO(const std::vector<int>& estadoAtual, int posi_ZERO, int posi_Destino);

        // printa como tabuleiro matriz cada estado do puzzle
    void print_como_matriz_Puzzle_Passos(const std::vector<std::vector<int>>& vetor_de_vetor_int, int dim);

        // gerar uma saida com colchetes para o padrao de matriz do html com javascript
    std::string saida_passos_para_html(const std::vector<std::vector<int>>& vetor_de_vetor_int, int dim);


    ////------------------------------------[[[]]]-------------------------------////


    ////------------------------------------PRINTAR TEMPO DE FORMA PRECISA-------------------------------////

        // função pra imprimir de forma mais compreensivel os tempos // horas , minutos , segundos , milesegundos ...
    void imprimir_duracao(const std::chrono::milliseconds& duracao);

    ////------------------------------------[[[]]]-------------------------------////


    ////------------------------------------FUNÇÕES PARA VERIFICAR SE O PUZZLE-------------------------------////
    ////--------------------------------------------É SOLUCIONAVEL-------------------------------////
    ////------------------------------------a função final é a "ehSolucionavel() "-------------------------------////

    // Função para contar o número de inversões no puzzle
    int contarInversoes(std::vector<int> &estado);

    // Função para encontrar a linha (a partir de baixo) onde o espaço vazio está localizado
    int encontrarLinhaVazia(std::vector<int> &estado, int dim);

    // Função para verificar se dois puzzle tem a mesma paridade
    bool temMesmaParidade(int inversoes1, int inversoes2, int linhaVazia1, int linhaVazia2, int dim);

    // Função para verificar se o puzzle inicial pode ser transformado no puzzle de destino
    bool ehSolucionavel(std::vector<int> &inicial, std::vector<int> &destino, int dim);

}
#endif //ESTRUTURAS_E_UTEIS_H
