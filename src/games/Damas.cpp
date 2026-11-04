#include "Damas.hpp"
#include "Validacao.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
struct Casa { int id = 0; bool dama = false; bool capturada = false; };
using Grade = std::array<std::array<Casa, 8>, 8>;
struct Passo { int l, c, capturaL, capturaC; };
const int direcoes[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
bool dentro(int l, int c) { return l >= 0 && l < 8 && c >= 0 && c < 8; }
void dimensoes(const Board* b) {
    if (!b) throw std::invalid_argument("Tabuleiro de Damas nulo.");
    if (b->getLinhas() != 8 || b->getColunas() != 8)
        throw std::invalid_argument("Damas exige um tabuleiro 8x8.");
}
Grade lerGrade(const Board& b) {
    dimensoes(&b);
    Grade g{};
    int quantidades[3] = {};
    for (int l = 0; l < 8; ++l) for (int c = 0; c < 8; ++c) {
        const Piece* p = b.consultarPosicao(l,c);
        if (!p) continue;
        const auto* dama = dynamic_cast<const PecaDamas*>(p);
        if (!dama || (p->ler_id_jogador() != 1 && p->ler_id_jogador() != 2))
            throw std::logic_error("Peca incompativel no tabuleiro de Damas.");
        g[l][c].id = p->ler_id_jogador();
        g[l][c].dama = dama->get_eh_damas();
        if (++quantidades[p->ler_id_jogador()] > 12)
            throw std::logic_error("Damas permite no maximo 12 pecas por jogador.");
    }
    return g;
}
std::vector<Passo> capturas(const Grade& g, int l, int c) {
    std::vector<Passo> opcoes;
    if (!dentro(l,c) || g[l][c].id == 0 || g[l][c].capturada) return opcoes;
    const Casa origem = g[l][c];
    for (const auto& d : direcoes) {
        int x = l + d[0], y = c + d[1];
        if (!origem.dama) {
            const int dx = l + 2*d[0], dy = c + 2*d[1];
            if (dentro(dx,dy) && g[x][y].id == 3-origem.id && !g[x][y].capturada && g[dx][dy].id == 0)
                opcoes.push_back({dx,dy,x,y});
        } else {
            while (dentro(x,y) && g[x][y].id == 0) { x+=d[0]; y+=d[1]; }
            if (!dentro(x,y) || g[x][y].id != 3-origem.id || g[x][y].capturada) continue;
            const int cx=x, cy=y;
            x+=d[0]; y+=d[1];
            while (dentro(x,y) && g[x][y].id == 0) {
                opcoes.push_back({x,y,cx,cy}); x+=d[0]; y+=d[1];
            }
        }
    }
    return opcoes;
}
Grade simular(Grade g, int l, int c, const Passo& p) {
    g[p.l][p.c] = g[l][c];
    g[l][c] = Casa{};
    // Capturadas continuam bloqueando caminhos ate o fim da sequencia.
    g[p.capturaL][p.capturaC].capturada = true;
    return g;
}
int maiorSequencia(const Grade& g, int l, int c) {
    int maior = 0;
    for (const auto& p : capturas(g,l,c))
        maior = std::max(maior, 1 + maiorSequencia(simular(g,l,c,p),p.l,p.c));
    return maior;
}
int maiorCaptura(const Grade& g, int id) {
    int maior = 0;
    for (int l=0;l<8;++l) for (int c=0;c<8;++c)
        if (g[l][c].id==id) maior=std::max(maior,maiorSequencia(g,l,c));
    return maior;
}
bool simples(const Grade& g,int l,int c,int dx,int dy) {
    if (!dentro(l,c) || !dentro(dx,dy) || g[l][c].id==0 || g[dx][dy].id!=0) return false;
    int dl=dx-l, dc=dy-c;
    if (dl==0 || std::abs(dl)!=std::abs(dc)) return false;
    if (!g[l][c].dama) return std::abs(dc)==1 && dl==(g[l][c].id==1?-1:1);
    const int pl=dl>0?1:-1, pc=dc>0?1:-1;
    for (int x=l+pl,y=c+pc;x!=dx;x+=pl,y+=pc) if (g[x][y].id) return false;
    return true;
}
bool haMovimento(const Grade& g,int id) {
    for(int l=0;l<8;++l) for(int c=0;c<8;++c) if(g[l][c].id==id) {
        if(!capturas(g,l,c).empty()) return true;
        for(const auto& d:direcoes) if(simples(g,l,c,l+d[0],c+d[1])) return true;
    }
    return false;
}
bool avaliar(const Grade& inicial,const std::vector<int>& coords,int id,std::vector<Passo>& passos) {
    if(coords.size()<4 || coords.size()>26 || coords.size()%2!=0) return false;
    for(std::size_t i=0;i<coords.size();i+=2) if(!dentro(coords[i],coords[i+1])) return false;
    int l=coords[0],c=coords[1];
    if(inicial[l][c].id!=id) return false;
    Grade g=inicial;
    std::vector<Passo> plano;
    for(std::size_t i=2;i<coords.size();i+=2) {
        int dx=coords[i],dy=coords[i+1];
        auto opcoes=capturas(g,l,c);
        const auto it=std::find_if(opcoes.begin(),opcoes.end(),[&](const Passo& p){return p.l==dx && p.c==dy;});
        if(it==opcoes.end()) {
            if(i==2 && coords.size()==4 && simples(g,l,c,dx,dy)) { passos.clear(); return true; }
            return false;
        }
        plano.push_back(*it);
        g=simular(g,l,c,*it);
        l=dx;c=dy;
    }
    if(!capturas(g,l,c).empty()) return false; // nao permite parar uma captura na metade
    passos=std::move(plano);
    return true;
}
}

