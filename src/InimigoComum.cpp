#include "InimigoComum.hpp"

// ============================================================================
// IMPLEMENTAÇÃO DA CLASSE: GOBLIN (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

Goblin::Goblin(std::string nome, int nivel)
    : Inimigo(nome, 40 * nivel, 4 * nivel, 8 * nivel, nivel, 20 * nivel) {}

void Goblin::executarTurno(Personagem& alvo) { int danoDoGoblin = _forcaBase; //Vai ser Alterado
    alvo.receberDano(danoDoGoblin, TipoHabilidade::FISICO); }

// ============================================================================
// IMPLEMENTAÇÃO DA CLASSE: SLIME (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

Slime::Slime(std::string nome, int nivel)
    : Inimigo(nome, 30 * nivel, 2 * nivel, 5 * nivel, nivel, 10 * nivel) {}

void Slime::executarTurno(Personagem& alvo) {int danoSlime = _forcaBase; //Vai ser Alterado
    alvo.receberDano(danoSlime, TipoHabilidade::FISICO);  }