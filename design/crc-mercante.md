# Cartões CRC – Itens e Inventário

## 🔗 Cartão CRC 5 – Merchant

| Responsabilidades | Colaborações |
|---|---|
| Armazena a lista de itens disponíveis para venda no estoque | `Item` |
| Define e calcula os preços de compra e venda baseados no valor base | `Item` |
| Processa a troca de ouro por itens do estoque para o jogador | `Inventory` |
| Compra itens do inventário do jogador e adiciona ouro ao saldo | `Inventory` |
| Exibe a interface de diálogo e opções de negociação | `GameUI` |
| Verifica se o jogador possui ouro suficiente para a transação | `Character` |
| Remove itens vendidos do próprio estoque e os transfere | `Inventory` |
| Atualiza o saldo de ouro do personagem após cada transação | `Character` |