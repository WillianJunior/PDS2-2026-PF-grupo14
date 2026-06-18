#include "Cena.hpp"

// ============================================================================
// CONSTRUTOR
// ============================================================================

Cena::Cena(int id, std::string texto, bool checkpoint): _id(id), _textoNarrativo(texto), _checkpoint(checkpoint) {
    ;
}

// ============================================================================
// MÉTODOS MEMBROS e GETTERS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void Cena::adicionarEscolha(Escolha e) { (void)e; }

int Cena::getID() const { return _id; }

std::string Cena::getTexto() const { return ""; }

bool Cena::isCheckpoint() const { return false; }

const std::vector<Escolha>& Cena::getEscolhas() const { return _escolhas; }