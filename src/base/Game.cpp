#include "Game.hpp"
#include "Validacao.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>

Game::Game(const std::string& nome, Board* b, RuleBook* r)
    : nomeJogo(nome), tabuleiro(b), regras(r), indiceJogadorDaVez(0),
      estadoPartida("em_andamento") {
    if (!validacao::textoVisivel(nome) || nome.size() > 80)
        throw std::invalid_argument("Nome do jogo invalido.");
    if (!b || !r) throw std::invalid_argument("Tabuleiro e regras nao podem ser nulos.");
}
Game::Game(const std::string& nome, std::unique_ptr<Board> b, std::unique_ptr<RuleBook> r)
    : Game(nome, b.get(), r.get()) {
    tabuleiroProprio = std::move(b);
    regrasProprias = std::move(r);
}
Game::~Game() = default;

void Game::criarJogadores(std::string simbolo1, std::string simbolo2) {
    if (!jogadores.empty()) 
        throw std::logic_error("Jogadores ja configurados.");

    std::unique_ptr<Player> p1(new Player("Jogador 1", simbolo1));
    std::unique_ptr<Player> p2(new Player("Jogador 2", simbolo2));

    jogadores.reserve(2);
    jogadoresProprios.reserve(2);

    jogadores.push_back(p1.get());
    jogadores.push_back(p2.get());

    jogadoresProprios.push_back(std::move(p1)); 
    jogadoresProprios.push_back(std::move(p2));
}

void Game::validarJogadorDaVez() const {
    if (indiceJogadorDaVez < 0 || static_cast<std::size_t>(indiceJogadorDaVez) >= jogadores.size()
        || jogadores[indiceJogadorDaVez] == nullptr)
        throw std::logic_error("Estado inconsistente: jogador da vez invalido.");
}

void Game::alternarTurno() {
    if (jogadores.empty()) {
        indiceJogadorDaVez = 0; 
        return; 
    }

    validarJogadorDaVez();

    const auto proximo = (static_cast<std::size_t>(indiceJogadorDaVez) + 1) % jogadores.size();
    if (!jogadores[proximo])
        throw std::logic_error("Proximo jogador nulo.");
    if (estadoPartida == "finalizada") 
        return;
    indiceJogadorDaVez = static_cast<int>(proximo);
}

void Game::reiniciarPartida() {
    // Cada jogo prepara a nova disposicao antes de trocar o tabuleiro.
    configurarDisposicaoInicial();
    indiceJogadorDaVez = 0;
    estadoPartida = "em_andamento";
}
void Game::finalizarPartida(Player* vencedor) {
    if (vencedor && std::find(jogadores.begin(), jogadores.end(), vencedor) == jogadores.end())
        throw std::invalid_argument("O vencedor precisa participar desta partida.");
    estadoPartida = "finalizada";
    // Preserva o contrato original: pontuacao e administrada por Player.
}

RuleBook* Game::get_regras() const { return regras; }

std::string Game::get_nomeJogo() const { return nomeJogo; }

std::string Game::getEstado() const { return estadoPartida; }

void Game::exibir(std::ostream& saida) const {
    saida << '\n' << nomeJogo << " | " << estadoPartida << '\n';
    for (int l = 0; l < tabuleiro->getLinhas(); ++l) {
        for (int c = 0; c < tabuleiro->getColunas(); ++c) {
            const Piece* p = tabuleiro->consultarPosicao(l, c);
            saida << (p ? p->ler_simbolo() : ".") << ' ';
        }
        saida << '\n';
    }
    if (estadoPartida == "em_andamento" && !jogadores.empty()) {
        validarJogadorDaVez();
        saida << "Vez de " << jogadores[indiceJogadorDaVez]->getNome() << '\n';
    }
}
