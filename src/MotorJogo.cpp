#include "MotorJogo.hpp"
#include "Inimigo.hpp"
#include "Combate.hpp"

// ============================================================================
// CONSTRUTOR (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

MotorJogo::MotorJogo(Aventureiro& player) 
    : _player(player) { _idCenaAtual = 0; }

// ============================================================================
// MÉTODOS PÚBLICOS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void MotorJogo::rodar() {}

bool MotorJogo::realizarCombate(std::string tipoInimigo) { (void)tipoInimigo; return false; }

// ============================================================================
// MÉTODOS PRIVADOS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void MotorJogo::inicializarHistoria() {}

void MotorJogo::processarDerrota() {}