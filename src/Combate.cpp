#include "Combate.hpp"
#include "InterfaceJogo.hpp"
#include <string>

// ============================================================================
// CONSTRUTOR
// ============================================================================

Combate::Combate(Aventureiro& jogador, Inimigo& inimigo)
    : _jogador(jogador), _inimigo(inimigo), _turnoAtual(0) {
}

// ============================================================================
// MÉTODOS PÚBLICOS E PRIVADOS
// ============================================================================

bool Combate::iniciar() {
    renderizarInicioCombate();

    // Loop principal baseado nas invariantes de vida
    while (verificarContinuacao()) {
        _turnoAtual++;
        renderizarInterfaceTurno();

        // 1. Turno do Jogador
        turnoDoJogador();

        // Se o inimigo morrer no turno do jogador, interrompe o loop
        if (!verificarContinuacao()) {
            break; 
        }

        // 2. Turno do Inimigo
        turnoDoInimigo();
    }

    // Ao fim do loop, alguém morreu. Descobrimos quem foi.
    bool jogadorVenceu = _jogador.estaVivo();
    
    // Resolvemos as consequências (XP ou Game Over)
    renderizarFimCombate(jogadorVenceu);

    return jogadorVenceu;
}

void Combate::turnoDoJogador() {
    _jogador.executarTurno(_inimigo);
}

void Combate::turnoDoInimigo() {
    _inimigo.executarTurno(_jogador);
}

bool Combate::verificarContinuacao() {
    // O combate continua apenas se AMBOS estiverem vivos
    return _jogador.estaVivo() && _inimigo.estaVivo();
}

void Combate::renderizarInterfaceTurno() {
    InterfaceJogo::exibirTexto("\n===========================================");
    InterfaceJogo::exibirTexto("                 TURNO " + std::to_string(_turnoAtual));
    InterfaceJogo::exibirTexto("===========================================");
    
    // Mostra um resumo rápido do HP do monstro (status completo do herói já é exibido no turno dele)
    InterfaceJogo::exibirTexto("-> " + _inimigo.getNome() + " [HP: " + std::to_string(_inimigo.getHP()) + "]");
}

void Combate::renderizarInicioCombate() {
    InterfaceJogo::exibirTexto("\n===========================================");
    InterfaceJogo::exibirTexto("             COMBATE INICIADO!");
    InterfaceJogo::exibirTexto("===========================================");
    InterfaceJogo::exibirTexto(_jogador.getNome() + " VS " + _inimigo.getNome() + "\n");
}

void Combate::renderizarFimCombate(bool jogadorVenceu) {
    InterfaceJogo::exibirTexto("\n===========================================");
    InterfaceJogo::exibirTexto("              FIM DE COMBATE");
    InterfaceJogo::exibirTexto("===========================================");

    if (jogadorVenceu) {
        InterfaceJogo::exibirTexto("Vitória! Você derrotou " + _inimigo.getNome() + ".");
        
        // Puxa a recompensa em XP da entidade Inimigo e aplica ao herói
        int xpGanha = _inimigo.getXPRecompensa();
        InterfaceJogo::exibirTexto("Recompensa: +" + std::to_string(xpGanha) + " XP.");
        _jogador.ganharExperiencia(xpGanha);
    } else {
        InterfaceJogo::exibirTexto("Derrota... A jornada de " + _jogador.getNome() + " termina aqui.");
    }
}