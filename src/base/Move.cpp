#include "Move.hpp"
#include <sstream>
#include <stdexcept>
#include <utility>

Move::Move(Player* j, int ox, int oy, int dx, int dy)
    : jogador(j), origemX(ox), origemY(oy), destinoX(dx), destinoY(dy) {
    if (!j) throw std::invalid_argument("Jogador nao pode ser nulo.");
    if (ox < 0 || oy < 0 || dx < 0 || dy < 0)
        throw std::out_of_range("Coordenadas nao podem ser negativas.");
    if (ox == dx && oy == dy) throw std::invalid_argument("Origem e destino devem ser diferentes.");
}
void Move::aplicar(Board* b) {
    if (!b) throw std::invalid_argument("Tabuleiro nao pode ser nulo.");
    if (aplicada) throw std::logic_error("Jogada ja aplicada.");
    // Todas as consultas/validacoes ocorrem antes de modificar qualquer casa.
    Piece* origem = b->consultarPosicao(origemX, origemY);
    Piece* destino = b->consultarPosicao(destinoX, destinoY);
    if (!origem) throw std::logic_error("Nao ha peca na origem.");
    if (origem->ler_status() != PieceStatus::ATIVA)
        throw std::logic_error("A peca esta inativa.");
    if (destino && destino->ler_id_jogador() == origem->ler_id_jogador())
        throw std::invalid_argument("Nao e permitido capturar uma peca da mesma equipe.");
    capturada = b->retirarPeca(destinoX, destinoY);
    auto peca = b->retirarPeca(origemX, origemY);
    movida = peca.get();
    b->inserirPeca(destinoX, destinoY, std::move(peca));
    aplicadoEm = b;
    revisaoAplicada = b->getRevisao();
    aplicada = true;
}
void Move::reverter(Board* b) {
    if (!b) throw std::invalid_argument("Tabuleiro nao pode ser nulo.");
    if (!aplicada || b != aplicadoEm) throw std::logic_error("Jogada nao aplicada neste tabuleiro.");
    // Impede restaurar ponteiros antigos se outra jogada/reinicio ocorreu.
    if (b->getRevisao() != revisaoAplicada || b->consultarPosicao(origemX, origemY)
        || b->consultarPosicao(destinoX, destinoY) != movida)
        throw std::logic_error("Tabuleiro alterado depois da jogada; desfazer recusado.");
    auto peca = b->retirarPeca(destinoX, destinoY);
    b->inserirPeca(origemX, origemY, std::move(peca));
    b->inserirPeca(destinoX, destinoY, std::move(capturada));
    aplicada = false;
    aplicadoEm = nullptr;
    movida = nullptr;
}
std::string Move::toString() const {
    std::ostringstream s;
    s << "Movimento de " << jogador->getNome() << " de (" << origemX << ',' << origemY
      << ") para (" << destinoX << ',' << destinoY << ')';
    return s.str();
}
