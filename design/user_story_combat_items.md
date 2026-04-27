# User Stories - Combate e itens

## User Story 1

### DESCRIPTION
Como [aventureiro],
quero [que o jogo alterne os turnos entre o meu personagem e o inimigo],
para [que eu possa escolher a minha ação (atacar ou usar item) antes da ação do adversário].

### ACCEPTANCE CRITERIA
- [O sistema deve exibir de quem é o turno atual].
- [O combate só deve avançar após a confirmação da ação do jogador].
- [O fluxo deve ser interrompido imediatamente se o HP de qualquer um dos combatentes chegar a 0].

---

## User Story 2

### DESCRIPTION
Como [aventureiro],
quero [poder selecionar e consumir um item do meu inventário durante o meu turno],
para [que eu possa recuperar pontos de vida ou pontos de uso de habilidade durante a batalha].

### ACCEPTANCE CRITERIA
- [Ao usar um item consumível, a quantidade disponível no inventário deve diminuir em 1].
- [O efeito do item (ex: +50 HP) deve ser aplicado instantaneamente aos atributos do personagem].
- [Se o item estiver esgotado, ele não pode ser utilizado].

---

## User Story 3

### DESCRIPTION
Como [aventureiro],
quero [que diferentes tipos de ataques (físico, à distância ou mágico) causem danos baseados nos atributos específicos do meu personagem],
para [que a escolha da minha classe (Guerreiro, Arqueiro ou Mago) influencie diretamente na estratégia de combate].

### ACCEPTANCE CRITERIA
- [O Guerreiro deve causar mais dano em ataques baseados em "Força"].
- [O Arqueiro deve causar mais dano em ataques baseados em "Agilidade"].
- [O Mago deve causar mais dano em habilidades baseadas em "Inteligência"].
- [O cálculo de dano deve ser impresso no log de combate (ex: "Mago usou Bola de Fogo e causou 40 de dano")].