#include "doctest.h"
#include "Damas.hpp"
#include <stdexcept>

class PecaDamaTeste: public PecaDamas {
    public:
        PecaDamaTeste(const std::string simbolo, int id):
        PecaDamas(simbolo, id) {};
};

TEST_CASE("Teste das propriedades e comportamentos de PecaDamas") {
    // Initialize a common piece
    PecaDamaTeste* peca = new PecaDamaTeste("b", 1);
    
    CHECK(peca->get_eh_damas() == false);
    
    // Dama promotion testing
    peca->promocao_dama();
    CHECK(peca->get_eh_damas() == true);
}

TEST_CASE("Teste de inicialização do jogo Damas") {
    Damas jogo;
    
    // The game should not be initialized in "finalizada" state
    CHECK_FALSE(jogo.FimdeJogo());
    
    // Verifies if there is no exception in board building
    CHECK_NOTHROW(jogo.organizar_tabuleiro_damas());
}

TEST_CASE("Teste de leitura de movimentos e ações") {
    Damas jogo;
    
    // Tests reading movement command
    std::vector<int> movimento = jogo._lendo_movimento("a1 para b2");
    
    // Verifies if the vector has the expected values
    CHECK_FALSE(movimento.empty());
}
