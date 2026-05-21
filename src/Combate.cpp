#include "Combate.hpp"

Combate::Combate(Aventureiro& jogador, Inimigo& inimigo) 
    : _jogador(jogador), _inimigo(inimigo), _turnoAtual(0) {}

bool Combate::iniciar() { return false; }

void Combate::turnoDoJogador() {}

void Combate::turnoDoInimigo() {}

bool Combate::verificarContinuacao() { return false; }
