#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include "Board.hpp"
#include "Player.hpp"
#include "RuleBook.hpp"

/**
 * @file Game.hpp
 * @brief Definição da classe base abstrata Game.
 */

/**
 * @class Game
 * @brief Armazena metadados do jogo, estado da partida e gerencia turnos e jogadas.
 */
class Game {
protected:
    std::string nomeJogo; ///< Nome de identificação e metadados do jogo.
    Board* tabuleiro; ///< Referência do tabuleiro associado à partida.
    RuleBook* regras; ///< Livro de regras associado ao jogo.
    std::vector<Player*> jogadores; ///< Lista de jogadores participantes.
    size_t indiceJogadorDaVez; ///< Apontador para o índice do jogador da vez.
    std::string estadoPartida; ///< Estado atual da partida (ex: "em_andamento", "finalizada").

public:
    /**
     * @brief Constrói a partida base.
     * @param nome Nome do jogo.
     * @param b Ponteiro para o tabuleiro.
     * @param r Ponteiro para o livro de regras.
     */
    Game(const std::string& nome, Board* b, RuleBook* r);

    /**
     * @brief Destrutor virtual.
     */
    virtual ~Game();

    /**
     * @brief Configura a disposição inicial das peças no tabuleiro.
     */
    virtual void configurarDisposicaoInicial() = 0;

    /**
     * @brief Alterna a vez entre os jogadores participantes.
     */
    void alternarTurno();

    /**
     * @brief Executa uma jogada válida e atualiza o estado do tabuleiro.
     * @param origemX Linha de origem.
     * @param origemY Coluna de origem.
     * @param destinoX Linha de destino.
     * @param destinoY Coluna de destino.
     * @return true se executada com sucesso.
     */
    bool executarJogada(int origemX, int origemY, int destinoX, int destinoY);

    /**
     * @brief Reinicia a partida restaurando seu estado inicial sem registrar derrota.
     */
    void reiniciarPartida();

    /**
     * @brief Registra o vencedor ou declara empate ao final da partida.
     * @param vencedor Ponteiro para o jogador vencedor (nullptr se empate).
     */
    void finalizarPartida(Player* vencedor);
};

#endif