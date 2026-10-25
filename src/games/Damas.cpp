#include <iostream>
#include <map>
#include <vector>
#include <sstream>

#include "Damas.hpp"
#include "Move.hpp"

Damas::Damas(Board* damasboard, RuleBook* damasrulebook): Game("Damas", damasboard, damasrulebook) {}

Damas::~Damas(){}

void Damas::configurarDisposicaoInicial() {
    organizar_tabuleiro_damas(tabuleiro);
}

void Damas::organizar_tabuleiro_damas(Board* damasboard) {
    // Configura o terminal para aceitar UTF-8 (necessário em alguns sistemas como Windows)
    std::setlocale(LC_ALL, "en_US.UTF-8"); 

    // Configure white pieces
    bool eh_par = true;
    for (int x = 0; x <= 7; x++) {
        if (eh_par) {
            damasboard->atualizarPosicao(x, 0, new PecaDamas("\u26C2", 1));
            damasboard->atualizarPosicao(x, 2, new PecaDamas("\u26C2", 1));
        } else {
            damasboard->atualizarPosicao(x, 1, new PecaDamas("\u26C2", 1));
        }
        eh_par = !eh_par;
    }
    // Configure black pieces
    for (int x = 0; x <= 7; x++) {
        eh_par = !eh_par;    
        if (eh_par) {
            damasboard->atualizarPosicao(x, 5, new PecaDamas("\u26C0", 2));
            damasboard->atualizarPosicao(x, 7, new PecaDamas("\u26C0", 2));
        } else {
            damasboard->atualizarPosicao(x, 6, new PecaDamas("\u26C0", 2));
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
        std::cerr << "Erro. Movimento precisa de uma posicao inicial e final." << std::endl;
    }

    for (std::string single_command: tokens){
        if (single_command.size() > 2) {
            std::cerr << "Erro. Comando de movimento invalido." << std::endl;
        }

        int column_command = board_columns.find(single_command[0]);
        
        if (!(column_command != std::string::npos)
            || !(board_lines.find(single_command[1]) != std::string::npos))
        {
            std::cerr << "Erro. Casa invalida." << std::endl;
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
            || single_command.size() != 2)
            return false;
    }
    return true;
}

bool Damas::executarJogada(const std::string acao) {
    std::vector<std::string> standardized_move = _lendo_movimento(acao);

    std::string posicao_inicial = standardized_move[0];
    int initial_x = posicao_inicial[0] - '0';
    int initial_y = posicao_inicial[1] - '0';
      
    Piece* peca_de_movimento = tabuleiro->consultarPosicao(initial_x, initial_y);
    PecaDamas* peca_damas_de_movimento = dynamic_cast<PecaDamas*>(peca_de_movimento);

    // verify if the place is occupied by any piece
    // and if it is, get this piece to move
    if (peca_damas_de_movimento == nullptr) {
        std::cerr << "Erro. Nenhuma peca na posicao inicial" << std::endl;
    } else if (peca_damas_de_movimento->ler_id_jogador() != indiceJogadorDaVez) {
        std::cerr << "Erro. Peca da posicao inicial nao pertence a voce" << std::endl;
    }

    Movimento_Valido* move = peca_damas_de_movimento->_movimento_valido(
        peca_damas_de_movimento->ler_id_jogador(), standardized_move
    );

    int final_x = move->coords[move->coords.size() - 2];
    int final_y = move->coords[move->coords.size() - 1];

    if (tabuleiro->consultarPosicao(final_x, final_y) != nullptr) {
        std::cerr << "Erro. Posicao final ja possui uma peca alocada" << std::endl;
    } else if (!move->eh_valido) {
        std::cerr << "Erro. Deslocamento invalido para a peca" << std::endl;
    }

    if (move->coords.size() > 2) {
        if (_captura_valida(*move)) {

        }
    } else {
        if (abs(initial_x - final_x) == 2 && abs(initial_y - final_y) == 2) {
            if (_captura_valida(*move)) {

            }
        }
    }
    
}

bool Damas::_captura_valida(Movimento_Valido acoes) {
    for (int index = 0; index <= (acoes.coords.size() - 4); index += 2){
        int xi = acoes.coords[index];
        int yi = acoes.coords[index + 1];
        int xj = acoes.coords[index + 2];

        int intermediario_x = xi + 1;
        int intermediario_y = yi + 1;
        
        Piece* peca_capturada = tabuleiro->consultarPosicao(intermediario_x, intermediario_y);

        if (peca_capturada == nullptr || peca_capturada->ler_id_jogador() == indiceJogadorDaVez) {
            std::cerr << "Erro. Tentativa de captura invalida (nenhuma peca a ser capturada ou peca a ser captura pertence a voce)" << std::endl;
            return false; 
        }
    }

    return false;
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

Movimento_Valido* PecaDamas::_movimento_valido(
    int indice_jogador,
    std::vector<std::string> movimento_padronizado
) const {
    Movimento_Valido* valid = new Movimento_Valido;

    for (std::string coord : movimento_padronizado) {
        int x = coord[0] - '0';
        int y = coord[1] - '0';

        valid->coords.push_back(x);
        valid->coords.push_back(y);
    }

    if (!eh_dama) {
        if (valid->coords.size() == 4) {
            int initial_x = valid->coords[0];
            int initial_y = valid->coords[1];
            int final_x = valid->coords[2];
            int final_y = valid->coords[3];

            if (
                (abs(initial_x - final_x) <= 2 && abs(initial_y - final_y) <= 2) &&
                (abs(initial_x - final_x) == abs(initial_y - final_y))
            ) 
                valid->eh_valido = true;

        } else {
            valid->eh_valido = true;

            for (int index = 0; index <= (valid->coords.size() - 4); index += 2) {
                int initial_x = valid->coords[index];
                int initial_y = valid->coords[index + 1];
                int final_x = valid->coords[index + 2];
                int final_y = valid->coords[index + 3];

                if (!(abs(initial_x - final_x) == 2 && abs(initial_y - final_y) == 2))
                    valid->eh_valido = false;
            }
        }
        
    } else {
        if (valid->coords.size() == 4) {
            int initial_x = valid->coords[0];
            int initial_y = valid->coords[1];
            int final_x = valid->coords[2];
            int final_y = valid->coords[3];

            if (abs(initial_x - final_x) == abs(initial_y - final_y)) 
                valid->eh_valido = true;

        } else {
            valid->eh_valido = true;

            for (int index = 0; index <= (valid->coords.size() - 4); index += 2) {
                int initial_x = valid->coords[index];
                int initial_y = valid->coords[index + 1];
                int final_x = valid->coords[index + 2];
                int final_y = valid->coords[index + 3];

                if (!(abs(initial_x - final_x) == abs(initial_y - final_y)))
                    valid->eh_valido = false;
            }
        }

    }

    return valid;
}
