# Cartões CRC

## Cartão CRC — Aventureiro

| Responsabilidades | Colaborações |
|---|---|
| Armazena atributos (HP, Força, Agilidade, Inteligência) | Cena |
| Executa ações de combate e decisões de diálogo | Combate, Cena |
| Retorna ao checkpoint do capítulo atual em caso de derrota | Cena |

## Cartão CRC — Cena (Substitui o Mapa)

| Responsabilidades | Colaborações |
|---|---|
| Armazena o texto descritivo e diálogos da etapa atual | InterfaceJogo |
| Gerencia a sequência lógica (Cena 1 -> Cena 2) | Aventureiro |
| Define se a cena termina em um Combate ou em uma Decisão | Combate |
| Armazena o estado do checkpoint (Capítulo atual) | Aventureiro |

## Cartão CRC — Combate

| Responsabilidades | Colaborações |
|---|---|
| Gerencia turnos entre Aventureiro e Inimigo | Aventureiro, Inimigo |
| Processa cálculos de dano, uso de habilidades e custo de Energia | Habilidade |
| Notifica a Cena quando o desafio for vencido para prosseguir | Cena |

## Cartão CRC — Inimigo

| Responsabilidades | Colaborações |
|---|---|
| Possui atributos de combate específicos para aquela cena | Combate |
| Executa ações ofensivas baseadas em script de turno | Aventureiro |

## Cartão CRC — Inventário e Itens

| Responsabilidades | Colaborações |
|---|---|
| Armazena consumíveis e equipamentos ganhos na história | Aventureiro |
| Aplica efeitos imediatos (Cura/Buff) durante o turno | Aventureiro |
| Gerencia frascos de cura (máximo 3, recarregados ao dormir) | Aventureiro |

## Cartão CRC — InterfaceJogo

| Responsabilidades | Colaborações |
|---|---|
| Exibe o log da história e as falas dos personagens | Cena |
| Apresenta menus de escolha (Ex: 1. Atacar, 2. Fugir, 3. Conversar) | Combate, Cena |
| Exibe atributos de combate: HP, MP, Energia, Escudo | Combate |
