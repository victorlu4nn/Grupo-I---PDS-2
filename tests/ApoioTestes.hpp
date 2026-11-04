#ifndef APOIO_TESTES_HPP
#define APOIO_TESTES_HPP
#include "Piece.hpp"
#include "Board.hpp"
#include <memory>
class PecaBasicaTeste : public Piece {
public:
    PecaBasicaTeste(const std::string& s,int id=1):Piece(s,id){}
    bool movimento_valido(int,int,int,int,const Board&)const override{return true;}
};
inline void colocar(Board& b,int l,int c,const std::string& s,int id=1){
    b.inserirPeca(l,c,std::unique_ptr<Piece>(new PecaBasicaTeste(s,id)));
}
#endif
