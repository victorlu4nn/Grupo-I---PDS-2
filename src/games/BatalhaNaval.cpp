#include <cctype>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "BatalhaNaval.hpp"

namespace {

// Each player owns a 10x10 sea. Both seas live side by side on the same Board:
// player 1 -> columns 0-9, player 2 -> columns 10-19 (see BatalhaNaval.hpp).
constexpr int TAMANHO_MAR = 10;

// Classic fleet: carrier, battleship, cruiser, submarine and destroyer.
// Placed from the biggest to the smallest to make random placement reliable.
const std::vector<int> FROTA = {5, 4, 3, 3, 2};

const int DIRECOES[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int oponente(int jogador) {
    return 3 - jogador;
}

// Translates a column of a player's sea into a column of the shared Board
int coluna_no_tabuleiro(int jogador, int coluna) {
    return (jogador - 1) * TAMANHO_MAR + coluna;
}

bool dentro_do_mar(int linha, int coluna) {
    return linha >= 0 && linha < TAMANHO_MAR && coluna >= 0 && coluna < TAMANHO_MAR;
}

// Returns the ship part at (linha, coluna) of the player's sea, or nullptr if
// the position is outside the sea, empty or only holds a "water" marker.
PecaBatalhaNaval* navio_em(const Board& tabuleiro, int jogador, int linha, int coluna) {
    if (!dentro_do_mar(linha, coluna)) {
        return nullptr;
    }
    Piece* peca = tabuleiro.consultarPosicao(linha, coluna_no_tabuleiro(jogador, coluna));
    PecaBatalhaNaval* navio = dynamic_cast<PecaBatalhaNaval*>(peca);
    if (navio == nullptr || !navio->get_eh_navio()) {
        return nullptr;
    }
    return navio;
}

// A ship is the set of ship parts connected orthogonally (ships are never
// allowed to touch each other, so this set is unambiguous). It is sunk when
// every one of its parts has been hit.
bool navio_afundou(const Board& tabuleiro, int jogador, int linha, int coluna) {
    std::vector<std::pair<int, int>> pendentes = {{linha, coluna}};
    std::set<std::pair<int, int>> visitadas = {{linha, coluna}};

    while (!pendentes.empty()) {
        std::pair<int, int> atual = pendentes.back();
        pendentes.pop_back();

        PecaBatalhaNaval* parte = navio_em(tabuleiro, jogador, atual.first, atual.second);
        if (parte == nullptr) {
            continue;
        }
        if (!parte->get_foi_atingido()) {
            return false;
        }

        for (const auto& direcao : DIRECOES) {
            std::pair<int, int> vizinha = {atual.first + direcao[0], atual.second + direcao[1]};
            if (navio_em(tabuleiro, jogador, vizinha.first, vizinha.second) != nullptr
                && visitadas.insert(vizinha).second) {
                pendentes.push_back(vizinha);
            }
        }
    }
    return true;
}

}  // namespace

// ---------------------------------------------------------------------------
// BatalhaNaval
// ---------------------------------------------------------------------------

BatalhaNaval::BatalhaNaval(Board* tabuleiro, RuleBook* regras)
    : Game("Batalha Naval", tabuleiro, regras),
      quantidade_navios_jogador1(0),
      quantidade_navios_jogador2(0) {
    // Game already rejected null pointers, so it is safe to use the board here
    if (!tabuleiro->validarCoordenadas(TAMANHO_MAR - 1, 2 * TAMANHO_MAR - 1)) {
        throw std::invalid_argument(
            "O tabuleiro de Batalha Naval precisa ter ao menos 10 linhas e 20 colunas.");
    }

    // Without players in the list Game::alternarTurno() would never switch turns
    jogadores.push_back(new Player("Jogador 1", '1'));
    jogadores.push_back(new Player("Jogador 2", '2'));
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
    tabuleiro->limpar();
    quantidade_navios_jogador1 = 0;
    quantidade_navios_jogador2 = 0;

    std::mt19937 gerador{std::random_device{}()};
    std::uniform_int_distribution<int> posicao(0, TAMANHO_MAR - 1);
    std::bernoulli_distribution sorteio_horizontal(0.5);

    for (int jogador = 1; jogador <= 2; ++jogador) {
        for (int tamanho : FROTA) {
            bool posicionado = false;

            for (int tentativa = 0; tentativa < 1000 && !posicionado; ++tentativa) {
                int linha = posicao(gerador);
                int coluna = posicao(gerador);
                bool horizontal = sorteio_horizontal(gerador);
                posicionado = posicionar_navio(jogador, linha, coluna, tamanho, horizontal);
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

    const int passo_linha = horizontal ? 0 : 1;
    const int passo_coluna = horizontal ? 1 : 0;

    // Both ends must be inside the sea (the ship is a straight line)
    if (!dentro_do_mar(linha, coluna)
        || !dentro_do_mar(linha + passo_linha * (tamanho - 1), coluna + passo_coluna * (tamanho - 1))) {
        return false;
    }

    // Every cell must be free and must not touch another ship
    for (int i = 0; i < tamanho; ++i) {
        int l = linha + passo_linha * i;
        int c = coluna + passo_coluna * i;

        if (tabuleiro->consultarPosicao(l, coluna_no_tabuleiro(jogador, c)) != nullptr) {
            return false;
        }
        for (const auto& direcao : DIRECOES) {
            if (navio_em(*tabuleiro, jogador, l + direcao[0], c + direcao[1]) != nullptr) {
                return false;
            }
        }
    }

    for (int i = 0; i < tamanho; ++i) {
        int l = linha + passo_linha * i;
        int c = coluna + passo_coluna * i;
        tabuleiro->atualizarPosicao(l, coluna_no_tabuleiro(jogador, c), new PecaBatalhaNaval("N", jogador));
    }

    if (jogador == 1) {
        ++quantidade_navios_jogador1;
    } else {
        ++quantidade_navios_jogador2;
    }
    return true;
}

std::vector<int> BatalhaNaval::_lendo_movimento(const std::string acao) const {
    // Accepts a letter A-J (column) followed by a number 1-10 (line), e.g. "B5".
    // Spaces around the command and lower case letters are tolerated.
    const std::string espacos = " \t\r\n";
    const size_t inicio = acao.find_first_not_of(espacos);
    if (inicio == std::string::npos) {
        return {};
    }
    const size_t fim = acao.find_last_not_of(espacos);
    const std::string comando = acao.substr(inicio, fim - inicio + 1);

    if (comando.size() < 2 || comando.size() > 3) {
        return {};
    }

    const char letra = static_cast<char>(std::toupper(static_cast<unsigned char>(comando[0])));
    if (letra < 'A' || letra >= 'A' + TAMANHO_MAR) {
        return {};
    }

    const std::string numero = comando.substr(1);
    if (numero[0] == '0') {
        return {};
    }
    for (char c : numero) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return {};
        }
    }

    const int linha = std::stoi(numero);
    if (linha < 1 || linha > TAMANHO_MAR) {
        return {};
    }

    return {linha - 1, letra - 'A'};
}

bool BatalhaNaval::_movimento_valido(int linha, int coluna) const {
    if (!dentro_do_mar(linha, coluna)) {
        return false;
    }

    const int alvo = oponente(indiceJogadorDaVez + 1);
    Piece* peca = tabuleiro->consultarPosicao(linha, coluna_no_tabuleiro(alvo, coluna));

    // Empty water can always be attacked
    if (peca == nullptr) {
        return true;
    }

    // A ship part can be attacked only once. Misses leave a "water" marker,
    // which also cannot be attacked again.
    PecaBatalhaNaval* navio = dynamic_cast<PecaBatalhaNaval*>(peca);
    return navio != nullptr && navio->get_eh_navio() && !navio->get_foi_atingido();
}

void BatalhaNaval::_executa_movimento(int linha, int coluna) {
    if (!_movimento_valido(linha, coluna)) {
        return;
    }

    const int atacante = indiceJogadorDaVez + 1;
    const int alvo = oponente(atacante);
    const int coluna_tabuleiro = coluna_no_tabuleiro(alvo, coluna);

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
    static_cast<PecaBatalhaNaval*>(peca)->atingir();

    if (!navio_afundou(*tabuleiro, alvo, linha, coluna)) {
        std::cout << "Acertou um navio!" << std::endl;
        return;
    }

    unsigned& restantes = (alvo == 1) ? quantidade_navios_jogador1 : quantidade_navios_jogador2;
    --restantes;
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

// ---------------------------------------------------------------------------
// PecaBatalhaNaval
// ---------------------------------------------------------------------------

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
