#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "BatalhaNaval.hpp"

// Each player owns a 10x10 sea. Both seas live side by side on the same Board:
// player 1 -> columns 0-9, player 2 -> columns 10-19 (see BatalhaNaval.hpp)
static const int TAMANHO_MAR = 10;

// Classic fleet: carrier, battleship, cruiser, submarine and destroyer
static const int QUANTIDADE_NAVIOS = 5;
static const int FROTA[QUANTIDADE_NAVIOS] = {5, 4, 3, 3, 2};

// Up, down, left and right
static const int DIRECOES[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

static int id_oponente(int jogador) {
    return 3 - jogador;
}

// Translates a column of a player's sea into a column of the shared Board
static int coluna_no_tabuleiro(int jogador, int coluna) {
    return (jogador - 1) * TAMANHO_MAR + coluna;
}

static bool dentro_do_mar(int linha, int coluna) {
    return linha >= 0 && linha < TAMANHO_MAR && coluna >= 0 && coluna < TAMANHO_MAR;
}

// Returns the ship part at a position of the player's sea, or nullptr if there
// is no ship there (empty water, water marker or outside the sea)
static PecaBatalhaNaval* navio_em(Board* tabuleiro, int jogador, int linha, int coluna) {
    if (!dentro_do_mar(linha, coluna)) {
        return nullptr;
    }

    Piece* peca = tabuleiro->consultarPosicao(linha, coluna_no_tabuleiro(jogador, coluna));
    PecaBatalhaNaval* navio = dynamic_cast<PecaBatalhaNaval*>(peca);

    if (navio == nullptr || !navio->get_eh_navio()) {
        return nullptr;
    }
    return navio;
}

// Ships are straight lines and never touch each other, so a ship is sunk when
// every ship part in line with the hit position (until the first gap) was hit
static bool verifica_navio_afundado(Board* tabuleiro, int jogador, int linha, int coluna) {
    for (int i = 0; i < 4; i++) {
        int l = linha + DIRECOES[i][0];
        int c = coluna + DIRECOES[i][1];

        PecaBatalhaNaval* parte = navio_em(tabuleiro, jogador, l, c);
        while (parte != nullptr) {
            if (!parte->get_foi_atingido()) {
                return false;
            }
            l += DIRECOES[i][0];
            c += DIRECOES[i][1];
            parte = navio_em(tabuleiro, jogador, l, c);
        }
    }
    return true;
}

BatalhaNaval::BatalhaNaval(Board* tabuleiro, RuleBook* regras)
    : Game("Batalha Naval", tabuleiro, regras),
      quantidade_navios_jogador1(0),
      quantidade_navios_jogador2(0) {
    // Game already rejected null pointers, so it is safe to use the board here
    if (!tabuleiro->validarCoordenadas(TAMANHO_MAR - 1, 2 * TAMANHO_MAR - 1)) {
        throw std::invalid_argument(
            "O tabuleiro de Batalha Naval precisa ter ao menos 10 linhas e 20 colunas.");
    }

    // Without players in the list Game::alternarTurno() has nobody to switch to
    jogadores.push_back(new Player("Jogador 1", '1'));
    jogadores.push_back(new Player("Jogador 2", '2'));

    // Game starts the turn index at 1, but here the Jogador 1 (index 0) must
    // always play first, exactly as after Game::reiniciarPartida()
    indiceJogadorDaVez = 0;
}

BatalhaNaval::~BatalhaNaval() {
    for (Player* jogador : jogadores) {
        delete jogador;
    }
    jogadores.clear();
}

void BatalhaNaval::configurarDisposicaoInicial() {
    organizar_tabuleiro_batalha_naval();
}

void BatalhaNaval::organizar_tabuleiro_batalha_naval() {
    // Seeds the random generator only once per program
    static bool semente_definida = false;
    if (!semente_definida) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        semente_definida = true;
    }

    tabuleiro->limpar();
    quantidade_navios_jogador1 = 0;
    quantidade_navios_jogador2 = 0;

    for (int jogador = 1; jogador <= 2; jogador++) {
        for (int i = 0; i < QUANTIDADE_NAVIOS; i++) {
            // Tries random positions until the ship fits
            bool posicionado = false;

            for (int tentativa = 0; tentativa < 1000 && !posicionado; tentativa++) {
                int linha = std::rand() % TAMANHO_MAR;
                int coluna = std::rand() % TAMANHO_MAR;
                bool horizontal = (std::rand() % 2 == 0);

                posicionado = posicionar_navio(jogador, linha, coluna, FROTA[i], horizontal);
            }

            if (!posicionado) {
                throw std::runtime_error("Nao foi possivel posicionar a frota no tabuleiro.");
            }
        }
    }
}

