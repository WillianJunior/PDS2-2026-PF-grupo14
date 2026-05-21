#include "Cena.hpp"

// ============================================================================
// CONSTRUTOR
// ============================================================================

Cena::Cena(int id, std::string texto, bool checkpoint) {
    (void)id; (void)texto; (void)checkpoint;
}

// ============================================================================
// MÉTODOS MEMBROS e GETTERS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void Cena::adicionarEscolha(Escolha e) { (void)e; }

int Cena::getID() const { return 0; }

std::string Cena::getTexto() const { return ""; }

bool Cena::isCheckpoint() const { return false; }

const std::vector<Escolha>& Cena::getEscolhas() const { return _escolhas; }