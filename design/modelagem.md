# 📝 Parte 2: User Stories (Histórias de Usuário)

---

## US01 — Navegação Narrativa

### História
Como jogador de RPG textual,  
quero visualizar descrições do ambiente e opções numeradas,  
para que eu possa tomar decisões e avançar na história.

### Critérios de Aceitação
- O console deve ser limpo ao carregar uma nova cena
- Apenas entradas válidas devem ser aceitas
- Inputs inválidos não podem travar o jogo

---

## US02 — Iniciação de Combates por Evento

### História
Como jogador explorando a narrativa,  
quero que o combate comece automaticamente após escolhas perigosas,  
para enfrentar imediatamente as consequências das minhas ações.

### Critérios de Aceitação
- A transição para combate ocorre se `geraCombate == true`
- O `MotorJogo` deve instanciar o inimigo correto usando `tipoInimigo`

---

## US03 — Gerenciamento Dual de Recursos

### História
Como jogador em combate,  
quero gastar MP em magias e Energia em técnicas físicas,  
para equilibrar estratégias ofensivas e defensivas.

### Critérios de Aceitação
- Habilidades físicas devem validar Energia suficiente
- O jogador recupera parcialmente MP e Energia a cada turno

---

## US04 — Sistema de Mitigação por Escudo

### História
Como jogador sob ataque pesado,  
quero usar uma ação defensiva baseada em energia,  
para reduzir drasticamente o próximo dano recebido.

### Critérios de Aceitação
- A flag `_escudoAtivo` deve ser ativada
- `receberDano()` deve aplicar mitigação extra
- O escudo deve ser desativado após o ataque recebido

---

## US05 — Recompensas e Evolução

### História
Como jogador vencedor de batalhas,  
quero ganhar XP e subir de nível,  
para fortalecer permanentemente meu personagem.

### Critérios de Aceitação
- `subirNivel()` deve ser acionado ao atingir o limite de XP
- O jogador deve visualizar os novos atributos após evolução

---

## US06 — Itens Consumíveis Limitados

### História
Como jogador ferido,  
quero utilizar frascos de cura durante o combate,  
para sobreviver em situações críticas.

### Critérios de Aceitação
- O jogo deve validar se há frascos disponíveis
- Cada uso consome um item
- O método `receberCura()` deve ser executado

---

## US07 — Checkpoint Pós-Derrota

### História
Como jogador derrotado,  
quero retornar ao último checkpoint seguro,  
para não perder todo o progresso da campanha.

### Critérios de Aceitação
- O `MotorJogo` deve chamar `processarDerrota()`
- O HP do jogador deve ser restaurado
- O jogador retorna ao checkpoint salvo

---

## US08 — Recompensas Narrativas

### História
Como jogador explorando caminhos opcionais,  
quero ganhar equipamentos especiais ao ajudar NPCs ou investigar áreas secretas,  
para fortalecer meu personagem sem depender apenas de combate.

### Critérios de Aceitação
- `checarEventosEspeciais()` deve analisar os metadados da escolha
- Se `itemGanhoID > 0`, o jogador recebe bônus permanentes

---

## US09 — IA dos Monstros

### História
Como jogador em combate,  
quero que os inimigos tenham comportamentos variados e estratégicos,  
para tornar as batalhas mais desafiadoras.

### Critérios de Aceitação
- Inimigos comuns executam ataques diretos
- Inimigos incomuns alternam:
  - ataques pesados
  - defesa
  - efeitos negativos

---

## US10 — Boss Fight em Múltiplas Fases

### História
Como jogador enfrentando o Dragão,  
quero que o comportamento do chefe mude conforme sua vida diminui,  
para que a batalha final seja intensa e dinâmica.

### Critérios de Aceitação
- O jogo deve alertar quando o Dragão entrar em fúria
- O chefe deve aumentar:
  - dano
  - agressividade
  - número de ações por turno
- A mudança deve ocorrer ao atingir o limiar de HP definido