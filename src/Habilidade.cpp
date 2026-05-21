#include "Habilidade.hpp"

// ============================================================================
// CONSTRUTOR
// ============================================================================

Habilidade::Habilidade(std::string nome, TipoHabilidade tipo, int custoE, int custoM, int valorBase, int duracao) {
    _nome = nome; _tipo = tipo; _custoEnergia = custoE; _custoMP = custoM; 
    _valorBase = valorBase; _duracao = duracao;
}

// ============================================================================
// GETTERS E MÉTODOS DE CHECAGEM (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

std::string Habilidade::getNome() const { return _nome; }

TipoHabilidade Habilidade::getTipo() const { return _tipo; }

int Habilidade::getCustoEnergia() const { return _custoEnergia; }

int Habilidade::getCustoMP() const { return _custoMP; }

int Habilidade::getValorBase() const { return _valorBase; }

int Habilidade::getDuracao() const { return _duracao; }

bool Habilidade::ehEfeitoContinuo() const { return _duracao > 0; }