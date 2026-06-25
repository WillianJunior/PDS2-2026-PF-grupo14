# 📝 User Stories — PDS2-2026-PF-grupo14

Histórias de usuário revisadas de acordo com o comportamento real implementado no código (`MotorJogo`, `Aventureiro`, `Inimigo` e subclasses, `Combate`, `Cena`, `InterfaceJogo`).

---

## US01 — Navegação Narrativa

**História**
Como jogador de RPG textual, quero visualizar descrições do ambiente e opções numeradas, para que eu possa tomar decisões e avançar na história.

**Critérios de Aceitação**
- O texto da cena atual é exibido via `InterfaceJogo::exibirTexto`
- As opções da cena são apresentadas como um menu numerado (`InterfaceJogo::solicitarEscolha`)
- Apenas entradas numéricas dentro do intervalo válido são aceitas
- Entradas inválidas (não numéricas ou fora do intervalo) exibem um aviso e solicitam a escolha novamente, sem travar o jogo

> **Nota técnica:** `MotorJogo::rodar()` atualmente **não chama `limparTela()`** ao carregar uma nova cena — o método existe em `InterfaceJogo`, mas não é utilizado no loop principal. Se a limpeza de tela entre cenas for um requisito desejado, é necessário adicionar essa chamada em `rodar()`.

---

## US02 — Iniciação de Combates por Evento

**História**
Como jogador explorando a narrativa, quero que o combate comece automaticamente após escolhas perigosas, para enfrentar imediatamente as consequências das minhas ações.

**Critérios de Aceitação**
- A transição para combate ocorre quando a `Escolha` selecionada possui `geraCombate == true`
- `MotorJogo::realizarCombate(tipoInimigo)` instancia o inimigo correto através da fábrica interna `criarInimigo`
- Tipos de inimigo não reconhecidos (ou string vazia) lançam `std::invalid_argument`
- Se o jogador perder o combate, `processarDerrota()` é chamado e a cena não avança para o destino da escolha

---

## US03 — Gerenciamento Dual de Recursos

**História**
Como jogador em combate, quero gastar MP em magias e Energia em técnicas físicas, para equilibrar estratégias ofensivas e defensivas.

**Critérios de Aceitação**
- Cada `Habilidade` possui custo próprio de Energia (`getCustoEnergia`) e de MP (`getCustoMP`)
- Antes de usar qualquer habilidade, o jogo valida se **MP e Energia atuais são suficientes simultaneamente** (`_mp >= custoMP && _energia >= custoEnergia`); se não forem, a ação é cancelada e o menu é reapresentado
- O jogador recupera passivamente **+8 de Energia e +2 de MP por turno** (`recuperarRecursos`), sem exceder os respectivos máximos

---

## US04 — Sistema de Mitigação por Escudo

**História**
Como jogador sob ataque pesado, quero usar uma ação defensiva baseada em energia, para reduzir drasticamente o próximo dano recebido.

**Critérios de Aceitação**
- `usarEscudo()` ativa a flag `_escudoAtivo` e consome 6 pontos de Energia
- `receberDano()` aplica mitigação extra enquanto o escudo está ativo: ataques `FISICO`/`ESPECIAL` têm o dano cortado pela metade; ataques `ULTRA` ignoram o escudo completamente; efeitos `DOT` atravessam o escudo sem mitigação adicional
- O escudo permanece ativo até o **início do próximo turno do próprio jogador** (`_escudoAtivo = false` no início de `executarTurno`), independentemente de o jogador ter ou não recebido um ataque nesse intervalo

---

## US05 — Recompensas e Evolução

**História**
Como jogador vencedor de batalhas, quero ganhar XP e subir de nível, para fortalecer permanentemente meu personagem.

**Critérios de Aceitação**
- `ganharExperiencia(xp)` acumula XP e, ao atingir ou superar `_xpProxNivel` (nível atual × 60), aciona `subirNivel()` — podendo subir múltiplos níveis de uma vez se o XP concedido for suficiente
- `subirNivel()` aumenta HP máximo, Força base e Defesa base, restaura o HP ao novo máximo, e exibe uma mensagem de progressão
- Novas habilidades são desbloqueadas automaticamente nos níveis 2 a 5 (ex.: Soco Forte e Usar Dipirona no nível 2; Ataque Mental no nível 5)
- XP negativo ou igual a zero é ignorado e não altera o nível

