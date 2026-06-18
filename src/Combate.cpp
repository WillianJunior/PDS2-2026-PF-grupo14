#include "Combate.hpp"
#include "InterfaceJogo.hpp"
#include <string>
#include <stdexcept>

// ============================================================================
// CONSTRUTOR — VALIDAÇÃO DEFENSIVA DE ESTADO INICIAL
// ============================================================================

Combate::Combate(Aventureiro& jogador, Inimigo& inimigo)
    : _jogador(jogador), _inimigo(inimigo), _turnoAtual(0)
{
    // Programação defensiva: impede construção de Combate com estado inválido.
    // Um combate com um combatente já derrotado nunca deveria acontecer.
    if (!_jogador.estaVivo()) {
        throw CombateInvalidoException(
            "Impossível iniciar combate: o aventureiro '" +
            _jogador.getNome() + "' já está derrotado.");
    }

    if (!_inimigo.estaVivo()) {
        throw CombateInvalidoException(
            "Impossível iniciar combate: o inimigo '" +
            _inimigo.getNome() + "' já está derrotado.");
    }
}

// ============================================================================
// MÉTODO PRINCIPAL
// ============================================================================

bool Combate::iniciar() {
    renderizarInicioCombate();

    // Loop principal: continua enquanto ambos estiverem vivos
    while (verificarContinuacao()) {
        _turnoAtual++;
        renderizarInterfaceTurno();

        // --- Turno do Jogador ---
        turnoDoJogador();

        // Verifica se o inimigo foi derrotado durante o turno do jogador
        if (!verificarContinuacao()) break;

        // --- Turno do Inimigo ---
        turnoDoInimigo();
    }

    bool jogadorVenceu = _jogador.estaVivo();
    renderizarFimCombate(jogadorVenceu);
    return jogadorVenceu;
}

// ============================================================================
// TURNOS
// ============================================================================

void Combate::turnoDoJogador() {
    // Proteção defensiva: não processa turno de combatente já morto
    // (pode ter sido morto por DoT antes da vez dele)
    if (!_jogador.estaVivo()) return;

    try {
        _jogador.executarTurno(_inimigo);
    }
    catch (const std::out_of_range& e) {
        // Captura acesso inválido a índices de vetores (ex: menu de habilidades)
        InterfaceJogo::exibirTexto(
            "[SISTEMA] Erro de acesso a dado invalido: " +
            std::string(e.what()) +
            "\nSua acao foi cancelada. O turno passou.");
    }
    catch (const std::invalid_argument& e) {
        // Captura entradas inválidas propagadas por métodos de combate
        InterfaceJogo::exibirTexto(
            "[SISTEMA] Acao invalida: " +
            std::string(e.what()) +
            "\nSua acao foi cancelada. O turno passou.");
    }
    catch (const std::exception& e) {
        // Rede de segurança para qualquer outra exceção padrão inesperada
        InterfaceJogo::exibirTexto(
            "[SISTEMA - ERRO CRITICO] Falha inesperada no turno do jogador: " +
            std::string(e.what()) +
            "\nO turno foi encerrado forcadamente.");
    }
}

void Combate::turnoDoInimigo() {
    // Proteção defensiva: não processa turno de inimigo já morto
    if (!_inimigo.estaVivo()) return;

    try {
        _inimigo.executarTurno(_jogador);
    }
    catch (const std::exception& e) {
        // Erros de IA não devem travar o combate — são logados e ignorados
        InterfaceJogo::exibirTexto(
            "[SISTEMA] Falha na IA do inimigo '" + _inimigo.getNome() + "': " +
            std::string(e.what()) +
            "\nO turno do inimigo foi pulado.");
    }
}

// ============================================================================
// VERIFICAÇÃO DE ESTADO
// ============================================================================

bool Combate::verificarContinuacao() const {
    return _jogador.estaVivo() && _inimigo.estaVivo();
}

// ============================================================================
// RENDERIZAÇÃO
// ============================================================================

void Combate::renderizarInterfaceTurno() const {
    InterfaceJogo::exibirTexto("\n===========================================");
    InterfaceJogo::exibirTexto("                 TURNO " + std::to_string(_turnoAtual));
    InterfaceJogo::exibirTexto("===========================================");
    InterfaceJogo::exibirTexto(
        "-> " + _inimigo.getNome() +
        " [HP: " + std::to_string(_inimigo.getHP()) +
        "/" + std::to_string(_inimigo.getHPMax()) + "]");
}

void Combate::renderizarInicioCombate() const {
    InterfaceJogo::exibirTexto("\n===========================================");
    InterfaceJogo::exibirTexto("             COMBATE INICIADO!");
    InterfaceJogo::exibirTexto("===========================================");
    InterfaceJogo::exibirTexto(_jogador.getNome() + " VS " + _inimigo.getNome() + "\n");
}

void Combate::renderizarFimCombate(bool jogadorVenceu) {
    InterfaceJogo::exibirTexto("\n===========================================");
    InterfaceJogo::exibirTexto("              FIM DE COMBATE");
    InterfaceJogo::exibirTexto("===========================================");

    if (jogadorVenceu) {
        InterfaceJogo::exibirTexto("Vitoria! Voce derrotou " + _inimigo.getNome() + ".");

        int xpGanha = _inimigo.getXPRecompensa();
        InterfaceJogo::exibirTexto("Recompensa: +" + std::to_string(xpGanha) + " XP.");

        // Proteção defensiva: ganharExperiencia também valida internamente (qtd <= 0),
        if (xpGanha > 0) {
            try {
                _jogador.ganharExperiencia(xpGanha);
            }
            catch (const std::exception& e) {
                InterfaceJogo::exibirTexto(
                    "[SISTEMA] Falha ao conceder XP: " + std::string(e.what()));
            }
        }
    } else {
        InterfaceJogo::exibirTexto(
            "Derrota... A jornada de " + _jogador.getNome() + " termina aqui.");
    }
}