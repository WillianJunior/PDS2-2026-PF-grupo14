#include "MotorJogo.hpp"

MotorJogo::MotorJogo(Aventureiro& player)
    : _player(player), _idCenaAtual(1) {}

void MotorJogo::rodar() {}

bool MotorJogo::realizarCombate(std::string tipoInimigo) { return false; }

void MotorJogo::inicializarHistoria() {}

void MotorJogo::processarDerrota() {}

void MotorJogo::checarEventosEspeciais(const Escolha& escolha) {}
