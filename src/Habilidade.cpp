#include "Habilidade.hpp"

// ============================================================================
// CONSTRUTOR
// ============================================================================

Habilidade::Habilidade(std::string nome,
                       TipoHabilidade tipo,
                       int custoE,
                       int custoM,
                       int valorBase,
                       std::string atributoAfetado,
                       int duracao)
{
    // -------------------------------------------------------------------------
    // PROGRAMAÇÃO DEFENSIVA: Todas as validações ANTES de atribuir qualquer
    // membro. Garante que o objeto nunca exista em estado inconsistente.
    // -------------------------------------------------------------------------

    if (nome.empty()) {
        throw HabilidadeInvalidaException(
            "O nome da habilidade nao pode ser vazio.");
    }

    if (custoE < 0) {
        throw HabilidadeInvalidaException(
            "O custo de Energia nao pode ser negativo. Recebido: " +
            std::to_string(custoE));
    }

    if (custoM < 0) {
        throw HabilidadeInvalidaException(
            "O custo de MP nao pode ser negativo. Recebido: " +
            std::to_string(custoM));
    }

    if (valorBase < 0) {
        throw HabilidadeInvalidaException(
            "O valor base nao pode ser negativo. Recebido: " +
            std::to_string(valorBase));
    }

    if (duracao < 0) {
        throw HabilidadeInvalidaException(
            "A duracao nao pode ser negativa. Recebido: " +
            std::to_string(duracao));
    }

    // Habilidades de efeito contínuo exigem duração explicitamente maior que zero.
    // Habilidades instantâneas (FISICO, ESPECIAL, ULTRA, CURA) aceitam duracao == 0.
    const bool ehEfeitoContinuoTipo =
        (tipo == TipoHabilidade::DOT    ||
         tipo == TipoHabilidade::HOT    ||
         tipo == TipoHabilidade::SUPORTE ||
         tipo == TipoHabilidade::DEBUFF);

    if (ehEfeitoContinuoTipo && duracao <= 0) {
        throw HabilidadeInvalidaException(
            "Habilidades do tipo DoT, HoT, Suporte e Debuff exigem duracao > 0. "
            "Habilidade: '" + nome + "'.");
    }

    // Buffs e Debuffs precisam de um atributo alvo definido
    if ((tipo == TipoHabilidade::SUPORTE || tipo == TipoHabilidade::DEBUFF) &&
         atributoAfetado.empty()) {
        throw HabilidadeInvalidaException(
            "Habilidades do tipo Suporte e Debuff exigem um atributo alvo "
            "(\"forca\" ou \"defesa\"). Habilidade: '" + nome + "'.");
    }

    // -------------------------------------------------------------------------
    // INICIALIZAÇÃO — somente após todas as validações passarem
    // -------------------------------------------------------------------------
    _nome            = std::move(nome);
    _tipo            = tipo;
    _custoEnergia    = custoE;
    _custoMP         = custoM;
    _valorBase       = valorBase;
    _atributoAfetado = std::move(atributoAfetado);
    _duracao         = duracao;
}

// ============================================================================
// GETTERS
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
    return _atributoAfetado;
}

int Habilidade::getDuracao() const {
    return _duracao;
}

int Habilidade::getDuracaoEfeito() const {
    return _duracao;
}

bool Habilidade::ehEfeitoContinuo() const {
    return _duracao > 0;
}