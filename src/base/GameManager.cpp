#include "GameManager.hpp"
#include "Damas.hpp"
#include "BatalhaNaval.hpp"
#include "Connect4.hpp"
#include "Validacao.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

GameManager::GameManager() = default;
GameManager::~GameManager() = default;
void GameManager::registrarJogo(const std::string& nome) {
    if(!validacao::textoVisivel(nome) || nome.size()>80 || nome!=validacao::aparar(nome))
        throw std::invalid_argument("Nome de jogo invalido.");
    if(std::find(catalogoJogos.begin(),catalogoJogos.end(),nome)!=catalogoJogos.end())
        throw std::invalid_argument("Jogo ja registrado.");
    catalogoJogos.push_back(nome);
}
std::vector<std::string> GameManager::listarJogos() const { return catalogoJogos; }
void GameManager::iniciarPartida(int indice) {
    if(indice<0 || static_cast<std::size_t>(indice)>=catalogoJogos.size())
        throw std::out_of_range("Opcao fora do catalogo.");
    const auto& nome=catalogoJogos[indice];
    // So substitui a partida atual depois que a nova estiver pronta.
    std::unique_ptr<Game> nova;
    if(nome=="Damas") nova.reset(new Damas());
    else if(nome=="Batalha Naval") nova.reset(new BatalhaNaval());
    else if(nome=="Connect4") nova.reset(new Connect4());
    else throw std::invalid_argument("Nao existe implementacao para este jogo.");
    nova->configurarDisposicaoInicial();
    partidaAtual=std::move(nova);
    sairPrograma=false;
}
void GameManager::processarComandoEncerramento() { partidaAtual.reset(); }
void GameManager::getDescricaoRegras() const {
    if(!partidaAtual) throw std::logic_error("Nenhuma partida ativa.");
    std::cout<<partidaAtual->get_regras()->getDescricaoRegras()<<'\n';
}
void GameManager::executarLoopPartida(){executarLoopPartida(std::cin,std::cout);}
void GameManager::executarLoopPartida(std::istream& entrada,std::ostream& saida) {
    if(!partidaAtual) return;
    saida<<"Comandos: regras, reiniciar, sair partida, sair programa.\n";
    while(partidaAtual) {
        partidaAtual->exibir(saida);
        if(partidaAtual->getEstado()=="finalizada") return;
        saida<<"> "<<std::flush;
        std::string linha;
        const auto leitura=validacao::lerLinha(entrada,linha);
        if(leitura==validacao::Leitura::FIM) {
            saida<<"\nEntrada encerrada.\n";sairPrograma=true;processarComandoEncerramento();return;
        }
        if(leitura==validacao::Leitura::LONGA) {saida<<"Comando muito longo (maximo 256 caracteres).\n";continue;}
        linha=validacao::aparar(linha);
        if(linha=="sair partida" || linha=="sair programa") {
            saida<<"Confirmar encerramento? (s/n): "<<std::flush;
            std::string resposta;
            const auto confirmacao=validacao::lerLinha(entrada,resposta);
            if(confirmacao==validacao::Leitura::FIM) {
                sairPrograma=true;processarComandoEncerramento();return;
            }
            if(confirmacao==validacao::Leitura::OK && validacao::aparar(resposta)=="s") {
                sairPrograma=linha=="sair programa";processarComandoEncerramento();return;
            }
            saida<<"Encerramento cancelado.\n";continue;
        }
        try {
            if(linha=="regras") saida<<partidaAtual->get_regras()->getDescricaoRegras()<<'\n';
            else if(linha=="reiniciar") partidaAtual->reiniciarPartida();
            else if(!partidaAtual->executarJogada(linha))
                saida<<"Jogada invalida. Consulte 'regras'; tente novamente.\n";
        } catch(const std::invalid_argument& e) {
            saida<<"Entrada invalida: "<<e.what()<<'\n';
        } catch(const std::out_of_range& e) {
            saida<<"Valor fora dos limites: "<<e.what()<<'\n';
        } catch(const std::logic_error& e) {
            saida<<"Partida encerrada por inconsistência: "<<e.what()<<'\n';
            processarComandoEncerramento();return;
        } catch(const std::runtime_error& e) {
            // Uma falha de preparacao do novo tabuleiro preserva o antigo.
            saida<<"Nao foi possivel concluir a operacao: "<<e.what()<<'\n';
        }
    }
}
