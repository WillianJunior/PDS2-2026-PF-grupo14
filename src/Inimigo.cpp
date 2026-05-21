#include "Inimigo.hpp"

Inimigo::Inimigo(std::string nome, int hp, int defesa, int nivel, int xp, std::string tipo)
    : Personagem(nome, hp, defesa, nivel) {}

void Inimigo::executarTurno(Personagem& alvo) {}

int Inimigo::getXPRecompensa() const { return 0; }
