#include "Cena.hpp"

// ============================================================================
// CONSTRUTOR
// ============================================================================

Cena::Cena(int id, std::string texto, bool checkpoint): _id(id), _textoNarrativo(texto), _checkpoint(checkpoint) {
}

// ============================================================================
// MÉTODOS MEMBROS e GETTERS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void Cena::adicionarEscolha(Escolha e) { 
    this->_escolhas.push_back(e);
 }

int Cena::getID() const { return _id; }

std::string Cena::getTexto() const { return _textoNarrativo; }

bool Cena::isCheckpoint() const { 
    if (_checkpoint) return true;
    else {return false;}
    
 }

const std::vector<Escolha>& Cena::getEscolhas() const { return _escolhas; }