#ifndef RULEBOOK_HPP
#define RULEBOOK_HPP

#include <string>
#include "Board.hpp"
#include "Player.hpp"

/**
 * @file RuleBook.hpp
 * @brief Definição da classe RuleBook (regras de negócio do jogo).
 */

/**
 * @class RuleBook
 * @brief Armazena o manual de regras, valida jogadas e verifica condições de vitória ou empate.
 */
class RuleBook {
private:
    std::string descricaoRegras; ///< Descrição completa das regras e manual do jogo.

public:
    /**
     * @brief Constrói o livro de regras com sua respectiva descrição.
     * @param descricao Texto explicativo das regras.
     */
    RuleBook(const std::string& descricao);

    /**
     * @brief Fornece o texto explicativo das regras para consulta.
     * @return std::string Manual de regras.
     */
    std::string getDescricaoRegras() const;

    /**
     * @brief Valida se uma jogada proposta é legal perante as regras.
     * @param tabuleiro Ponteiro para o tabuleiro atual.
     * @param origemX Linha de origem.
     * @param origemY Coluna de origem.
     * @param destinoX Linha de destino.
     * @param destinoY Coluna de destino.
     * @return true se a jogada for válida, false caso contrário.
     */
    bool validarJogada(Board* tabuleiro, int origemX, int origemY, int destinoX, int destinoY) const;

    /**
     * @brief Verificar se o estado atual do tabuleiro configura vitória.
     * @param tabuleiro Ponteiro para o tabuleiro atual.
     * @return true se houver condição de vitória.
     */
    bool verificarVitoria(Board* tabuleiro) const;

    /**
     * @brief Verificar se o estado da partida configura empate.
     * @param tabuleiro Ponteiro para o tabuleiro atual.
     * @return true se houver condição de empate.
     */
    bool verificarEmpate(Board* tabuleiro) const;
};

#endif