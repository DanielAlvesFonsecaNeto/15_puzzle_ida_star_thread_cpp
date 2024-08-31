//
// Created by Daniel on 06/08/2024.
//

#ifndef MENOR_CAMINHO_H
#define MENOR_CAMINHO_H

#include "estruturas_e_uteis.h"
#include "heuristica.h"
#include "grafo_puzzle.h"
#include <thread>
#include <mutex>

/* nesse arquivo tem o cabeçalhos dos algortimos de busca como a* e ida* e a interface pai menor_caminho
 * novos algortimos de busca serão declarados aqui e implementas em seus respectivos .cpp
 * os arquivos que implentam devem começar com MC_
*/
namespace menorCam {

    using namespace heu;

    // Classe ABSTRATA PAI
    class menor_caminho {
    protected:
        std::vector<int> inicio;
        std::vector<int> destino;
        int dim;

        gp::grafoPP grafo1;
        gp::grafoPP grafo2; // caso precise para mult treahds, Caso venha implementar thread pra ida e vinda

        std::vector<std::vector<int>> movimentoPossiveis; // matriz com os possiveis movimento

    public:
        //!!! !!! caso o uso de thread para limite de tempo não funcione , tirar o cosnt de const heuristica* heuristica_ pode ser a solução
        menor_caminho(const std::vector<int>& inicio, const std::vector<int>& destino, int dim) : inicio(inicio), destino(destino), dim(dim), grafo1(gp::grafoPP(new gp::No(inicio,0,0))), grafo2(gp::grafoPP(new gp::No(destino,0,0))) {
            movimentoPossiveis = eeu::gerarMatrizMovimentosPossiveis(dim);
        };

        virtual eeu::solucao gerar(const std::vector<int>& inicio, const std::vector<int>& destino, int dim, heuristica* heuristica_, long tempoLimite, long interacoesLimite) =0; // função que retorna uma solucao, definido esse tipo em estruturas e uteis
        virtual ~menor_caminho() {
            grafo1.deletaGrafo();
            grafo2.deletaGrafo();
        };
    };



    // Classe que implementa a INTERFACE menor_caminho :: A estrela , implmentação ta no arquivo MC_a_star.cpp
    class a_star : public menor_caminho {
    public:
        a_star(const std::vector<int>& inicio, const std::vector<int>& destino, int dim) : menor_caminho(inicio, destino, dim){}
        eeu::solucao gerar(const std::vector<int>& inicio, const std::vector<int>& destino, int dim, heuristica* heuristica_, long tempoLimite, long interacoesLimite) override;
        ~a_star() override;
    };



    // Classe que implementa a INTERFACE menor_caminho :: IDA estrela , implmentação ta no arquivo MC_ida_star.cpp
    class ida_star : public menor_caminho {
    public:
        ida_star(const std::vector<int>& inicio, const std::vector<int>& destino, int dim) : menor_caminho(inicio, destino, dim){}
        eeu::solucao gerar(const std::vector<int>& inicio, const std::vector<int>& destino, int dim, heuristica* heuristica_, long tempoLimite, long interacoesLimite) override;
        ~ida_star() override;
    };



    // Classe que implementa a INTERFACE menor_caminho :: IDA estrela 2 , implmentação ta no arquivo MC_ida_star2.cpp
    class ida_star2 : public menor_caminho {
    public:
        ida_star2(const std::vector<int>& inicio, const std::vector<int>& destino, int dim) : menor_caminho(inicio, destino, dim){}
        eeu::solucao gerar(const std::vector<int>& inicio, const std::vector<int>& destino, int dim, heuristica* heuristica_, long tempoLimite, long interacoesLimite) override;

        gp::grafoPP& acessoGrafo1(){return grafo1;}
        gp::grafoPP& acessoGrafo2(){return grafo1;}

        ~ida_star2() override;
    };



} // menorCam
#endif //MENOR_CAMINHO_H