---

## US06 — Itens Consumíveis Limitados

**História**
Como jogador ferido, quero utilizar frascos de cura durante o combate, para sobreviver em situações críticas.

**Critérios de Aceitação**
- O jogo valida se há frascos disponíveis (`_frascos > 0`) antes de permitir o uso
- Cada uso consome um frasco e cura 40% do HP máximo do jogador via `receberCura()`
- Sem frascos disponíveis, a ação é bloqueada com uma mensagem de erro e o menu é reapresentado
- Os frascos são totalmente recarregados (3 unidades) ao usar `dormir()`

---

## US07 — Checkpoint Pós-Derrota

**História**
Como jogador derrotado, quero retornar ao último checkpoint seguro, para não perder todo o progresso da campanha.

**Critérios de Aceitação**
- Cenas marcadas como checkpoint atualizam `_idCenaCheckpoint` do jogador ao serem visitadas (`setIDCheckpoint`)
- Ao perder um combate, `MotorJogo::processarDerrota()` é chamado automaticamente
- `processarDerrota()` chama `dormir()`, restaurando HP, MP, Energia e frascos de cura ao máximo
- O jogador é reposicionado na última cena de checkpoint salva (`_idCenaAtual = _player.getIDCheckpoint()`)

---

## US08 — Recompensas Narrativas

**História**
Como jogador explorando caminhos opcionais, quero ganhar equipamentos especiais ao ajudar ou conversar com NPCs, para fortalecer meu personagem sem depender apenas de combate.

**Critérios de Aceitação**
- `checarEventosEspeciais()` analisa o campo `itemGanhoID` da escolha selecionada
- Se `itemGanhoID > 0`, o nome do item é resolvido em uma tabela interna e exibido ao jogador
- Itens reconhecidos aplicam um bônus permanente de Força ou Defesa (via `buffArma`/`buffArmadura`) através de uma tabela de mapeamento item → ação de buff
- Itens sem buff cadastrado ainda exibem a mensagem de obtenção, sem aplicar efeito numérico

---

## US09 — IA dos Monstros

**História**
Como jogador em combate, quero que os inimigos tenham comportamentos variados e estratégicos, para tornar as batalhas mais desafiadoras.

**Critérios de Aceitação**
- Inimigos comuns (`InimigoComum`) executam principalmente ataques diretos, com pequenas variações: `TrabalhadorNoturno` tem chance de crítico, `SegurancaDeBalada` assume postura defensiva periodicamente
- Inimigos incomuns (`InimigoIncomum`) alternam, conforme um contador interno de turnos, entre ataque padrão, aplicação de efeitos contínuos (DoT/HoT) e modificadores de atributo (Buff/Debuff) em si mesmos ou no jogador
- A rotação de cada inimigo incomum é determinística e baseada em múltiplos do contador de turnos (ex.: a cada 3, 4 ou 5 turnos)
- Inimigos derrotados (`!estaVivo()`) não executam ações em seu turno

---

## US10 — Boss Fight em Múltiplas Fases

**História**
Como jogador enfrentando Tyler Durden, quero que o comportamento do chefe mude conforme sua vida diminui, para que a batalha final seja intensa e dinâmica.

**Critérios de Aceitação**
- O chefe opera em duas fases baseadas na proporção de seu próprio HP: **Fase 1** acima de 50% e **Fase 2** entre 20% e 50%
- A transição para a Fase 2 é anunciada uma única vez ao jogador (`_fase2Anunciada`)
- Na Fase 2, o ataque principal passa de físico para especial com dano base maior (`_forcaBase + 5 × nível`), e o chefe pode acumular múltiplos efeitos no mesmo turno (debuff de defesa no jogador, sangramento contínuo e cura contínua em si mesmo)
- **Cura Estóica**: dispara uma única vez quando o próprio HP do chefe cai abaixo de 20%, independentemente da fase, recuperando HP fixo
- **Desconstrução Total**: dispara uma única vez quando o HP do jogador cai abaixo de 30%, independentemente da fase, causando dano massivo (`força × 3`) que ignora o escudo do jogador
