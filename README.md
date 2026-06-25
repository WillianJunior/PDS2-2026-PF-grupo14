# PDS2-2026-PF-grupo14

## Nome dos Integrantes

* Davi Costa
* Gustavo Ribeiro
* Henrique Garcia
* João Pedro Guimarães Carvalho
* Kaio Gomes Vieira

## Descrição do Projeto

O projeto é um **RPG textual de narrativa linear interativa**, ambientado em uma releitura do universo de *Fight Club*. A progressão não depende de exploração geográfica, mas da superação de combates e, principalmente, das escolhas do jogador em uma sequência de cenas narrativas (insônia, grupos de apoio, o encontro com Tyler Durden, missões do Projeto Mayhem e o confronto final).

O sistema gerencia um grafo de cenas (`Cena` + `Escolha`), onde cada decisão pode: avançar a narrativa, disparar um combate por turnos contra um inimigo específico, conceder um item/bônus permanente, ou marcar um checkpoint de salvamento. O estado do herói (vida, recursos, nível, equipamentos) é preservado entre cenas e combates, e a derrota em batalha não encerra o jogo — o herói é restaurado e devolvido ao último checkpoint visitado.

## 🎯 Objetivos Principais

* **Arquitetura Baseada em Cenas e Escolhas** — nós narrativos (`Cena`) conectados por opções (`Escolha`) que definem o próximo estado da história, podendo carregar metadados de combate ou recompensa.
* **Sistema de Combate Tático por Turnos** — mediado pela classe `Combate`, que alterna turnos entre `Aventureiro` e `Inimigo`, com gerenciamento de Energia e MP, efeitos contínuos (DoT/HoT) e modificadores temporários de atributo (Buff/Debuff).
* **Persistência de Checkpoints** — cenas marcadas como `checkpoint` atualizam o ponto de retorno do jogador; uma derrota em combate aciona `processarDerrota()`, que restaura os recursos do herói (`dormir()`) e o reposiciona no último checkpoint.
* **Interface Orientada a Eventos** — toda a entrada/saída do jogo passa pela classe estática `InterfaceJogo`, que isola a lógica de jogo do I/O de console (menus numerados, exibição de status, pausas).

## 💡 Motivação

Escolhemos o RPG com narrativa linear para aplicar de forma prazerosa os conceitos de POO trabalhados em sala — **herança** (hierarquia `Personagem` → `Aventureiro`/`Inimigo` → subtipos de inimigo), **encapsulamento** (atributos protegidos/privados acessíveis somente por getters e métodos de domínio), e **polimorfismo** (`executarTurno` e `getDeclaracaoStatus` como métodos virtuais puros, despachados dinamicamente para cada tipo de combatente). O tema de *Fight Club* foi escolhido por afinidade do grupo com RPGs e por se encaixar bem na ideia de uma jornada psicológica conduzida por escolhas.

## 🏗️ Arquitetura de Classes

### `Personagem` (abstrata)
Base de qualquer entidade de combate. Centraliza HP/HP máximo, defesa e força base, nível, estado vital (`_vivo`), postura de escudo (`_escudoAtivo`) e os repositórios de efeitos ativos (`_efeitosAtivos` para DoT/HoT, `_modificadoresAtivos` para Buff/Debuff) e habilidades conhecidas (privado).

Métodos-chave: `receberDano`, `receberCura`, `aplicarBuff`, `aplicarDebuff`, `aplicarDoT`, `aplicarHoT`, `processarEfeitosContinuos`, `limparEfeitosTemporarios`, além dos métodos virtuais puros `executarTurno(Personagem&)` e `getDeclaracaoStatus() const`.

A mitigação de dano segue a fórmula `100 / (100 + defesa)`; ataques do tipo `ULTRA` ignoram completamente o escudo, efeitos `DOT` agem "por baixo" do escudo, e os demais tipos têm o dano cortado pela metade quando o escudo está ativo. Todo dano positivo garante no mínimo 1 ponto de dano real.

### `Aventureiro` (herói jogável)
Estende `Personagem` com dois recursos próprios — **Energia** (regenera +8/turno) e **MP** (regenera +2/turno) — além de frascos de cura (3 cargas, cura 40% do HP máximo), bônus permanentes de equipamento (`_bonusArma`, `_bonusArmadura`) e sistema de XP/nível.

`executarTurno` abre um menu interativo via `InterfaceJogo` com 4 opções: Ataque Básico (grátis), Habilidades (sub-menu dinâmico conforme MP/Energia disponíveis), Erguer Escudo (6 de Energia) e Usar Frasco de Cura.

Progressão por nível (`subirNivel`, privado):

| Nível | Habilidades desbloqueadas |
|---|---|
| 2 | Soco Forte (FISICO), Usar Dipirona (HOT) |
| 3 | Perfurar (DOT), Usar Spray de Pimenta (DEBUFF) |
| 4 | Aplicar Adrenalina (CURA), Focar (SUPORTE) |
| 5 | Ataque Mental (ULTRA) |

