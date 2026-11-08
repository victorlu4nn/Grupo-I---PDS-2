#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <memory>
#include <clocale>
#include <cstdlib>
#include <stdexcept>

#include "Damas.hpp"
#include "Move.hpp"
#include "Player.hpp"

Damas::Damas(): Game(
    "Damas",
    std::unique_ptr<Board>(new Board(8, 8)),
    std::unique_ptr<RuleBook>(new RuleBook(
        "Jogada: informe as casas separadas por espaco ou por 'para' (ex.: c3 d4 ou c3 para d4). "
        "Em capturas em cadeia informe todas as casas (ex.: c3 e5 c7 ou c3 para e5 para c7). "
        "Pedras andam uma casa na diagonal para frente e capturam para frente e para tras. "
        "Damas andam e capturam em qualquer diagonal. "
        "Uma captura em cadeia nao pode parar enquanto ainda houver peca a capturar. "
        "Comandos: 'empate' encerra empatado e 'abandonar' da a vitoria ao adversario. "
        "Empate apos 40 lances so com damas sem captura ou 5 lances em final reduzido."
    ))
) {
    Player* jogador1 = new Player("Jogador 1", "b");
    Player* jogador2 = new Player("Jogador 2", "p");

    jogadores.push_back(jogador1);
    jogadores.push_back(jogador2);

    quantidade_pecas_brancas = 0;
    quantidade_pecas_pretas = 0;

    configurarDisposicaoInicial();
}

Damas::Damas(Board* damasboard, RuleBook* damasrulebook): Game("Damas", damasboard, damasrulebook) {
    if (damasboard->getLinhas() != 8 || damasboard->getColunas() != 8)
        throw std::invalid_argument("Damas exige um tabuleiro 8x8.");

    Player* jogador1 = new Player("Jogador 1", "b");
    Player* jogador2 = new Player("Jogador 2", "p");

    jogadores.push_back(jogador1);
    jogadores.push_back(jogador2);

    quantidade_pecas_brancas = 0;
    quantidade_pecas_pretas = 0;

    configurarDisposicaoInicial();
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
            if (quantidades[0] == 1 && quantidades[1] == 1) {
                return true;
            }
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

    // Clean the board and counters (also used when the game restarts)
    damasboard->limpar();
    quantidade_pecas_brancas = 0;
    quantidade_pecas_pretas = 0;
    contagem_lances_dama = 0;
    contagem_finais_reduzidos = 0;
    id_vencedor = -1;

    // Configure white pieces
    bool eh_par = true;
    for (int x = 0; x <= 7; x++) {
        if (eh_par) {
            damasboard->atualizarPosicao(x, 0, new PecaDamas("\u26C0", 0));
            damasboard->atualizarPosicao(x, 2, new PecaDamas("\u26C0", 0));
            quantidade_pecas_brancas += 2;
        } else {
            damasboard->atualizarPosicao(x, 1, new PecaDamas("\u26C0", 0));
            quantidade_pecas_brancas++;
        }
        eh_par = !eh_par;
    }
    // Configure black pieces
    for (int x = 0; x <= 7; x++) {
        eh_par = !eh_par;    
        if (eh_par) {
            damasboard->atualizarPosicao(x, 5, new PecaDamas("\u26C2", 1));
            damasboard->atualizarPosicao(x, 7, new PecaDamas("\u26C2", 1));
            quantidade_pecas_pretas += 2;
        } else {
            damasboard->atualizarPosicao(x, 6, new PecaDamas("\u26C2", 1));
            quantidade_pecas_pretas++;
        }
    }

    // Amount of each type of piece on the board
    mapa_pecas_no_tabuleiro["\u26C2"] = quantidade_pecas_brancas;
    mapa_pecas_no_tabuleiro["\u26C3"] = 0;
    mapa_pecas_no_tabuleiro["\u26C0"] = quantidade_pecas_pretas;
    mapa_pecas_no_tabuleiro["\u26C1"] = 0;
}

