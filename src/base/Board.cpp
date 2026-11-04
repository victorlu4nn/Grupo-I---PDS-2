#include "Board.hpp"
#include <stdexcept>
#include <utility>

Board::Board(int n, int m) : linhas(n), colunas(m) {
    if (n <= 0 || m <= 0) throw std::invalid_argument("Dimensoes devem ser positivas.");
    // Limite explicito adequado aos jogos (o maior tabuleiro usa 200 casas).
    // A comparacao ocorre ANTES da multiplicacao, evitando overflow.
    if (n > 100 || m > 100) throw std::length_error("Limite: 100 linhas e 100 colunas.");
    celulas.resize(n, std::vector<Piece*>(m, nullptr));
}
Board::~Board() { limpar(); }
bool Board::validarCoordenadas(int x, int y) const {
    return x >= 0 && x < linhas && y >= 0 && y < colunas;
}
void Board::exigirCoordenadas(int x, int y) const {
    // Verifica a matriz real, mesmo se uma subclasse mudar validarCoordenadas.
    if (x < 0 || x >= linhas || y < 0 || y >= colunas)
        throw std::out_of_range("Coordenada fora do tabuleiro.");
}
Piece* Board::consultarPosicao(int x, int y) const {
    exigirCoordenadas(x, y);
    return celulas[x][y];
}
void Board::atualizarPosicao(int x, int y, Piece* peca) {
    exigirCoordenadas(x, y);
    if (celulas[x][y] == peca) return;
    if (celulas[x][y] != nullptr)
        throw std::logic_error("Casa ocupada. Use retirarPeca ou removerPeca antes.");
    if (peca != nullptr) {
        for (const auto& linha : celulas)
            for (const auto* existente : linha)
                if (existente == peca)
                    throw std::logic_error("A mesma peca nao pode ocupar duas casas.");
    }
    celulas[x][y] = peca;
    ++revisao;
}
void Board::inserirPeca(int x, int y, std::unique_ptr<Piece> peca) {
    atualizarPosicao(x, y, peca.get());
    peca.release();
}
std::unique_ptr<Piece> Board::retirarPeca(int x, int y) {
    exigirCoordenadas(x, y);
    std::unique_ptr<Piece> retirada(celulas[x][y]);
    celulas[x][y] = nullptr;
    ++revisao;
    return retirada;
}
void Board::removerPeca(int x, int y) { retirarPeca(x, y); }
void Board::limpar() noexcept {
    for (auto& linha : celulas)
        for (auto& peca : linha) { delete peca; peca = nullptr; }
    ++revisao;
}
void Board::trocarConteudo(Board& outro) {
    if (linhas != outro.linhas || colunas != outro.colunas)
        throw std::invalid_argument("A troca exige tabuleiros com as mesmas dimensoes.");
    celulas.swap(outro.celulas);
    ++revisao;
    ++outro.revisao;
}
