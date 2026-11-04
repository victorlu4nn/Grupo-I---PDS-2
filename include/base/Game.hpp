#ifndef GAME_HPP
#define GAME_HPP
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "Board.hpp"
#include "Player.hpp"
#include "RuleBook.hpp"

class Game {
protected:
    std::string nomeJogo;
    Board* tabuleiro;
    RuleBook* regras;
    std::vector<Player*> jogadores;
    int indiceJogadorDaVez;
    std::string estadoPartida;
    // Construtor para jogos que criam seus proprios recursos, como Connect4.
    Game(const std::string& nome, std::unique_ptr<Board> b, std::unique_ptr<RuleBook> r);
    void criarJogadores(char simbolo1, char simbolo2);
    void validarJogadorDaVez() const;
private:
    std::unique_ptr<Board> tabuleiroProprio;
    std::unique_ptr<RuleBook> regrasProprias;
    std::vector<std::unique_ptr<Player>> jogadoresProprios;
public:
    // Ponteiros externos sao emprestados: devem viver mais que a partida.
    Game(const std::string& nome, Board* b, RuleBook* r);
    virtual ~Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    virtual void configurarDisposicaoInicial() = 0;
    virtual bool executarJogada(const std::string acao) = 0;
    void alternarTurno();
    void reiniciarPartida();
    void finalizarPartida(Player* vencedor);
    RuleBook* get_regras() const;
    std::string get_nomeJogo() const;
    std::string getEstado() const;
    virtual void exibir(std::ostream& saida) const;
};
#endif
