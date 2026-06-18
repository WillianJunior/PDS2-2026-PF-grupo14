#include "InimigoIncomum.hpp"

// ============================================================================
// IMPLEMENTAÇÃO DA CLASSE: BRUXA (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

Bruxa::Bruxa(std::string nome, int nivel)
    : Inimigo(nome, 50 * nivel, 3 * nivel, 12 * nivel, nivel, 40 * nivel) { _mana = 30 * nivel; }

void Bruxa::executarTurno(Personagem& alvo) { (void)alvo; }

// ============================================================================
// IMPLEMENTAÇÃO DA CLASSE: GOLEM DE PEDRA (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

GolemPedra::GolemPedra(std::string nome, int nivel)
    : Inimigo(nome, 100 * nivel, 12 * nivel, 15 * nivel, nivel, 50 * nivel) {}

void GolemPedra::executarTurno(Personagem& alvo) { (void)alvo; }

