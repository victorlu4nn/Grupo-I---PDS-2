#ifndef DAMAS_HPP
#define DAMAS_HPP

#include "Piece.hpp"
#include "Move.hpp"
#include "Game.hpp"

class Damas: public Game {
    private:
        unsigned quantidade_pecas_brancas;
        unsigned quantidade_pecas_pretas;
        bool eh_dama;

        void promocao_dama();
    public:
        Damas();

        ~Damas();

        bool _movimento_valido(
            bool dama, int x_orig, int y_orig, int x_dest, int y_dest
        ) const;
        bool _captura_valida();
        void organizar_tabuleiro_damas();
};

#endif