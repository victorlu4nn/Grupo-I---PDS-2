#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <vector>
#include <string>
#include "Game.hpp"

/**
 * @file GameManager.hpp
 * @brief Definição da classe GameManager para controle do catálogo e fluxo principal.
 */

/**
 * @class GameManager
 * @brief Mantém o catálogo de jogos, gerencia instâncias de partidas e o ciclo de execução.
 */
class GameManager {
private:
    std::vector<std::string> catalogoJogos; ///< Catálogo de jogos registrados na engine.
    Game* partidaAtual; ///< Ponteiro para a partida atualmente em execução.

public:
    /**
     * @brief Constrói o gerenciador e inicializa as estruturas.
     */
    GameManager();

    /**
     * @brief Destrutor para limpeza de memória da partida ativa.
     */
    ~GameManager();

    /**
     * @brief Registra novos jogos por meio de uma interface padronizada.
     * @param nomeJogo Nome do jogo a ser adicionado ao catálogo.
     */
    void registrarJogo(const std::string& nomeJogo);

    /**
     * @brief Disponibiliza a listagem de jogos para a interface do usuário.
     * @return std::vector<std::string> Lista com os nomes dos jogos disponíveis.
     */
    std::vector<std::string> listarJogos() const;

    /**
     * @brief Instancia e inicializa a partida escolhida pelo jogador.
     * @param indice Índice do jogo no catálogo.
     */
    void iniciarPartida(int indice);

    /**
     * @brief Gerencia o ciclo de vida da execução e turnos da partida.
     */
    void executarLoopPartida();

    /**
     * @brief Processa comandos de encerramento e reinicialização com confirmação.
     */
    void processarComandoEncerramento();
};

#endif