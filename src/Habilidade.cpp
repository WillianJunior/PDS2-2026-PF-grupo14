#include "Habilidade.hpp"
#include <stdexcept>

// ============================================================================
// CONSTRUTOR
// ============================================================================

Habilidade::Habilidade(std::string nome, TipoHabilidade tipo, int custoE, int custoM, int valorBase, std::string atributoAfetado, int duracao)
    : _nome(nome), _tipo(tipo), _custoEnergia(custoE), _custoMP(custoM), _valorBase(valorBase), _duracao(duracao), _atributoAfetado(atributoAfetado) {
          // ============================================================================
    // TRAVAS DE SEGURANÇA 
    // ============================================================================
    if (nome.empty()) {
        throw std::invalid_argument("Erro: O nome da habilidade nao pode ser vazio.");
    }
    if (custoE < 0 || custoM < 0) {
        throw std::invalid_argument("Erro: Os custos de recursos (Energia/MP) nao podem ser negativos.");
    }
    if (valorBase < 0) {
        throw std::invalid_argument("Erro: O valor base de dano ou cura nao pode ser negativo.");
    }
    if ((tipo == TipoHabilidade::DOT || 
         tipo == TipoHabilidade::HOT || 
         tipo == TipoHabilidade::SUPORTE || 
         tipo == TipoHabilidade::DEBUFF) && duracao <= 0) {
        throw std::invalid_argument("Erro: Habilidades de efeito continuo (DoT, HoT, Buff, Debuff) exigem duracao maior que zero.");
    }

}

// ============================================================================
// GETTERS E MÉTODOS DE CHECAGEM
// ============================================================================

std::string Habilidade::getNome() const { 
    return _nome; 
}

TipoHabilidade Habilidade::getTipo() const { 
    return _tipo; 
}

int Habilidade::getCustoEnergia() const { 
    return _custoEnergia; 
}

int Habilidade::getCustoMP() const { 
    return _custoMP; 
}

int Habilidade::getValorBase() const { 
    return _valorBase; 
}

std::string Habilidade::getAtributoAlvo() const { 
    return _atributoAfetado; }

int Habilidade::getDuracaoEfeito() const { 
    return _duracao; 
}

bool Habilidade::ehEfeitoContinuo() const { 
    return _duracao > 0; 
}