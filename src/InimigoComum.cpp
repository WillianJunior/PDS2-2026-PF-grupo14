#include "InimigoComum.hpp"

Goblin::Goblin(std::string nome, int nivel)
    : Inimigo(nome, 0, 0, nivel, 0, "Goblin") {}

void Goblin::executarTurno(Personagem& alvo) {}

Slime::Slime(std::string nome, int nivel)
    : Inimigo(nome, 0, 0, nivel, 0, "Slime") {}

void Slime::executarTurno(Personagem& alvo) {}
