# Projeto trab02 - Resolução de Puzzles 8 e 15

Este projeto contém a implementação de algoritmos para a resolução de puzzles do tipo 8-puzzle e 15-puzzle utilizando os algoritmos A* e IDA*.

## Compilação

Para obter melhor desempenho, o código foi compilado usando a flag `-O3`:

```bash
g++ *.cpp -O3 -o trab02
```

**Atenção:** A utilização de threads pode não ser suportada por algumas versões do compilador `g++`. Caso encontre problemas, recomendo utilizar uma versão mais recente. Eu compilei usando WSL no Windows com `g++ 11`.

### Atualização do MinGW

Caso esteja utilizando Windows, você pode baixar uma versão atualizada do MinGW através do link:
[MinGW atualizado](https://github.com/skeeto/w64devkit/releases)

## Personalização da Saída

Para obter saídas específicas, edite o arquivo `main.cpp`, comentando ou descomentando as linhas apropriadas.

### Exemplos de Saídas Incluídas

- **8-puzzle:** 1 solução com 31 movimentos utilizando A*.
- **15-puzzle:** 1 solução com 40 movimentos utilizando IDA*.
- **Estatísticas 8-puzzle:** Resultados estatísticos para todas as entradas fornecidas.
- **Duas Estatísticas 15-puzzle:** Resultados estatísticos para todas as entradas fornecidas -1.
- **Saída HTML interativa:** Exemplo de saída visual.

## Instruções de Uso

### Compilação

Certifique-se de utilizar `g++ 11` ou superior com suporte a threads.

1. Abra um terminal na pasta raiz do projeto.
2. Digite o comando:

   ```bash
   g++ *.cpp -O3 -o trab02
   ```

### Execução

1. No terminal, após a compilação:

   - **Windows:** `trab02.exe`
   - **Linux:** `./trab02`

### Geração de Saída em TXT

1. No terminal, após a compilação:

   - **Windows:** `trab02.exe > saida.txt`
   - **Linux:** `./trab02 > saida.txt`

### Geração de Saída em HTML

1. Descomente a linha no `main.cpp`:

   ```cpp
   p.geraSaidaPraHTML("pasta","saida");
   ```

## Arquivos Necessários

O projeto inclui 16 arquivos essenciais para a compilação:

### Arquivos .h

- `entrardas_PUZZLE.h`
- `estruturas_e_uteis.h`
- `gerarHTML.h`
- `grafo_puzzle.h`
- `heuristica.h`
- `menor_caminho.h`
- `npuzzle.h`

### Arquivos .cpp

- `estruturas_e_uteis.cpp`
- `gerarHTML.cpp`
- `grafo_puzzle.cpp`
- `heuristica.cpp`
- `main.cpp`
- `MC_a_star.cpp`
- `MC_ida_star.cpp`
- `MC_ida_star2.cpp`
- `npuzzle.cpp`