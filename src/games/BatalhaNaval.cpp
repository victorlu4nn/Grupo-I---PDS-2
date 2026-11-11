#include <cctype>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <random>
#include "Validacao.hpp"
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

BatalhaNaval::BatalhaNaval()
    : Game("Batalha Naval", std::unique_ptr<Board>(new Board(10, 20)),
           std::unique_ptr<RuleBook>(new RuleBook(
               "Ataque usando A1 a J10. Cada jogador possui cinco navios. "
               "Nao repita ataques. A vez alterna mesmo ao acertar. "
               "Vence quem afundar toda a frota adversaria."))),
      quantidade_navios_jogador1(0), quantidade_navios_jogador2(0) {
    criarJogadores("1", "2");
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

    criarJogadores("1", "2");
}

BatalhaNaval::~BatalhaNaval() = default;

void BatalhaNaval::configurarDisposicaoInicial() {
    organizar_tabuleiro_batalha_naval();
}

void BatalhaNaval::organizar_tabuleiro_batalha_naval() {
    // Monta em um tabuleiro temporario. Se faltar memoria ou nao houver
    // posicionamento, a partida anterior continua intacta.
    Board novo(tabuleiro->getLinhas(), tabuleiro->getColunas());
    BatalhaNaval preparacao(&novo, regras);
    static std::mt19937 gerador(std::random_device{}());
    std::uniform_int_distribution<int> coordenada(0, 9);
    std::uniform_int_distribution<int> orientacao(0, 1);
    for (int jogador = 1; jogador <= 2; ++jogador) {
        for (int tamanho : FROTA) {
            bool posicionado = false;
            for (int tentativa = 0; tentativa < 1000 && !posicionado; ++tentativa)
                posicionado = preparacao.posicionar_navio(
                    jogador, coordenada(gerador), coordenada(gerador), tamanho,
                    orientacao(gerador) != 0);
            if (!posicionado)
                throw std::runtime_error("Nao foi possivel posicionar a frota. Tente reiniciar novamente.");
        }
    }
    estadoPartida = "em_andamento";
    tabuleiro->trocarConteudo(novo);
    quantidade_navios_jogador1 = preparacao.quantidade_navios_jogador1;
    quantidade_navios_jogador2 = preparacao.quantidade_navios_jogador2;
    indiceJogadorDaVez = 0;
    ataques_iniciados = false;
}

bool BatalhaNaval::posicionar_navio(int jogador, int linha, int coluna, int tamanho, bool horizontal) {
    if (ataques_iniciados || FimdeJogo() || (jogador != 1 && jogador != 2) ||
        tamanho <= 0 || tamanho > TAMANHO_MAR || !dentro_do_mar(linha, coluna)) {
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

    // Aloca o navio inteiro antes de inserir qualquer segmento.
    std::vector<std::unique_ptr<Piece>> partes;
    for (int i = 0; i < tamanho; ++i)
        partes.emplace_back(new PecaBatalhaNaval("N", jogador));
    for (int i = 0; i < tamanho; ++i)
        tabuleiro->inserirPeca(linha + passo_linha * i,
            coluna_no_tabuleiro(jogador, coluna + passo_coluna * i), std::move(partes[i]));

    if (jogador == 1) {
        quantidade_navios_jogador1++;
    } else {
        quantidade_navios_jogador2++;
    }
    return true;
}

std::vector<int> BatalhaNaval::_lendo_movimento(const std::string acao) const {
    if (!validacao::comandoValido(acao)) return {};
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
    if (FimdeJogo() || !dentro_do_mar(linha, coluna) ||
        quantidade_navios_jogador1 == 0 || quantidade_navios_jogador2 == 0) return false;
    validarJogadorDaVez();

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
        throw std::invalid_argument("Ataque invalido: confira a coordenada, as frotas e os tiros anteriores.");
    }

    int alvo = id_oponente(indiceJogadorDaVez + 1);
    int coluna_tabuleiro = coluna_no_tabuleiro(alvo, coluna);

    Piece* peca = tabuleiro->consultarPosicao(linha, coluna_tabuleiro);

    // Miss: leave a marker so the same position is not attacked twice
    if (peca == nullptr) {
        std::unique_ptr<PecaBatalhaNaval> agua(new PecaBatalhaNaval("o", alvo, false));
        agua->atingir();
        tabuleiro->inserirPeca(linha, coluna_tabuleiro, std::move(agua));
        ataques_iniciados = true;

        std::cout << "Agua!" << std::endl;
        return;
    }

    // Hit (_movimento_valido guarantees this is an intact ship part)
    PecaBatalhaNaval* navio = dynamic_cast<PecaBatalhaNaval*>(peca);
    unsigned& restantes = (alvo == 1) ? quantidade_navios_jogador1 : quantidade_navios_jogador2;
    if (!navio || restantes == 0) throw std::logic_error("Estado da frota inconsistente.");
    navio->atingir();
    ataques_iniciados = true;

    if (!verifica_navio_afundado(tabuleiro, alvo, linha, coluna)) {
        std::cout << "Acertou um navio!" << std::endl;
        return;
    }

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

    // When the game ends the winner is shown by exibir
    if (!FimdeJogo()) {
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
    : Piece(simb, id_jogador), eh_navio(navio), foi_atingido(false) {
    if (id_jogador != 1 && id_jogador != 2)
        throw std::invalid_argument("Jogador da Batalha Naval deve ser 1 ou 2.");
}

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
void BatalhaNaval::exibir(std::ostream& saida) const {
    validarJogadorDaVez();
    const int jogador = indiceJogadorDaVez + 1;
    saida << "\nBatalha Naval | Vez de " << jogadores[indiceJogadorDaVez]->getNome()
          << "\nSeus ataques (navios adversarios intactos ficam ocultos):\n  A B C D E F G H I J\n";
    for (int l = 0; l < TAMANHO_MAR; ++l) {
        saida << l + 1 << (l == 9 ? " " : "  ");
        for (int c = 0; c < TAMANHO_MAR; ++c) {
            const auto* p = dynamic_cast<const PecaBatalhaNaval*>(
                tabuleiro->consultarPosicao(l, coluna_no_tabuleiro(id_oponente(jogador), c)));
            saida << (!p || !p->get_foi_atingido() ? '.' : (p->get_eh_navio() ? 'X' : 'o')) << ' ';
        }
        saida << '\n';
    }
    saida << "Navios restantes: jogador 1 = " << quantidade_navios_jogador1
          << ", jogador 2 = " << quantidade_navios_jogador2 << '\n';
    if (estadoPartida == "finalizada")
        exibirResultado(saida);
}
