#include "GameManager.hpp"
#include "Validacao.hpp"
#include <iostream>
#include <new>
#include <sstream>
#include <stdexcept>

int main() {
    try {
        GameManager gerenciador;
        gerenciador.registrarJogo("Connect4");
        gerenciador.registrarJogo("Damas");
        gerenciador.registrarJogo("Batalha Naval");
        while(!gerenciador.solicitouSaida()) {
            const auto jogos=gerenciador.listarJogos();
            std::cout<<"\n=== Jogos de tabuleiro ===\n0 - Sair\n";
            for(std::size_t i=0;i<jogos.size();++i)std::cout<<i+1<<" - "<<jogos[i]<<'\n';
            std::cout<<"Escolha: "<<std::flush;
            std::string texto;
            const auto leitura=validacao::lerLinha(std::cin,texto);
            if(leitura==validacao::Leitura::FIM) break;
            if(leitura==validacao::Leitura::LONGA){std::cout<<"Entrada muito longa.\n";continue;}
            std::istringstream entrada(texto);
            int opcao;
            if(!(entrada>>opcao) || !(entrada>>std::ws).eof()){
                std::cout<<"Digite apenas o numero de uma opcao.\n";continue;
            }
            if(opcao==0)break;
            if(opcao<1 || static_cast<std::size_t>(opcao)>jogos.size()){
                std::cout<<"Opcao fora do menu.\n";continue;
            }
            try {
                gerenciador.iniciarPartida(opcao-1);
                gerenciador.executarLoopPartida();
            } catch(const std::invalid_argument& e) {
                std::cerr<<"Configuracao invalida: "<<e.what()<<'\n';
            } catch(const std::out_of_range& e) {
                std::cerr<<"Opcao invalida: "<<e.what()<<'\n';
            }
        }
        std::cout<<"Programa encerrado.\n";
        return 0;
    } catch(const std::bad_alloc&) {
        std::cerr<<"Memoria insuficiente. O programa sera encerrado.\n";
    } catch(const std::ios_base::failure& e) {
        std::cerr<<"Falha de entrada/saida: "<<e.what()<<'\n';
    } catch(const std::exception& e) {
        std::cerr<<"Nao foi possivel continuar: "<<e.what()<<'\n';
    }
    return 1;
}
