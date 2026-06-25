# 📇 Cartões CRC — PDS2-2026-PF-grupo14

Os cartões CRC mapeiam as responsabilidades e colaborações das principais classes do sistema, atualizados de acordo com a implementação atual do projeto.

---

## 1. Classe: `Personagem` (Abstrata)

**Descrição:** Superclasse abstrata responsável pelos atributos básicos e pelas regras gerais de combate.

**Responsabilidades**
- Armazenar: nome, HP atual/máximo, defesa base, força base, nível, estado vital (`_vivo`) e postura de escudo (`_escudoAtivo`)
- Fornecer lógica base para sofrer dano (`receberDano`, com mitigação por defesa e reação ao escudo) e recuperar vida (`receberCura`)
- Aplicar e processar efeitos temporários: Buff/Debuff (`aplicarBuff`/`aplicarDebuff`) e DoT/HoT (`aplicarDoT`/`aplicarHoT`/`processarEfeitosContinuos`)
- Gerenciar o conjunto de habilidades conhecidas (`adicionarHabilidade`/`getHabilidades`)
- Limpar efeitos temporários ao fim de um combate (`limparEfeitosTemporarios`)
- Definir os contratos abstratos: `executarTurno()` e `getDeclaracaoStatus()`

**Colaboradores**
- `Habilidade`
- `InterfaceJogo`

---

## 2. Classe: `Aventureiro`

**Descrição:** Representa o personagem controlado pelo jogador, contendo progressão, recursos e evolução.

**Responsabilidades**
- Exibir o menu de ações durante o turno do jogador (Ataque Básico, Habilidades, Escudo, Frasco de Cura)
- Gerenciar MP e Energia (consumo e regeneração passiva por turno)
- Acumular XP e gerenciar subida de nível, desbloqueando novas habilidades nos níveis 2 a 5
- Aplicar bônus permanentes de armas e armaduras
- Gerenciar frascos de cura e a ação de Dormir (restaura HP, MP, Energia e frascos)
- Controlar o ID da cena usada como checkpoint

**Colaboradores**
- `Personagem`
- `Habilidade`
- `InterfaceJogo`

---

## 3. Classe: `Inimigo` (Abstrata)

**Descrição:** Superclasse abstrata que define o comportamento base das inteligências artificiais dos monstros.

**Responsabilidades**
- Armazenar o XP concedido ao jogador após derrota
- Controlar contador interno de turnos da IA, usado pelas subclasses para alternar habilidades

**Colaboradores**
- `Personagem`

---

## 4. Classe: `InimigoComum` — DesafianteDoBar, TrabalhadorNoturno, SegurancaDeBalada

**Descrição:** Inimigos comuns utilizados em batalhas iniciais, sem rotação de efeitos contínuos.

**Responsabilidades**
- Implementar IAs simples:
  - `DesafianteDoBar`: ataque físico único e direto
  - `TrabalhadorNoturno`: 40% de chance de crítico (dano dobrado)
  - `SegurancaDeBalada`: assume postura defensiva (escudo) a cada 3 turnos
- Fornecer recompensas de XP baixas/médias (60 a 200)

**Colaboradores**
- `Inimigo`
- `Personagem`
- `Habilidade`
- `InterfaceJogo`

---

## 5. Classe: `InimigoIncomum` — RecrutaDoutrinado, GuardaPatrimonial, SabotadorRival

**Descrição:** Inimigos intermediários com rotação de habilidades baseada no contador de turnos.

**Responsabilidades**
- `RecrutaDoutrinado`: ataque físico + sangramento (DoT) + buff de força em si + debuff de defesa no alvo
- `GuardaPatrimonial`: ataque físico + debuffs de força/defesa no alvo + cura contínua (HoT) em si mesmo
- `SabotadorRival`: dano especial + queimadura (DoT) recorrente + explosão (ULTRA) que ignora escudo

**Colaboradores**
- `Inimigo`
- `Personagem`
- `Habilidade`
- `InterfaceJogo`

---

## 6. Classe: `InimigoBoss` — TylerDurden

**Descrição:** Chefe final da campanha, com máquina de estados de fase.

**Responsabilidades**
- Implementar IA avançada baseada em fases de HP (Fase 1 acima de 50%, Fase 2 entre 20% e 50%)
- Entrar em estado de cura única (Cura Estóica) quando o próprio HP cai abaixo de 20%
- Executar um ataque ULTRA único (Desconstrução Total) quando o HP do alvo cai abaixo de 30%
- Alternar ataques físico/especial e efeitos de buff/debuff/DoT/HoT conforme a fase atual

**Colaboradores**
- `Inimigo`
- `Personagem`
- `Habilidade`
- `InterfaceJogo`

---

## 7. Classe: `MotorJogo`

**Descrição:** Engine principal responsável pelo fluxo geral do jogo.

**Responsabilidades**
- Executar o loop principal (`rodar`)
- Carregar e armazenar o roteiro completo de cenas (`inicializarHistoria`)
- Interpretar escolhas do jogador e transicionar entre cenas
- Iniciar combates automaticamente quando uma escolha exige (`realizarCombate`)
- Instanciar inimigos dinamicamente via fábrica interna (`criarInimigo`)
- Conceder itens/bônus narrativos (`checarEventosEspeciais`)
- Restaurar o jogador e reposicioná-lo no checkpoint após derrota (`processarDerrota`)

**Colaboradores**
- `Aventureiro`
- `Cena`
- `Combate`
- `Inimigo`
- `InterfaceJogo`

---

## 8. Classe: `Cena`

**Descrição:** Representa um nó narrativo da campanha.

**Responsabilidades**
- Armazenar ID da cena, texto descritivo e indicador de checkpoint
- Armazenar a lista de escolhas possíveis a partir deste nó

**Colaboradores**
- `Escolha` (struct)

---

## 9. Classe: `Combate`

**Descrição:** Gerenciador das batalhas em turnos.

**Responsabilidades**
- Validar o estado inicial dos combatentes antes de iniciar (lança `CombateInvalidoException`)
- Controlar o fluxo de turnos alternados até a derrota de um dos lados
- Tratar exceções dos turnos sem interromper o combate
- Exibir mensagens de início, turno e fim de combate
- Conceder XP ao jogador após vitória e limpar efeitos temporários de ambos os lados

**Colaboradores**
- `Aventureiro`
- `Inimigo`
- `InterfaceJogo`

---

## 10. Classe: `InterfaceJogo`

**Descrição:** Responsável exclusivamente pela entrada e saída de dados no console.

**Responsabilidades**
- Exibir textos, narrativa e mensagens de evento
- Apresentar menus numerados e validar entradas do usuário
- Exibir o status (HP, defesa, nível) de qualquer `Personagem` (via polimorfismo)
- Pausar a execução aguardando o jogador

**Colaboradores**
- `Personagem`
