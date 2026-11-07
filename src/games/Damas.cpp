#include <iostream>
#include <map>
#include <vector>
#include <sstream>

#include "Damas.hpp"
#include "Move.hpp"
#include "Player.hpp"

Damas::Damas(Board* damasboard, RuleBook* damasrulebook): Game("Damas", damasboard, damasrulebook) {
    Player* jogador1 = new Player("Jogador 1", 'b');
    Player* jogador2 = new Player("Jogador 2", 'p');

    jogadores.push_back(jogador1);
    jogadores.push_back(jogador2);
}

Damas::~Damas(){
    tabuleiro->limpar();
    for (Player* jogador: jogadores) {
        delete jogador;
    }
}

// Private Methods

void Damas::contagem_lances_com_damas(PecaDamas* Peca, bool captura) {
    if (!captura && Peca->get_eh_damas()) 
        contagem_lances_dama++;
    else
        contagem_lances_dama = 0;
}

bool Damas::_verifica_final_reduzido() const {
    if (quantidade_pecas_brancas > 2 || quantidade_pecas_pretas > 2)
        return false;

    std::vector<std::string> simbolos_pecas = {
        "\u26C3", "\u26C2", "\u26C1", "\u26C0"
    };
    std::vector<unsigned short> quantidades; 
    
    for (std::string simbolo: simbolos_pecas) {
        unsigned short quantidade = mapa_pecas_no_tabuleiro.at(simbolo);
        quantidades.push_back(quantidade);
    }

    if (quantidades[0] > 0 && quantidades[2] > 0) {
        if (quantidades[0] == 2 && quantidades[1] == 0) {
            if (
                (quantidades[2] == 2 && quantidades[3] == 0) ||
                (quantidades[2] == 1 && quantidades[3] == 1) ||
                (quantidades[2] == 1 && quantidades[3] == 0)
            ) { return true; }
        } else if (quantidades[2] == 2 && quantidades[3] == 0) {
            if (
                (quantidades[0] == 1 && quantidades[1] == 1) ||
                (quantidades[0] == 1 && quantidades[1] == 0)
            ) { return true; }
        } else if (quantidades[0] == 1 && quantidades[1] == 0) {
            if (
                (quantidades[2] == 1 && quantidades[3] == 1) ||
                (quantidades[2] == 1 && quantidades[3] == 0)
            ) { return true; }
        } else if (quantidades[2] == 1 && quantidades[3] == 0) {
            if (quantidades[0] == 1 && quantidades[1] == 1)
                return true;
        }
    }

    return false;
}


void Damas::contagem_lances_final_reduzido(bool lance_decisivo) {
    if (!lance_decisivo)
        contagem_finais_reduzidos++;
    else
        contagem_finais_reduzidos = 0;
}


bool Damas::houve_empate() const {
    if (contagem_lances_dama == 40 || contagem_finais_reduzidos == 5) {
        return true;
    }
    return false;
}



// Public Methods

void Damas::configurarDisposicaoInicial() {
    organizar_tabuleiro_damas(tabuleiro);
}

bool verifica_peca_eh_jogador_da_vez(Piece* peca, int id) {
    return peca->ler_id_jogador() == id;
}

