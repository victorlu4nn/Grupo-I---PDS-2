#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "Game.hpp"

class GameManager {
private:
    std::vector<std::string> catalogoJogos;
    std::unique_ptr<Game> partidaAtual;
    bool sairPrograma = false;
public:
    GameManager();
    ~GameManager();
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    void registrarJogo(const std::string& nomeJogo);
    std::vector<std::string> listarJogos() const;
    void iniciarPartida(int indice);
    void executarLoopPartida();
    // Sobrecarga permite testar o terminal sem depender do teclado real.
    void executarLoopPartida(std::istream& entrada, std::ostream& saida);
    void processarComandoEncerramento();
    void getDescricaoRegras() const;
    bool solicitouSaida() const noexcept { return sairPrograma; }
    const Game* getPartidaAtual() const noexcept { return partidaAtual.get(); }
};
#endif