bool BatalhaNaval::posicionar_navio(int jogador, int linha, int coluna, int tamanho, bool horizontal) {
    if ((jogador != 1 && jogador != 2) || tamanho <= 0) {
        return false;
    }

    int passo_linha = horizontal ? 0 : 1;
    int passo_coluna = horizontal ? 1 : 0;

    // The ship is a straight line, so both ends must be inside the sea
    int ultima_linha = linha + passo_linha * (tamanho - 1);
    int ultima_coluna = coluna + passo_coluna * (tamanho - 1);

    if (!dentro_do_mar(linha, coluna) || !dentro_do_mar(ultima_linha, ultima_coluna)) {
        return false;
    }

    // Every position must be free and must not touch another ship
    for (int i = 0; i < tamanho; i++) {
        int l = linha + passo_linha * i;
        int c = coluna + passo_coluna * i;

        if (tabuleiro->consultarPosicao(l, coluna_no_tabuleiro(jogador, c)) != nullptr) {
            return false;
        }

        for (int d = 0; d < 4; d++) {
            if (navio_em(tabuleiro, jogador, l + DIRECOES[d][0], c + DIRECOES[d][1]) != nullptr) {
                return false;
            }
        }
    }

    for (int i = 0; i < tamanho; i++) {
        int l = linha + passo_linha * i;
        int c = coluna + passo_coluna * i;

        tabuleiro->atualizarPosicao(l, coluna_no_tabuleiro(jogador, c), new PecaBatalhaNaval("N", jogador));
    }

    if (jogador == 1) {
        quantidade_navios_jogador1++;
    } else {
        quantidade_navios_jogador2++;
    }
    return true;
}

std::vector<int> BatalhaNaval::_lendo_movimento(const std::string acao) const {
    const std::string board_columns = "ABCDEFGHIJ";
    std::vector<int> movimento;

    // The command is a single word (e.g. "B5"), spaces around it are ignored
    std::stringstream input_stringstream(acao);
    std::string comando;
    std::string sobra;

    input_stringstream >> comando;
    if (comando.size() < 2 || comando.size() > 3 || (input_stringstream >> sobra)) {
        return movimento;
    }

    // First character is the column (A-J, upper or lower case)
    size_t coluna = board_columns.find(static_cast<char>(std::toupper(static_cast<unsigned char>(comando[0]))));
    if (coluna == std::string::npos) {
        return movimento;
    }

    // The rest is the line (1-10), without leading zeros
    std::string numero = comando.substr(1);
    if (numero.find_first_not_of("0123456789") != std::string::npos || numero[0] == '0') {
        return movimento;
    }

    int linha = std::stoi(numero);
    if (linha < 1 || linha > TAMANHO_MAR) {
        return movimento;
    }

    movimento.push_back(linha - 1);
    movimento.push_back(static_cast<int>(coluna));
    return movimento;
}

bool BatalhaNaval::_movimento_valido(int linha, int coluna) const {
    if (!dentro_do_mar(linha, coluna)) {
        return false;
    }

    int alvo = id_oponente(indiceJogadorDaVez + 1);
    Piece* peca = tabuleiro->consultarPosicao(linha, coluna_no_tabuleiro(alvo, coluna));

    // Empty water can always be attacked
    if (peca == nullptr) {
        return true;
    }

    // A ship part can be attacked only once. Misses leave a water marker,
    // which also cannot be attacked again
    PecaBatalhaNaval* navio = dynamic_cast<PecaBatalhaNaval*>(peca);
    return navio != nullptr && navio->get_eh_navio() && !navio->get_foi_atingido();
}

