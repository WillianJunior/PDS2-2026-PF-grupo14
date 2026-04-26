# User Stories - RPG

## User Story 1

### DESCRIPTION
Como [aventureiro],
quero [desbloquear o acesso à próxima sala],
para [fugir da dungeon].

### ACCEPTANCE CRITERIA
- [O RPG deve verificar que todos os inimigos da sala foram derrotados]
- [O RPG deve verificar que a chave foi encontrada]
- [O RPG deve desbloquear a porta se a chave for encontrada *e* todos os inimigos da sala foram derrotados]

---

## User Story 2

### DESCRIPTION
Como [aventureiro],
quero [explorar áreas escondidas],
para [acessar o boss da área mais rápido].

### ACCEPTANCE CRITERIA
- [O RPG deve abrir a porta secreta se a alavanca escondida for acionada]
- [O RPG deve verificar que os inimigos da passagem secreta foram derrotados]
- [O RPG deve liberar o atalho para o boss da área se os inimigos da passagem secreta foram derrotados]

---

## User Story 3

### DESCRIPTION
Como [aventureiro],
quero [marcar checkpoint],
para [não perder meu progresso].

### ACCEPTANCE CRITERIA
- [O RPG deve registrar que o checkpoint foi marcado na primeira sala da última área visitada]
- [O RPG deve spawnar o aventureiro no útlimo checkpoint marcado se ele morrer]
- [O RPG deve spawnar o aventureiro no início do jogo se não houver checkpoint marcado]