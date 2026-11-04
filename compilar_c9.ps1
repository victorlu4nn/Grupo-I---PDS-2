# Execute na raiz do repositorio: .\compilar_c9.ps1
# Nao depende de Make e nao altera as configuracoes do VS Code.
$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot
if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    throw 'g++ nao encontrado. Configure o compilador no PATH e reabra o terminal. Confira com g++ --version.'
}
$fontesCore = @(
    'src/base/Board.cpp', 'src/base/Piece.cpp', 'src/base/Player.cpp',
    'src/base/RuleBook.cpp', 'src/base/Game.cpp', 'src/base/Move.cpp',
    'src/base/GameManager.cpp', 'src/games/Connect4.cpp',
    'src/games/Damas.cpp', 'src/games/BatalhaNaval.cpp'
)
$fontesTeste = @(
    'tests/test_main.cpp', 'tests/base/TesteBoard.cpp',
    'tests/base/TestePiece.cpp', 'tests/base/TestePlayer.cpp',
    'tests/base/TesteRuleBook.cpp', 'tests/base/TesteGame.cpp',
    'tests/base/TesteMove.cpp', 'tests/base/TesteGameManager.cpp',
    'tests/games/TesteConnect4.cpp', 'tests/games/TesteDamas.cpp',
    'tests/games/TesteBatalhaNaval.cpp', 'tests/TesteDefensiva.cpp'
)
$opcoes = @('-std=c++11', '-Wall', '-Wextra', '-Wpedantic', '-g',
    '-Iinclude', '-Iinclude/base', '-Iinclude/games', '-Itests')
$necessarios = $fontesCore + $fontesTeste + @('src/main.cpp', 'include/doctest.h', 'tests/ApoioTestes.hpp')
foreach ($arquivo in $necessarios) {
    if (-not (Test-Path -LiteralPath $arquivo -PathType Leaf)) {
        throw "Arquivo ausente: $arquivo. Copie o conteudo do pacote para a raiz do repositorio."
    }
}
& g++ @opcoes @fontesCore src/main.cpp -o jogos_c9.exe
if ($LASTEXITCODE -ne 0) { throw 'Falha ao compilar o programa. Nao execute um binario antigo.' }
& g++ @opcoes @fontesCore @fontesTeste -o testes_c9.exe
if ($LASTEXITCODE -ne 0) { throw 'Falha ao compilar os testes. Nao execute um binario antigo.' }
& .\testes_c9.exe
if ($LASTEXITCODE -ne 0) { throw 'A suite de testes falhou.' }
Write-Host 'Compilacao e testes concluidos. Para jogar: .\jogos_c9.exe'