void BatalhaNaval::_executa_movimento(int linha, int coluna) {
    if (!_movimento_valido(linha, coluna)) {
        return;
    }

    int alvo = id_oponente(indiceJogadorDaVez + 1);
    int coluna_tabuleiro = coluna_no_tabuleiro(alvo, coluna);

    Piece* peca = tabuleiro->consultarPosicao(linha, coluna_tabuleiro);

    // Miss: leave a marker so the same position is not attacked twice
    if (peca == nullptr) {
        PecaBatalhaNaval* agua = new PecaBatalhaNaval("o", alvo, false);
        agua->atingir();
        tabuleiro->atualizarPosicao(linha, coluna_tabuleiro, agua);

        std::cout << "Agua!" << std::endl;
        return;
    }

    // Hit (_movimento_valido guarantees this is an intact ship part)
    PecaBatalhaNaval* navio = dynamic_cast<PecaBatalhaNaval*>(peca);
    navio->atingir();

    if (!verifica_navio_afundado(tabuleiro, alvo, linha, coluna)) {
        std::cout << "Acertou um navio!" << std::endl;
        return;
    }

    unsigned& restantes = (alvo == 1) ? quantidade_navios_jogador1 : quantidade_navios_jogador2;
    restantes--;
    std::cout << "Voce afundou um navio! Restam " << restantes << " para o oponente." << std::endl;

    if (restantes == 0) {
        finalizarPartida(jogadores[indiceJogadorDaVez]);
    }
}

bool BatalhaNaval::executarJogada(const std::string acao) {
    if (FimdeJogo()) {
        std::cerr << "Erro. A partida ja foi finalizada." << std::endl;
        return false;
    }

    std::vector<int> alvo = _lendo_movimento(acao);
    if (alvo.empty()) {
        std::cerr << "Erro. Comando invalido. Use uma letra de A a J seguida de um numero de 1 a 10 (ex: B5)."
                  << std::endl;
        return false;
    }

    if (!_movimento_valido(alvo[0], alvo[1])) {
        std::cerr << "Erro. Essa posicao ja foi atacada." << std::endl;
        return false;
    }

    _executa_movimento(alvo[0], alvo[1]);

    if (FimdeJogo()) {
        std::cout << "Fim de jogo! " << Vencedor()->getNome() << " venceu!" << std::endl;
    } else {
        alternarTurno();
    }
    return true;
}

unsigned BatalhaNaval::navios_restantes(int jogador) const {
    if (jogador == 1) {
        return quantidade_navios_jogador1;
    }
    if (jogador == 2) {
        return quantidade_navios_jogador2;
    }
    return 0;
}

bool BatalhaNaval::FimdeJogo() const {
    return estadoPartida == "finalizada";
}

Player* BatalhaNaval::Vencedor() const {
    // Nobody wins while the game is running or when it was just abandoned
    // (finalizarPartida(nullptr) with both fleets still afloat)
    if (!FimdeJogo()) {
        return nullptr;
    }
    if (quantidade_navios_jogador2 == 0 && quantidade_navios_jogador1 > 0) {
        return jogadores[0];
    }
    if (quantidade_navios_jogador1 == 0 && quantidade_navios_jogador2 > 0) {
        return jogadores[1];
    }
    return nullptr;
}

PecaBatalhaNaval::PecaBatalhaNaval(std::string simb, int id_jogador, bool navio)
    : Piece(simb, id_jogador), eh_navio(navio), foi_atingido(false) {}

PecaBatalhaNaval::~PecaBatalhaNaval() {}

bool PecaBatalhaNaval::get_eh_navio() const {
    return eh_navio;
}

bool PecaBatalhaNaval::get_foi_atingido() const {
    return foi_atingido;
}

void PecaBatalhaNaval::atingir() {
    foi_atingido = true;
    set_status(PieceStatus::INATIVA);
}

// Pieces of Batalha Naval never move after being placed
bool PecaBatalhaNaval::movimento_valido(int, int, int, int, const Board&) const {
    return false;
}