XP necessário para o próximo nível: `nivel atual × 60`.

### `Inimigo` (abstrata) → hierarquia de adversários
Estende `Personagem` adicionando contador de turnos (`_contadorTurnos`) e recompensa de XP (`_xpRecompensa`).

**`InimigoComum`** — oponentes introdutórios, sem efeitos especiais:

| Classe | HP | Defesa | Força | XP | Particularidade |
|---|---|---|---|---|---|
| `DesafianteDoBar` | 150 | 15 | 15 | 60 | Apenas ataque físico direto |
| `TrabalhadorNoturno` | 150 | 15 | 20 | 90 | 40% de chance de crítico (dano x2) |
| `SegurancaDeBalada` | 140 | 25 | 15 | 200 | A cada 3 turnos ativa escudo (mitiga metade do dano comum) |

**`InimigoIncomum`** — combatentes intermediários com rotação de habilidades baseada no contador de turnos:

| Classe | HP | Defesa | Força | XP | Rotação |
|---|---|---|---|---|---|
| `RecrutaDoutrinado` | 300 | 30 | 35 | 400 | Soco padrão · Sangramento (a cada 2) · Grito de Doutrina/buff em si (a cada 4) · Intimidação/debuff de defesa no alvo (a cada 5) |
| `GuardaPatrimonial` | 300 | 40 | 25 | 400 | Cassetete padrão · Atordoamento/debuff de força (a cada 3) · Gás Lacrimogêneo/debuff de defesa (a cada 4) · Bandagem de Campo/HoT em si (a cada 5) |
| `SabotadorRival` | 300 | 20 | 45 | 400 | Molotov + Queimadura todo turno · Gás Paralisante (a cada 3) · Explosão Total/ULTRA que ignora escudo (a cada 5, substitui o ataque padrão) |

**`InimigoBoss`** — chefão final com máquina de estados de fase:

`TylerDurden` (HP 400, Defesa 30, Força 40, XP `500 × nível`):
- **Fase 1** (HP > 50%): Soco do Caos (FISICO); a cada 3 turnos aplica Sangramento (DoT); a cada 4 turnos aplica Foco Destrutivo (buff de força em si mesmo).
- **Fase 2** (HP entre 20% e 50%): troca para Impacto Psicológico (ESPECIAL); a cada 3 turnos aplica Humilhação (debuff de defesa no alvo) e Sangramento; a cada 4 turnos usa Resiliência Estóica (HoT em si mesmo).
- **Cura Estóica** (HP próprio < 20%, dispara uma única vez): recupera `30 × nível` de HP e ainda ataca no mesmo turno.
- **Desconstrução Total** (HP do alvo < 30%, dispara uma única vez): ataque ULTRA que ignora escudo, com dano `força × 3` (substitui o turno normal).

### `Habilidade`
*Value object* imutável que descreve uma ação (nome, `TipoHabilidade`, custo de Energia/MP, valor base, atributo afetado e duração). Validado defensivamente no construtor — lança `HabilidadeInvalidaException` para nome vazio, custos/valor negativos, duração ausente em efeitos contínuos (DoT/HoT/Suporte/Debuff) ou atributo alvo ausente em Buff/Debuff.

`TipoHabilidade`: `FISICO`, `ESPECIAL`, `ULTRA`, `CURA`, `SUPORTE` (buff), `DEBUFF`, `DOT`, `HOT`.

### `Combate`
Mediador do confronto por turnos entre `Aventureiro` e `Inimigo` (padrão *Mediator*). Valida no construtor que ambos os combatentes estejam vivos (lança `CombateInvalidoException` caso contrário), alterna `turnoDoJogador()`/`turnoDoInimigo()` protegidos por `try/catch`, renderiza o estado do combate via `InterfaceJogo` e, ao final, concede XP em caso de vitória e limpa todos os efeitos temporários de ambos os lados.

### `Cena` e `Escolha`
`Cena` é um nó do grafo narrativo (ID, texto, flag de checkpoint e uma lista de `Escolha`). `Escolha` é a aresta: texto exibido, ID de destino, flag de combate (`geraCombate`) com o tipo de inimigo associado (`tipoInimigo`), e um ID de item de recompensa (`itemGanhoID`).

### `MotorJogo`
Motor/engine do jogo. Mantém o roteiro completo (`std::map<int, Cena>`), o ID da cena atual e a referência ao jogador. Principais responsabilidades:
- `rodar()` — loop principal: exibe a cena, marca checkpoint se aplicável, apresenta as escolhas e processa a transição (incluindo combate, se a escolha exigir).
- `realizarCombate(tipoInimigo)` — fábrica que instancia o inimigo correto via `unique_ptr` (RAII) e delega o confronto a um objeto `Combate`.
- `processarDerrota()` — restaura o herói (`dormir()`) e retorna ao checkpoint salvo.
- `checarEventosEspeciais(escolha)` — concede itens narrativos que aplicam buffs permanentes de força ou defesa (tabela interna de IDs 101–105).

