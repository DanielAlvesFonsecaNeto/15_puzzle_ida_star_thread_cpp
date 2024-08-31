//
// Created by Daniel on 06/08/2024.
//

#include "menor_caminho.h"
#include <stack>
#include <queue>
#include <functional>
#include <cstdint>
#include <set>
#include <thread>
#include <mutex>

/*
    Seguinte!!, a diferença das funções busca_thread 1 e 2 é que :
    thread1 :
        ListaFilhosSelecionados_PQ armazena os filhos com menor F por ultimo pra na hora de joga na pilha , os filhos com menor F serem os primeiros da pilha
        -- ComparePesoAcumulado_Fila_Inverso

    thread2 :
        ListaFilhosSelecionados_PQ armazena os filhos com maior F por ultimo pra na hora de joga na pilha , os filhos com maior F serem os primeiros da pilha
        -- ComparePesoAcumulado_Fila

    basicamente essa é a unica diferença :D
 */


namespace menorCam {
    // Função para gerar a solução usando threads
    eeu::solucao ida_star2::gerar(const std::vector<int> &inicio, const std::vector<int> &destino, int dim, heuristica *heuristica_, long tempoLimite, long interacoesLimite)
    {
        grafo1.reconstruir(new gp::No(inicio, 0, heuristica_->calcularH(inicio, destino, dim)));
        grafo2.reconstruir(new gp::No(inicio, 0, heuristica_->calcularH(inicio, destino, dim)));
        eeu::solucao _solucao;

        // Variáveis e mutexes necessários para as funções de busca
        int contador = 0; // Contador compartilhado entre as threads,
        int f_MAX = grafo1.referenciaRaiz()->f;
        std::mutex mtx;   // Mutex para proteger o contador

        // funções lambda para busca_thread_1 e busca_thread_2
        // fiz assim pras threads terem acesso aos parametros da função gerar
        // o auto busca_thread_1 = [this ....] captura os valores

        /////////////////////////////////------------------ thread 1 --------------------------------///////////////////////////////

        auto busca_thread_1 = [this, dim, &contador,&f_MAX, &_solucao, &mtx, inicio, destino, heuristica_, tempoLimite, interacoesLimite]() {
            long int contagemDeNosVisitados = 0;
            int fmax = grafo1.referenciaRaiz()->f; // fmax é igaul o f da raiz

            while (contador==0)
            {
                //std::cout<<"\n_T1 : "<< f_MAX<<" \n";

                fmax = f_MAX;

                // caso não tenha encontrado na interação anterior, reconstroi o grafo (libera memoria) pra proxima iteração
                grafo1.reconstruir(new gp::No(inicio,0,heuristica_->calcularH(inicio,destino,dim)));

                int fpro = INT32_MAX; // fpro seria o proximo fmax a ser tentando na proxima interação, ele começa com o maior valor poossivel de int

                std::stack<gp::No *> Pilha_Aberta;  // Enquanto tiver elementos nessa pilha, continue explorando
                std::stack<gp::No *> Pilha_Caminho; // pilha do caminho da raiz até o nó atual daquela interação, serve de controle para delete de nós sem solução
                std::set<gp::No*, eeu::comparaEstados_GP_PP> ListaFechada_CS; //elementos dessa lista são iguais elmentos da Pilha_Caminho, mas foi feita pra comparar os elementos existentes de Pilha_Caminho


                // insere primeiro a riaz na pilha aberta
                Pilha_Aberta.push(grafo1.referenciaRaiz());

                //enquanto tiver itens na pilha aberta, continue buscando em profundiade
                while (!Pilha_Aberta.empty() && contador ==0)
                {
                    contagemDeNosVisitados++; // contador de vezes que um nó foi acessado como "atual"

                    // atual é o nó que verficamos se é solução ou se tem filhos para entrar nessa interação de profundiade maxima
                    gp::No *atual = Pilha_Aberta.top();
                    // desempilha o nó
                    Pilha_Aberta.pop();

                    if (Pilha_Caminho.empty()) // se for verdade, então a raiz deve ser o nó atual
                    {
                        Pilha_Caminho.push(atual); // coloca o nó atual(raiz) na pilha caminho
                    }
                    else
                    {
                        if (atual->pai == Pilha_Caminho.top()) // se verdade, esse é filho do ultimo que tava na pilha caminho
                        {
                            Pilha_Caminho.push(atual); // coloca o nó atual na pilha caminho
                        }
                        else
                        { // se não era filho, entao tempos que tirar o ultimo nó da pilha caminho
                            grafo1.deletaNo(Pilha_Caminho.top()); // libera memoria
                            ListaFechada_CS.erase(Pilha_Caminho.top()); // tira da lista fechada
                            Pilha_Caminho.pop(); // retira esse endereço de nó

                            while(Pilha_Caminho.top()->filhos.empty()) { // verifica se os nós do topo da pilha tem filhos, se nao tiverem é pq não são solução
                                grafo1.deletaNo(Pilha_Caminho.top()); // libera memoria
                                ListaFechada_CS.erase(Pilha_Caminho.top()); // tira da lista fechada
                                Pilha_Caminho.pop(); // retira esse endereço de nó
                            }

                            Pilha_Caminho.push(atual); // coloca o nó atual na pilha caminho, caso ele não faça parte da solução, ele será removido nas proxima interações
                        }
                    }

                    // verifica se o nó atual é ultimo nó da solução e retorna
                    if (atual->estado == destino)
                    {

                        {   // se algo estiver errado, talvez seja aqui
                            std::lock_guard<std::mutex> lock(mtx); // Bloqueia o mutex
                            if(contador == 0) {
                                contador = 1;
                                _solucao.reconstruir_caminho_com_grafoPP(atual);
                                _solucao.qtdNosVisitados = contagemDeNosVisitados;
                                _solucao.aThreadQueEcontrou = 1;
                                return;
                            }
                        } // O mutex é desbloqueado automaticamente ao sair do escopo

                    }

                    // vou usar essa fila para jogar os maiores filhos mais pro fundo da pilha, para serem visitados depois dos menores
                    using FilaPrioridadeMaiorPeso = std::priority_queue<gp::No *, std::vector<gp::No *>, eeu::ComparePesoAcumulado_Fila_Inverso>;
                    FilaPrioridadeMaiorPeso ListaFilhosSelecionados_PQ; // esse _PQ siginifica que é uma fila de prioridade , no caso os maiores filhos primeiro

                    int indice = eeu::encontra_indice_do_ZERO(atual->estado); // usado para saber proximos movimentos do estado a partir da posição vazia do puzzle
                    int ondeTavaOPai = -1; // só o atual(raiz) deveria ter esse valor = -1
                    if (atual->pai != nullptr)
                    {
                        ondeTavaOPai = eeu::encontra_indice_do_ZERO(atual->pai->estado); // simples, se o pai não é a raiz, eu verifico em qual posição da matriz 8puzzle o pai do nó atual estava
                    }

                    // esse IDA* nem chega a criar os filhos caso eles tenham um F > fmax
                    for (int i = 0; i < movimentoPossiveis[indice].size(); i++)
                    { // para cada possivel movimento a partir do estado atual, repete esse for

                        // contruindo os dados do filho
                        std::vector<int> novoEstado = eeu::fazer_movimento_do_ZERO(atual->estado, indice, movimentoPossiveis[indice][i]);
                        int novoG = atual->g + 1;
                        int novaHeuristica = heuristica_->calcularH(novoEstado, destino, dim);

                        // impede a criação de um estado filho igual o estado do seu avó
                        if(movimentoPossiveis[indice][i] == ondeTavaOPai && ondeTavaOPai != -1) {
                            continue; // skipa valor igual de filho do pai do atual, ele nao repete o estado do seu avô
                        }

                        // verifica se o F do filho vai ser maior que Fmax, se for atualiza o Fpro e skipa esse filho
                        if ((novoG + novaHeuristica) > fmax)
                        {
                            if ((novoG + novaHeuristica) < fpro) // fpro pega o menor F maior que o Fmax de um filho que não foi criado nessa interação
                            {
                                fpro = novoG + novaHeuristica;
                            }
                            continue; // skipa a criação desse filho
                        }
                        // construindo o filho
                        gp::No *novoNo = new gp::No(novoEstado, novoG, novaHeuristica);
                        // colocando novo filho no pai

                        // verifica primeiro se esse estado ja não existe na lista fechada, explorados
                        auto elemento = ListaFechada_CS.find(novoNo);
                        if (elemento != ListaFechada_CS.end()) {
                            grafo1.deletaNo(novoNo);
                            continue; // skipa esse loop de while e tira o elemento da lista aberta
                        }

                        grafo1.AddFilho(atual, novoNo);
                        // colocando na lista aberta esse novo filho
                        ListaFilhosSelecionados_PQ.push(novoNo);
                    }

                    while (!ListaFilhosSelecionados_PQ.empty())
                    {
                        // Remove o elemento do topo da fila de prioridade
                        gp::No *topo = ListaFilhosSelecionados_PQ.top();
                        ListaFilhosSelecionados_PQ.pop();

                        // Adiciona o elemento à pilha
                        Pilha_Aberta.push(topo);
                    }
                }


                {   // se algo estiver errado, talvez seja aqui
                    std::lock_guard<std::mutex> lock(mtx); // Bloqueia o mutex
                    if(f_MAX < fpro) {
                        f_MAX = fpro; // atualiza o fmax pra buscar mais fundo
                    }
                } // O mutex é desbloqueado automaticamente ao sair do escopo
            }
        };

        /////////////////////////////////------------------ FIM thread 1 --------------------------------///////////////////////////////



        /////////////////////////////////------------------ thread 2 --------------------------------///////////////////////////////

        auto busca_thread_2 = [this, dim, &contador,&f_MAX, &_solucao, &mtx, inicio, destino, heuristica_, tempoLimite, interacoesLimite]() {

            long int contagemDeNosVisitados = 0;
            int fmax = grafo2.referenciaRaiz()->f; // fmax é igaul o f da raiz

            while (contador==0)
            {

                //std::cout<<"\n_T2 : "<< f_MAX<<" \n";
                fmax = f_MAX;

                // caso não tenha encontrado na interação anterior, reconstroi o grafo (libera memoria) pra proxima iteração
                grafo2.reconstruir(new gp::No(inicio,0,heuristica_->calcularH(inicio,destino,dim)));

                int fpro = INT32_MAX; // fpro seria o proximo fmax a ser tentando na proxima interação, ele começa com o maior valor poossivel de int

                std::stack<gp::No *> Pilha_Aberta;  // Enquanto tiver elementos nessa pilha, continue explorando
                std::stack<gp::No *> Pilha_Caminho; // pilha do caminho da raiz até o nó atual daquela interação, serve de controle para delete de nós sem solução
                std::set<gp::No*, eeu::comparaEstados_GP_PP> ListaFechada_CS;
                // --> criar lista fechada, elementos dessa lista são igual elmentos da Pilha_Caminho, mas foi feita pra comparar os elementos existentes de Pilha_Caminho ??

                // insere primeiro a riaz na pilha aberta
                Pilha_Aberta.push(grafo2.referenciaRaiz());

                //enquanto tiver itens na pilha aberta, continue buscando em profundiade
                while (!Pilha_Aberta.empty() && contador ==0)
                {
                    contagemDeNosVisitados++; // contador de vezes que um nó foi acessado como "atual"

                    // atual é o nó que verficamos se é solução ou se tem filhos para entrar nessa interação de profundiade maxima
                    gp::No *atual = Pilha_Aberta.top();
                    // desempilha o nó
                    Pilha_Aberta.pop();

                    if (Pilha_Caminho.empty()) // se for verdade, então a raiz deve ser o nó atual
                    {
                        Pilha_Caminho.push(atual); // coloca o nó atual(raiz) na pilha caminho
                    }
                    else
                    {
                        if (atual->pai == Pilha_Caminho.top()) // se verdade, esse é filho do ultimo que tava na pilha caminho
                        {
                            Pilha_Caminho.push(atual); // coloca o nó atual na pilha caminho
                        }
                        else
                        { // se não era filho, entao tempos que tirar o ultimo nó da pilha caminho
                            grafo2.deletaNo(Pilha_Caminho.top()); // libera memoria
                            ListaFechada_CS.erase(Pilha_Caminho.top()); // tira da lista fechada
                            Pilha_Caminho.pop(); // retira esse endereço de nó


                            while(Pilha_Caminho.top()->filhos.empty()) { // verifica se os nós do topo da pilha tem filhos, se nao tiverem é pq não são solução
                                grafo2.deletaNo(Pilha_Caminho.top()); // libera memoria
                                ListaFechada_CS.erase(Pilha_Caminho.top()); // tira da lista fechada
                                Pilha_Caminho.pop(); // retira esse endereço de nó
                            }

                            Pilha_Caminho.push(atual); // coloca o nó atual na pilha caminho, caso ele não faça parte da solução, ele será removido nas proxima interações
                        }
                    }

                    // verifica se o nó atual é ultimo nó da solução e retorna
                    if (atual->estado == destino)
                    {

                        {   // se algo estiver errado, talvez seja aqui
                            std::lock_guard<std::mutex> lock(mtx); // Bloqueia o mutex
                            if(contador == 0) {
                                contador = 2;
                                _solucao.reconstruir_caminho_com_grafoPP(atual);
                                _solucao.qtdNosVisitados = contagemDeNosVisitados;
                                _solucao.aThreadQueEcontrou = 2;
                                return;
                            }
                        } // O mutex é desbloqueado automaticamente ao sair do escopo

                    }

                    // vou usar essa fila para jogar os menores filhos mais pro fundo da pilha, para serem visitados depois dos maiores
                    using FilaPrioridadeMenorPeso = std::priority_queue<gp::No *, std::vector<gp::No *>, eeu::ComparePesoAcumulado_Fila>;
                    FilaPrioridadeMenorPeso ListaFilhosSelecionados_PQ; // esse _PQ siginifica que é uma fila de prioridade , no caso os maiores filhos primeiro

                    int indice = eeu::encontra_indice_do_ZERO(atual->estado); // usado para saber proximos movimentos do estado a partir da posição vazia do puzzle
                    int ondeTavaOPai = -1; // só o atual(raiz) deveria ter esse valor = -1
                    if (atual->pai != nullptr)
                    {
                        ondeTavaOPai = eeu::encontra_indice_do_ZERO(atual->pai->estado); // simples, se o pai não é a raiz, eu verifico em qual posição da matriz 8puzzle o pai do nó atual estava
                    }

                    // esse IDA* nem chega a criar os filhos caso eles tenham um F > fmax
                    for (int i = 0; i < movimentoPossiveis[indice].size(); i++)
                    { // para cada possivel movimento a partir do estado atual, repete esse for

                        // contruindo os dados do filho
                        std::vector<int> novoEstado = eeu::fazer_movimento_do_ZERO(atual->estado, indice, movimentoPossiveis[indice][i]);
                        int novoG = atual->g + 1;
                        int novaHeuristica = heuristica_->calcularH(novoEstado, destino, dim);

                        // impede a criação de um estado filho igual o estado do seu avó
                        if(movimentoPossiveis[indice][i] == ondeTavaOPai && ondeTavaOPai != -1) {
                            continue; // skipa valor igual de filho do pai do atual, ele nao repete o estado do seu avô
                        }

                        // verifica se o F do filho vai ser maior que Fmax, se for atualiza o Fpro e skipa esse filho
                        if ((novoG + novaHeuristica) > fmax)
                        {
                            if ((novoG + novaHeuristica) < fpro) // fpro pega o menor F maior que o Fmax de um filho que não foi criado nessa interação
                            {
                                fpro = novoG + novaHeuristica;
                            }
                            continue; // skipa a criação desse filho
                        }

                        // construindo o filho
                        gp::No *novoNo = new gp::No(novoEstado, novoG, novaHeuristica);
                        // colocando novo filho no pai

                        // verifica primeiro se esse estado ja não existe na lista fechada, explorados
                        auto elemento = ListaFechada_CS.find(novoNo);
                        if (elemento != ListaFechada_CS.end()) {
                            grafo2.deletaNo(novoNo);
                            continue; // skipa esse loop de while e tira o elemento da lista aberta
                        }

                        grafo2.AddFilho(atual, novoNo);
                        // colocando na lista aberta esse novo filho
                        ListaFilhosSelecionados_PQ.push(novoNo);
                    }

                    while (!ListaFilhosSelecionados_PQ.empty())
                    {
                        // Remove o elemento do topo da fila de prioridade
                        gp::No *topo = ListaFilhosSelecionados_PQ.top();
                        ListaFilhosSelecionados_PQ.pop();

                        // Adiciona o elemento à pilha
                        Pilha_Aberta.push(topo);
                    }
                }


                {   // se algo estiver errado, talvez seja aqui
                    std::lock_guard<std::mutex> lock(mtx); // Bloqueia o mutex
                    if(f_MAX < fpro) {
                        f_MAX = fpro; // atualiza o fmax pra buscar mais fundo
                    }
                } // O mutex é desbloqueado automaticamente ao sair do escopo
            }
        };
        /////////////////////////////////------------------ FIM thread 2 --------------------------------///////////////////////////////

        // Inicia as duas threads, passando as funções lambda
        std::thread t1(busca_thread_1);
        std::thread t2(busca_thread_2);

        // Espera as threads terminarem
        t1.join();
        t2.join();

        return _solucao;
    }

    ida_star2::~ida_star2()
    {
        // Implementação do destrutor se necessário
    }
}
// namespace menorCam


