//
// Created by Daniel on 01/08/2024.
//

#ifndef NPUZZLE_H
#define NPUZZLE_H

#include <vector>
#include <string>
#include "heuristica.h"
#include "menor_caminho.h"
#include "estruturas_e_uteis.h"
#include "gerarHTML.h"




namespace npuzzle {


    using namespace std;
    using namespace menorCam;
    using namespace heu;


class puzzle {
private:
    heuristica* heuristica_;  // heuristica que vai ser usada - padrao : heuristica_Tudo0
    menor_caminho* menor_caminho_; // bsuca que vai ser usada - padrão : a_star

    vector<int> inicio; // estado inicial do puzzle
    vector<int> destino; // estado destino do puzzle
    int dim;

    string heuristicaUsada = "";
    string algoritmoBuscaUsado = "";

    eeu::solucao _solucao;
    bool solucionado = false;


public:

    // lembrar de deletar heuristica e menor_caminho antes de recontruir o obj dessa classe
    puzzle(const vector<int>& inicio,const vector<int>& destino,const string& tipoHeuristica, const string& tipoBuscaMenorCaminho); // construtor

    void RECONSTRUIR(const vector<int>& Inicio,const vector<int>& Destino,const string& TipoHeuristica, const string& TipoBuscaMenorCaminho);

    void buscarNOPRINT();

    void printaSolucao();

    void printaStatisticas();

    bool verificaSeEhSolucionavel();

    void geraSaidaPraHTML(const std::string& pasta, const std::string& nomeSaida);

    void printaValorHEUpassos();


    ~puzzle();
};


} // npuzzle namespace
#endif //NPUZZLE_H
