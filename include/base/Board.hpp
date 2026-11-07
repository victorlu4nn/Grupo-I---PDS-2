#ifndef BOARD_HPP
#define BOARD_HPP
#include <cstdint>
#include <memory>
#include <vector>
#include "Piece.hpp"

// Board e dono das pecas inseridas com sucesso. Ponteiros consultados sao
// emprestados: nao devem ser deletados pelo chamador nem sobreviver ao tabuleiro.
class Board {
protected:
    const int linhas;
    const int colunas;
    std::vector<std::vector<Piece*>> celulas;
private:
    std::uint64_t revisao = 0;
    void exigirCoordenadas(int x, int y) const;
public:
    Board(int n, int m);
    virtual ~Board();
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
    virtual bool validarCoordenadas(int x, int y) const;
    Piece* consultarPosicao(int x, int y) const;
    // Nao sobrescreve uma casa ocupada. Em falha, peca continua com o chamador.
    void atualizarPosicao(int x, int y, Piece* peca);
    // Versao recomendada: mantem a memoria protegida mesmo se houver excecao.
    void inserirPeca(int x, int y, std::unique_ptr<Piece> peca);
    // Retira sem destruir e transfere a propriedade ao chamador.
    std::unique_ptr<Piece> retirarPeca(int x, int y);
    void removerPeca(int x, int y);
    void limpar() noexcept;
    int getLinhas() const noexcept { return linhas; }
    int getColunas() const noexcept { return colunas; }
    std::uint64_t getRevisao() const noexcept { return revisao; }
    // Permite preparar um reinicio inteiro antes de substituir a partida.
    void trocarConteudo(Board& outro);
};
#endif
