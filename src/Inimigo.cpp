#include "Inimigo.hpp"

// ============================================================================
// CONSTRUTOR
// ============================================================================

Inimigo::Inimigo(std::string nome, int hp, int defesa, int forca, int nivel, int xp)
    : Personagem(nome, hp, defesa, forca, nivel) { _contadorTurnos = 0; _xpRecompensa = xp; }

// ============================================================================
// MÉTODOS MEMBROS E GETTERS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void Inimigo::executarTurno(Personagem& alvo) { (void)alvo; }

int Inimigo::getXPRecompensa() const { return 0; }

std::string Inimigo::getDeclaracaoStatus() const { return ""; }