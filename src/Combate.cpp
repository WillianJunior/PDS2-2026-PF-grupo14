#include "Combate.hpp"

// ============================================================================
// CONSTRUTOR
// ============================================================================

Combate::Combate(Aventureiro& jogador, Inimigo& inimigo)
    : _jogador(jogador), _inimigo(inimigo) { _turnoAtual = 0; }

// ============================================================================
// MÉTODOS PÚBLICOS E PRIVADOS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

bool Combate::iniciar() { return false; }

void Combate::turnoDoJogador() {}

void Combate::turnoDoInimigo() {}

bool Combate::verificarContinuacao() { return false; }

void Combate::renderizarInterfaceTurno() {}

void Combate::renderizarInicioCombate() {}

void Combate::renderizarFimCombate(bool jogadorVenceu) { (void)jogadorVenceu; }