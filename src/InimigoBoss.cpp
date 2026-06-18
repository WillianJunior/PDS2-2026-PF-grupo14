#include "InimigoBoss.hpp"

// ============================================================================
// CONSTRUTOR - DRAGAO
// ============================================================================

Dragao::Dragao(std::string nome, int nivel)
    : Inimigo(nome, 150 * nivel, 15 * nivel, 20 * nivel, nivel, 100 * nivel) { _emFuria = false; _furiaContador = 0; }

// ============================================================================
// MÉTODOS MEMBROS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void Dragao::executarTurno(Personagem& alvo) { (void)alvo; }