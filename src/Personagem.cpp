#include "Personagem.hpp"

Personagem::Personagem(std::string nome, int hp, int defesa, int nivel) {}

void Personagem::receberDano(int dano) {}

void Personagem::receberCura(int valor) {}

int Personagem::getDefesa() const { return 0; }

int Personagem::getHP() const { return 0; }

int Personagem::getHPMax() const { return 0; }

bool Personagem::estaVivo() const { return false; }

std::string Personagem::getNome() const { return ""; }

int Personagem::getNivel() const { return 0; }
