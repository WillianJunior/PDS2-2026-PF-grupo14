#include "InimigoBoss.hpp"
#include "InterfaceJogo.hpp"
#include <stdexcept>
#include <string>

// ============================================================================
// Utilitário interno: valida nivel e lança std::invalid_argument se inválido
// ============================================================================
namespace {
    /**
     * @brief Valida que o nível é pelo menos 1.
     * @param nivel Valor a validar.
     * @param contexto Nome da classe para compor a mensagem de erro.
     * @throws std::invalid_argument Se nivel < 1.
     */
    void validarNivel(int nivel, const std::string& contexto) {
        if (nivel < 1) {
            throw std::invalid_argument(
                contexto + ": nivel deve ser >= 1, recebido: "
                + std::to_string(nivel));
        }
    }
} // namespace anônimo

// ============================================================================
// CONSTRUTOR
// ============================================================================

TylerDurden::TylerDurden(std::string nome, int nivel)
    : Inimigo(
        nome.empty() ? "Tyler Durden" : nome,
        /*hp*/     400,
        /*defesa*/  30,
        /*forca*/   40,
        nivel,
        /*xp*/     500 * nivel),
      _curaEstoicaUsada(false),
      _fase2Anunciada(false),
      _ultraUsado(false)
{
    validarNivel(nivel, "TylerDurden");

    // ── Fase 1: dano físico massivo ──────────────────────────────────────────
    adicionarHabilidade(Habilidade(
        "Soco do Caos",
        TipoHabilidade::FISICO,
        0, 0, _forcaBase, ""));

    // ── DOT: sangramento físico por 3 turnos (ambas as fases) ────────────────
    adicionarHabilidade(Habilidade(
        "Sangramento (Tyler)",
        TipoHabilidade::DOT,
        0, 0, 5 * nivel, "", 3));

    // ── SUPORTE: buff de força em si mesmo por 2 turnos (Fase 1) ─────────────
    adicionarHabilidade(Habilidade(
        "Foco Destrutivo",
        TipoHabilidade::SUPORTE,
        0, 0, 4 * nivel, "forca", 2));

    // ── Fase 2: dano mágico que ignora defesa física ──────────────────────────
    adicionarHabilidade(Habilidade(
        "Impacto Psicológico",
        TipoHabilidade::ESPECIAL,
        0, 0, _forcaBase+(2* nivel) , ""));

    // ── DEBUFF: reduz defesa do jogador por 2 turnos (Fase 2) ────────────────
    adicionarHabilidade(Habilidade(
        "Humilhação",
        TipoHabilidade::DEBUFF,
        0, 0, 3 * nivel, "defesa", 2));

    // ── HOT: cura contínua em si mesmo por 2 turnos (Fase 2) ─────────────────
    adicionarHabilidade(Habilidade(
        "Resiliência Estóica",
        TipoHabilidade::HOT,
        0, 0, 6 * nivel, "", 2));

    // ── ULTRA: golpe final perfura escudo — dispara 1 vez quando alvo < 30% HP
    adicionarHabilidade(Habilidade(
        "Desconstrução Total",
        TipoHabilidade::ULTRA,
        0, 0, _forcaBase * 2, ""));

    // ── Cura Estóica: recuperação pontual única quando Tyler < 20% HP ─────────
    adicionarHabilidade(Habilidade(
        "Cura Estóica",
        TipoHabilidade::CURA,
        0, 0, 30 * nivel, ""));
}

// ============================================================================
// EXECUÇÃO DE TURNO — SISTEMA DE FASES + ARSENAL COMPLETO
// ============================================================================

