//
// Created by Daniel on 06/08/2024.
//

#include "estruturas_e_uteis.h"

namespace eeu
{

    ////------------------------------------STRUCUT SOLUÇÃO-------------------------------////

    solucao::solucao() : qtdMovimentos(-1), caminhoSolucao() {}

    // Construtor com parâmetros
    solucao::solucao(int qtdMovimentos, int qtdNosVisitados, std::vector<std::vector<int>> caminho)
        : qtdMovimentos(qtdMovimentos), qtdNosVisitados(qtdNosVisitados), caminhoSolucao(caminho) {}

    // Método para imprimir a solução
    void solucao::printaSol() const
    {
        std::cout << "Quantidade de Movimentos: " << qtdMovimentos << "\n";
        std::cout << "Quantidade de Nos visitados: " << qtdNosVisitados << "\n";
        std::cout << "Caminho:\n{";

        if (!caminhoSolucao.empty())
        {
            for (const auto &linha : caminhoSolucao)
            {
                std::cout << "[ ";
                for (int valor : linha)
                {
                    std::cout << valor << ' ';
                }
                std::cout << "]\n";
            }
            std::cout << "}";
        }

        if (foiInterrompido)
        {
            std::cout << "\nFoi interrompido\n";
        }
        else
        {
            std::cout << "\nNao foi interrompido\n";
        }
    }

    // Reconstroi o caminho "caminhoSolucao" a partir do Nó final, tbm define o valor de "qtdMovimentos"
    void solucao::reconstruir_caminho_com_grafoPP(gp::No *UltimoEstado)
    {
        qtdMovimentos = -1; // -1 para ignorar o estado inicial como uma interação, ja que se estado ini = fim , então nao deveria ter nenhuma intereção mas a solução armazena o estado
        std::stack<std::vector<int>> pilha;

        // Empilha os estados do caminho até a raiz
        gp::No *atual = UltimoEstado;
        while (atual != nullptr)
        {
            pilha.push(atual->estado);
            atual = atual->pai;
        }

        // Desempilha os estados e os armazena no vetor de vetores
        while (!pilha.empty())
        {
            caminhoSolucao.push_back(pilha.top());
            pilha.pop();
            qtdMovimentos++;
        }
    }

    solucao instanciarS(){
        solucao _solucao;
        return _solucao;
    }

    ////------------------------------------[[[]]]-------------------------------////

    ////------------------------------------FUNÇÕES PARA COMPARAÇÕES COM NO DO GRAFO PP-------------------------------////
    ////------------------------------------[[[]]]-------------------------------////

    ////------------------------------------GERAR MATRIZ MOVIMENTOS POSSIVEIS-------------------------------////
    /// ////------------------------------------PRINTAR MATRIZ VEC VEC INT-------------------------------////
    /// /// ////------------------------------------PRINTAR VETOR VEC INT-------------------------------////
    /// /// /// ////------------------------------------Calcular DIM-----------------------------------////
    /// /// /// /// ////------------------------------------ENCONTRA O INDICE DO 0 NO VETOR------------////
    /// /// /// /// /// ////---------------------------------MOVIMENTAR O ZERO------------////
    /// /// /// /// /// /// ////------------------------------Printar vec vec int como matriz------------////
    /// /// // // // // ///////------------------------------saida vetor de matriz para HTML------------////

    // função que gera a matriz de filhos movimentos para o espaço vazio dado um indice
    std::vector<std::vector<int>> gerarMatrizMovimentosPossiveis(int dim)
    {
        int tamanho = dim * dim;
        std::vector<std::vector<int>> matriz(tamanho);
        for (int indiceZero = 0; indiceZero < tamanho; indiceZero++)
        {
            int i = indiceZero / dim;
            int j = indiceZero % dim;
            if (i - 1 >= 0)
            {
                matriz[indiceZero].push_back((i - 1) * dim + j);
            }
            if (i + 1 < dim)
            {
                matriz[indiceZero].push_back((i + 1) * dim + j);
            }
            if (j - 1 >= 0)
            {
                matriz[indiceZero].push_back(i * dim + (j - 1));
            }
            if (j + 1 < dim)
            {
                matriz[indiceZero].push_back(i * dim + (j + 1));
            }
        }

        return matriz;
    }

