//
// Created by Daniel on 06/08/2024.
//

#include "menor_caminho.h"
#include <set>
#include <queue>
#include <functional>


namespace menorCam {
    eeu::solucao a_star::gerar(const std::vector<int>& inicio, const std::vector<int>& destino, int dim, heuristica* heuristica_, long tempoLimite, long interacoesLimite) {

        long int contagemDeNosVisitados = 0;

        // reconstruindo o grafo só por garantia, já que o grafo1 é atributo da classe e não vairavel interna da função
        grafo1.reconstruir(new gp::No(inicio,0,heuristica_->calcularH(inicio,destino,dim)));

        // criando lista aberta
        using FilaPrioridadeMenorPeso = std::priority_queue<gp::No*, std::vector<gp::No*>, eeu::ComparePesoAcumulado_Fila>;
        FilaPrioridadeMenorPeso ListaAberta_PQ; // esse PQ siginifica que é uma fila de prioridade

        //criando lista fechada
        // Criando o set com a função de comparação personalizada
        std::set<gp::No*, eeu::comparaEstados_GP_PP> ListaFechada_CS;  // CS = conatainer set

        //colocar o estado inicial na lista aberta , raiz do grafo1
        ListaAberta_PQ.push(grafo1.referenciaRaiz());

        //// repete o while enquanto existe itens na lista aberta
        while(!ListaAberta_PQ.empty()){
            contagemDeNosVisitados ++;

            //pega o elemento com o menor F da lista aberta
            gp::No* atual = ListaAberta_PQ.top();

            // verifica se é o estado final
            if(atual->estado == destino) {
                // criando solução que vai ser retornada
                eeu::solucao _solucao;
                // inserindo o caminho da solução do nó inicial até o final
                _solucao.reconstruir_caminho_com_grafoPP(atual);

                // apagando a lista aberta pra tirar os endereços de memoria que vão ser liberados
                while (!ListaAberta_PQ.empty()) {
                    ListaAberta_PQ.pop();
                }
                // apagando a lista fechada pra tirar os endereços de memoria que vão ser liberados
                ListaFechada_CS.clear();
                // deletando grafo pra liberar memoria
                grafo1.deletaGrafo();
                
                _solucao.qtdNosVisitados = contagemDeNosVisitados;

                return _solucao;
            }

            // verifica primeiro se esse estado ja não existe na lista fechada, explorados
            auto elemento = ListaFechada_CS.find(atual);
            if (elemento != ListaFechada_CS.end()) {
                //caso exista esse estado na lista fechada
                ListaAberta_PQ.pop();
                continue; // skipa esse loop de while e tira o elemento da lista aberta
            }
            //joga esse elemento pra lista fechada
            ListaFechada_CS.insert(atual);
            // e tira ele da lista aberta
            ListaAberta_PQ.pop();

            //// -----------------------------------------criando os filhos do no atual ------------------------------------////

            int indice = eeu::encontra_indice_do_ZERO(atual->estado);
            int ondeTavaOPai = -1; // se tudo estiver certo, nunca vai ser -1 , se for o codigo deve quebrar "esepro que quebre nesse caso"
            if(atual->pai != nullptr) {
                ondeTavaOPai = eeu::encontra_indice_do_ZERO(atual->pai->estado); // simples, se o pai não é a raiz, eu verifico em qual posição da matriz 8puzzle o pai do nó atual estava
            }

            for(int i = 0; i < movimentoPossiveis[indice].size(); i++) { // para cada possivel movimento a partir do estado atual, repete esse for
                /**
                 * vou explicar esses  2 IFs, o 1ª IF ta verificando se o estado atual tem um pai, se não tiver é pq é raiz, ai ele gera os filhos normalmente
                 * o 2ª IF é caso não seja a raiz, ele serve pra não gerar o estado pai como um possivel filho, vai ser bom pra reduzir repetição de estado pra ñ entrar na verificação lista fechada
                 */
                if(atual->pai == nullptr) { // pro caso de ser a raiz
                    // contruindo os dados do filho
                    std::vector<int> novoEstado = eeu::fazer_movimento_do_ZERO(atual->estado,indice,movimentoPossiveis[indice][i]);
                    int novoG = atual->g + 1;
                    int novaHeuristica = heuristica_->calcularH(novoEstado,destino,dim);
                    // construindo o filho
                    gp::No* novoNo = new gp::No(novoEstado, novoG, novaHeuristica);
                    //colocando novo filho no pai
                    grafo1.AddFilho(atual,novoNo);
                    //colocando na lista aberta esse novo filho
                    ListaAberta_PQ.push(novoNo);
                }
                else if(movimentoPossiveis[indice][i] != ondeTavaOPai ) { // skip a posição que tava o pai
                    // contruindo os dados do filho
                    std::vector<int> novoEstado = eeu::fazer_movimento_do_ZERO(atual->estado,indice,movimentoPossiveis[indice][i]);
                    int novoG = atual->g + 1;
                    int novaHeuristica = heuristica_->calcularH(novoEstado,destino,dim);
                    // construindo o filho
                    gp::No* novoNo = new gp::No(novoEstado, novoG, novaHeuristica);
                    //colocando novo filho no pai
                    grafo1.AddFilho(atual,novoNo);
                    //colocando na lista aberta esse novo filho
                    ListaAberta_PQ.push(novoNo);
                }
            }//// ------------------------------------ fim do FOR de criar filhos----------------------------------////
        } //// fim do WHILE

        // caso tenha acabado a lista aberta, é pq ja explorou tudo oq dava
        std::cout<<"\n\nnao encontrou nada, talvez pecorreu toda a lista aberta ate acabar\n\n";
        eeu::solucao _solucao;
        _solucao.foiInterrompido = true;

        ListaFechada_CS.clear();
        grafo1.deletaGrafo();

        _solucao.qtdNosVisitados = contagemDeNosVisitados;
        return _solucao;
    }

    a_star::~a_star() {
    }
}
