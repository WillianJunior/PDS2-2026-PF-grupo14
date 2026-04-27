|Responsabilidades|                                                    |COLABORADOR|

Cartão CRC Combate
                                                                       
|Inicia o encontro entre o jogador e os inimigos da sala|               |Map|

|Gerencia a alternância de turnos entre os combatentes|                 |Character|

|Pausa o fluxo para aguardar a decisão do jogador (Ataque/Item)|        |GameUI|

|Processa o cálculo de dano baseado nos atributos do atacante|          |Character|

|Verifica se o HP de algum combatente chegou a zero|                    |Character|

|Notifica o mapa quando todos os inimigos da sala forem derrotados|     |Map|

|Sinaliza a derrota do jogador para o sistema de respawn|               |Map|

|Envia as informações de ações e status de batalha para exibição|       |GameUI|

Cartão CRC MAPA

|Armazena o estado atual das salas (inimigos vivos ou mortos)|          |Combat|

|Controla o bloqueio e desbloqueio de portas e passagens|               |Character|

|Verifica se o jogador possui a chave necessária para progredir|        |Inventory|

|Gerencia o acionamento de alavancas e mecanismos secretos|             |GameUI|

|Registra a localização do último checkpoint visitado|                  |Character|

|Realiza o spawn do jogador no local correto após a morte|              |Character|

|Carrega a próxima área ao cumprir os requisitos de saída|              |Combat|

|Identifica a presença de itens de drop ou baús na sala|                |Inventory|
