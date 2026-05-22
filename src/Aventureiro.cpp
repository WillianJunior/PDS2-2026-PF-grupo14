#include "Aventureiro.hpp"
#include <iostream>

// ============================================================================
// CONSTRUTOR
// ============================================================================

Aventureiro::Aventureiro(std::string nome, int hp, int defesa, int forca)
    : Personagem(nome, hp, defesa, forca, 1) { 
    _energia = 100; _energiaMax = 100; _mp = 50; _mpMax = 50; _frascos = 3;
    _bonusArma = 0; _bonusArmadura = 0; _xp = 0; _xpProxNivel = 100;
    _idCenaCheckpoint = 0; _escudoAtivo = false;
}

// ============================================================================
// MÉTODOS DE COMBATE E AÇÕES (Estilo Esqueleto/TDD Red)
// ============================================================================

void Aventureiro::executarTurno(Personagem& alvo) { (void)alvo; }
void Aventureiro::receberDano(int dano) { (void)dano; }
int Aventureiro::getDefesa() const { return 0; }
void Aventureiro::recuperarRecursos() {}
void Aventureiro::consumirMP(int qtd) { (void)qtd; }
void Aventureiro::consumirEnergia(int qtd) { (void)qtd; }
void Aventureiro::usarEscudo() {}
bool Aventureiro::usarFrasco() { return false; }
void Aventureiro::dormir() {}

// ============================================================================
// MÉTODOS DE PROGRESSÃO E BUFFS
// ============================================================================

void Aventureiro::aprenderHabilidade(Habilidade hb) { (void)hb; }
bool Aventureiro::ganharExperiencia(int qtd) { (void)qtd; return false; }
void Aventureiro::buffArma(int valor) { (void)valor; }
void Aventureiro::buffArmadura(int valor) { (void)valor; }
std::string Aventureiro::getDeclaracaoStatus() const { return ""; }

// ============================================================================
// GETTERS E SETTERS DE ESTADO
// ============================================================================

int Aventureiro::getForcaTotal() const { return 0; }
int Aventureiro::getIDCheckpoint() const { return 0; }
void Aventureiro::setIDCheckpoint(int id) { (void)id; }
int Aventureiro::getMP() const { return 0; }
int Aventureiro::getMPMax() const { return 0; }
int Aventureiro::getEnergia() const { return 0; }
int Aventureiro::getEnergiaMax() const { return 0; }

const std::vector<Habilidade>& Aventureiro::getHabilidades() const { 
    return _habilidadesConhecidas; 
}

// ============================================================================
// MÉTODO PRIVADO
// ============================================================================

void Aventureiro::subirNivel() {}