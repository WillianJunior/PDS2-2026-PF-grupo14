#include "Habilidade.hpp"

Habilidade::Habilidade(std::string nome, TipoHabilidade tipo, int custoE, int custoM, int valorBase) {}

std::string Habilidade::getNome() const { return ""; }

TipoHabilidade Habilidade::getTipo() const { return TipoHabilidade::FISICO; }

int Habilidade::getCustoEnergia() const { return 0; }

int Habilidade::getCustoMP() const { return 0; }

int Habilidade::getValorBase() const { return 0; }
