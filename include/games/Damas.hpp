#ifndef DAMAS_HPP
#define DAMAS_HPP

#include <string>
#include <vector>

#include "Piece.hpp"
#include "Move.hpp"
#include "Game.hpp"

class Damas: public Game, public PecaDamas {
    private:
        // Variables that are going to determine how many pieces 
        // of each team there are on the board
        unsigned quantidade_pecas_brancas;
        unsigned quantidade_pecas_pretas;
    public:
        // Constructor and Destructor
        Damas();

        ~Damas();

        // Function that reads and organize actions based on the input reading
        // the main idea is to be able to read and struture multiple movements
        // when there are multiple captures
        std::vector<int> _lendo_movimento(std::string acao) const;

        // Verifies if the capture is valid. Takes the vector as an argument
        // and checks if every capture command is valid to validate it
        bool _captura_valida(std::vector<int>);
        
        // Stores all captured pieces into a vector for multiple taken pieces
        // due to the game capture mechanics
        std::vector<Piece*> pecas_capturadas();

        // Initialize the damas board given a fixed structure
        void organizar_tabuleiro_damas();

        // Returns if the game has ended
        bool FimdeJogo() const;

        // Returns the winner
        Player* Vencedor() const;
};

enum class EquipePeca {
    BRANCA,
    PRETA
};

class PecaDamas: public Piece {
    private:
        // Attribute that verifies if a piece is Dama, due to different
        // mobility mechanics
        bool eh_dama;
        const EquipePeca equipe;

    public:
        // Constructor that initializes a piece of Damas and will work with its
        // symbol, status, boolean value eh_dama
        PecaDamas(std::string simb, int id_jogador);

        // Destructor that is called when piece is captured and excluded from the board
        ~PecaDamas();

        // Getter para ler se peça eh dama
        bool get_eh_damas() const;

        // Update symbol when piece turns into dama
        void atualizar_simbolo();

        // Exclusive function that changes a piece status from normal to
        // dama when it reaches the end of the board and it is called
        void promocao_dama();

        // Function that will validate a movement for damas especifically
        // It takes bool dama as a parameter to verify if 
        bool _movimento_valido(bool dama, Move movimento) const;

        // Literally calls movement and updates piece locals
        void _executa_movimento(Move movimento);

        // Determines if piece reached the final opposite row of the board
        bool final_tabuleiro() const;
};

#endif