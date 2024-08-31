//
// Created by Daniel on 01/08/2024.
//

#include "npuzzle.h"
#include <iostream>
#include <chrono>

#include <fstream>
#include <sys/stat.h> // Para funções de manipulação de diretórios
#include <cstring>    // Para funções de manipulação de strings
#include <iostream>   // Para saída de erros

#ifdef _WIN32
#include <direct.h> // Para _mkdir no Windows
#else
#include <unistd.h> // Para POSIX
#endif

namespace npuzzle
{ // por incrivel que pareça, essa chave '{' ta certa ai no lugar dela

    using namespace eeu;

    // Construtor
    puzzle::puzzle(const vector<int> &inicio, const vector<int> &destino, const string &tipoHeuristica, const string &tipoBuscaMenorCaminho) : inicio(inicio), destino(destino)
    {

        heuristica_ = nullptr;
        menor_caminho_ = nullptr;
        RECONSTRUIR(inicio, destino, tipoHeuristica, tipoBuscaMenorCaminho);
        _solucao = instanciarS();
        
    }

    // reconstruir , usado no construtor
    void puzzle::RECONSTRUIR(const vector<int> &Inicio, const vector<int> &Destino, const string &TipoHeuristica, const string &TipoBuscaMenorCaminho)
    {

        if (heuristica_ != nullptr)
        {
            delete heuristica_;
        }
        if (menor_caminho_ != nullptr)
        {
            delete menor_caminho_;
        }

        inicio = Inicio;
        destino = Destino;
        heuristica_ = nullptr;
        menor_caminho_ = nullptr;
        solucionado = false;

        dim = calcular_dim(inicio);

        // heuristica
        if (TipoHeuristica == "md")
        {
            heuristica_ = new heuristica_manhatanDistance();
            heuristicaUsada = "manhattan distance";
        }
        else if (TipoHeuristica == "mdpc")
        {
            heuristica_ = new heuristica_manhatanDistancePC();
            heuristicaUsada = "manhattan distance pre processamento";
        }
        else if (TipoHeuristica == "cl")
        {
            heuristica_ = new heuristica_conflitoLinear();
            heuristicaUsada = "conflito linear";
        }
        else if (TipoHeuristica == "clmd")
        {
            heuristica_ = new heuristica_CL_MD();
            heuristicaUsada = "conflito linear + manhatan distance";
        }
        else
        { // padrão é tudo 0
            heuristica_ = new heuristica_Tudo0();
            heuristicaUsada = "sem heuristica (dijkstra/caso a*)";
        }

        // algoritimo de busca
        if (TipoBuscaMenorCaminho == "a*")
        {
            menor_caminho_ = new a_star(inicio, destino, dim);
            algoritmoBuscaUsado = "A*";
        }
        else if (TipoBuscaMenorCaminho == "ida*")
        {
            menor_caminho_ = new ida_star(inicio, destino, dim);
            algoritmoBuscaUsado = "IDA*";
        }
        else if (TipoBuscaMenorCaminho == "ida*2")
        {
            menor_caminho_ = new ida_star2(inicio, destino, dim);
            algoritmoBuscaUsado = "IDA* 2 threads";
        }
        else
        { // padrão é A*
            menor_caminho_ = new a_star(inicio, destino, dim);
            algoritmoBuscaUsado = "A*";
        }
    }

    // faz a busca sem printar nada
    void puzzle::buscarNOPRINT()
    {
        solucao _solucao = menor_caminho_->gerar(inicio, destino, dim, heuristica_, 0, 0);
        solucionado = true;
    }

    // metodo que resolve e printa solução
    void puzzle::printaSolucao()
    {
        if (verificaSeEhSolucionavel())
        {
            auto START = std::chrono::high_resolution_clock::now();

            cout << "\n"
                 << algoritmoBuscaUsado << "  " << heuristicaUsada << endl
                 << endl;

            if(!solucionado){
                _solucao = menor_caminho_->gerar(inicio, destino, dim, heuristica_, 0, 0);
                solucionado = true;
            }
            print_como_matriz_Puzzle_Passos(_solucao.caminhoSolucao, dim);

            cout << "\nalgoritimo de busca usado : " << algoritmoBuscaUsado << endl;
            cout << "\nheuristica usada : " << heuristicaUsada << endl;
            cout << "\nquantidade de movimentos : " << _solucao.qtdMovimentos << endl;
            cout << "\nquantidade de nos visitados : " << _solucao.qtdNosVisitados << "\n";
            if (_solucao.aThreadQueEcontrou != 0)
            {
                cout << "econtrando pela thread : " << _solucao.aThreadQueEcontrou << endl
                     << endl;
            }

            auto END = std::chrono::high_resolution_clock::now();
            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(END - START);
            cout << "tempo de execucao : ";
            imprimir_duracao(duracao);
        }
        else
        {
            cout << "\nEsse Puzzle nao eh solucionavel !!! ";
            cout << "\n~estado INICIAL : ";
            printarVecInt(inicio);
            cout << "~\n~estado FINAL   : ";
            printarVecInt(destino);
            cout << "~" << endl
                 << endl;
        }
    }

