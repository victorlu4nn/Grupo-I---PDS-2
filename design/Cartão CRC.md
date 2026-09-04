[
  {
    "name": "GameManager",
    "superclasses": "",
    "subclasses": "",
    "type": 1,
    "responsibilities": [
      "Manter o catálogo de jogos registrados na engine",
      "Armazenar a instância da partida atualmente em execução",
      "Registrar novos jogos por meio de uma interface padronizada",
      "Disponibilizar a listagem de jogos para a interface do usuário",
      "Instanciar e inicializar a partida escolhida pelo jogador",
      "Gerenciar o ciclo de vida da execução e turnos da partida",
      "Processar comandos de encerramento e reinicialização com confirmação",
      "Validar as opções digitadas pelo usuário no menu principal"
    ],
    "collaborators": [
      "Game",
      "UIConsole"
    ]
  },
  {
    "name": "Board",
    "superclasses": "",
    "subclasses": "",
    "type": 1,
    "responsibilities": [
      "Armazenar as dimensões N x M da matriz do tabuleiro",
      "Manter a alocação dinâmica e segura da grade de células",
      "Validar dimensões no construtor lançando exceções se <= 0",
      "Verificar se coordenadas (x, y) estão dentro dos limites válidos",
      "Consultar a peça ou elemento presente em uma coordenada",
      "Inserir ou atualizar uma peça em uma posição específica",
      "Remover peças de posições determinadas do tabuleiro",
      "Liberar a memória alocada dinamicamente no término",
      "Restaurar a matriz para a configuração inicial limpa"
    ],
    "collaborators": [
      "Piece",
      "Move"
    ]
  },
  {
    "name": "Game",
    "superclasses": "",
    "subclasses": "Dama, Connect 4",
    "type": 2,
    "responsibilities": [
      "Armazenar o nome de identificação e metadados do jogo",
      "Manter a referência do tabuleiro associado à partida",
      "Manter a lista de jogadores e o apontador para o jogador da vez",
      "Armazenar o estado atual da partida (em andamento, finalizada, etc)",
      "Configurar a disposição inicial das peças no tabuleiro",
      "Alternar a vez entre os jogadores participantes",
      "Executar uma jogada válida e atualizar o estado do tabuleiro",
      "Reiniciar a partida restaurando seu estado inicial sem registrar derrota",
      "Registrar o vencedor ou declarar empate ao final da partida"
    ],
    "collaborators": [
      "Board",
      "RuleBook",
      "Player",
      "Move"
    ]
  },
  {
    "name": "RuleBook",
    "superclasses": "",
    "subclasses": "CheckersRuleBook, ReversiRuleBook, TicTacToeRuleBook",
    "type": 2,
    "responsibilities": [
      "Armazenar a descrição completa das regras e manual do jogo",
      "Armazenar as condições formais de vitória e empate",
      "Fornecer o texto explicativo das regras para consulta",
      "Definir qual jogador realiza a jogada inicial da partida",
      "Validar se uma jogada proposta é legal perante as regras",
      "Verificar se o estado atual do tabuleiro configura vitória",
      "Verificar se o estado da partida configura empate",
      "Fornecer o motivo específico quando uma jogada for considerada inválida"
    ],
    "collaborators": [
      "Board",
      "Move",
      "Player"
    ]
  },
  {
    "name": "Player",
    "superclasses": "",
    "subclasses": "HumanPlayer",
    "type": 1,
    "responsibilities": [
      "Armazenar o nome ou apelido de identificação do participante",
      "Conhecer o símbolo ou cor atribuído às suas peças",
      "Armazenar o histórico de pontuação ou partidas vencidas",
      "Propor uma ação ou jogada a ser realizada no tabuleiro",
      "Solicitar acesso ao manual de regras do jogo",
      "Solicitar reinicialização ou encerramento da partida atual",
      "Receber e registrar a notificação do resultado da partida"
    ],
    "collaborators": [
      "Move",
      "Game"
    ]
  },
  {
    "name": "Move",
    "superclasses": "",
    "subclasses": "",
    "type": 1,
    "responsibilities": [
      "Conhecer o jogador responsável pela realização da jogada",
      "Armazenar as coordenadas de origem (linha e coluna)",
      "Armazenar as coordenadas de destino (linha e coluna)",
      "Armazenar dados extras da jogada (como tipo de captura ou peça envolvida)",
      "Aplicar o deslocamento ou inserção das peças no tabuleiro",
      "Reverter a alteração no tabuleiro para desfazer a jogada",
      "Fornecer representação textual legível da jogada para o console"
    ],
    "collaborators": [
      "Player",
      "Board"
    ]
  }
]