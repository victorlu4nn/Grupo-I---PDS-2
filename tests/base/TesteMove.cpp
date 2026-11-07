#include "doctest.h"
#include "Move.hpp"
#include "ApoioTestes.hpp"
#include <stdexcept>

TEST_CASE("Move - deslocamento e reversao preservam a identidade da peca") {
    Board b(8,8); Player j("Isaac Newton",'b');
    colocar(b,1,1,"U+2617",1);
    Piece* p=b.consultarPosicao(1,1);
    Move m(&j,1,1,2,2);
    m.aplicar(&b);
    CHECK(b.consultarPosicao(1,1)==nullptr);
    CHECK(b.consultarPosicao(2,2)==p);
    m.reverter(&b);
    CHECK(b.consultarPosicao(1,1)==p);
    CHECK(b.consultarPosicao(2,2)==nullptr);
}
TEST_CASE("Move - captura e reversao restauram ambas as pecas") {
    Board b(8,8); Player j("Isaac Newton",'b');
    colocar(b,1,1,"b",1);colocar(b,2,2,"p",2);
    Piece* a=b.consultarPosicao(1,1);Piece* c=b.consultarPosicao(2,2);
    Move m(&j,1,1,2,2);
    m.aplicar(&b);CHECK(b.consultarPosicao(2,2)==a);
    m.reverter(&b);CHECK(b.consultarPosicao(1,1)==a);CHECK(b.consultarPosicao(2,2)==c);
}
TEST_CASE("Move - rejeita argumentos e operacoes fora de ordem") {
    Board b(4,4);Player j("A",'a');
    CHECK_THROWS_AS(Move(nullptr,0,0,1,1),std::invalid_argument);
    CHECK_THROWS_AS(Move(&j,-1,0,1,1),std::out_of_range);
    CHECK_THROWS_AS(Move(&j,0,0,0,0),std::invalid_argument);
    Move m(&j,0,0,1,1);
    CHECK_THROWS_AS(m.aplicar(nullptr),std::invalid_argument);
    CHECK_THROWS_AS(m.reverter(&b),std::logic_error);
    CHECK_THROWS_AS(m.aplicar(&b),std::logic_error);
    colocar(b,0,0,"A");m.aplicar(&b);
    CHECK_THROWS_AS(m.aplicar(&b),std::logic_error);
    Board outro(4,4);CHECK_THROWS_AS(m.reverter(&outro),std::logic_error);
    colocar(b,3,3,"B",2);CHECK_THROWS_AS(m.reverter(&b),std::logic_error);
    REQUIRE(b.consultarPosicao(1,1)!=nullptr);
    CHECK(b.consultarPosicao(1,1)->ler_simbolo()=="A");
}
TEST_CASE("Move - falhas nao apagam a origem nem o destino") {
    Board b(4,4);Player j("A",'a');colocar(b,0,0,"A");colocar(b,1,1,"B");
    Move m(&j,0,0,1,1);CHECK_THROWS_AS(m.aplicar(&b),std::invalid_argument);
    REQUIRE(b.consultarPosicao(0,0)!=nullptr);REQUIRE(b.consultarPosicao(1,1)!=nullptr);
    Move fora(&j,0,0,99,99);CHECK_THROWS_AS(fora.aplicar(&b),std::out_of_range);
    CHECK(b.consultarPosicao(0,0)->ler_simbolo()=="A");
    CHECK(b.consultarPosicao(1,1)->ler_simbolo()=="B");
}