    // verificar se tem solução as entradas do puzzle
    bool puzzle::verificaSeEhSolucionavel()
    {
        return eeu::ehSolucionavel(inicio, destino, dim);
    }

    // printa apenas as estatisticas sem o passo a passo
    void puzzle::printaStatisticas()
    {
        if (verificaSeEhSolucionavel())
        {
            auto START = std::chrono::high_resolution_clock::now();

            cout << "\nestado INICIAL : ";
            printarVecInt(inicio);
            cout << "\nestado FINAL   : ";
            printarVecInt(destino);
            cout << endl;

            if(!solucionado){
                _solucao = menor_caminho_->gerar(inicio, destino, dim, heuristica_, 0, 0);
                solucionado = true;
            }

            cout << "\nalgoritimo de busca usado : " << algoritmoBuscaUsado << endl;
            cout << "heuristica usada : " << heuristicaUsada << endl;
            cout << "quantidade de movimentos : " << _solucao.qtdMovimentos << endl;
            cout << "quantidade de nos visitados : " << _solucao.qtdNosVisitados << endl;
            if (_solucao.aThreadQueEcontrou != 0)
            {
                cout << "econtrando pela thread : " << _solucao.aThreadQueEcontrou << endl;
            }

            auto END = std::chrono::high_resolution_clock::now();
            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(END - START);
            cout << "tempo de execucao : ";
            imprimir_duracao(duracao);
        }
        else
        {
            cout << "\nEsse Puzzle nao eh solucionavel !!! ";
            cout << "\n~estado INICIAL : ";
            printarVecInt(inicio);
            cout << "~\n~estado FINAL   : ";
            printarVecInt(destino);
            cout << "~" << endl
                 << endl;
        }
    }

    // gera uma saida html com o passo a passo interativo do puzzle em uma pasta destino
    void puzzle::geraSaidaPraHTML(const std::string &pasta, const std::string &nomeSaida)
    {
        if (verificaSeEhSolucionavel())
        {
            // Gerar a solução
            if(!solucionado){
                _solucao = menor_caminho_->gerar(inicio, destino, dim, heuristica_, 0, 0);
                solucionado = true;
            }
            std::string htmlConteudo = gHTML::htmlContruido(eeu::saida_passos_para_html(_solucao.caminhoSolucao, dim));

            // Verificar se a pasta existe, se não existir, criar
            struct stat info;
            if (stat(pasta.c_str(), &info) != 0)
            {
                // A pasta não existe, então vamos criá-la
#ifdef _WIN32
                if (_mkdir(pasta.c_str()) != 0)
                { // No Windows, _mkdir usa apenas um argumento
#else
                if (mkdir(pasta.c_str(), 0777) != 0)
                { // No Linux/macOS, mkdir usa dois argumentos
#endif
                    std::cerr << "Erro ao criar o diretório: " << pasta << std::endl;
                    return;
                }
            }
            else if (!(info.st_mode & S_IFDIR))
            {
                // Existe, mas não é um diretório
                std::cerr << pasta << " existe, mas não é um diretório." << std::endl;
                return;
            }

            // Garantir que o nome tenha a extensão ".html"
            std::string nomeArquivoComExtensao = nomeSaida;
            if (nomeArquivoComExtensao.substr(nomeArquivoComExtensao.find_last_of(".") + 1) != "html")
            {
                nomeArquivoComExtensao += ".html";
            }

            // Montar o caminho completo do arquivo
            std::string caminhoCompleto = pasta + "/" + nomeArquivoComExtensao;

            // Criar e escrever o arquivo
            std::ofstream arquivoSaida(caminhoCompleto);
            if (arquivoSaida.is_open())
            {
                arquivoSaida << htmlConteudo;
                arquivoSaida.close();
            }
            else
            {
                // Tratar erro caso o arquivo não possa ser criado/aberto
                std::cerr << "Erro ao abrir/criar o arquivo: " << caminhoCompleto << std::endl;
            }
        }
        else
        {
            cout << "\nEsse Puzzle nao eh solucionavel !!! ";
            cout << "\n~estado INICIAL : ";
            printarVecInt(inicio);
            cout << "~\n~estado FINAL   : ";
            printarVecInt(destino);
            cout << "~" << endl
                 << endl;
        }
    }

    // printa o valor da heuristica de cada estado
    void puzzle::printaValorHEUpassos()
    {
        if (verificaSeEhSolucionavel())
        {
            solucao _solucao = menor_caminho_->gerar(inicio, destino, dim, heuristica_, 0, 0);
            for (auto entrada : _solucao.caminhoSolucao)
            {
                std::cout << " " << heuristica_->calcularH(entrada, destino, dim) << "\n";
            }
        }
        else
        {
            cout << "\nEsse Puzzle nao eh solucionavel !!! ";
            cout << "\n~estado INICIAL : ";
            printarVecInt(inicio);
            cout << "~\n~estado FINAL   : ";
            printarVecInt(destino);
            cout << "~" << endl
                 << endl;
        }
    }

    // Destruidor
    puzzle::~puzzle()
    {
        if (heuristica_ != nullptr)
        {
            delete heuristica_;
        }
        if (menor_caminho_ != nullptr)
        {
            delete menor_caminho_;
        }
    }

} // npuzzle namespace
