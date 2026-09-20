#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>

/**
 * @file Board.hpp
 * @brief Definição da classe Board para manipulação da matriz do tabuleiro.
 */

/**
 * @class Board
 * @brief Controla as dimensões N x M, alocação dinâmica e posições das células.
 */
class Board {
private:
    int linhas; ///< Número de linhas do tabuleiro (N).
    int colunas; ///< Número de colunas do tabuleiro (M).
    std::vector<std::vector<std::string>> celulas; ///< Alocação da grade de células.

public:
    /**
     * @brief Constrói o tabuleiro validando dimensões (lança exceção se <= 0).
     * @param n Número de linhas.
     * @param m Número de colunas.
     */
    Board(int n, int m);

    /**
     * @brief Verifica se coordenadas (x, y) estão dentro dos limites válidos.
     * @param x Coordenada da linha.
     * @param y Coordenada da coluna.
     * @return true se estiver dentro dos limites, false caso contrário.
     */
    bool validarCoordenadas(int x, int y) const;

    /**
     * @brief Consulta a peça ou elemento presente em uma coordenada.
     * @param x Coordenada da linha.
     * @param y Coordenada da coluna.
     * @return std::string Representação da peça na posição.
     */
    std::string consultarPosicao(int x, int y) const;

    /**
     * @brief Insere ou atualiza uma peça em uma posição específica.
     * @param x Coordenada da linha.
     * @param y Coordenada da coluna.
     * @param peca Representação da peça.
     */
    void atualizarPosicao(int x, int y, const std::string& peca);

    /**
     * @brief Remove peças de posições determinadas do tabuleiro.
     * @param x Coordenada da linha.
     * @param y Coordenada da coluna.
     */
    void removerPeca(int x, int y);

    /**
     * @brief Restaura a matriz para a configuração inicial limpa.
     */
    void limpar();
};

#endif
