#include "Personagem.hpp"

// ============================================================================
// CONSTRUTOR
// ============================================================================

Personagem::Personagem(std::string nome, int hp, int defesa, int forca, int nivel) {
    _nome = nome;
    _hp = hp;
    _hpMax = hp;
    _defesaBase = defesa;
    _forcaBase = forca;
    _nivel = nivel;
}

// ============================================================================
// GERENCIAMENTO DE HABILIDADES
// ============================================================================

void Personagem::adicionarHabilidade(const Habilidade& habilidade) {
    _habilidades.push_back(habilidade);
}

const std::vector<Habilidade>& Personagem::getHabilidades() const {
    return _habilidades;
}

// ============================================================================
// SISTEMA DE COMBATE (IMPLEMENTAÇÃO PADRÃO)
// ============================================================================

void Personagem::receberDano(int dano) {
    int danoFinal = dano - getDefesa();
    if (danoFinal < 0) danoFinal = 0;
    
    _hp -= danoFinal;
    if (_hp < 0) _hp = 0;
}

void Personagem::receberCura(int valor) {
    _hp += valor;
    if (_hp > _hpMax) _hp = _hpMax;
}

int Personagem::getDefesa() const {
    return _defesaBase;
}

// ============================================================================
// GETTERS (A PARTE QUE ESTAVA FALTANDO!)
// ============================================================================

int Personagem::getHP() const {
    return _hp;
}

int Personagem::getHPMax() const {
    return _hpMax;
}

bool Personagem::estaVivo() const {
    return _hp > 0;
}

std::string Personagem::getNome() const {
    return _nome;
}

int Personagem::getNivel() const {
    return _nivel;
}