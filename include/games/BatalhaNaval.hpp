
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

/*
 * Board layout: each player owns a 10x10 "sea" and both seas live side by side
 * on the same Board, which must have at least 10 rows and 20 columns:
 *   player 1 -> columns 0-9, player 2 -> columns 10-19.
 * Every coordinate used by the public helpers below is relative to ONE sea
 * (line 0-9, column 0-9). The translation to Board columns is internal.
 * Piece ids: 1 = player 1, 2 = player 2.
 */
class BatalhaNaval : public Game {
private:
    // Number of ships belonging to each player
    unsigned quantidade_navios_jogador1;
    unsigned quantidade_navios_jogador2;

public:
    // Constructor and Destructor
    BatalhaNaval(Board* tabuleiro, RuleBook* regras);
    ~BatalhaNaval() override;

    // The game owns its Player objects, so copying it would double-delete them
    BatalhaNaval(const BatalhaNaval&) = delete;
    BatalhaNaval& operator=(const BatalhaNaval&) = delete;

    // Configures the initial arrangement of the ships
    void configurarDisposicaoInicial() override;

    // Executes a player's action
    bool executarJogada(const std::string acao) override;

    // Clears the board and randomly places the whole fleet of both players
    void organizar_tabuleiro_batalha_naval();

    // Places one ship in the sea of 'jogador' (1 or 2), starting at
    // (linha, coluna) and growing right (horizontal) or down (vertical).
    // Fails (returns false) if it leaves the sea, overlaps or touches
    // (orthogonally) another ship.
    bool posicionar_navio(int jogador, int linha, int coluna, int tamanho, bool horizontal);

    // Converts the player's input (e.g. "B5") into {linha, coluna} of the
    // attacked sea. Returns an empty vector if the input is invalid.
    std::vector<int> _lendo_movimento(const std::string acao) const;

    // Checks whether the current player can attack (linha, coluna) of the
    // opponent's sea (inside the sea and not attacked yet)
    bool _movimento_valido(int linha, int coluna) const;

    // Executes the attack on the opponent's sea and checks its result
    void _executa_movimento(int linha, int coluna);

    // Number of ships of 'jogador' that are not sunk yet
    unsigned navios_restantes(int jogador) const;

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
    // Constructor (navio = false creates a "water" marker, i.e. a missed shot)
    PecaBatalhaNaval(std::string simb, int id_jogador, bool navio = true);

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