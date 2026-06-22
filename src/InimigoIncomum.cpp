#include "InimigoIncomum.hpp"
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
// RECRUTA DOUTRINADO
// ============================================================================

RecrutaDoutrinado::RecrutaDoutrinado(std::string nome, int nivel)
    : Inimigo(
        nome.empty() ? "Recruta Doutrinado" : nome,
        /*hp*/     300,
        /*defesa*/ 30,   // alta defesa física — força uso de ESPECIAL
        /*forca*/  35,
        nivel,
        /*xp*/     400)
{
    validarNivel(nivel, "RecrutaDoutrinado");

    // Ataque físico padrão
    adicionarHabilidade(Habilidade(
        "Soco Treinado",
        TipoHabilidade::FISICO,
        0, 0, _forcaBase, ""));

    // DOT: sangramento físico por 3 turnos
    adicionarHabilidade(Habilidade(
        "Sangramento",
        TipoHabilidade::DOT,
        0, 0, 4 * nivel, "", 3));

    // SUPORTE: buff de força em si mesmo por 2 turnos (doutrinação amplifica a raiva)
    adicionarHabilidade(Habilidade(
        "Grito de Doutrina",
        TipoHabilidade::SUPORTE,
        0, 0, 5 * nivel, "forca", 2));

    // DEBUFF: reduz defesa do alvo por 2 turnos (quebra a guarda para o próximo golpe)
    adicionarHabilidade(Habilidade(
        "Intimidação",
        TipoHabilidade::DEBUFF,
        0, 0, 6 * nivel, "defesa", 2));
}

void RecrutaDoutrinado::executarTurno(Personagem& alvo) {
    if (!alvo.estaVivo()) return;

    processarEfeitosContinuos();
    if (!estaVivo()) return;

    _contadorTurnos++;

    // A cada 5 turnos: Intimidação — debuff de defesa no alvo (maior prioridade)
    if (_contadorTurnos % 5 == 0) {
        InterfaceJogo::exibirTexto(
            _nome + " invade seu espaço e grita na sua cara — Intimidação! "
            "Sua defesa cai por 2 turnos!");
        alvo.aplicarDebuff("Intimidação", "defesa", 6 * _nivel, 2);
        return;
    }

    // A cada 4 turnos: Grito de Doutrina — buff de força em si mesmo
    if (_contadorTurnos % 4 == 0) {
        InterfaceJogo::exibirTexto(
            _nome + " recita as regras do clube em voz alta — "
            "Grito de Doutrina! Sua força aumenta por 2 turnos!");
        aplicarBuff("Grito de Doutrina", "forca", 5 * _nivel, 2);
        // Ainda ataca no mesmo turno com a força potencializada
        InterfaceJogo::exibirTexto(
            _nome + " avança com vigor redobrado!");
        alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
        return;
    }

    // A cada 2 turnos: Sangramento — DoT físico por 3 turnos
    if (_contadorTurnos % 2 == 0) {
        InterfaceJogo::exibirTexto(
            _nome + " rasga um corte profundo — Sangramento aplicado!");
        alvo.aplicarDoT("Sangramento", 4 * _nivel, 3);
        return;
    }

    // Turno padrão: Soco Treinado
    InterfaceJogo::exibirTexto(
        _nome + " avança sem recuar, ignorando a própria dor!");
    alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
}

// ============================================================================
// GUARDA PATRIMONIAL
// ============================================================================

GuardaPatrimonial::GuardaPatrimonial(std::string nome, int nivel)
    : Inimigo(
        nome.empty() ? "Guarda Patrimonial" : nome,
        /*hp*/     300,
        /*defesa*/ 40,   // alta defesa física e mágica (equipamento completo)
        /*forca*/  25,
        nivel,
        /*xp*/     400)
{
    validarNivel(nivel, "GuardaPatrimonial");

    // Ataque físico com cassetete
    adicionarHabilidade(Habilidade(
        "Cassetete",
        TipoHabilidade::FISICO,
        0, 0, _forcaBase, ""));

    // DEBUFF: atordoamento — força cai drasticamente por 1 turno
    adicionarHabilidade(Habilidade(
        "Atordoamento",
        TipoHabilidade::DEBUFF,
        0, 0, 12 * nivel, "forca", 1));

    // DEBUFF: gás lacrimogêneo — reduz defesa do alvo por 2 turnos
    adicionarHabilidade(Habilidade(
        "Gás Lacrimogêneo",
        TipoHabilidade::DEBUFF,
        0, 0, 8 * nivel, "defesa", 2));

    // HOT: bandagem de campo — cura contínua em si mesmo por 2 turnos
    adicionarHabilidade(Habilidade(
        "Bandagem de Campo",
        TipoHabilidade::HOT,
        0, 0, 6 * nivel, "", 2));
}