Damas::Damas()
    : Game("Damas",std::unique_ptr<Board>(new Board(8,8)),
           std::unique_ptr<RuleBook>(new RuleBook(
               "Use a3 para b4; em capturas encadeadas informe todas as casas. "
               "Captura obrigatoria pela maior sequencia; damas percorrem diagonais. "
               "Brancas comecam embaixo. Empate simplificado apos 40 meias-jogadas "
               "apenas com damas, sem captura."))) { criarJogadores('b','p'); }
Damas::Damas(Board* b,RuleBook* r) : Game("Damas",b,r) {
    dimensoes(b); criarJogadores('b','p');
}
Damas::~Damas() = default;
void Damas::organizar_tabuleiro_damas(Board* b) {
    dimensoes(b);
    if(b!=tabuleiro) throw std::invalid_argument("Use o tabuleiro associado a partida.");
    Board novo(8,8);
    for(int l=0;l<8;++l) for(int c=0;c<8;++c) {
        if((l+c)%2==0 || (l>=3 && l<=4)) continue;
        const int id=l<3?2:1;
        novo.inserirPeca(l,c,std::unique_ptr<Piece>(new PecaDamas(id==1?"b":"p",id)));
    }
    estadoPartida="em_andamento";
    tabuleiro->trocarConteudo(novo);
    quantidade_pecas_brancas=quantidade_pecas_pretas=12;
    contagem_lances_dama=0;
    indiceJogadorDaVez=0;
    mapa_pecas_no_tabuleiro.clear();
}
void Damas::configurarDisposicaoInicial(){organizar_tabuleiro_damas(tabuleiro);}
std::vector<std::string> Damas::_lendo_movimento(const std::string acao) const {
    if(!validacao::comandoValido(acao)) return {};
    std::istringstream entrada(acao);
    std::vector<std::string> resultado;
    std::string token;
    bool esperaCasa=true;
    while(entrada>>token) {
        if(!esperaCasa) { if(token!="para") return {}; }
        else {
            if(token.size()!=2 || token[1]<'1' || token[1]>'8') return {};
            char letra=static_cast<char>(std::tolower(static_cast<unsigned char>(token[0])));
            if(letra<'a' || letra>'h') return {};
            resultado.push_back(std::to_string('8'-token[1])+std::to_string(letra-'a'));
            if(resultado.size()>13) return {};
        }
        esperaCasa=!esperaCasa;
    }
    return !esperaCasa && resultado.size()>=2 ? resultado : std::vector<std::string>{};
}
bool Damas::executarJogada(const std::string acao) {
    if(estadoPartida=="finalizada") return false;
    const auto tokens=_lendo_movimento(acao);
    if(tokens.empty()) return false;
    validarJogadorDaVez();
    std::vector<int> coords;
    for(const auto& t:tokens){coords.push_back(t[0]-'0');coords.push_back(t[1]-'0');}
    const Grade grade=lerGrade(*tabuleiro);
    const int id=indiceJogadorDaVez+1;
    std::vector<Passo> plano;
    if(!avaliar(grade,coords,id,plano) || static_cast<int>(plano.size())!=maiorCaptura(grade,id)) return false;
    const int ox=coords[0],oy=coords[1],dx=coords[coords.size()-2],dy=coords.back();
    auto* p=dynamic_cast<PecaDamas*>(tabuleiro->consultarPosicao(ox,oy));
    if(!p || p->ler_status()!=PieceStatus::ATIVA) return false;
    const bool eraDama=p->get_eh_damas();
    // A sequencia inteira foi validada em uma copia antes de alterar o original.
    if(p->final_tabuleiro(dx)) p->promocao_dama();
    auto movida=tabuleiro->retirarPeca(ox,oy);
    for(const auto& passo:plano) tabuleiro->removerPeca(passo.capturaL,passo.capturaC);
    tabuleiro->inserirPeca(dx,dy,std::move(movida));
    contagem_lances_dama=eraDama && plano.empty()?contagem_lances_dama+1:0;
    quantidade_pecas_brancas=quantidade_pecas_pretas=0;
    for(int l=0;l<8;++l) for(int c=0;c<8;++c) {
        const Piece* q=tabuleiro->consultarPosicao(l,c);
        if(q){if(q->ler_id_jogador()==1)++quantidade_pecas_brancas;else ++quantidade_pecas_pretas;}
    }
    alternarTurno();
    if(Player* vencedor=Vencedor()) finalizarPartida(vencedor);
    else if(contagem_lances_dama>=40) finalizarPartida(nullptr);
    return true;
}
bool Damas::_captura_valida(Movimento_Valido acoes,PecaDamas* peca,
        std::vector<PecaDamas*>& capturadas,std::vector<int>& posicoes) {
    if(!peca || !acoes.eh_valido || acoes.coords.size()<4) return false;
    if(!dentro(acoes.coords[0],acoes.coords[1]) ||
       tabuleiro->consultarPosicao(acoes.coords[0],acoes.coords[1])!=peca) return false;
    std::vector<Passo> plano;
    if(!avaliar(lerGrade(*tabuleiro),acoes.coords,peca->ler_id_jogador(),plano) || plano.empty()) return false;
    std::vector<PecaDamas*> novas; std::vector<int> casas;
    for(const auto& p:plano) {
        novas.push_back(dynamic_cast<PecaDamas*>(tabuleiro->consultarPosicao(p.capturaL,p.capturaC)));
        casas.push_back(p.capturaL);casas.push_back(p.capturaC);
    }
    capturadas=std::move(novas);posicoes=std::move(casas);return true;
}
bool Damas::_verifica_se_ha_movimento(std::vector<std::vector<PecaDamas*>> celulas) const {
    if(celulas.size()!=8) throw std::invalid_argument("Matriz deve ter 8 linhas.");
    Grade g{};
    for(int l=0;l<8;++l) {
        if(celulas[l].size()!=8) throw std::invalid_argument("Matriz deve ter 8 colunas.");
        for(int c=0;c<8;++c) if(celulas[l][c]) {
            g[l][c].id=celulas[l][c]->ler_id_jogador();g[l][c].dama=celulas[l][c]->get_eh_damas();
        }
    }
    validarJogadorDaVez();return haMovimento(g,indiceJogadorDaVez+1);
}
Player* Damas::Vencedor() const {
    const Grade g=lerGrade(*tabuleiro);
    int quantidades[3]={};
    for(const auto& linha:g) for(const auto& casa:linha) ++quantidades[casa.id];
    if(quantidades[1]==0 && quantidades[2]==0)return nullptr;
    if(quantidades[1]==0)return jogadores.at(1);
    if(quantidades[2]==0)return jogadores.at(0);
    validarJogadorDaVez();
    return !haMovimento(g,indiceJogadorDaVez+1)?jogadores.at(1-indiceJogadorDaVez):nullptr;
}
bool Damas::FimdeJogo() const {return estadoPartida=="finalizada" || Vencedor()!=nullptr;}
void Damas::exibir(std::ostream& saida) const {
    saida<<"\nDamas (b/B = brancas, p/P = pretas; maiuscula = dama)\n";
    for(int l=0;l<8;++l){saida<<8-l<<"  ";for(int c=0;c<8;++c){
        const Piece* p=tabuleiro->consultarPosicao(l,c);saida<<(p?p->ler_simbolo():".")<<' ';}saida<<'\n';}
    saida<<"   a b c d e f g h\n";
    if(Player* p=Vencedor())saida<<p->getNome()<<" venceu!\n";
    else if(estadoPartida=="finalizada")saida<<"Empate.\n";
    else {validarJogadorDaVez();saida<<"Vez de "<<jogadores[indiceJogadorDaVez]->getNome()<<". Exemplo: a3 para b4\n";}
}
PecaDamas::PecaDamas(std::string simb,int id):Piece(simb,id){
    if(id!=1 && id!=2)throw std::invalid_argument("Jogador de Damas deve ser 1 ou 2.");
}
PecaDamas::~PecaDamas()=default;
bool PecaDamas::get_eh_damas() const{return eh_dama;}
void PecaDamas::atualizar_simbolo(){simbolo=id_jogador==1?"B":"P";}
void PecaDamas::promocao_dama(){atualizar_simbolo();eh_dama=true;}
bool PecaDamas::final_tabuleiro(int linha) const {
    if(linha<0 || linha>7)throw std::out_of_range("Linha invalida.");
    return linha==(id_jogador==1?0:7);
}
bool PecaDamas::movimento_valido(int ox,int oy,int dx,int dy,const Board& b) const {
    if(!dentro(ox,oy)||!dentro(dx,dy)||ler_status()!=PieceStatus::ATIVA)return false;
    const Grade g=lerGrade(b);
    if(b.consultarPosicao(ox,oy)!=this)return false;
    if(simples(g,ox,oy,dx,dy))return true;
    for(const auto& p:capturas(g,ox,oy))if(p.l==dx && p.c==dy)return true;
    return false;
}
Movimento_Valido* PecaDamas::_movimento_valido(std::vector<std::string> tokens,Board* b) const {
    dimensoes(b);
    std::unique_ptr<Movimento_Valido> r(new Movimento_Valido());
    if(tokens.size()<2 || tokens.size()>13)return r.release();
    for(const auto& t:tokens){
        if(t.size()!=2 || t[0]<'0'||t[0]>'7'||t[1]<'0'||t[1]>'7')return r.release();
        r->coords.push_back(t[0]-'0');r->coords.push_back(t[1]-'0');
    }
    if(b->consultarPosicao(r->coords[0],r->coords[1])!=this)return r.release();
    std::vector<Passo> plano;
    r->eh_valido=ler_status()==PieceStatus::ATIVA && avaliar(lerGrade(*b),r->coords,id_jogador,plano);
    return r.release();
}
void PecaDamas::_executa_movimento(Move& movimento,Board& b){movimento.aplicar(&b);}
