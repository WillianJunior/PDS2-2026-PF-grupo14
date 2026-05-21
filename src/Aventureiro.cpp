#include "Aventureiro.hpp"

Aventureiro::Aventureiro(std::string nome, int hp, int defesa, int forca) 
    : Personagem(nome, hp, defesa, 1) {}

void Aventureiro::executarTurno(Personagem& alvo) {}

void Aventureiro::receberDano(int dano) {}

int Aventureiro::getDefesa() const { return 0; }

void Aventureiro::recuperarRecursos() {}

void Aventureiro::consumirMP(int qtd) {}

void Aventureiro::consumirEnergia(int qtd) {}

void Aventureiro::usarEscudo() {}

bool Aventureiro::usarFrasco() { return false; }

void Aventureiro::dormir() {}

void Aventureiro::aprenderHabilidade(Habilidade hb) {}

bool Aventureiro::ganharExperiencia(int qtd) { return false; }

void Aventureiro::buffArma(int valor) {}

void Aventureiro::buffArmadura(int valor) {}

int Aventureiro::getForcaTotal() const { return 0; }

int Aventureiro::getIDCheckpoint() const { return 0; }

const std::vector<Habilidade>& Aventureiro::getHabilidades() const { 
    return _habilidadesConhecidas; 
}

void Aventureiro::subirNivel() {}
