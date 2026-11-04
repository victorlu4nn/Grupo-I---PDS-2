#ifndef VALIDACAO_HPP
#define VALIDACAO_HPP

#include <cctype>
#include <istream>
#include <stdexcept>
#include <string>

namespace validacao {
constexpr std::size_t MAX_COMANDO = 256;
inline bool textoVisivel(const std::string& texto) {
    if (texto.empty()) return false;
    bool visivel = false;
    for (unsigned char c : texto) {
        if (std::iscntrl(c)) return false;
        if (!std::isspace(c)) visivel = true;
    }
    return visivel;
}
inline bool comandoValido(const std::string& texto) {
    if (texto.empty() || texto.size() > MAX_COMANDO) return false;
    for (unsigned char c : texto)
        if (std::iscntrl(c) && c != '\t') return false;
    return true;
}
inline std::string aparar(const std::string& texto) {
    const auto inicio = texto.find_first_not_of(" \t\r\n");
    if (inicio == std::string::npos) return {};
    const auto fim = texto.find_last_not_of(" \t\r\n");
    return texto.substr(inicio, fim - inicio + 1);
}
enum class Leitura { OK, FIM, LONGA };
// Nao usa getline sem limite: descarta o excesso sem aumentar a string.
inline Leitura lerLinha(std::istream& entrada, std::string& linha) {
    linha.clear();
    bool longa = false, recebeu = false;
    char c;
    while (entrada.get(c)) {
        recebeu = true;
        if (c == '\n') break;
        if (linha.size() < MAX_COMANDO) linha.push_back(c);
        else longa = true;
    }
    if (entrada.bad() || (entrada.fail() && !entrada.eof()))
        throw std::ios_base::failure("Falha ao ler a entrada do terminal.");
    if (!recebeu && entrada.eof()) return Leitura::FIM;
    if (!linha.empty() && linha.back() == '\r') linha.pop_back();
    return longa ? Leitura::LONGA : Leitura::OK;
}
}
#endif
