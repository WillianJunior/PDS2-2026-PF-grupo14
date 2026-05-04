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

5. **Renomeação de SP para MP:** A energia mental responsável pelos ataques especiais passou a se chamar MP, refletindo melhor sua função narrativa no sistema.
