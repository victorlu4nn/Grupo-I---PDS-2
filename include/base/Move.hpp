#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>
#include "Player.hpp"
#include "Board.hpp"

/**
 * @file Move.hpp
 * @brief Definição da classe Move para encapsular os movimentos do jogo.
 */

/**
 * @class Move
 * @brief Armazena dados de origem, destino e aplica/reverte alterações no tabuleiro.
 */
class Move {
private:
    Player* jogador; ///< Jogador responsável pela realização da jogada.
    int origemX; ///< Coordenada de origem (linha).
    int origemY; ///< Coordenada de origem (coluna).
    int destinoX; ///< Coordenada de destino (linha).
    int destinoY; ///< Coordenada de destino (coluna).
    std::string tipoCaptura; ///< Dados extras da jogada (como tipo de captura).

public:
    /**
     * @brief Constrói uma nova jogada.
     * @param j Ponteiro para o jogador.
     * @param ox Linha de origem.
     * @param oy Coluna de origem.
     * @param dx Linha de destino.
     * @param dy Coluna de destino.
     */
    Move(Player* j, int ox, int oy, int dx, int dy);

    /**
     * @brief Aplica o deslocamento ou inserção das peças no tabuleiro.
     * @param tabuleiro Ponteiro para o tabuleiro da partida.
     */
    void aplicar(Board* tabuleiro);

    /**
     * @brief Reverter a alteração no tabuleiro para desfazer a jogada.
     * @param tabuleiro Ponteiro para o tabuleiro da partida.
     */
    void reverter(Board* tabuleiro);

    /**
     * @brief Fornece representação textual legível da jogada para o console.
     * @return std::string Texto descritivo do movimento.
     */
    std::string toString() const;
};

#endif
