
#ifndef BATALHANAVAL_HPP
#define BATALHANAVAL_HPP

#include <string>
#include <vector>

#include "Piece.hpp"
#include "Move.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "RuleBook.hpp"
#include "Player.hpp"

class BatalhaNaval : public Game {
private:
    // Number of ships belonging to each player
    unsigned quantidade_navios_jogador1;
    unsigned quantidade_navios_jogador2;

public:
    // Constructor and Destructor
    BatalhaNaval(Board* tabuleiro, RuleBook* regras);
    ~BatalhaNaval() override;

    // Configures the initial arrangement of the ships
    void configurarDisposicaoInicial() override;

    // Executes a player's action
    bool executarJogada(const std::string acao) override;

    // Places the ships on the board
    void organizar_tabuleiro_batalha_naval();

    // Converts the player's input into coordinates
    std::vector<int> _lendo_movimento(const std::string acao) const;

    // Checks whether the attack is valid
    bool _movimento_valido(Move movimento) const;

    // Executes the attack and checks its result
    void _executa_movimento(Move movimento);

    // Checks whether the game has ended
    bool FimdeJogo() const;

    // Returns the winning player
    Player* Vencedor() const;
};

class PecaBatalhaNaval : public Piece {
private:
    // Indicates whether this position contains a ship
    bool eh_navio;

    // Indicates whether this position has been hit
    bool foi_atingido;

public:
    // Constructor
    PecaBatalhaNaval(std::string simb, int id_jogador);

    // Destructor
    ~PecaBatalhaNaval() override;

    // Returns whether this position contains a ship
    bool get_eh_navio() const;

    // Returns whether this position has been hit
    bool get_foi_atingido() const;

    // Marks this position as hit
    void atingir();

    // Implements the abstract method inherited from Piece
    bool movimento_valido(
        int x_orig,
        int y_orig,
        int x_dest,
        int y_dest,
        const Board& board_status
    ) const override;
};

#endif
