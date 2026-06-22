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
// MÉTODO PRINCIPAL REFORMULADO
// ============================================================================

bool Combate::iniciar() {
    renderizarInicioCombate();
    InterfaceJogo::pausar(); // Pausa inicial para começar a luta

    // Loop principal
    while (verificarContinuacao()) {
        _turnoAtual++;
        
        // 1. Mostra o cabeçalho do turno
        renderizarInterfaceTurno();

        // 2. O jogador escolhe e EXECUTA o ataque completo (Texto + Dano)
        turnoDoJogador();
        
        // 3. AGORA SIM O JOGADOR CLICA (Ele lê quem ele atacou e quanto de dano deu)
        InterfaceJogo::pausar(); 

        // Verifica se o oponente morreu no seu ataque
        if (!verificarContinuacao()) break;

        // 4. O inimigo escolhe e EXECUTA o contra-ataque completo (Texto + Dano)
        turnoDoInimigo();
        
        // 5. SEGUNDO CLIQUE (O jogador lê o ataque do inimigo e o dano que sofreu juntos)
        InterfaceJogo::pausar(); 
    }

    bool jogadorVenceu = _jogador.estaVivo();
    
    renderizarFimCombate(jogadorVenceu);
    InterfaceJogo::pausar(); // Pausa final para fechar a tela de combate

    _jogador.limparEfeitosTemporarios();
    _inimigo.limparEfeitosTemporarios();
    return jogadorVenceu;
}

// ============================================================================
// TURNOS
// ============================================================================

void Combate::turnoDoJogador() {
    if (!_jogador.estaVivo()) return;

    try {
        _jogador.executarTurno(_inimigo);
    }
    catch (const std::out_of_range& e) {
        InterfaceJogo::exibirTexto(
            "[SISTEMA] Erro de acesso a dado invalido: " +
            std::string(e.what()) +
            "\nSua acao foi cancelada. O turno passou.");
    }
    catch (const std::invalid_argument& e) {
        InterfaceJogo::exibirTexto(
            "[SISTEMA] Acao invalida: " +
            std::string(e.what()) +
            "\nSua acao foi cancelada. O turno passou.");
    }
    catch (const std::exception& e) {
        InterfaceJogo::exibirTexto(
            "[SISTEMA - ERRO CRITICO] Falha unexpected no turno do jogador: " +
            std::string(e.what()) +
            "\nO turno foi encerrado forcadamente.");
    }
}

void Combate::turnoDoInimigo() {
    if (!_inimigo.estaVivo()) return;

    try {
        // Linha divisória sutil para indicar visualmente a resposta do oponente
        InterfaceJogo::exibirTexto("\n⚡ RESPOSTA DE " + _inimigo.getNome() + ":");
        _inimigo.executarTurno(_jogador);
    }
    catch (const std::exception& e) {
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
// RENDERIZAÇÃO E ESPAÇAMENTO VISUAL
// ============================================================================

void Combate::renderizarInterfaceTurno() const {
    InterfaceJogo::exibirTexto("\n=======================================================");
    InterfaceJogo::exibirTexto("                  ⚔️  TURNO " + std::to_string(_turnoAtual) + "  ⚔️");
    InterfaceJogo::exibirTexto("=======================================================");
    InterfaceJogo::exibirTexto("-> " + _inimigo.getNome() + " [HP: " + 
                               std::to_string(_inimigo.getHP()) + "/" + 
                               std::to_string(_inimigo.getHPMax()) + "]");
    InterfaceJogo::exibirTexto("-------------------------------------------------------");
}

void Combate::renderizarInicioCombate() const {
    InterfaceJogo::exibirTexto("\n=======================================================");
    InterfaceJogo::exibirTexto("                 💥 COMBATE INICIADO! 💥");
    InterfaceJogo::exibirTexto("=======================================================");
    InterfaceJogo::exibirTexto("       " + _jogador.getNome() + "   VS   " + _inimigo.getNome());
    InterfaceJogo::exibirTexto("=======================================================");
}

void Combate::renderizarFimCombate(bool jogadorVenceu) {
    InterfaceJogo::exibirTexto("\n=======================================================");
    InterfaceJogo::exibirTexto("                 💀 FIM DE COMBATE 💀");
    InterfaceJogo::exibirTexto("=======================================================");

    if (jogadorVenceu) {
        InterfaceJogo::exibirTexto("✨ Vitória! Você derrotou " + _inimigo.getNome() + ".");

        int xpGanha = _inimigo.getXPRecompensa();
        InterfaceJogo::exibirTexto("🎁 Recompensa: +" + std::to_string(xpGanha) + " XP.");

        if (xpGanha > 0) {
            try {
                _jogador.ganharExperiencia(xpGanha);
            }
            catch (const std::exception& e) {
                InterfaceJogo::exibirTexto("[SISTEMA] Falha ao conceder XP: " + std::string(e.what()));
            }
        }
    } else {
        InterfaceJogo::exibirTexto("❌ Derrota... A jornada de " + _jogador.getNome() + " termina aqui.");
    }
    InterfaceJogo::exibirTexto("=======================================================");
}