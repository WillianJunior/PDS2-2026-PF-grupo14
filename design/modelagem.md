# Modelagem do Sistema RPG (Versão Narrativa Linear)

## Histórias de Usuário — Progressão e História

### História do Usuário 1: Avanço de Capítulo
**Descrição:** Como aventureiro, quero que a história avance automaticamente após eu superar um desafio, para que eu possa vivenciar a narrativa sem precisar procurar o caminho.

**Critérios de Aceitação:**
- O sistema deve carregar a próxima "Cena" assim que o combate atual terminar.
- O sistema deve exibir o texto narrativo de transição entre os eventos.
- Não deve haver comandos de movimentação (Norte/Sul); o progresso é gatilhado por ações.

### História do Usuário 2: Pontos de Salvamento Narrativos
**Descrição:** Como aventureiro, quero que meu progresso seja salvo no início de cada capítulo, para que eu não perca minha evolução na história.

**Critérios de Aceitação:**
- O sistema deve registrar automaticamente o checkpoint ao iniciar uma nova cena relevante.
- Em caso de derrota, o jogador deve retornar ao início da cena/diálogo atual.

## Histórias de Usuário — Combate e Itens

### História do Usuário 1: Combate Scriptado
**Descrição:** Como aventureiro, quero encontrar inimigos em pontos específicos da trama, para que as lutas tenham significado narrativo.

**Critérios de Aceitação:**
- O combate deve ser iniciado por um evento de texto (Ex: "Um guarda barra seu caminho").
- A vitória deve desbloquear o próximo parágrafo da história ou item de recompensa.

---

## Regras do Sistema de Combate

### Turnos
- Revezamento dos oponentes em realizar ações.
- Você realiza ações e encerra o turno quando quiser para passar a vez para o adversário.

### Ataque
- Causa dano nos seus adversários.
- Você pode atacar e usar ataques especiais durante os turnos.
- Você deve digitar os números indicados para o seu personagem executar uma ação.
- Você pode usar um ataque ultra-especial que consome todo o MP.
- Seus ataques serão amortecidos pela defesa de seu adversário.
- Ataques que possuem um asterisco (*) no nome causam dano por turno.

### HP (Saúde do Jogador)
- Ao se curar (+), você não passa o turno.
- Você possui 3 frascos de cura, mas também pode se curar usando magia.
- Você recupera os frascos ao dormir.

### MP (Energia Mental)
- Energia mental que possibilita realizar ataques especiais.
- Você recupera 1 ponto de MP por turno.

### Energia
- Disposição para realizar uma ação.
- Você recupera 5 pontos de energia por turno.
- Você pode realizar mais de uma ação durante o seu turno, desde que tenha energia.
- Toda ação possui um custo de energia.
- Alguns exemplos: atacar=3, curar=2, escudo=6.

### Escudo
- Aumenta sua defesa.
- Ao usar o escudo, sua defesa será multiplicada por 4.
- Para usar o escudo, será preciso ter 6 pontos de energia.
- Ataques ultra-especiais ignoram o escudo.
- Outros ataques também ignoram o escudo e serão simbolizados com (-).

### Outros
- Você ganhará experiência e ouro ao vencer batalhas.
- Você pode equipar diferentes espadas e armaduras que aumentam diferentes atributos.
- Você não possui tempo para jogar, portanto, se perdeu alguma informação, basta rolar para cima.

---

## O que mudou na Modelagem?

1. **Remoção da Lógica de Salas:** O cartão Mapa foi substituído pelo cartão Cena. Não existe mais "verificar se a porta está trancada" via coordenadas, mas sim "verificar se o requisito da cena foi batido".

2. **Automação do Fluxo:** As Histórias de Usuário agora focam na continuidade narrativa. O aventureiro não "explora áreas escondidas" por conta própria; as áreas são reveladas conforme as escolhas feitas nos diálogos.

3. **Checkpoints Simplificados:** O salvamento agora é vinculado ao progresso na história (Capítulos) e não mais a um objeto físico ou sala específica no mapa.

4. **Sistema de Combate Detalhado:** Adicionadas as regras completas de Turnos, Ataque, HP, MP, Energia e Escudo, formalizando a mecânica de jogo conforme especificado.


# Melhorias na Arquitetura do Projeto 07/05/2026

## 1. Generalização de Personagens (Herança)

### O que mudou
Em vez de classes isoladas para `Aventureiro` e `Inimigo`, foi criada uma classe base chamada `Personagem`.

### Vantagem
Evita repetição de código. Lógicas de vida, atributos básicos e a função de “receber dano” ficam centralizadas em um único lugar.  
As classes `Aventureiro` e `Inimigo` apenas herdam essa base e adicionam suas particularidades.

---

## 2. Criação do Motor do Jogo (`MotorJogo`)

### O que mudou
A responsabilidade de “rodar o jogo” saiu da `main` ou da classe `Cena` e foi transferida para uma classe controladora dedicada.

### Vantagem
Separa o **Roteiro** (dados da cena) da **Execução** (lógica do loop).  

O `MotorJogo` decide:
- quando iniciar um combate;
- quando carregar diálogos;
- quando verificar derrota do jogador.

Isso mantém o fluxo organizado e desacoplado.

---

## 3. Objetivação de Habilidades (`Habilidade`)

### O que mudou
As ações de combate (ataques, magias e buffs) deixaram de ser apenas strings ou estruturas `if/else` dentro do jogador e passaram a ser objetos próprios.

### Vantagem
Facilita a expansão do jogo.  

Exemplo:
- Para criar um “Ataque Especial com Sangramento”, basta criar uma nova `Habilidade` com parâmetros específicos, sem alterar o código principal do sistema de combate.

---

## 4. Isolamento da Interface (`InterfaceJogo`)

### O que mudou
Foi feita uma separação rigorosa entre:
- lógica do jogo;
- entrada e saída de dados.

Nenhuma classe de lógica (`Combate`, `Cena`, etc.) deve usar `std::cout` ou `std::cin`.

### Vantagem
O código fica muito mais fácil de testar e manter.  

Caso futuramente o terminal seja substituído por uma interface gráfica, basta reescrever apenas a classe `InterfaceJogo`.

---

## 5. Gestão de Fluxo Narrativo (`Cena` vs `Combate`)

### O que mudou
A classe `Cena` tornou-se apenas um contêiner de dados responsável por:
- textos;
- escolhas;
- próximos destinos da narrativa.

Já a classe `Combate` é temporária e existe somente enquanto a batalha acontece.

### Vantagem
Melhora a clareza das responsabilidades e evita desperdício de memória.  

Uma cena de diálogo não carrega recursos de combate desnecessários, e o combate existe apenas quando realmente necessário.

---

# Resumo das Novas Classes

| Classe | Papel Principal |
|---|---|
| `Personagem` | Base para vida, defesa e sistema de dano (Herança). |
| `Aventureiro` | Gerencia recursos exclusivos do jogador (Energia, MP, Itens). |
| `Inimigo` | Define comportamento e recompensas dos inimigos. |
| `Cena` | Contém texto, escolhas e progressão narrativa. |
| `Combate` | Gerencia turnos, dano e regras da batalha. |
| `Habilidade` | Define custos e efeitos especiais dos golpes. |
| `MotorJogo` | Coordena o fluxo principal do jogo. |
| `InterfaceJogo` | Responsável pela entrada e saída no terminal. |
