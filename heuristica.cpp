//
// Created by Daniel on 02/08/2024.
//

#include "heuristica.h"

namespace heu
{
    using namespace std;

    /////////////--------------------------------- HEURISTICA MANHATAN DISTANCE--------------------------------------------------- /////////

    // distancia de manhtan , heuristica que server para qualquer start(atual) e qualquer destino
    int heuristica_manhatanDistance::calcularH(const std::vector<int> &atual, const std::vector<int> &destino, const int dim)
    {
        int soma = 0;

        for (int i = 0; i < atual.size(); i++)
        {
            if (atual[i] != 0)
            {
                // Encontra a posição do valor atual no estado final
                int posFinal;
                for (int j = 0; j < destino.size(); j++)
                {
                    if (destino[j] == atual[i])
                    {
                        posFinal = j;
                        break;
                    }
                }

                // Calcula a posição (linha, coluna) no grid atual
                int linhaAtual = i / dim;
                int colunaAtual = i % dim;

                // Calcula a posição (linha, coluna) no grid final
                int linhaFinal = posFinal / dim;
                int colunaFinal = posFinal % dim;

                // Soma a distância de Manhattan para este bloco
                soma += std::abs(linhaAtual - linhaFinal) + std::abs(colunaAtual - colunaFinal);
            }
        }

        return soma;
    }

    void heuristica_manhatanDistance::prepararH(const std::vector<int> &atual, const std::vector<int> &destino, const int dim) {}

    /////////////--------------------------------- [~~~~~~~~~~~~~~~~~~]--------------------------------------------------- /////////

    /////////////--------------------------------- HEURISTICA MANHATAN DISTANCE com pré calculo de matriz --------------------------------------------------- /////////

    // manhatan distance com pré calculo de todas as distancia para agilizar retorna durante interações
    int heuristica_manhatanDistancePC::calcularH(const std::vector<int> &atual, const std::vector<int> &destino, const int dim)
    {
        if (!preparado)
        {
            prepararH(atual, destino, dim);
        }

        int soma = 0;
        for (int i = 0; i < destino.size(); i++)
        {
            if (atual[i] != 0)
            {
                soma = soma + MatrizPC[i][atual[i]];
            }
        }
        return soma;
    }

    // gasta um tempo inicial para criar uma matriz para ter acesso direto a informção sem precisar ficar recalculando
    void heuristica_manhatanDistancePC::prepararH(const std::vector<int> &atual, const std::vector<int> &destino, const int dim)
    {

        int valor;
        MatrizPC.clear();

        std::vector<int> auxiliar(destino.size(), 0); // define o tamanho do vetor auxiliar

        for (size_t i = 0; i < destino.size(); ++i)
        {
            auxiliar[destino[i]] = i;
        }

        for (int i = 0; i < destino.size(); i++)
        {
            std::vector<int> atual;
            for (int j = 0; j < destino.size(); j++)
            {
                atual.push_back((abs((i / dim) - (auxiliar[j] / dim)) + abs((i % dim) - (auxiliar[j] % dim))));
            }
            MatrizPC.push_back(atual);
        }

        preparado = true;
    }

    /////////////--------------------------------- [~~~~~~~~~~~~~~~~~~]--------------------------------------------------- /////////

    /////////////--------------------------------- HEURISTICA conflito linear--------------------------------------- /////////
    int heuristica_conflitoLinear::calcularH(const std::vector<int> &atual, const std::vector<int> &destino, int dim)
    {
        int conflitos = 0;

        // Verificar conflitos em linhas
        for (int linha = 0; linha < dim; ++linha)
        {
            for (int i = 0; i < dim; ++i)
            {
                for (int j = i + 1; j < dim; ++j)
                {
                    int bloco1 = atual[linha * dim + i];
                    int bloco2 = atual[linha * dim + j];

                    if (bloco1 != 0 && bloco2 != 0)
                    {
                        int pos1Final = std::find(destino.begin(), destino.end(), bloco1) - destino.begin();
                        int pos2Final = std::find(destino.begin(), destino.end(), bloco2) - destino.begin();

                        if (pos1Final / dim == linha && pos2Final / dim == linha && pos1Final > pos2Final)
                        {
                            conflitos += 2;
                        }
                    }
                }
            }
        }

        // Verificar conflitos em colunas
        for (int coluna = 0; coluna < dim; ++coluna)
        {
            for (int i = 0; i < dim; ++i)
            {
                for (int j = i + 1; j < dim; ++j)
                {
                    int bloco1 = atual[i * dim + coluna];
                    int bloco2 = atual[j * dim + coluna];

                    if (bloco1 != 0 && bloco2 != 0)
                    {
                        int pos1Final = std::find(destino.begin(), destino.end(), bloco1) - destino.begin();
                        int pos2Final = std::find(destino.begin(), destino.end(), bloco2) - destino.begin();

                        if (pos1Final % dim == coluna && pos2Final % dim == coluna && pos1Final > pos2Final)
                        {
                            conflitos += 2;
                        }
                    }
                }
            }
        }

        return conflitos;
    }

