#include "Connect4.hpp"
#include <sstream>
#include <memory>
#include <stdexcept>
#include "Validacao.hpp"

// Recursos proprios ficam protegidos por unique_ptr em Game.
Connect4::Connect4()
    : Game("Connect4", std::unique_ptr<Board>(new Board(LINHAS, COLUNAS)),
           std::unique_ptr<RuleBook>(new RuleBook("Dois jogadores alternam a insercao de fichas nas "
                        "colunas de 0 a 6. A ficha cai ate a ultima casa livre. "
                        "Vence quem alinhar quatro fichas na horizontal, "
                        "vertical ou diagonal. Tabuleiro cheio sem vencedor "
                        "significa empate."))),
      quantidade_pecas_jogador1(0), quantidade_pecas_jogador2(0) {
    criarJogadores("\u2B24", "\u25EF");
    configurarDisposicaoInicial();
}

Connect4::~Connect4() = default;

void Connect4::configurarDisposicaoInicial() {
    estadoPartida = "em_andamento";
    tabuleiro->limpar();
    quantidade_pecas_jogador1 = 0;
    quantidade_pecas_jogador2 = 0;
    indiceJogadorDaVez = 0;
}

bool Connect4::executarJogada(const std::string acao) {
    if (!validacao::comandoValido(acao)) return false;
    std::istringstream entrada(acao);
    int coluna;
   
    if (!(entrada >> coluna)) return false;
    entrada >> std::ws;
    if (!entrada.eof()) return false;
    return soltarPeca(coluna);
}

bool Connect4::soltarPeca(int coluna) {
    if (coluna < 0 || coluna >= COLUNAS ||
        estadoPartida == "finalizada" || FimdeJogo()) return false;

   
    int linha = LINHAS - 1;
    while (linha >= 0 && tabuleiro->consultarPosicao(linha, coluna) != nullptr)
        --linha;
    if (linha < 0) return false;

    validarJogadorDaVez();
    const bool primeiro = indiceJogadorDaVez == 0;
    tabuleiro->inserirPeca(linha, coluna,
        std::unique_ptr<Piece>(new PecaConnect4(primeiro ? "\u2B24" : "\u25EF", primeiro ? 1 : 2,
                        primeiro ? EquipeConnect4::VERMELHO
                                 : EquipeConnect4::AMARELO)));
    if (primeiro) ++quantidade_pecas_jogador1;
    else ++quantidade_pecas_jogador2;

    if (verificar4EmLinha(linha, coluna)) {
        finalizarPartida(jogadores[indiceJogadorDaVez]);
    } else if (FimdeJogo()) {
        finalizarPartida(nullptr);
    } else {
        alternarTurno();
    }
    return true;
}

bool Connect4::verificar4EmLinha(int linha, int coluna) const {
    if (!tabuleiro->validarCoordenadas(linha, coluna)) return false;
    const Piece* origem = tabuleiro->consultarPosicao(linha, coluna);
    if (origem == nullptr) return false;

    const int id = origem->ler_id_jogador();
    const int direcoes[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    for (const auto& direcao : direcoes) {
        int total = 1;
       
        for (int sentido : {-1, 1}) {
            int l = linha + sentido * direcao[0];
            int c = coluna + sentido * direcao[1];
            while (tabuleiro->validarCoordenadas(l, c)) {
                const Piece* peca = tabuleiro->consultarPosicao(l, c);
                if (peca == nullptr || peca->ler_id_jogador() != id) break;
                ++total;
                l += sentido * direcao[0];
                c += sentido * direcao[1];
            }
        }
        if (total >= 4) return true;
    }
    return false;
}

Player* Connect4::Vencedor() const {
    for (int linha = 0; linha < LINHAS; ++linha) {
        for (int coluna = 0; coluna < COLUNAS; ++coluna) {
            if (verificar4EmLinha(linha, coluna)) {
                const int id = tabuleiro->consultarPosicao(linha, coluna)
                                   ->ler_id_jogador();
                if (id == 1 || id == 2) return jogadores.at(id - 1);
            }
        }
    }
    return nullptr;
}

bool Connect4::FimdeJogo() const {
    if (estadoPartida == "finalizada") return true;
    if (Vencedor() != nullptr) return true;
   
    for (int linha = 0; linha < LINHAS; ++linha)
        for (int coluna = 0; coluna < COLUNAS; ++coluna)
            if (tabuleiro->consultarPosicao(linha, coluna) == nullptr)
                return false;
    return true;
}

PecaConnect4::PecaConnect4(std::string simbolo, int id_jogador,
                         EquipeConnect4 eq)
    : Piece(simbolo, id_jogador), equipe(eq) {
    if ((id_jogador != 1 && id_jogador != 2) ||
        (eq != EquipeConnect4::VERMELHO && eq != EquipeConnect4::AMARELO))
        throw std::invalid_argument("ID ou equipe invalida para Connect4.");
    if ((id_jogador == 1) != (eq == EquipeConnect4::VERMELHO))
        throw std::invalid_argument("Equipe nao corresponde ao jogador.");
}

PecaConnect4::~PecaConnect4() = default;

bool PecaConnect4::movimento_valido(int, int, int, int, const Board&) const {
    return false;
}

EquipeConnect4 PecaConnect4::getEquipe() const { return equipe; }

void Connect4::exibir(std::ostream& saida) const {
    Game::exibir(saida);
    saida << "0 1 2 3 4 5 6 <- colunas\n";
    if (estadoPartida == "finalizada")
        exibirResultado(saida);
}