### `InterfaceJogo`
Classe inteiramente estática responsável por todo o I/O de console: exibição de texto, menus numerados com validação de entrada (`solicitarEscolha`), exibição de status (`exibirStatus`), limpeza de tela e pausa (`pausar`). Isola o restante do sistema de chamadas diretas a `std::cin`/`std::cout`.

## ⚔️ Mecânicas de Combate

* **Decisões narrativas** — o fluxo da história é alterado por menus de escolha numerados, ex.: `1. Lutar  2. Fugir  3. Conversar`.
* **Gestão de Energia** — recuperação de **8 pontos por turno**; ações custam entre 2 e 6 pontos (ex.: erguer escudo custa 6).
* **Mecânica de MP** — recuperação de **2 pontos por turno**; consumida por habilidades especiais (ofensivas, de cura ou de suporte).
* **Escudo** — custa 6 de Energia, mitiga pela metade ataques físicos/especiais comuns, é ignorado por ataques `ULTRA` e não impede `DOT`; expira no início do turno seguinte do herói.
* **Ciclo de sobrevivência** — frascos de cura (3 cargas, restauram 40% do HP máximo) e o ato de "Dormir" (em pontos específicos da trama) restauram HP, MP, Energia e os frascos por completo.
* **Efeitos contínuos** — Buffs/Debuffs alteram temporariamente Força ou Defesa; DoT/HoT aplicam dano ou cura a cada turno, ambos com duração decrescente até expirar.

> **Nota técnica:** atualmente o parâmetro `nivel` recebido pelos construtores dos inimigos (`Inimigo`, `InimigoComum`, `InimigoIncomum`, `InimigoBoss`) é usado apenas para repassar `_nivel` e calcular alguns valores de habilidade (ex.: `4 * nivel`), mas **HP, Defesa e Força base são valores fixos** — não escalam automaticamente com o nível do inimigo. Isso é relevante caso o grupo queira implementar escalonamento real de dificuldade por nível no futuro.

## 🧪 Testes e Cobertura

A suíte de testes usa **doctest** (single header) com **gcov/gcovr** para análise de cobertura, organizada em um arquivo de teste por classe (`test_Personagem`/`test_Aventureiro`, `test_InimigoComum`, `test_InimigoIncomum`, `test_InimigoBoss`, `test_Habilidade`, `test_Combate`, `test_Cena`, `test_MotorJogo`, `test_InterfaceJogo`) mais um `main_test.cpp` com `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`. A meta do grupo é manter a cobertura de linhas acima de 80%.

## 📁 Estrutura de Arquivos

```
.
├── Personagem.hpp / .cpp        # Classe base abstrata
├── Aventureiro.hpp / .cpp       # Herói jogável
├── Inimigo.hpp / .cpp           # Base abstrata dos inimigos
├── InimigoComum.hpp / .cpp      # DesafianteDoBar, TrabalhadorNoturno, SegurancaDeBalada
├── InimigoIncomum.hpp / .cpp    # RecrutaDoutrinado, GuardaPatrimonial, SabotadorRival
├── InimigoBoss.hpp / .cpp       # TylerDurden
├── Habilidade.hpp / .cpp        # Value object de habilidades + TipoHabilidade
├── Combate.hpp / .cpp           # Mediador do loop de batalha
├── Cena.hpp / .cpp              # Nó narrativo + struct Escolha
├── MotorJogo.hpp / .cpp         # Engine / roteiro / fábrica de inimigos
├── InterfaceJogo.hpp / .cpp     # Camada de I/O de console
├── main.cpp                     # Ponto de entrada do jogo
├── doctest.h                    # Framework de testes (single header)
├── main_test.cpp                # Runner dos testes (doctest)
└── test_*.cpp                   # Testes unitários por classe
```

## 🚀 Como Compilar e Rodar

**Jogo:**
```bash
g++ -std=c++17 -I. main.cpp Personagem.cpp Aventureiro.cpp Inimigo.cpp \
    InimigoComum.cpp InimigoIncomum.cpp InimigoBoss.cpp Habilidade.cpp \
    Combate.cpp Cena.cpp MotorJogo.cpp InterfaceJogo.cpp -o jogo
./jogo
```

**Testes (com cobertura):**
```bash
g++ -std=c++17 -I. --coverage -fprofile-arcs -ftest-coverage \
    Personagem.cpp Aventureiro.cpp Inimigo.cpp InimigoComum.cpp InimigoIncomum.cpp \
    InimigoBoss.cpp Habilidade.cpp Combate.cpp Cena.cpp MotorJogo.cpp InterfaceJogo.cpp \
    test_*.cpp main_test.cpp -o run_tests
./run_tests
gcovr --root . --html-details -o coverage.html
```

## 📚 Documentação Técnica

Detalhes sobre a modelagem de classes, diagramas e regras completas de combate estão disponíveis na pasta `/design`.