void GuardaPatrimonial::executarTurno(Personagem& alvo) {
    if (!alvo.estaVivo()) return;

    processarEfeitosContinuos();
    if (!estaVivo()) return;

    _contadorTurnos++;

    // A cada 5 turnos: Bandagem de Campo — HOT em si mesmo (maior prioridade)
    if (_contadorTurnos % 5 == 0) {
        InterfaceJogo::exibirTexto(
            _nome + " abre o kit de primeiros socorros rapidamente — "
            "Bandagem de Campo! Recuperando HP por 2 turnos.");
        aplicarHoT("Bandagem de Campo", 6 * _nivel, 2);
        // Ainda ataca no mesmo turno
        InterfaceJogo::exibirTexto(
            _nome + " avança sem hesitar!");
        alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
        return;
    }

    // A cada 4 turnos: Gás Lacrimogêneo — debuff de defesa no alvo
    if (_contadorTurnos % 4 == 0) {
        InterfaceJogo::exibirTexto(
            _nome + " dispara o spray de gás lacrimogêneo — "
            "Sua defesa cai por 2 turnos!");
        alvo.aplicarDebuff("Gás Lacrimogêneo", "defesa", 8 * _nivel, 2);
        return;
    }

    // A cada 3 turnos: Atordoamento — debuff de força severo por 1 turno
    if (_contadorTurnos % 3 == 0) {
        InterfaceJogo::exibirTexto(
            _nome + " acerta um golpe preciso no crânio — "
            "Atordoamento! Sua força cai drasticamente por 1 turno!");
        alvo.aplicarDebuff("Atordoamento", "forca", 12 * _nivel, 1);
        return;
    }

    // Turno padrão: Cassetete
    InterfaceJogo::exibirTexto(
        _nome + " avança com o cassetete!");
    alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
}

// ============================================================================
// SABOTADOR RIVAL — o mais perigoso dos incomuns
// ============================================================================

SabotadorRival::SabotadorRival(std::string nome, int nivel)
    : Inimigo(
        nome.empty() ? "Sabotador Rival" : nome,
        /*hp*/     25 * nivel,   // frágil — eliminar rapidamente
        /*defesa*/  3 * nivel,
        /*forca*/  15 * nivel,   // dano altíssimo
        nivel,
        /*xp*/     60 * nivel)
{
    validarNivel(nivel, "SabotadorRival");

    // ESPECIAL: explosivo direto — dano mágico padrão
    adicionarHabilidade(Habilidade(
        "Coquetel Molotov",
        TipoHabilidade::ESPECIAL,
        0, 0, _forcaBase, ""));

    // DOT: queimadura química por 3 turnos
    adicionarHabilidade(Habilidade(
        "Queimadura Química",
        TipoHabilidade::DOT,
        0, 0, 5 * nivel, "", 3));

    // DEBUFF: gás paralisante — reduz força do alvo por 2 turnos
    adicionarHabilidade(Habilidade(
        "Gás Paralisante",
        TipoHabilidade::DEBUFF,
        0, 0, 10 * nivel, "forca", 2));

    // ULTRA: explosão total — perfura escudo completamente, dano máximo
    adicionarHabilidade(Habilidade(
        "Explosão Total",
        TipoHabilidade::ULTRA,
        0, 0, _forcaBase * 2, ""));
}

void SabotadorRival::executarTurno(Personagem& alvo) {
    if (!alvo.estaVivo()) return;

    processarEfeitosContinuos();
    if (!estaVivo()) return;

    _contadorTurnos++;

    // A cada 5 turnos: EXPLOSÃO TOTAL (ULTRA) — substitui o Molotov, perfura escudo
    if (_contadorTurnos % 5 == 0) {
        InterfaceJogo::exibirTexto(
            "\n[PERIGO] " + _nome + " detona tudo que tem — EXPLOSÃO TOTAL! "
            "Nenhum escudo vai te proteger disso!");
        alvo.receberDano(_forcaBase * 2, TipoHabilidade::ULTRA);

        // Queimadura se aplica normalmente após a explosão
        InterfaceJogo::exibirTexto(
            _nome + " deixa o ambiente em chamas — Queimadura Química aplicada!");
        alvo.aplicarDoT("Queimadura Química", 5 * _nivel, 3);
        return;
    }

    // A cada 3 turnos: Gás Paralisante adicional — debuff de força por 2 turnos
    if (_contadorTurnos % 3 == 0) {
        InterfaceJogo::exibirTexto(
            _nome + " arremessa uma granada de gás — "
            "Gás Paralisante! Sua força cai por 2 turnos!");
        alvo.aplicarDebuff("Gás Paralisante", "forca", 10 * _nivel, 2);
    }

    // Turno padrão (sempre acontece se não for turno múltiplo de 5): Molotov + Queimadura
    InterfaceJogo::exibirTexto(
        _nome + " arremessa um explosivo improvisado!");
    alvo.receberDano(_forcaBase, TipoHabilidade::ESPECIAL);

    InterfaceJogo::exibirTexto(
        _nome + " derrama ácido — Queimadura Química aplicada!");
    alvo.aplicarDoT("Queimadura Química", 5 * _nivel, 3);
}
