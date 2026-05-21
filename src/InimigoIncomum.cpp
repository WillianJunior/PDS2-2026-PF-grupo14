#include "InimigoIncomum.hpp"

Bruxa::Bruxa(std::string nome, int nivel)
    : Inimigo(nome, 0, 0, nivel, 0, "Bruxa"), _mana(0) {}

void Bruxa::executarTurno(Personagem& alvo) {}

GolemPedra::GolemPedra(std::string nome, int nivel)
    : Inimigo(nome, 0, 0, nivel, 0, "GolemPedra") {}

void GolemPedra::executarTurno(Personagem& alvo) {}

void GolemPedra::receberDano(int dano) {}
