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
        if (eh_par) {
            damasboard->atualizarPosicao(x, 5, peca_normal_2);
            damasboard->atualizarPosicao(x, 7, peca_normal_2);
        } else {
            damasboard->atualizarPosicao(x, 6, peca_normal_2);
        }
        !eh_par;
    }
}

std::vector<int> Damas::_lendo_movimento(const std::string acao) const {
    const std::string board_columns = "aAbBcCdDeEfFgGhH";

    std::stringstream input_stringstream(acao);
    std::vector<std::string> tokens;
    std::string token;

    while (std::getline(input_stringstream, token, ' ')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 2) {
        std::cerr << "Error. Invalid comand." << std::endl;
    }
    if (tokens.size() > 2) {

    }
}

bool verify_structures(
    std::vector<std::string> commands,
    const std::string columns
) {
    for (std::string single_command: commands) {
        if (!(columns.find(single_command[0]) != std::string::npos)){
            return false;
        } else if ()
    }
}