void Damas::organizar_tabuleiro_damas(Board* damasboard) {
    // Configura o terminal para aceitar UTF-8 (necessário em alguns sistemas como Windows)
    std::setlocale(LC_ALL, "en_US.UTF-8"); 

    // Configure white pieces
    bool eh_par = true;
    for (int x = 0; x <= 7; x++) {
        if (eh_par) {
            damasboard->atualizarPosicao(x, 0, new PecaDamas("\u26C2", 0));
            damasboard->atualizarPosicao(x, 2, new PecaDamas("\u26C2", 0));
            quantidade_pecas_brancas += 2;
        } else {
            damasboard->atualizarPosicao(x, 1, new PecaDamas("\u26C2", 0));
            quantidade_pecas_brancas++;
        }
        eh_par = !eh_par;
    }
    // Configure black pieces
    for (int x = 0; x <= 7; x++) {
        eh_par = !eh_par;    
        if (eh_par) {
            damasboard->atualizarPosicao(x, 5, new PecaDamas("\u26C0", 1));
            damasboard->atualizarPosicao(x, 7, new PecaDamas("\u26C0", 1));
            quantidade_pecas_pretas += 2;
        } else {
            damasboard->atualizarPosicao(x, 6, new PecaDamas("\u26C0", 1));
            quantidade_pecas_pretas++;
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
        if (tokens[0] == "empate") {
            pattern_movement = {"empate"};
            return pattern_movement;
        } else if (tokens[0] == "abandonar") {
            pattern_movement = {"abandonar"};
            return pattern_movement;
        } else {
            std::cerr << "Erro. Movimento precisa de uma posicao inicial e final." << std::endl;
        }
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
    std::vector<PecaDamas*> pecas_capturadas;
    std::vector<int> posicoes_capturadas;
    bool lance_decisivo = false;

    if (standardized_move[0] == "empate") {
        Vencedor(-1);
        estadoPartida = "finalizada";
        return true;
    } else if (standardized_move[0] == "abandonar") {
        int id_ganhador = (indiceJogadorDaVez + 1) % 2;
        Vencedor(id_ganhador);
        estadoPartida = "finalizada";
        return true;
    }

    std::string posicao_inicial = standardized_move[0];
    int initial_x = posicao_inicial[0] - '0';
    int initial_y = posicao_inicial[1] - '0';
      
    Piece* peca_de_movimento = tabuleiro->consultarPosicao(initial_x, initial_y);
    PecaDamas* peca_damas_de_movimento = dynamic_cast<PecaDamas*>(peca_de_movimento);

    // verify if the place is occupied by any piece
    // and if it is, get this piece to move
    if (peca_damas_de_movimento == nullptr) {
        std::cerr << "Erro. Nenhuma peca na posicao inicial" << std::endl;
        return false;

    } else if (!verifica_peca_eh_jogador_da_vez(peca_damas_de_movimento, indiceJogadorDaVez)) {
        std::cerr << "Erro. Peca da posicao inicial nao pertence a voce" << std::endl;
        return false;
    }

    Movimento_Valido* move = peca_damas_de_movimento->_movimento_valido(standardized_move, tabuleiro);

    int final_x = move->coords[move->coords.size() - 2];
    int final_y = move->coords[move->coords.size() - 1];

    if (tabuleiro->consultarPosicao(final_x, final_y) != nullptr) {
        std::cerr << "Erro. Posicao final ja possui uma peca alocada" << std::endl;
        return false;

    } else if (!move->eh_valido) {
        std::cerr << "Erro. Deslocamento invalido para a peca" << std::endl;
        return false;
    }

    for (int idx = 0; idx <= move->coords.size() - 2; idx += 2) {
        int x = move->coords[idx];
        int y = move->coords[idx + 1];

        if (!tabuleiro->validarCoordenadas(x, y)) {
            std::cerr << "Erro. Coordenada fora dos limites do tabuleiro" << std::endl;
            return false;
        }
    }

    if (move->coords.size() > 2) {
        if (_captura_valida(*move, peca_damas_de_movimento, pecas_capturadas, posicoes_capturadas)) {
            int pecas_idx = 0;
            for (PecaDamas* peca: pecas_capturadas) {
                int x_peca = posicoes_capturadas[pecas_idx];
                pecas_idx++;
                int y_peca = posicoes_capturadas[pecas_idx];

                mapa_pecas_no_tabuleiro.at(peca->ler_simbolo()) -= 1;

                tabuleiro->removerPeca(x_peca, y_peca);

                if (peca_damas_de_movimento->ler_id_jogador() == 0) {
                    quantidade_pecas_pretas--;
                } else {
                    quantidade_pecas_brancas--;
                }

                pecas_idx++;
            }

            Move movimento_Damas = Move(
                jogadores[indiceJogadorDaVez], 
                initial_x,
                initial_y,
                final_x,
                final_y    
            );

            movimento_Damas.aplicar(tabuleiro);

            return true;
        }
    } else {
        if (abs(initial_x - final_x) == 2 && abs(initial_y - final_y) == 2) {
            if (_captura_valida(*move, peca_damas_de_movimento, pecas_capturadas, posicoes_capturadas)) {
                int x_peca = posicoes_capturadas[0];
                int y_peca = posicoes_capturadas[1];

                mapa_pecas_no_tabuleiro.at(pecas_capturadas[0]->ler_simbolo()) -= 1;
                
                tabuleiro->removerPeca(x_peca, y_peca);

                if (peca_damas_de_movimento->ler_id_jogador() == 0) {
                    quantidade_pecas_pretas--;
                } else {
                    quantidade_pecas_brancas--;
                }

                Move movimento_Damas = Move(
                    jogadores[indiceJogadorDaVez], 
                    initial_x,
                    initial_y,
                    final_x,
                    final_y    
                );

                movimento_Damas.aplicar(tabuleiro);

                return true;
            } else {
                std::cerr << "Erro. Movimentacao invalida, sem captura." << std::endl;
            }
        } else {
            if (verify_if_step_back(peca_damas_de_movimento, indiceJogadorDaVez, move->coords)) {
                std::cerr << "Erro. movimento nao pode ser feito na direcao contraria de locomocao" << std::endl;
            }

            Move movimento_Damas = Move(
                jogadores[indiceJogadorDaVez], 
                initial_x,
                initial_y,
                final_x,
                final_y    
            );

            movimento_Damas.aplicar(tabuleiro);

            return true;
        }
    }
}

bool verify_if_step_back(PecaDamas* peca, int id, std::vector<int> coords) {
    if (!peca->get_eh_damas()) {
        int y0 = coords[1];
        int y1 = coords[3];

        if (id == 0 && y1 < y0) {
            return true;
        } else if (id == 1 && y1 > y0) {
            return true;
        }
    }

    return false;
}

bool Damas::_captura_valida(
    Movimento_Valido acoes, 
    PecaDamas* peca_que_captura,
    std::vector<PecaDamas*>& pecas_capturadas,
    std::vector<int>& posicoes_capturadas
) {
    if (!peca_que_captura->get_eh_damas()) {
        for (int index = 0; index <= (acoes.coords.size() - 4); index += 2){
            int xi = acoes.coords[index];
            int yi = acoes.coords[index + 1];
            int xf = acoes.coords[index + 2];
            int yf = acoes.coords[index + 3];

            int intermediario_x = (xi + xf)/2;
            int intermediario_y = (yi + yf)/2;
            
            Piece* peca_capturada = tabuleiro->consultarPosicao(intermediario_x, intermediario_y);

            if (peca_capturada == nullptr || verifica_peca_eh_jogador_da_vez(peca_capturada, indiceJogadorDaVez)) {
                std::cerr << "Erro. Tentativa de captura invalida (nenhuma peca a ser capturada ou peca a ser captura pertence a voce)" << std::endl;
                return false; 
            }
            posicoes_capturadas.push_back(intermediario_x);
            posicoes_capturadas.push_back(intermediario_y);

            PecaDamas* peca_dama_capturada = dynamic_cast<PecaDamas*>(peca_capturada);
            pecas_capturadas.push_back(peca_dama_capturada);
        }
    } else {
        for (int idx = 0; idx <= acoes.coords.size() - 4; idx += 2) {
            unsigned short pecas_entre_lance = 0;

            int x_initial = acoes.coords[idx];
            int y_initial = acoes.coords[idx + 1];
            int x_lance = acoes.coords[idx + 2];
            int y_lance = acoes.coords[idx + 3];

            PecaDamas* peca_capturada = nullptr;

            for (int i = 1; i <= abs(x_initial - x_lance); i++) {
                Piece* peca_intermediaria = tabuleiro->consultarPosicao(x_initial + i, y_initial + i);
                PecaDamas* peca_capturada = dynamic_cast<PecaDamas*>(peca_intermediaria);

                if (peca_intermediaria != nullptr) {
                    if (verifica_peca_eh_jogador_da_vez(peca_intermediaria, indiceJogadorDaVez)) {
                        std::cerr << "Erro. Lance passa por uma peca da mesma equipe" << std::endl;
                    } else {
                        if (pecas_entre_lance == 1) {
                            std::cerr << "Erro. Lance passa por mais de uma peça de uma vez" << std::endl;
                        }
                        pecas_entre_lance++;
                    }
                }   
            }

            if (pecas_entre_lance == 0 && acoes.coords.size() > 2) {
                std::cerr << "Erro. Combinacao de movimentos invalida. Multiplas casas"
                    "sao permitidas apenas em captura. Nenhuma peca de captura em um dos movimentos" << std::endl;
            } 

            pecas_capturadas.push_back(peca_capturada);
        }
    }

    return true;
}

void Damas::final_tabuleiro(PecaDamas* peca, int y_final) {
    if ((indiceJogadorDaVez == 0 && y_final == 7) || 
        (indiceJogadorDaVez == 1 && y_final == 0))
    {
        peca->promocao_dama();       
        if (indiceJogadorDaVez == 0) {
            mapa_pecas_no_tabuleiro.at("\u26C2") -= 1;
            mapa_pecas_no_tabuleiro.at("\u26C3") += 1;
        } else {
            mapa_pecas_no_tabuleiro.at("\u26C0") -= 1;
            mapa_pecas_no_tabuleiro.at("\u26C1") += 1;
    } 
    }
}


bool Damas::FimdeJogo(int id_jogador) const {
    if (quantidade_pecas_brancas == 0 || quantidade_pecas_pretas == 0 || !_verifica_se_ha_movimento(id_jogador)) {
        return true;
    }
    return false;
}


Player* Damas::Vencedor(int id_ganhador) const {
    if (id_ganhador == -1) 
        return nullptr;
    
    Player* ganhador = jogadores[id_ganhador];
    return ganhador;
}



//
//
// Implemented PecaDamas
//
//

PecaDamas::PecaDamas(std::string simb, int id): Piece(simb, id) {};

PecaDamas::~PecaDamas() {};

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
    std::vector<std::string> movimento_padronizado,
    const Board* tabuleiro_damas
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
                (abs(initial_x - final_x) == abs(initial_y - final_y)) &&
                tabuleiro_damas->validarCoordenadas(initial_x, initial_y) &&
                tabuleiro_damas->validarCoordenadas(final_x, final_y)
            ) 
                valid->eh_valido = true;

        } else {
            valid->eh_valido = true;

            for (int index = 0; index <= (valid->coords.size() - 4); index += 2) {
                int initial_x = valid->coords[index];
                int initial_y = valid->coords[index + 1];
                int final_x = valid->coords[index + 2];
                int final_y = valid->coords[index + 3];

                if ((!(abs(initial_x - final_x) == 2 && abs(initial_y - final_y) == 2)) &&
                !tabuleiro_damas->validarCoordenadas(initial_x, initial_y) &&
                !tabuleiro_damas->validarCoordenadas(final_x, final_y))
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

bool PecaDamas::movimento_valido(
    int x_orig,
    int y_orig,
    int x_dest,
    int y_dest,
    const Board& board_status
) const {
    std::vector<std::string> movimento_padrozinado;
    movimento_padrozinado.push_back(std::to_string(x_orig));
    movimento_padrozinado.push_back(std::to_string(y_orig));
    movimento_padrozinado.push_back(std::to_string(x_dest));
    movimento_padrozinado.push_back(std::to_string(y_dest));

    Movimento_Valido* movimento_valido = _movimento_valido(movimento_padrozinado, &board_status);
    if (movimento_valido->eh_valido)
        return true;
    else 
        return false;
}