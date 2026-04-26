# Cartões CRC – Itens e Inventário

## Cartão CRC 1 – `Item`

| Responsabilidades | Colaborações |
|---|---|
| Conhece seu nome, descrição e ID único | `Inventory` |
| Conhece seu tipo (consumível, equipamento, acessório) | `Inventory` |
| Conhece seu valor em ouro (para compra e venda) | `Merchant` |
| Conhece se é acumulável (stackable) e o limite máximo por pilha | `Inventory` |
| Conhece a quantidade atual na pilha | `Inventory` |
| Realiza seu efeito ao ser usado (restaura HP, restaura PP, etc.) | `Character` |
| Sabe se pode ser equipado e em qual slot de equipamento pertence | `Character` |
| Verifica se é de uso único e sinaliza remoção após uso | `Inventory` |

---

## Cartão CRC 2 – `Inventory`

| Responsabilidades | Colaborações |
|---|---|
| Conhece a lista de slots (máximo 20) e o item em cada slot | `Item` |
| Verifica se há espaço disponível antes de adicionar um item | `Item` |
| Adiciona um item ao inventário, empilhando se o item for acumulável | `Item` |
| Remove um item ou decrementa sua pilha após uso ou venda | `Item` |
| Exibe o conteúdo completo do inventário ao jogador | `GameUI` |
| Equipa um item, alocando-o no slot de equipamento correspondente | `Character` |
| Desequipa um item, devolvendo-o ao inventário se houver espaço disponível | `Character` |
| Gerencia itens recebidos por drop ao derrotar monstros | `Enemy` |
| Permite a venda de itens para NPCs mercadores | `Merchant` |
| Verifica a quantidade disponível de um item específico na pilha | `Character` |
