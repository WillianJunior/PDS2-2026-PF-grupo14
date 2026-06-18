# PDS2-2026-PF-grupo14

## Nome dos Integrantes
- Davi Costa
- Gustavo Ribeiro
- Henrique Garcia
- João Pedro Guimarães Carvalho
- Kaio Gomes Vieira

---

## Descrição do Projeto

O projeto aborda o desafio de criar uma experiência de RPG textual onde a progressão é conduzida por uma **Narrativa Linear Interativa**. O software gerencia um fluxo sequencial de cenas onde o avanço não depende de exploração geográfica, mas sim da superação de desafios e, crucialmente, das escolhas do jogador.

O sistema deve processar diferentes ramificações de diálogo e decisões morais ou táticas que alteram o rumo da história imediata. O problema central reside em manter a coerência dessa progressão, garantindo que o estado do mundo e do personagem reflita as decisões tomadas, enquanto gerencia gatilhos automáticos de cena após eventos de combate ou diálogos decisivos.

---

## 🎯 Objetivos Principais

### Arquitetura Baseada em Cenas e Escolhas
Implementar um sistema de nós narrativos onde o usuário interage através de menus de múltipla escolha que definem o próximo estado da história.

### Sistema de Combate Tático
Desenvolver uma mecânica de turnos baseada em **Gerenciamento de Energia**, exigindo que o jogador equilibre custos de ações (**Ataque, Cura, Escudo**) com uma recuperação fixa por turno.

### Persistência de Checkpoints
Garantir que o progresso seja salvo automaticamente no início de cada capítulo, permitindo que o jogador retorne ao ponto de decisão anterior em caso de derrota.

### Interface Orientada a Eventos
Construir um console que apresente dinamicamente os atributos (**HP, MP, Energia**) e os logs de eventos baseados nas ações escolhidas.

---

## 💡 Motivação

Escolhemos o RPG com narrativa linear com o objetivo de aplicar os conhecimentos de **POO (Programação Orientada a Objetos)** passados em sala de aula de forma prazerosa para os membros do grupo, já que todos gostam de jogar.

Além disso, é um tema em que já temos conhecimento prévio sobre o que um RPG deve conter.

A estrutura de um RPG exige uma arquitetura rigorosa, onde conceitos como:

- **Herança**: para diferentes tipos de habilidades e inimigos;
- **Encapsulamento**: para proteger os atributos do aventureiro;
- **Polimorfismo**: para tratar diversas ações de combate de forma genérica;

são aplicados de maneira indispensável.

---

## 🛠️ Regras de Destaque

### Decisões Narrativas
O fluxo da história é alterado por menus de escolha.

**Exemplo:**
1. Lutar  
2. Fugir  
3. Conversar  

### Gestão de Energia
- Recuperação de **5 pontos de energia por turno**
- Ações custam entre **2 e 6 pontos**

### Mecânica de MP
Energia mental utilizada para ataques especiais, com recuperação lenta de **1 ponto por turno**.

### Ciclo de Sobrevivência
Recursos como frascos de cura e MP são restaurados através da ação **"Dormir"** em pontos específicos da trama.

### Documentação Técnica
Detalhes sobre a modelagem de classes, diagramas e regras completas de combate estão disponíveis na pasta `/design`.

---