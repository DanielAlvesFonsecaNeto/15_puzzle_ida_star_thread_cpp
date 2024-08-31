//
// Created by Daniel on 06/08/2024.
//

#include "npuzzle.h"
#include "entradas_PUZZLE.h"

using namespace npuzzle;
using namespace std;

int main()
{
    /*
      para usar no construtor de um puzzle,
      colocar no parametro do construtor :  Inicio do puzzle, Destino do puzzle, heuristica pra usar, algoritimo de busca

      para os tipo de heuristica :
            md = manhattan distance
            mdpc = manhattan distance com pré calculo
            cl = conflitos lineares, é bem ruim sozinha
            clmd = conflitos lineares + manhatan distance pc
            0 =  = tudo 0 (qualquer entrada diferente das de cima coloca essa heuristica no lugar)

            "queria implementar a walkin distance, mas é muito DIFICIL de entender"

      para os tipos de algoritimos de busca :
            a* = A*
            ida* = IDA*
            ida*2 - IDA* 2 threads , vai consumir 2 nucleos de processador 
                  (resultado pode talvez ser melhor que ida caso seja encontrado pela thread 2 )
            "qualquer valor" = A* (padrão)
     */

    //-------------------------- entradas de teste -----------------------
    // 3x3
    vector<int> inicio3x3 = {8, 6, 7, 2, 5, 4, 3, 0, 1};
    vector<int> destino3x3 = {1, 2, 3, 4, 5, 6, 7, 8, 0};

    // 4x4
    vector<int> inicio4x4 = {5, 12, 10, 7, 15, 11, 14, 0, 8, 2, 1, 13, 3, 4, 9, 6};
    vector<int> destino4x4 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    //---------------------------------------------------------------------


    puzzle p(inicio3x3,destino3x3,"md","a*"); // exemplo 3x3 com a*  ~~ aproximandamente 1 segundo , 31 movimentos
    //p.printaSolucao();       // descomente para printar o passo a passo e estatisticas
    //p.printaStatisticas();   // descomente para printar estatisticas(apenas)
    //p.geraSaidaPraHTML("pasta","saida1");   // gera o passo a passo em um html VISUAL

    puzzle p2(inicio4x4,destino4x4,"mdpc","ida*"); // exemplo 4x4 com ida* ~~ aproximandamente 36 segundos , 56 movimentos
    //p2.printaSolucao();       // descomente para printar o passo a passo e estatisticas
    //p2.printaStatisticas();   // descomente para printar estatisticas(apenas)
    //p2.geraSaidaPraHTML("pasta","saida2");   // gera o passo a passo em um html VISUAL

    puzzle p3(inicio4x4,destino4x4,"mdpc","ida*2"); // exemplo 4x4 com ida* com 2 threads ~~ aproximandamente 27 segundos , 56 movimentos
    //p3.printaSolucao();       // descomente para printar o passo a passo e estatisticas
    //p3.printaStatisticas();   // descomente para printar estatisticas(apenas)
    //p3.geraSaidaPraHTML("pasta","saida3");   // gera o passo a passo em um html VISUAL

    puzzle p4(inicio4x4,destino4x4,"clmd","ida*2"); // exemplo 4x4 com ida* com 2 threads e clmd ~~ aproximandamente 9 segundos , 56 movimentos
    //p4.printaSolucao();       // descomente para printar o passo a passo e estatisticas
    //p4.printaStatisticas();   // descomente para printar estatisticas(apenas)
    //p4.geraSaidaPraHTML("pasta","saida4");   // gera o passo a passo em um html VISUAL



    /* !! descomente algumas das linhas a baixo para receber apenas estatisticas de varias entradas,
       são mais de 100 entradas  para cada um dos "iniciarSolucaoes" */


    //entrPZ::iniciarSolucoe8p("md","a*");       // todas as entradas de 8p com A*
    //entrPZ::iniciarSolucoe8p("md","ida*");     // todas as entradas de 8p mas resolvendo com IDA*
    //entrPZ::iniciarSolucoe15p("clmd","ida*2"); // todas as entradas de 15p mas resolvendo com IDA* com duas threads e pre processamento na heuristica + conflito lineares
    
    return 0;
}