void TylerDurden::executarTurno(Personagem& alvo) {
    if (!alvo.estaVivo()) return;

    processarEfeitosContinuos();
    if (!estaVivo()) return;

    // Pré-condição defensiva: HPMax deve ser positivo para evitar divisão por zero
    if (_hpMax <= 0) {
        throw std::logic_error(
            "TylerDurden::executarTurno — _hpMax invalido: "
            + std::to_string(_hpMax));
    }
    if (alvo.getHPMax() <= 0) {
        throw std::logic_error(
            "TylerDurden::executarTurno — HPMax do alvo invalido: "
            + std::to_string(alvo.getHPMax()));
    }

    _contadorTurnos++;

    // ── GATILHO GLOBAL: Cura Estóica (Tyler < 20% HP, apenas 1 vez) ──────────
    // Verificado antes de qualquer outra ação — Tyler nunca morre sem reagir
    const float hpTylerRatio = static_cast<float>(_hp) / static_cast<float>(_hpMax);

    if (hpTylerRatio < 0.20f && !_curaEstoicaUsada) {
        _curaEstoicaUsada = true;
        const int valorCura = 40 * _nivel;
        InterfaceJogo::exibirTexto(
            "\n[TYLER] \"A dor é purificadora.\"\n"
            "Tyler fecha os olhos por um instante e recupera o controle — "
            "Cura Estóica! (+" + std::to_string(valorCura) + " HP)");
        receberCura(valorCura);
    }

    // Recalcula após possível cura
    const float hpTyler = static_cast<float>(_hp) / static_cast<float>(_hpMax);

    // ── GATILHO GLOBAL: Desconstrução Total (alvo < 30% HP, apenas 1 vez) ────
    const float hpAlvo = static_cast<float>(alvo.getHP()) / static_cast<float>(alvo.getHPMax());

    if (hpAlvo < 0.30f && !_ultraUsado) {
        _ultraUsado = true;
        InterfaceJogo::exibirTexto(
            "\n[TYLER] \"Você estava morto muito antes de entrar nessa sala.\"\n"
            + _nome + " desfere a DESCONSTRUÇÃO TOTAL — nenhum escudo vai parar isso!");
        alvo.receberDano(_forcaBase * 3, TipoHabilidade::ULTRA);
        return;
    }

    // ════════════════════════════════════════════════════════════════════════
    // FASE 2: HP de Tyler entre 20% e 50%
    // ════════════════════════════════════════════════════════════════════════
    if (hpTyler <= 0.50f) {

        if (!_fase2Anunciada) {
            _fase2Anunciada = true;
            InterfaceJogo::exibirTexto(
                "\n[TYLER] \"Você me fez assim. Agora enfrente as consequências.\"\n"
                "Tyler muda de postura — os ataques agora penetram sua mente!");
        }

        // A cada 4 turnos: Resiliência Estóica — HOT em si mesmo por 2 turnos
        if (_contadorTurnos % 4 == 0) {
            InterfaceJogo::exibirTexto(
                _nome + " respira fundo e ativa sua resiliência interna — "
                "Resiliência Estóica! Recuperando HP por 2 turnos.");
            aplicarHoT("Resiliência Estóica", 12 * _nivel, 2);
        }

        // A cada 3 turnos: Humilhação — debuff de defesa no alvo por 2 turnos
        if (_contadorTurnos % 3 == 0) {
            InterfaceJogo::exibirTexto(
                _nome + " destrói sua autoestima com uma risada — "
                "Humilhação! Sua defesa cai por 2 turnos!");
            alvo.aplicarDebuff("Humilhação", "defesa", 10 * _nivel, 2);
        }

        // Ataque principal da fase 2: Impacto Psicológico (ESPECIAL — ignora defesa física)
        InterfaceJogo::exibirTexto(
            _nome + " invade sua psique com um Impacto Psicológico!");
        alvo.receberDano(_forcaBase + (5 * _nivel), TipoHabilidade::ESPECIAL);

        // Sangramento periódico na fase 2
        if (_contadorTurnos % 3 == 0) {
            InterfaceJogo::exibirTexto(
                _nome + " abre outro corte fundo — Sangramento aplicado!");
            alvo.aplicarDoT("Sangramento (Tyler)", 8 * _nivel, 3);
        }

        return;
    }

    // ════════════════════════════════════════════════════════════════════════
    // FASE 1: HP de Tyler > 50%
    // ════════════════════════════════════════════════════════════════════════

    // A cada 4 turnos: Foco Destrutivo — buff de força em si mesmo por 2 turnos
    if (_contadorTurnos % 4 == 0) {
        InterfaceJogo::exibirTexto(
            "\n[TYLER] \"Sem medo. Sem hesitação.\"\n"
            + _nome + " canaliza raiva pura — Foco Destrutivo! "
            "Sua força aumenta por 2 turnos!");
        aplicarBuff("Foco Destrutivo", "forca", 8 * _nivel, 2);
    }

    // A cada 3 turnos: Sangramento — DoT por 3 turnos
    if (_contadorTurnos % 3 == 0) {
        InterfaceJogo::exibirTexto(
            _nome + " te abre com precisão cirúrgica — Sangramento aplicado!");
        alvo.aplicarDoT("Sangramento (Tyler)", 8 * _nivel, 3);
    }

    // Ataque principal da fase 1: Soco do Caos (FISICO)
    InterfaceJogo::exibirTexto(
        _nome + " desfere um soco devastador!");
    alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
}
