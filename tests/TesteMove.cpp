#include "doctest.h"
#include "Piece.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Move.hpp"
#include <stdexcept>

class PecaQualquer: public Piece {
    public:
        PecaQualquer(std::string simb, int id): Piece(simb, id) {};
        bool movimento_valido(
            int x_orig, 
            int y_orig, 
            int x_dest, 
            int y_dest, 
            const Board& board_status
        ) const override {
            return true;
        }
};

class JogadorQualquer: public Player {
    public:
        JogadorQualquer(const std::string& n, char simb): Player(n, simb) {};
};

TEST_CASE("Analisando insercao de pecas"){
    Board tabuleiro(8, 8);
    JogadorQualquer jogador1("Isaac Newton", 'b');

    PecaQualquer* peca1 = new PecaQualquer("U+2617", 1);
    tabuleiro.atualizarPosicao(1, 1, peca1);

    SUBCASE("Atualizando a posição de peças") {
        Move movimento(&jogador1, 1, 1, 2, 2);
        movimento.aplicar(&tabuleiro);

        CHECK(tabuleiro.consultarPosicao(1, 1) == "");
        CHECK(tabuleiro.consultarPosicao(2, 2) == "U+2617");
    }
    SUBCASE("Reverter a jogada") {
        Move movimento(&jogador1, 1, 1, 2, 2);
        movimento.aplicar(&tabuleiro);
        movimento.reverter(&tabuleiro);

        CHECK(tabuleiro.consultarPosicao(1, 1) == "U+2617");
        CHECK(tabuleiro.consultarPosicao(2, 2) == "");
    }
    SUBCASE("Aplicação de lógica de captura") {
        PecaQualquer* peca2 = new PecaQualquer("U+2616", 2);
        tabuleiro.atualizarPosicao(2, 2, peca2);

        Move movimento(&jogador1, 1, 1, 3, 3);
        movimento.aplicar(&tabuleiro);

        CHECK(tabuleiro.consultarPosicao(1, 1) == "");
        CHECK(tabuleiro.consultarPosicao(3, 3) == "U+2617");
    }
}