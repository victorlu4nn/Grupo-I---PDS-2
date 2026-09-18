#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

/**
 * @file Player.hpp
 * @brief Definição da classe Player para identificação dos participantes.
 */

/**
 * @class Player
 * @brief Armazena nome, símbolo/cor de peças e histórico de pontuação do participante.
 */
class Player {
private:
    std::string nome; ///< Nome ou apelido de identificação do participante.
    char simboloPeca; ///< Símbolo ou cor atribuído às suas peças.
    int pontuacao; ///< Histórico de pontuação ou partidas vencidas.

public:
    /**
     * @brief Constrói um novo Player.
     * @param nome Nome do jogador.
     * @param simbolo Símbolo da peça.
     */
    Player(const std::string& nome, char simbolo);

    /**
     * @brief Retorna o nome do jogador.
     * @return std::string Nome do jogador.
     */
    std::string getNome() const;

    /**
     * @brief Retorna o símbolo associado às peças do jogador.
     * @return char Símbolo da peça.
     */
    char getSimboloPeca() const;

    /**
     * @brief Retorna a pontuação atual do jogador.
     * @return int Pontuação acumulada.
     */
    int getPontuacao() const;

    /**
     * @brief Adiciona pontos ao histórico do jogador.
     * @param pontos Quantidade de pontos a somar.
     */
    void adicionarPontos(int pontos);
};

#endif