# 📇 Parte 1: Cartões CRC (Até 10 Classes Principais)

Os cartões CRC mapeiam as responsabilidades e colaborações das principais classes do sistema.

---

## 1. Classe: `Personagem` *(Abstrata)*

**Descrição:**  
Superclasse abstrata responsável pelos atributos básicos e pelas regras gerais de combate.

### Responsabilidades
- Armazenar:
  - nome
  - HP atual
  - HP máximo
  - nível
  - defesa base
- Fornecer lógica base para:
  - sofrer dano (`receberDano`)
  - recuperar vida (`receberCura`)
- Gerenciar o conjunto de habilidades do personagem
- Definir os contratos abstratos:
  - `executarTurno()`
  - `getDeclaracaoStatus()`

### Colaboradores
- `Habilidade`

---

## 2. Classe: `Aventureiro`

**Descrição:**  
Representa o personagem controlado pelo jogador, contendo progressão, recursos e evolução.

### Responsabilidades
- Exibir o menu de ações durante o turno do jogador
- Gerenciar:
  - MP
  - Energia
- Acumular XP
- Gerenciar subida de nível (*Level Up*)
- Aplicar bônus permanentes de:
  - armas
  - armaduras
- Controlar o ID da cena usada como checkpoint

### Colaboradores
- `Personagem`
- `Habilidade`
- `InterfaceJogo`

---

## 3. Classe: `Inimigo` *(Abstrata)*

**Descrição:**  
Superclasse abstrata que define o comportamento base das inteligências artificiais dos monstros.

### Responsabilidades
- Armazenar o XP concedido ao jogador após derrota
- Controlar contador interno de turnos da IA

### Colaboradores
- `Personagem`

---

## 4. Classe: `Slime / Goblin`

**Descrição:**  
Inimigos comuns utilizados em batalhas iniciais ou tutoriais.

### Responsabilidades
- Implementar IA simples:
  - ataque básico
  - habilidade simples
- Fornecer recompensas reduzidas

### Colaboradores
- `Inimigo`
- `Personagem`

---

## 5. Classe: `Bruxa / GolemPedra`

**Descrição:**  
Inimigos intermediários com mecânicas especiais e padrões alternados.

### Responsabilidades
- Implementar IA baseada em turnos alternados
  - Exemplo:
    - Golem usa defesa em um turno
    - Bruxa drena mana no outro

### Colaboradores
- `Inimigo`
- `Personagem`

---

## 6. Classe: `Dragao`

**Descrição:**  
Chefe principal da campanha ou de uma grande ramificação da história.

### Responsabilidades
- Implementar IA avançada baseada na vida restante
- Entrar em estado de fúria abaixo de determinado HP
- Executar múltiplos ataques por turno
- Utilizar habilidades de área massivas

### Colaboradores
- `Inimigo`
- `Personagem`

---

## 7. Classe: `MotorJogo`

**Descrição:**  
Engine principal responsável pelo fluxo geral do jogo.

### Responsabilidades
- Executar o loop principal (`rodar`)
- Carregar e transicionar cenas da história
- Interpretar escolhas do jogador
- Iniciar combates automaticamente
- Instanciar inimigos dinamicamente
- Resetar estado do jogo após derrota
- Reposicionar jogador no checkpoint

### Colaboradores
- `Aventureiro`
- `Cena`
- `Combate`

---

## 8. Classe: `Cena`

**Descrição:**  
Representa um nó narrativo da campanha.

### Responsabilidades
- Armazenar:
  - ID da cena
  - texto descritivo
  - indicador de checkpoint
- Armazenar lista de escolhas possíveis

### Colaboradores
- `Escolha` *(struct)*

---

## 9. Classe: `Combate`

**Descrição:**  
Gerenciador das batalhas em turnos.

### Responsabilidades
- Controlar o fluxo de turnos até a derrota de um lado
- Exibir mensagens de dano e cura
- Conceder XP ao jogador após vitória

### Colaboradores
- `Aventureiro`
- `Inimigo`

---

## 10. Classe: `InterfaceJogo`

**Descrição:**  
Responsável exclusivamente pela entrada e saída de dados no console.

### Responsabilidades
- Exibir caixas de diálogo e narrativa
- Renderizar barras visuais de:
  - HP
  - MP
  - Energia
- Capturar e validar entradas numéricas do usuário

### Colaboradores
- `Personagem`
- `Aventureiro`

---
