#ifndef CONNECT4_HPP
#define CONNECT4_HPP

#include <string>
#include <vector>

#include "Piece.hpp"
#include "Move.hpp"
#include "Game.hpp"

class PecaConnect4; // Declaração antecipada

/**
 * @class Connect4
 * @brief Implementa as regras e o fluxo específico do jogo Connect 4 (Lig 4).
 */
class Connect4 : public Game {
private:
    // Dimensões padrão do tabuleiro de Connect 4
    static const int LINHAS = 6;
    static const int COLUNAS = 7;

    unsigned quantidade_pecas_jogador1;
    unsigned quantidade_pecas_jogador2;

public:
    /**
     * @brief Constrói uma partida de Connect 4 inicializando tabuleiro 6x7 e regras.
     */
    Connect4();

    /**
     * @brief Destrutor da partida de Connect 4.
     */
    ~Connect4() override;

    /**
     * @brief Configura o tabuleiro inicial limpo para o Connect 4.
     */
    void configurarDisposicaoInicial() override;

    /**
     * @brief Insere (solta) uma peça em uma coluna específica, fazendo-a cair 
     * para a linha mais baixa disponível naquela coluna.
     * @param coluna Índice da coluna (0 a 6).
     * @return true se a jogada foi válida e realizada com sucesso.
     */
    bool soltarPeca(int coluna);

    /**
     * @brief Verifica se o jogo terminou (por 4 em linha ou tabuleiro cheio/empate).
     * @return true se a partida acabou.
     */
    bool FimdeJogo() const;

    /**
     * @brief Retorna o ponteiro para o jogador vencedor, ou nullptr em caso de empate/em andamento.
     * @return Player* Vencedor da partida.
     */
    Player* Vencedor() const;

    /**
     * @brief Verifica se existe uma sequência de 4 peças iguais a partir de uma coordenada.
     * @param linha Linha da última peça jogada.
     * @param coluna Coluna da última peça jogada.
     * @return true se formou 4 em linha.
     */
    bool verificar4EmLinha(int linha, int coluna) const;
};

/**
 * @enum EquipeConnect4
 * @brief Identifica as equipes/cores dos jogadores no Connect 4.
 */
enum class EquipeConnect4 {
    VERMELHO,
    AMARELO
};

/**
 * @class PecaConnect4
 * @brief Representa o disco/ficha utilizado por um jogador no Connect 4.
 */
class PecaConnect4 : public Piece {
private:
    EquipeConnect4 equipe;

public:
    /**
     * @brief Constrói uma peça do Connect 4.
     * @param simbolo Símbolo visual (ex: "R" ou "A").
     * @param id_jogador Identificador do dono.
     * @param eq Equipe/Cor da peça.
     */
    PecaConnect4(std::string simbolo, int id_jogador, EquipeConnect4 eq);

    /**
     * @brief Destrutor da peça.
     */
    ~PecaConnect4() override;

    /**
     * @brief No Connect 4 as peças não se movem após colocadas. 
     * A validação padrão retorna falso para deslocamentos tradicionais.
     */
    bool movimento_valido(int x_orig, int y_orig, int x_dest, int y_dest, const Board& board_status) const override;

    /**
     * @brief Retorna a equipe associada à peça.
     * @return EquipeConnect4 
     */
    EquipeConnect4 getEquipe() const;
};

#endif