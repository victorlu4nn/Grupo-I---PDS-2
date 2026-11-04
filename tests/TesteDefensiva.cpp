#include "doctest.h"
#include "ApoioTestes.hpp"
#include "BatalhaNaval.hpp"
#include "Connect4.hpp"
#include "Damas.hpp"
#include "GameManager.hpp"
#include "Validacao.hpp"
#include <climits>
#include <memory>
#include <sstream>
#include <type_traits>

class DamasInspecao : public Damas {
public:
    using Damas::Damas;
    Board& board(){return *tabuleiro;}
    int turno()const{return indiceJogadorDaVez;}
    Player* jogador(int i)const{return jogadores.at(i);}
};
class NavalInspecao : public BatalhaNaval {
public:
    using BatalhaNaval::BatalhaNaval;
    Board& board(){return *tabuleiro;}
    int turno()const{return indiceJogadorDaVez;}
};
static void dama(Board& b,int l,int c,int id,bool promovida=false){
    std::unique_ptr<PecaDamas> p(new PecaDamas(id==1?"b":"p",id));
    if(promovida)p->promocao_dama();
    b.inserirPeca(l,c,std::move(p));
}
static std::string retrato(const Board& b){
    std::string s;
    for(int l=0;l<b.getLinhas();++l)for(int c=0;c<b.getColunas();++c){
        const Piece* p=b.consultarPosicao(l,c);
        s+=p?p->ler_simbolo():".";s+=';';
    }
    return s;
}
TEST_CASE("C9 - limites de recursos e coordenadas extremas"){
    CHECK_THROWS_AS(Board(INT_MAX,INT_MAX),std::length_error);
    Board b(8,8);
    CHECK_THROWS_AS(b.consultarPosicao(INT_MIN,0),std::out_of_range);
    CHECK_THROWS_AS(b.removerPeca(0,INT_MAX),std::out_of_range);
    CHECK_THROWS_AS(b.atualizarPosicao(99,0,nullptr),std::out_of_range);
    colocar(b,0,0,"A");
    Piece* p=b.consultarPosicao(0,0);
    CHECK_THROWS_AS(b.atualizarPosicao(0,1,p),std::logic_error);
    CHECK_THROWS_AS(b.atualizarPosicao(0,0,nullptr),std::logic_error);
    CHECK_THROWS_AS(b.inserirPeca(0,0,std::unique_ptr<Piece>(new PecaBasicaTeste("B"))),std::logic_error);
    CHECK(b.consultarPosicao(0,0)==p);
    CHECK(b.consultarPosicao(0,1)==nullptr);
    auto dono=b.retirarPeca(0,0);CHECK(dono.get()==p);CHECK(b.consultarPosicao(0,0)==nullptr);
    CHECK_NOTHROW(b.limpar());CHECK_NOTHROW(b.limpar());
}
TEST_CASE("C9 - nomes simbolos status e overflow"){
    CHECK_THROWS_AS(Player("   ",'A'),std::invalid_argument);
    CHECK_THROWS_AS(Player("Nome",'\n'),std::invalid_argument);
    CHECK_THROWS_AS(Player("A\nB",'A'),std::invalid_argument);
    CHECK_THROWS_AS(PecaBasicaTeste("\t"),std::invalid_argument);
    CHECK_THROWS_AS(PecaBasicaTeste(std::string("a\0b",3)),std::invalid_argument);
    CHECK_THROWS_AS(RuleBook("   "),std::invalid_argument);
    Player p("A",'A');p.adicionarPontos(INT_MAX);
    CHECK_THROWS_AS(p.adicionarPontos(1),std::overflow_error);
    CHECK(p.getPontuacao()==INT_MAX);
    PecaBasicaTeste q("X");CHECK_THROWS_AS(q.set_status(static_cast<PieceStatus>(42)),std::invalid_argument);
    CHECK(q.ler_status()==PieceStatus::ATIVA);
    CHECK_THROWS_AS(PecaConnect4("R",2,EquipeConnect4::VERMELHO),std::invalid_argument);
    CHECK_THROWS_AS(PecaDamas("b",0),std::invalid_argument);
    CHECK_THROWS_AS(PecaBatalhaNaval("N",3),std::invalid_argument);
}
TEST_CASE("C9 - tabuleiros incompativeis e ponteiros nulos"){
    Board b(3,3);RuleBook r("Regras");
    CHECK_THROWS_AS(Damas(&b,&r),std::invalid_argument);
    CHECK_THROWS_AS(BatalhaNaval(&b,&r),std::invalid_argument);
    CHECK_THROWS_AS(Damas(nullptr,&r),std::invalid_argument);
    CHECK_THROWS_AS(BatalhaNaval(nullptr,&r),std::invalid_argument);
    Board damas(8,8);CHECK_THROWS_AS(r.verificarVitoria(&damas),std::logic_error);
    CHECK_THROWS_AS(r.verificarEmpate(&damas),std::logic_error);
    CHECK_THROWS_AS(damas.trocarConteudo(b),std::invalid_argument);
}
TEST_CASE("C9 - Damas rejeita comandos malformados sem alterar a partida"){
    DamasInspecao j;j.configurarDisposicaoInicial();
    const auto antes=retrato(j.board());
    for(const std::string& comando:std::vector<std::string>{""," ","a","a3","para","a0 para b2","a9 para b2",
            "a3 b4","a3 para","a3 para z9","a3 para b4 lixo","a3 para a3",std::string(10000,'x'),std::string("a3\0 para b4",11)}){
        CHECK_FALSE(j.executarJogada(comando));CHECK(j.turno()==0);CHECK(retrato(j.board())==antes);
    }
    REQUIRE(j.executarJogada(" A3 para B4 "));CHECK(j.turno()==1);
    CHECK(j.board().consultarPosicao(5,0)==nullptr);
    REQUIRE(j.board().consultarPosicao(4,1)!=nullptr);
    CHECK(j.board().consultarPosicao(4,1)->ler_id_jogador()==1);
    CHECK_FALSE(j.executarJogada("b4 para c5")); // equipe errada
}
TEST_CASE("C9 - Damas valida toda a captura antes de modificar"){
    DamasInspecao j;auto& b=j.board();
    dama(b,5,0,1);dama(b,4,1,2);dama(b,2,3,2);
    auto antes=retrato(b);
    CHECK_FALSE(j.executarJogada("a3 para c5")); // falta a segunda captura
    CHECK_FALSE(j.executarJogada("a3 para c5 para f8")); // destino incorreto
    CHECK(retrato(b)==antes);CHECK(j.turno()==0);
    REQUIRE(j.executarJogada("a3 para c5 para e7"));
    CHECK(b.consultarPosicao(4,1)==nullptr);CHECK(b.consultarPosicao(2,3)==nullptr);
    CHECK(j.Vencedor()==j.jogador(0));CHECK(j.FimdeJogo());
    CHECK_FALSE(j.executarJogada("e7 para f8"));
}
TEST_CASE("C9 - Damas exige captura maxima e permite captura para tras"){
    DamasInspecao j;auto& b=j.board();
    dama(b,3,2,1);dama(b,4,3,2);dama(b,6,5,2);dama(b,2,1,2);
    const auto antes=retrato(b);
    CHECK_FALSE(j.executarJogada("c5 para a7")); // uma captura contra duas
    CHECK(retrato(b)==antes);
    REQUIRE(j.executarJogada("c5 para e3 para g1"));
    CHECK(b.consultarPosicao(4,3)==nullptr);CHECK(b.consultarPosicao(6,5)==nullptr);
    REQUIRE(b.consultarPosicao(7,6)!=nullptr);
    CHECK(b.consultarPosicao(7,6)->ler_id_jogador()==1);
}
TEST_CASE("C9 - Damas promocao e deslocamento de dama"){
    DamasInspecao j;auto& b=j.board();
    dama(b,1,2,1);dama(b,2,7,2);
    REQUIRE(j.executarJogada("c7 para b8"));
    auto* p=dynamic_cast<PecaDamas*>(b.consultarPosicao(0,1));REQUIRE(p!=nullptr);
    CHECK(p->get_eh_damas());
    REQUIRE(j.executarJogada("h6 para g5"));
    REQUIRE(j.executarJogada("b8 para e5"));
    CHECK(b.consultarPosicao(3,4)==p);
}
TEST_CASE("C9 - auxiliares de Damas rejeitam vetores incompletos"){
    DamasInspecao j;j.configurarDisposicaoInicial();
    auto* p=dynamic_cast<PecaDamas*>(j.board().consultarPosicao(5,0));REQUIRE(p!=nullptr);
    for(auto tokens:std::vector<std::vector<std::string>>{{},{""},{"50",""},{"50","999"},{"-1","41"}}){
        std::unique_ptr<Movimento_Valido> m(p->_movimento_valido(tokens,&j.board()));
        CHECK_FALSE(m->eh_valido);
    }
    CHECK_THROWS_AS(p->_movimento_valido({"50","41"},nullptr),std::invalid_argument);
    CHECK_FALSE(p->movimento_valido(INT_MIN,0,INT_MAX,0,j.board()));
    CHECK_THROWS_AS(j._verifica_se_ha_movimento({}),std::invalid_argument);
    std::vector<PecaDamas*> capturadas;std::vector<int> posicoes;Movimento_Valido m;
    CHECK_FALSE(j._captura_valida(m,nullptr,capturadas,posicoes));
}
TEST_CASE("C9 - Damas reinicio restaura 24 pecas e o primeiro turno"){
    DamasInspecao j;j.configurarDisposicaoInicial();const auto inicio=retrato(j.board());
    REQUIRE(j.executarJogada("a3 para b4"));j.reiniciarPartida();
    CHECK(retrato(j.board())==inicio);CHECK(j.turno()==0);CHECK_FALSE(j.FimdeJogo());
}
TEST_CASE("C9 - Naval posicionamento extremo nao causa overflow ou estado parcial"){
    NavalInspecao j;const auto antes=retrato(j.board());
    for(int n:{INT_MIN,-1,0,11,INT_MAX}) CHECK_FALSE(j.posicionar_navio(1,0,0,n,true));
    CHECK_FALSE(j.posicionar_navio(1,INT_MAX,0,3,false));
    CHECK_FALSE(j.posicionar_navio(INT_MAX,0,0,3,true));
    CHECK(retrato(j.board())==antes);CHECK(j.navios_restantes(1)==0);
    REQUIRE(j.posicionar_navio(1,0,0,2,true));
    CHECK_FALSE(j.posicionar_navio(1,0,1,3,true));
    CHECK_FALSE(j.posicionar_navio(1,1,0,3,true));
    CHECK(j.navios_restantes(1)==1);
}
TEST_CASE("C9 - Naval tiro repetido e acesso direto apos encerramento"){
    NavalInspecao j;
    REQUIRE(j.posicionar_navio(1,0,0,2,true));REQUIRE(j.posicionar_navio(2,0,0,2,true));
    REQUIRE(j.executarJogada("A1"));CHECK(j.turno()==1);
    REQUIRE(j.executarJogada("J10"));CHECK(j.turno()==0);
    CHECK_FALSE(j.executarJogada("A1"));CHECK(j.turno()==0);
    CHECK_FALSE(j.posicionar_navio(1,5,5,2,true)); // nao muda a frota apos iniciar ataques
    REQUIRE(j.executarJogada("B1"));CHECK(j.FimdeJogo());REQUIRE(j.Vencedor()!=nullptr);
    CHECK(j.navios_restantes(2)==0);
    CHECK_THROWS_AS(j._executa_movimento(0,1),std::invalid_argument);
    CHECK_FALSE(j.executarJogada("B1"));CHECK(j.navios_restantes(2)==0);
    j.reiniciarPartida();CHECK_FALSE(j.FimdeJogo());CHECK(j.turno()==0);
    CHECK(j.navios_restantes(1)==5);CHECK(j.navios_restantes(2)==5);
}
TEST_CASE("C9 - Naval entrada vazia gigante ou com lixo"){
    NavalInspecao j;j.configurarDisposicaoInicial();auto antes=retrato(j.board());
    for(const auto& comando:std::vector<std::string>{""," ","A","A01","A-1","A1 lixo","J11",std::string(10000,'9')}){
        CHECK_FALSE(j.executarJogada(comando));CHECK(j.turno()==0);CHECK(retrato(j.board())==antes);
    }
    CHECK_THROWS_AS(j._executa_movimento(INT_MAX,INT_MIN),std::invalid_argument);
}
TEST_CASE("C9 - frota aleatoria sempre tem 17 partes e cinco navios por jogador"){
    NavalInspecao j;
    for(int rodada=0;rodada<20;++rodada){
        j.configurarDisposicaoInicial();int partes[3]={};
        for(int l=0;l<10;++l)for(int c=0;c<20;++c){auto* p=j.board().consultarPosicao(l,c);if(p)++partes[p->ler_id_jogador()];}
        CHECK(partes[1]==17);CHECK(partes[2]==17);CHECK(j.navios_restantes(1)==5);CHECK(j.navios_restantes(2)==5);
    }
}
TEST_CASE("C9 - Connect4 rejeita comando gigante e bloqueia partida finalizada"){
    Connect4 j;CHECK_FALSE(j.executarJogada(std::string(10000,'1')));
    CHECK_FALSE(j.executarJogada(std::string("3\0",2)));
    j.finalizarPartida(nullptr);CHECK(j.FimdeJogo());CHECK_FALSE(j.executarJogada("0"));
    j.reiniciarPartida();REQUIRE(j.executarJogada("0"));
}
TEST_CASE("C9 - gerenciador preserva partida se a nova configuracao falha"){
    GameManager m;m.registrarJogo("Connect4");m.registrarJogo("Nao implementado");
    m.iniciarPartida(0);const Game* antes=m.getPartidaAtual();
    CHECK_THROWS_AS(m.iniciarPartida(1),std::invalid_argument);CHECK(m.getPartidaAtual()==antes);
    CHECK_THROWS_AS(m.iniciarPartida(INT_MIN),std::out_of_range);CHECK(m.getPartidaAtual()==antes);
    CHECK_THROWS_AS(m.registrarJogo("Connect4"),std::invalid_argument);
    CHECK_THROWS_AS(m.registrarJogo("   "),std::invalid_argument);
    m.processarComandoEncerramento();CHECK_THROWS_AS(m.getDescricaoRegras(),std::logic_error);
}
TEST_CASE("C9 - terminal recupera entrada invalida e trata EOF"){
    GameManager m;m.registrarJogo("Connect4");m.iniciarPartida(0);
    std::istringstream entrada("abc\n7\n"+std::string(1000,'x')+"\n0\n");std::ostringstream saida;
    CHECK_NOTHROW(m.executarLoopPartida(entrada,saida));CHECK(m.solicitouSaida());
    CHECK(m.getPartidaAtual()==nullptr);
    CHECK(saida.str().find("Jogada invalida")!=std::string::npos);
    CHECK(saida.str().find("Comando muito longo")!=std::string::npos);
    CHECK(saida.str().find("Jogador 2")!=std::string::npos);
}
TEST_CASE("C9 - terminal confirma encerramento e permite cancelar"){
    GameManager m;m.registrarJogo("Damas");m.iniciarPartida(0);
    std::istringstream entrada("sair partida\nn\na3 para b4\nsair partida\ns\n");std::ostringstream saida;
    CHECK_NOTHROW(m.executarLoopPartida(entrada,saida));CHECK_FALSE(m.solicitouSaida());CHECK(m.getPartidaAtual()==nullptr);
    CHECK(saida.str().find("Encerramento cancelado")!=std::string::npos);
}
TEST_CASE("C9 - leitura limitada drena linha longa e preserva a proxima"){
    std::istringstream entrada(std::string(1000,'x')+"\n2\n");std::string s;
    CHECK(validacao::lerLinha(entrada,s)==validacao::Leitura::LONGA);CHECK(s.size()==256);
    CHECK(validacao::lerLinha(entrada,s)==validacao::Leitura::OK);CHECK(s=="2");
    CHECK(validacao::lerLinha(entrada,s)==validacao::Leitura::FIM);
    std::istringstream ruim;ruim.setstate(std::ios::badbit);
    CHECK_THROWS_AS(validacao::lerLinha(ruim,s),std::ios_base::failure);
}
static_assert(!std::is_copy_constructible<Board>::value,"Tabuleiro nao pode duplicar posse das pecas");
static_assert(!std::is_copy_constructible<GameManager>::value,"Gerenciador nao pode duplicar partida");
static_assert(!std::is_copy_constructible<Move>::value,"Jogada nao pode duplicar posse de captura");