std::vector<std::string> Damas::_lendo_movimento(const std::string acao) const {
    const std::string board_columns = "aAbBcCdDeEfFgGhH";
    const std::string board_lines = "12345678";

    std::stringstream input_stringstream(acao);
    std::vector<std::string> tokens;
    std::string token;

    std::vector<std::string> pattern_movement;

    while (input_stringstream >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() == 0) {
        std::cerr << "Erro. Nenhum comando informado." << std::endl;
        return {};
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
            return {};
        }
    }

    // "para" is optional, but it can only be between two positions
    bool ultimo_foi_para = true;

    for (std::string single_command: tokens){
        if (single_command == "para") {
            if (ultimo_foi_para) {
                std::cerr << "Erro. 'para' deve ficar entre duas casas." << std::endl;
                return {};
            }
            ultimo_foi_para = true;
            continue;
        }
        ultimo_foi_para = false;

        if (single_command.size() != 2) {
            std::cerr << "Erro. Comando de movimento invalido." << std::endl;
            return {};
        }

        size_t column_command = board_columns.find(single_command[0]);
        size_t line_command = board_lines.find(single_command[1]);
        
        if (column_command == std::string::npos || line_command == std::string::npos) {
            std::cerr << "Erro. Casa invalida." << std::endl;
            return {};
        }

        // Each column appears twice in board_columns (lower and upper case)
        int column_pattern = column_command / 2;
        int line_pattern = line_command;

        std::string pattern_move = std::to_string(column_pattern) + std::to_string(line_pattern);
        pattern_movement.push_back(pattern_move);
    }

    if (ultimo_foi_para || pattern_movement.size() < 2) {
        std::cerr << "Erro. Movimento precisa de uma posicao inicial e final." << std::endl;
        return {};
    }

    return pattern_movement;
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

// Checks if all squares between the initial and final positions are empty
bool caminho_livre(Board* tabuleiro, int x_inicial, int y_inicial, int x_final, int y_final) {
    int passo_x = 1;
    int passo_y = 1;

    if (x_final < x_inicial) 
        passo_x = -1;
    if (y_final < y_inicial) 
        passo_y = -1;

    for (int i = 1; i < abs(x_final - x_inicial); i++) {
        if (tabuleiro->consultarPosicao(x_inicial + i * passo_x, y_inicial + i * passo_y) != nullptr)
            return false;
    }
    return true;
}

// While the move is validated the piece is still on the board,
// so its initial position is considered empty
Piece* casa_durante_lance(Board* tabuleiro, int x, int y, int x_origem, int y_origem) {
    if (x == x_origem && y == y_origem)
        return nullptr;

    return tabuleiro->consultarPosicao(x, y);
}

bool ja_foi_capturada(Piece* peca, std::vector<PecaDamas*>& pecas_capturadas) {
    for (PecaDamas* capturada: pecas_capturadas) {
        if (capturada == peca)
            return true;
    }
    return false;
}