    // função para printar vector de vector de int MATRIZ
    void printarVecVecInt(const std::vector<std::vector<int>> &matriz)
    {
        for (const auto &linha : matriz)
        {
            for (int item : linha)
            {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
    }

    // função para printar vector  int Linha
    void printarVecInt(const std::vector<int> &vetor)
    {
        for (int item : vetor)
        {
            std::cout << item << " ";
        }
        // std::cout << std::endl;
    }

    // função pra calcular dimensão passando um vetor com n elementos , dim = raiz de n
    int calcular_dim(const std::vector<int> &entrada)
    {
        int tamanho = entrada.size();
        int raiz = static_cast<int>(std::sqrt(tamanho));

        if (raiz * raiz == tamanho)
        {
            return raiz;
        }
        else
        {
            return -1; // retorna -1 caso não seja uma raiz perfeita
        }
    }

    // função para encontrar o indice que esta 0 de um estado de puzzle
    int encontra_indice_do_ZERO(const std::vector<int> &estado)
    {
        for (int i = 0; i < estado.size(); i++)
        {
            if (estado[i] == 0)
            {
                return i;
            }
        }
        return 0;
    }

    // gera um vetor com o movimento que o espaço vazio deve fazer, troca de lugar o zero com o destino
    std::vector<int> fazer_movimento_do_ZERO(const std::vector<int> &estadoAtual, int posi_ZERO, int posi_Destino)
    {
        std::vector<int> estadoCalculado = estadoAtual;
        estadoCalculado[posi_ZERO] = estadoAtual[posi_Destino];
        estadoCalculado[posi_Destino] = estadoAtual[posi_ZERO];
        return estadoCalculado;
    }

    // printa como tabuleiro matriz cada estado do puzzle
    void print_como_matriz_Puzzle_Passos(const std::vector<std::vector<int>> &vetor_de_vetor_int, int dim)
    {
        std::cout << std::endl;

        // Calcula o tamanho fixo dos campos para manter o alinhamento
        const int field_width = 4;

        for (const auto &vec : vetor_de_vetor_int)
        {
            for (int i = 0; i < dim; i++)
            {
                std::cout << "_____";
            }
            std::cout << std::endl;
            for (int i = 0; i < dim; ++i)
            {
                for (int j = 0; j < dim; ++j)
                {
                    int valor = vec[i * dim + j];
                    if (valor == 0)
                    {
                        std::cout << std::setw(field_width) << " "; // Imprime um espaço vazio com largura fixa para o valor 0
                    }
                    else
                    {
                        std::cout << "[" << std::setw(2) << valor << "]"; // Imprime o valor dentro de colchetes com largura fixa
                    }
                    std::cout << " ";
                }
                if (i < dim - 1)
                {
                    std::cout << std::endl
                              << std::endl;
                }
            }
            std::cout << std::endl;
            for (int i = 0; i < dim; i++)
            {
                std::cout << "-----";
            }
            std::cout << std::endl
                      << std::endl
                      << std::endl;
        }
    }
    // gerar uma saida com colchetes para o padrao de matriz do html com javascript
    std::string saida_passos_para_html(const std::vector<std::vector<int>> &vetor_de_vetor_int, int dim)
    {
        std::ostringstream oss;
        oss << "[" << std::endl;

        // Itera sobre cada vetor dentro de vetor_de_vetor_int
        for (size_t i = 0; i < vetor_de_vetor_int.size(); ++i)
        {
            oss << "    [";
            for (int j = 0; j < dim; ++j)
            {
                if (j > 0)
                {
                    oss << ", ";
                }
                oss << "[";
                for (int k = 0; k < dim; ++k)
                {
                    oss << vetor_de_vetor_int[i][j * dim + k];
                    if (k < dim - 1)
                    {
                        oss << ", ";
                    }
                }
                oss << "]";
            }
            oss << "]";
            if (i != vetor_de_vetor_int.size() - 1)
            {
                oss << ",";
            }
            oss << std::endl;
        }
        oss << "]\n";
        return oss.str();
    }

    ////------------------------------------[[[]]]-------------------------------////

    ////------------------------------------PRINTAR TEMPO DE FORMA PRECISA-------------------------------////

    // função pra imprimir de forma mais compreensivel os tempos // horas , minutos , segundos , milesegundos ...
    void imprimir_duracao(const std::chrono::milliseconds &duracao)
    {
        // Duração em diferentes unidades
        auto segundos_totais = std::chrono::duration_cast<std::chrono::seconds>(duracao);
        auto milissegundos = std::chrono::duration_cast<std::chrono::milliseconds>(duracao) % 1000;
        auto minutos_totais = std::chrono::duration_cast<std::chrono::minutes>(segundos_totais);
        auto horas_totais = std::chrono::duration_cast<std::chrono::hours>(minutos_totais);

        if (horas_totais.count() > 0)
        {
            // Se a duração for maior que uma hora
            std::cout << horas_totais.count() << " horas "
                      << (minutos_totais.count() % 60) << " minutos "
                      << (segundos_totais.count() % 60) << " segundos "
                      << std::fixed << std::setprecision(3) << (milissegundos.count() / 1000.0) << " ms" << std::endl;
        }
        else if (minutos_totais.count() > 0)
        {
            // Se a duração for maior que um minuto
            std::cout << minutos_totais.count() << " minutos "
                      << (segundos_totais.count() % 60) << " segundos "
                      << std::fixed << std::setprecision(3) << (milissegundos.count() / 1000.0) << " ms" << std::endl;
        }
        else if (segundos_totais.count() > 1)
        {
            // Se a duração for maior que um segundo
            std::cout << segundos_totais.count() << " segundos "
                      << std::fixed << std::setprecision(3) << (milissegundos.count() / 1000.0) << " ms" << std::endl;
        }
        else
        {
            // Duração menor que um segundo
            std::cout << milissegundos.count() << " ms" << std::endl;
        }
    }

    ////------------------------------------[[[]]]-------------------------------////


    ////------------------------------------FUNÇÕES PARA VERIFICAR SE O PUZZLE-------------------------------////
    ////--------------------------------------------É SOLUCIONAVEL-------------------------------////
    ////------------------------------------a função final é a "ehSolucionavel() "-------------------------------////

    // Função para contar o número de inversões no puzzle
    int contarInversoes(std::vector<int> &estado)
    {
        int inversoes = 0;
        for (int i = 0; i < estado.size(); i++)
        {
            if (estado[i] == 0)
                continue; // Ignora o espaço vazio
            for (int j = i + 1; j < estado.size(); j++)
            {
                if (estado[j] == 0)
                    continue;
                if (estado[i] > estado[j])
                    inversoes++;
            }
        }
        return inversoes;
    }

    // Função para encontrar a linha (a partir de baixo) onde o espaço vazio está localizado
    int encontrarLinhaVazia(std::vector<int> &estado, int dim)
    {
        int indiceVazio = 0;
        for (int i = 0; i < estado.size(); i++)
        {
            if (estado[i] == 0)
            {
                indiceVazio = i;
                break;
            }
        }
        return (dim - 1) - (indiceVazio / dim); // Linha a partir do fundo
    }

    // Função para verificar se dois puzzle tem a mesma paridade
    bool temMesmaParidade(int inversoes1, int inversoes2, int linhaVazia1, int linhaVazia2, int dim)
    {
        if (dim % 2 != 0)
        {
            return (inversoes1 % 2 == inversoes2 % 2);
        }
        else
        {
            return (inversoes1 % 2 == inversoes2 % 2) == (linhaVazia1 % 2 == linhaVazia2 % 2);
        }
    }

    // Função para verificar se o puzzle inicial pode ser transformado no puzzle de destino
    bool ehSolucionavel(std::vector<int> &inicial, std::vector<int> &destino, int dim)
    {
        if(inicial.size() != destino.size()){
            return false;
        }

        int inversoesInicial = contarInversoes(inicial);
        int inversoesDestino = contarInversoes(destino);

        int linhaVaziaInicial = encontrarLinhaVazia(inicial, dim);
        int linhaVaziaDestino = encontrarLinhaVazia(destino, dim);

        return temMesmaParidade(inversoesInicial, inversoesDestino, linhaVaziaInicial, linhaVaziaDestino, dim);
    }

}