TEST_CASE("C9 - reinicio de Naval depois de tiros recupera contadores e turno") {
    NavalInspecao j;j.configurarDisposicaoInicial();
    REQUIRE(j.executarJogada("A1"));CHECK(j.turno()==1);
    j.reiniciarPartida();CHECK(j.turno()==0);
    CHECK(j.navios_restantes(1)==5);CHECK(j.navios_restantes(2)==5);
    for(int l=0;l<10;++l)for(int c=0;c<20;++c){
        auto* p=dynamic_cast<PecaBatalhaNaval*>(j.board().consultarPosicao(l,c));
        if(p)CHECK_FALSE(p->get_foi_atingido());
    }
}
TEST_CASE("C9 - Damas impede ultrapassar pecas proprias com uma dama") {
    DamasInspecao j;auto& b=j.board();
    dama(b,7,0,1,true);dama(b,5,2,1);dama(b,0,3,2);
    auto antes=retrato(b);CHECK_FALSE(j.executarJogada("a1 para e5"));
    CHECK(retrato(b)==antes);CHECK(j.turno()==0);
}
TEST_CASE("C9 - Damas dama captura a distancia") {
    DamasInspecao j;auto& b=j.board();
    dama(b,7,0,1,true);dama(b,4,3,2);
    REQUIRE(j.executarJogada("a1 para f6"));CHECK(b.consultarPosicao(4,3)==nullptr);
    CHECK(j.FimdeJogo());CHECK(j.Vencedor()==j.jogador(0));
}
TEST_CASE("C9 - Damas encerra por bloqueio do adversario") {
    DamasInspecao j;auto& b=j.board();
    dama(b,5,0,1);dama(b,7,6,2); // preta na ultima linha sem promocao, estado de teste
    REQUIRE(j.executarJogada("a3 para b4"));CHECK(j.FimdeJogo());CHECK(j.Vencedor()==j.jogador(0));
}
TEST_CASE("C9 - terminal sair programa e EOF na confirmacao") {
    for(const auto* texto:{"sair programa\ns\n","sair partida\n"}) {
        GameManager m;m.registrarJogo("Connect4");m.iniciarPartida(0);
        std::istringstream entrada(texto);std::ostringstream saida;
        m.executarLoopPartida(entrada,saida);CHECK(m.solicitouSaida());CHECK(m.getPartidaAtual()==nullptr);
    }
}