// Checks if the piece can still capture from (x, y). Captured pieces are still
// on the board, so they block the way and cannot be captured again
bool ainda_pode_capturar(
    Board* tabuleiro,
    PecaDamas* peca,
    int x,
    int y,
    int x_origem,
    int y_origem,
    std::vector<PecaDamas*>& pecas_capturadas
) {
    int direcoes[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    for (int d = 0; d < 4; d++) {
        int passo_x = direcoes[d][0];
        int passo_y = direcoes[d][1];
        int x_alvo = x + passo_x;
        int y_alvo = y + passo_y;

        // Dama walks through the empty squares until it finds a piece
        if (peca->get_eh_damas()) {
            while (tabuleiro->validarCoordenadas(x_alvo, y_alvo) &&
                   casa_durante_lance(tabuleiro, x_alvo, y_alvo, x_origem, y_origem) == nullptr) {
                x_alvo += passo_x;
                y_alvo += passo_y;
            }
        }

        int x_depois = x_alvo + passo_x;
        int y_depois = y_alvo + passo_y;

        if (!tabuleiro->validarCoordenadas(x_alvo, y_alvo) || !tabuleiro->validarCoordenadas(x_depois, y_depois))
            continue;

        Piece* alvo = casa_durante_lance(tabuleiro, x_alvo, y_alvo, x_origem, y_origem);
        Piece* depois = casa_durante_lance(tabuleiro, x_depois, y_depois, x_origem, y_origem);

        if (alvo != nullptr && alvo->ler_id_jogador() != peca->ler_id_jogador() &&
            !ja_foi_capturada(alvo, pecas_capturadas) && depois == nullptr)
            return true;
    }

    return false;
}

bool Damas::executarJogada(const std::string acao) {
    if (estadoPartida == "finalizada") {
        std::cerr << "Erro. A partida ja foi finalizada" << std::endl;
        return false;
    }

    std::vector<std::string> standardized_move = _lendo_movimento(acao);
    std::vector<PecaDamas*> pecas_capturadas;
    std::vector<int> posicoes_capturadas;
    bool lance_decisivo = false;
    bool houve_captura = false;

    if (standardized_move.size() == 0) {
        return false;
    }

    if (standardized_move[0] == "empate") {
        id_vencedor = -1;
        finalizarPartida(Vencedor(id_vencedor));
        return true;
    } else if (standardized_move[0] == "abandonar") {
        id_vencedor = (indiceJogadorDaVez + 1) % 2;
        finalizarPartida(Vencedor(id_vencedor));
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

    // Copy the result and free the memory allocated by _movimento_valido
    Movimento_Valido* resultado = peca_damas_de_movimento->_movimento_valido(standardized_move, tabuleiro);
    Movimento_Valido move = *resultado;
    delete resultado;

    if (!move.eh_valido) {
        std::cerr << "Erro. Deslocamento invalido para a peca" << std::endl;
        return false;
    }

    int tamanho = move.coords.size();

    for (int idx = 0; idx <= tamanho - 2; idx += 2) {
        int x = move.coords[idx];
        int y = move.coords[idx + 1];

        if (!tabuleiro->validarCoordenadas(x, y)) {
            std::cerr << "Erro. Coordenada fora dos limites do tabuleiro" << std::endl;
            return false;
        }
    }

    int final_x = move.coords[tamanho - 2];
    int final_y = move.coords[tamanho - 1];

    if (casa_durante_lance(tabuleiro, final_x, final_y, initial_x, initial_y) != nullptr) {
        std::cerr << "Erro. Posicao final ja possui uma peca alocada" << std::endl;
        return false;
    }

    bool era_dama = peca_damas_de_movimento->get_eh_damas();

    if (tamanho > 4) {
        // Multiple captures
        if (!_captura_valida(move, peca_damas_de_movimento, pecas_capturadas, posicoes_capturadas)) {
            return false;
        }
        houve_captura = true;
    } else {
        // Normal piece jumping 2 squares or dama with pieces on the way means capture
        bool tentativa_de_captura = false;
        if (era_dama) {
            tentativa_de_captura = !caminho_livre(tabuleiro, initial_x, initial_y, final_x, final_y);
        } else {
            tentativa_de_captura = abs(initial_x - final_x) == 2 && abs(initial_y - final_y) == 2;
        }

        if (tentativa_de_captura) {
            if (!_captura_valida(move, peca_damas_de_movimento, pecas_capturadas, posicoes_capturadas)) {
                std::cerr << "Erro. Movimentacao invalida, sem captura." << std::endl;
                return false;
            }
            houve_captura = true;
        } else if (verify_if_step_back(peca_damas_de_movimento, indiceJogadorDaVez, move.coords)) {
            std::cerr << "Erro. movimento nao pode ser feito na direcao contraria de locomocao" << std::endl;
            return false;
        }
    }

    // A capture cannot stop while there are still pieces to capture
    if (houve_captura && ainda_pode_capturar(tabuleiro, peca_damas_de_movimento, final_x, final_y,
                                             initial_x, initial_y, pecas_capturadas)) {
        std::cerr << "Erro. A captura nao pode parar no meio. Informe todas as casas da sequencia" << std::endl;
        return false;
    }

    // The whole move is valid, so now the captured pieces are removed
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

    // In a circular capture the piece ends on its initial position
    if (initial_x != final_x || initial_y != final_y) {
        Move movimento_Damas(
            jogadores[indiceJogadorDaVez], 
            initial_x,
            initial_y,
            final_x,
            final_y    
        );

        movimento_Damas.aplicar(tabuleiro);
    }

    contagem_lances_com_damas(peca_damas_de_movimento, houve_captura);

    final_tabuleiro(peca_damas_de_movimento, final_y);

    // Capture or promotion restart the reduced ending count
    lance_decisivo = houve_captura || (!era_dama && peca_damas_de_movimento->get_eh_damas());

    if (_verifica_final_reduzido()) {
        contagem_lances_final_reduzido(lance_decisivo);
    } else {
        contagem_finais_reduzidos = 0;
    }

    alternarTurno();

    // The player of the turn has no pieces or no moves left
    if (FimdeJogo(indiceJogadorDaVez)) {
        id_vencedor = (indiceJogadorDaVez + 1) % 2;
        finalizarPartida(Vencedor(id_vencedor));
    } else if (houve_empate()) {
        id_vencedor = -1;
        finalizarPartida(Vencedor(id_vencedor));
    }

    return true;
}

bool Damas::_captura_valida(
    Movimento_Valido acoes, 
    PecaDamas* peca_que_captura,
    std::vector<PecaDamas*>& pecas_capturadas,
    std::vector<int>& posicoes_capturadas
) {
    int tamanho = acoes.coords.size();

    if (peca_que_captura == nullptr || tamanho < 4) {
        return false;
    }

    // The captured pieces are only removed after the whole move is validated,
    // so they keep blocking the way until the end of the sequence
    int x_origem = acoes.coords[0];
    int y_origem = acoes.coords[1];

    if (!peca_que_captura->get_eh_damas()) {
        for (int index = 0; index <= tamanho - 4; index += 2){
            int xi = acoes.coords[index];
            int yi = acoes.coords[index + 1];
            int xf = acoes.coords[index + 2];
            int yf = acoes.coords[index + 3];

            if (casa_durante_lance(tabuleiro, xf, yf, x_origem, y_origem) != nullptr) {
                std::cerr << "Erro. Casa de destino de uma das capturas ja possui uma peca" << std::endl;
                return false;
            }

            int intermediario_x = (xi + xf)/2;
            int intermediario_y = (yi + yf)/2;
            
            Piece* peca_capturada = casa_durante_lance(tabuleiro, intermediario_x, intermediario_y, x_origem, y_origem);

            if (peca_capturada == nullptr || verifica_peca_eh_jogador_da_vez(peca_capturada, indiceJogadorDaVez)) {
                std::cerr << "Erro. Tentativa de captura invalida (nenhuma peca a ser capturada ou peca a ser captura pertence a voce)" << std::endl;
                return false; 
            }

            if (ja_foi_capturada(peca_capturada, pecas_capturadas)) {
                std::cerr << "Erro. Uma peca nao pode ser capturada duas vezes" << std::endl;
                return false;
            }

            posicoes_capturadas.push_back(intermediario_x);
            posicoes_capturadas.push_back(intermediario_y);

            PecaDamas* peca_dama_capturada = dynamic_cast<PecaDamas*>(peca_capturada);
            pecas_capturadas.push_back(peca_dama_capturada);
        }
    } else {
        for (int idx = 0; idx <= tamanho - 4; idx += 2) {
            unsigned short pecas_entre_lance = 0;

            int x_initial = acoes.coords[idx];
            int y_initial = acoes.coords[idx + 1];
            int x_lance = acoes.coords[idx + 2];
            int y_lance = acoes.coords[idx + 3];

            if (casa_durante_lance(tabuleiro, x_lance, y_lance, x_origem, y_origem) != nullptr) {
                std::cerr << "Erro. Casa de destino de uma das capturas ja possui uma peca" << std::endl;
                return false;
            }

            // Direction of the move
            int passo_x = 1;
            int passo_y = 1;

            if (x_lance < x_initial) 
                passo_x = -1;
            if (y_lance < y_initial) 
                passo_y = -1;

            PecaDamas* peca_capturada = nullptr;
            int x_capturada = 0;
            int y_capturada = 0;

            for (int i = 1; i < abs(x_initial - x_lance); i++) {
                int x = x_initial + i * passo_x;
                int y = y_initial + i * passo_y;

                Piece* peca_intermediaria = casa_durante_lance(tabuleiro, x, y, x_origem, y_origem);

                if (peca_intermediaria != nullptr) {
                    if (verifica_peca_eh_jogador_da_vez(peca_intermediaria, indiceJogadorDaVez)) {
                        std::cerr << "Erro. Lance passa por uma peca da mesma equipe" << std::endl;
                        return false;
                    } else if (ja_foi_capturada(peca_intermediaria, pecas_capturadas)) {
                        std::cerr << "Erro. Lance passa por uma peca ja capturada" << std::endl;
                        return false;
                    } else {
                        if (pecas_entre_lance == 1) {
                            std::cerr << "Erro. Lance passa por mais de uma peça de uma vez" << std::endl;
                            return false;
                        }
                        pecas_entre_lance++;
                        peca_capturada = dynamic_cast<PecaDamas*>(peca_intermediaria);
                        x_capturada = x;
                        y_capturada = y;
                    }
                }   
            }

            if (pecas_entre_lance == 0) {
                std::cerr << "Erro. Combinacao de movimentos invalida. Multiplas casas "
                    "sao permitidas apenas em captura. Nenhuma peca de captura em um dos movimentos" << std::endl;
                return false;
            } 

            posicoes_capturadas.push_back(x_capturada);
            posicoes_capturadas.push_back(y_capturada);
            pecas_capturadas.push_back(peca_capturada);
        }
    }

    return true;
}

bool Damas::_verifica_se_ha_movimento(int id_jogador) const {
    int direcoes[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    // White pieces move up (y + 1) and black pieces move down (y - 1)
    int frente = 1;
    if (id_jogador == 1)
        frente = -1;

    for (int x = 0; x <= 7; x++) {
        for (int y = 0; y <= 7; y++) {
            PecaDamas* peca = dynamic_cast<PecaDamas*>(tabuleiro->consultarPosicao(x, y));

            if (peca == nullptr || peca->ler_id_jogador() != id_jogador)
                continue;

            // For a dama it is enough to check the neighbours: any longer
            // move or capture starts with an empty neighbour
            for (int d = 0; d < 4; d++) {
                int x_vizinho = x + direcoes[d][0];
                int y_vizinho = y + direcoes[d][1];

                if (!tabuleiro->validarCoordenadas(x_vizinho, y_vizinho))
                    continue;

                Piece* vizinho = tabuleiro->consultarPosicao(x_vizinho, y_vizinho);

                // Empty square: dama moves in any direction, normal piece only forward
                if (vizinho == nullptr) {
                    if (peca->get_eh_damas() || direcoes[d][1] == frente)
                        return true;
                    continue;
                }

                // Enemy piece with an empty square after it: capture in any direction
                int x_depois = x_vizinho + direcoes[d][0];
                int y_depois = y_vizinho + direcoes[d][1];

                if (vizinho->ler_id_jogador() != id_jogador &&
                    tabuleiro->validarCoordenadas(x_depois, y_depois) &&
                    tabuleiro->consultarPosicao(x_depois, y_depois) == nullptr)
                    return true;
            }
        }
    }

    return false;
}

void Damas::final_tabuleiro(PecaDamas* peca, int y_final) {
    if (peca->get_eh_damas())
        return;

    if ((indiceJogadorDaVez == 0 && y_final == 7) || 
        (indiceJogadorDaVez == 1 && y_final == 0))
    {
        peca->promocao_dama();       
        if (indiceJogadorDaVez == 0) {
            mapa_pecas_no_tabuleiro.at("\u26C0") -= 1;
            mapa_pecas_no_tabuleiro.at("\u26C1") += 1;
        } else {
            mapa_pecas_no_tabuleiro.at("\u26C2") -= 1;
            mapa_pecas_no_tabuleiro.at("\u26C3") += 1;
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

void Damas::exibir(std::ostream& saida) const {
    saida << "\nDamas (brancas: \u26C2  dama \u26C3  | pretas: \u26C0  dama \u26C1 )\n\n";

    // Line 8 on top and line 1 on the bottom
    for (int y = 7; y >= 0; y--) {
        saida << y + 1 << "  ";
        for (int x = 0; x <= 7; x++) {
            Piece* peca = tabuleiro->consultarPosicao(x, y);
            // Pieces only stay on dark squares (x + y even)
            if (peca != nullptr)
                saida << peca->ler_simbolo() << "  ";
            else if ((x + y) % 2 == 0)
                saida << "\u25A0  ";
            else
                saida << "\u25A1  ";
        }
        saida << '\n';
    }
    saida << "\n   a  b  c  d  e  f  g  h\n";

    if (estadoPartida == "finalizada") {
        Player* ganhador = Vencedor(id_vencedor);
        if (ganhador != nullptr)
            saida << ganhador->getNome() << " venceu!\n";
        else
            saida << "Empate.\n";
    } else {
        std::string cor = "brancas";
        if (indiceJogadorDaVez == 1)
            cor = "pretas";

        saida << "Vez de " << jogadores[indiceJogadorDaVez]->getNome() << " (" << cor << ")."
              << " Exemplo: c3 d4 ou c3 para d4\n";
    }
}



//
//
// Implemented PecaDamas
//
//

PecaDamas::PecaDamas(std::string simb, int id): Piece(simb, id) {}

PecaDamas::~PecaDamas() {}

bool PecaDamas::get_eh_damas() const {
    return eh_dama;
}

void PecaDamas::atualizar_simbolo() {
    if (id_jogador == 0)
        simbolo = "\u26C1";
    else 
        simbolo = "\u26C3";
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

    if (tabuleiro_damas == nullptr)
        return valid;

    for (std::string coord : movimento_padronizado) {
        // Each position has two digits: column and line
        if (coord.size() != 2 || coord[0] < '0' || coord[0] > '9' || coord[1] < '0' || coord[1] > '9')
            return valid;

        int x = coord[0] - '0';
        int y = coord[1] - '0';

        if (!tabuleiro_damas->validarCoordenadas(x, y))
            return valid;

        valid->coords.push_back(x);
        valid->coords.push_back(y);
    }

    int tamanho = valid->coords.size();

    // A move needs at least an initial and a final position
    if (tamanho < 4)
        return valid;

    if (!eh_dama) {
        if (tamanho == 4) {
            int initial_x = valid->coords[0];
            int initial_y = valid->coords[1];
            int final_x = valid->coords[2];
            int final_y = valid->coords[3];

            if (
                initial_x != final_x &&
                (abs(initial_x - final_x) <= 2 && abs(initial_y - final_y) <= 2) &&
                (abs(initial_x - final_x) == abs(initial_y - final_y)) &&
                tabuleiro_damas->validarCoordenadas(initial_x, initial_y) &&
                tabuleiro_damas->validarCoordenadas(final_x, final_y)
            ) 
                valid->eh_valido = true;

        } else {
            valid->eh_valido = true;

            for (int index = 0; index <= tamanho - 4; index += 2) {
                int initial_x = valid->coords[index];
                int initial_y = valid->coords[index + 1];
                int final_x = valid->coords[index + 2];
                int final_y = valid->coords[index + 3];

                if ((!(abs(initial_x - final_x) == 2 && abs(initial_y - final_y) == 2)) ||
                !tabuleiro_damas->validarCoordenadas(initial_x, initial_y) ||
                !tabuleiro_damas->validarCoordenadas(final_x, final_y))
                    valid->eh_valido = false;
            }
        }
        
    } else {
        if (tamanho == 4) {
            int initial_x = valid->coords[0];
            int initial_y = valid->coords[1];
            int final_x = valid->coords[2];
            int final_y = valid->coords[3];

            if (initial_x != final_x && abs(initial_x - final_x) == abs(initial_y - final_y)) 
                valid->eh_valido = true;

        } else {
            valid->eh_valido = true;

            for (int index = 0; index <= tamanho - 4; index += 2) {
                int initial_x = valid->coords[index];
                int initial_y = valid->coords[index + 1];
                int final_x = valid->coords[index + 2];
                int final_y = valid->coords[index + 3];

                if (initial_x == final_x || !(abs(initial_x - final_x) == abs(initial_y - final_y)))
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
    if (!board_status.validarCoordenadas(x_orig, y_orig) || !board_status.validarCoordenadas(x_dest, y_dest))
        return false;

    // Each position is written as "xy", like in _lendo_movimento
    std::vector<std::string> movimento_padrozinado;
    movimento_padrozinado.push_back(std::to_string(x_orig) + std::to_string(y_orig));
    movimento_padrozinado.push_back(std::to_string(x_dest) + std::to_string(y_dest));

    Movimento_Valido* movimento_valido = _movimento_valido(movimento_padrozinado, &board_status);
    bool eh_valido = movimento_valido->eh_valido;
    delete movimento_valido;

    return eh_valido;
}
