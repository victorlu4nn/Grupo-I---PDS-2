#ifndef MOVE_HPP
#define MOVE_HPP
#include <cstdint>
#include <memory>
#include <string>
#include "Player.hpp"
#include "Board.hpp"

// Move desloca pecas; a regra especifica do jogo deve ser validada antes.
// Jogador e tabuleiro devem permanecer vivos enquanto a jogada for utilizada.
class Move {
private:
    Player* jogador;
    int origemX, origemY, destinoX, destinoY;
    std::unique_ptr<Piece> capturada; // guarda a captura para permitir desfazer
    Piece* movida = nullptr; // emprestada: pertence ao tabuleiro
    Board* aplicadoEm = nullptr;
    std::uint64_t revisaoAplicada = 0;
    bool aplicada = false;
public:
    Move(Player* j, int ox, int oy, int dx, int dy);
    Move(const Move&) = delete;
    Move& operator=(const Move&) = delete;
    void aplicar(Board* tabuleiro);
    void reverter(Board* tabuleiro);
    std::string toString() const;
};
#endif
