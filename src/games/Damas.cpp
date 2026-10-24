#include <iostream>
#include <map>
#include <vector>
#include <sstream>

#include "Damas.hpp"

void Damas::configurarDisposicaoInicial() {
    Board* DamasBoard = new Board(8, 8);
    organizar_tabuleiro_damas(DamasBoard);
}

void Damas::organizar_tabuleiro_damas(Board* damasboard) {
    // Configura o terminal para aceitar UTF-8 (necessário em alguns sistemas como Windows)
    std::setlocale(LC_ALL, "en_US.UTF-8"); 

    PecaDamas* peca_normal_1 = new PecaDamas("\u26C2", 1);
    PecaDamas* peca_normal_2 = new PecaDamas("\u26C0", 2);

    // Configure white pieces
    bool eh_par = true;
    for (int x = 0; x <= 7; x++) {
        if (eh_par) {
            damasboard->atualizarPosicao(x, 0, peca_normal_1);
            damasboard->atualizarPosicao(x, 2, peca_normal_1);
        } else {
            damasboard->atualizarPosicao(x, 1, peca_normal_1);
        }
        !eh_par;
    }
    // Configure black pieces
    for (int x = 0; x <= 7; x++) {
        !eh_par;    
        if (eh_par) {
            damasboard->atualizarPosicao(x, 5, peca_normal_2);
            damasboard->atualizarPosicao(x, 7, peca_normal_2);
        } else {
            damasboard->atualizarPosicao(x, 6, peca_normal_2);
        }
    }
}

std::vector<std::string> Damas::_lendo_movimento(const std::string acao) const {
    const std::string board_columns = "aAbBcCdDeEfFgGhH";
    const std::string board_lines = "12345678";

    std::stringstream input_stringstream(acao);
    std::vector<std::string> tokens;
    std::string token;

    std::vector<std::string> pattern_movement;

    while (std::getline(input_stringstream, token, ' ')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 2) {
        std::cerr << "Error. Movement needs an inital position and last position." << std::endl;
    }

    for (std::string single_command: tokens){
        if (single_command.size() > 2) {
            std::cerr << "Error. Invalid movement command." << std::endl;
        }

        int column_command = board_columns.find(single_command[0]);
        
        if (!(column_command != std::string::npos)
                || !(board_lines.find(single_command[1]) != std::string::npos))
        {
            std::cerr << "Error. Invalid movement command." << std::endl;
        }

        int column_pattern = column_command % 2;

        std::string pattern_move = std::to_string(column_pattern) + single_command[1];
        pattern_movement.push_back(pattern_move);
    }

    return pattern_movement;
}

bool verify_structures(
    std::vector<std::string> commands,
    const std::string columns,
    const std::string lines
) {
    for (std::string single_command: commands) {
        if (!(columns.find(single_command[0]) != std::string::npos)
            || !(lines.find(single_command[1]) != std::string::npos)
            || single_command.size() > 2)
            return false;
    }
    return true;
}

bool Damas::executarJogada(const std::string acao) {

}


PecaDamas::PecaDamas(std::string simb, int id): Piece(simb, id) {};


bool PecaDamas::get_eh_damas() const {
    return eh_dama;
}

void PecaDamas::atualizar_simbolo() {
    if (id_jogador == 1)
        simbolo = "\u26C3";
    else 
        simbolo = "\u26C1";
}

void PecaDamas::promocao_dama() {
    eh_dama = true;
    atualizar_simbolo();
}