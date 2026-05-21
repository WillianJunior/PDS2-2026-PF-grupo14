#include "Cena.hpp"

Cena::Cena(int id, std::string texto, bool checkpoint) {}

void Cena::adicionarEscolha(Escolha e) {}

int Cena::getID() const { return 0; }

std::string Cena::getTexto() const { return ""; }

const std::vector<Escolha>& Cena::getEscolhas() const { return _escolhas; }

bool Cena::isCheckpoint() const { return false; }