    void heuristica_conflitoLinear::prepararH(const std::vector<int> &atual, const std::vector<int> &destino, const int dim) {}

    /////////////--------------------------------- [~~~~~~~~~~~~~~~~~~]--------------------------------------------------- /////////

    /////////////--------------------------------- HEURISTICA conflito linear + manhatan distance -------------------------------- /////////

    int heuristica_CL_MD::calcularH(const std::vector<int> &atual, const std::vector<int> &destino, const int dim)
    {
        if (!preparado)
        {
            prepararH(atual, destino, dim);
        }

        int soma = 0;
        for (int i = 0; i < destino.size(); i++)
        {
            if (atual[i] != 0)
            {
                soma = soma + MatrizPC[i][atual[i]];
            }
        }

        int conflitos = 0;

        // Verificar conflitos em linhas
        for (int linha = 0; linha < dim; ++linha)
        {
            for (int i = 0; i < dim; ++i)
            {
                for (int j = i + 1; j < dim; ++j)
                {
                    int bloco1 = atual[linha * dim + i];
                    int bloco2 = atual[linha * dim + j];

                    if (bloco1 != 0 && bloco2 != 0)
                    {
                        int pos1Final = std::find(destino.begin(), destino.end(), bloco1) - destino.begin();
                        int pos2Final = std::find(destino.begin(), destino.end(), bloco2) - destino.begin();

                        if (pos1Final / dim == linha && pos2Final / dim == linha && pos1Final > pos2Final)
                        {
                            conflitos += 2;
                        }
                    }
                }
            }
        }

        // Verificar conflitos em colunas
        for (int coluna = 0; coluna < dim; ++coluna)
        {
            for (int i = 0; i < dim; ++i)
            {
                for (int j = i + 1; j < dim; ++j)
                {
                    int bloco1 = atual[i * dim + coluna];
                    int bloco2 = atual[j * dim + coluna];

                    if (bloco1 != 0 && bloco2 != 0)
                    {
                        int pos1Final = std::find(destino.begin(), destino.end(), bloco1) - destino.begin();
                        int pos2Final = std::find(destino.begin(), destino.end(), bloco2) - destino.begin();

                        if (pos1Final % dim == coluna && pos2Final % dim == coluna && pos1Final > pos2Final)
                        {
                            conflitos += 2;
                        }
                    }
                }
            }
        }

        return soma + conflitos;
    }

    void heuristica_CL_MD::prepararH(const std::vector<int> &atual, const std::vector<int> &destino, const int dim)
    {

        int valor;
        MatrizPC.clear();

        std::vector<int> auxiliar(destino.size(), 0); // define o tamanho do vetor auxiliar

        for (size_t i = 0; i < destino.size(); ++i)
        {
            auxiliar[destino[i]] = i;
        }

        for (int i = 0; i < destino.size(); i++)
        {
            std::vector<int> atual;
            for (int j = 0; j < destino.size(); j++)
            {
                atual.push_back((abs((i / dim) - (auxiliar[j] / dim)) + abs((i % dim) - (auxiliar[j] % dim))));
            }
            MatrizPC.push_back(atual);
        }

        preparado = true;
    }

    /////////////--------------------------------- [~~~~~~~~~~~~~~~~~~]--------------------------------------------------- /////////


    /////////////--------------------------------- HEURISTICA TUDO 0--------------------------------------------------- /////////

    // apenas retorna o valor 0
    int heuristica_Tudo0::calcularH(const std::vector<int> &atual, const std::vector<int> &destino, const int dim)
    {
        return 0;
    }

    void heuristica_Tudo0::prepararH(const std::vector<int> &atual, const std::vector<int> &destino, const int dim) {}
    /////////////--------------------------------- [~~~~~~~~~~~~~~~~~~]--------------------------------------------------- /////////
}
