#include "InimigoBoss.hpp"

Dragao::Dragao(std::string nome, int nivel)
    : Inimigo(nome, 0, 0, nivel, 0, "Dragao"), _emFuria(false), _furiaContador(0) {}

void Dragao::executarTurno(Personagem& alvo) {}
