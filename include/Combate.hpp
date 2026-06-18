#ifndef COMBATE_HPP
#define COMBATE_HPP

#include "Aventureiro.hpp"
#include "Inimigo.hpp"
#include <stdexcept>

/**
 * @file Combate.hpp
 * @brief Gerenciador do ciclo de vida de um confronto no jogo.
 */

// ============================================================================
// EXCEÇÃO CUSTOMIZADA DO DOMÍNIO DE COMBATE
// ============================================================================

/**
 * @class CombateInvalidoException
 * @brief Exceção lançada quando o estado dos combatentes impede o início do combate.
 *
 * Exemplos de situações inválidas:
 * - Tentar iniciar um combate com um combatente já morto.
 * - Combatente nulo passado por referência inválida.
 */
class CombateInvalidoException : public std::logic_error {
public:
    explicit CombateInvalidoException(const std::string& mensagem)
        : std::logic_error("[Combate] " + mensagem) {}
};

// ============================================================================
// CLASSE COMBATE
// ============================================================================

/**
 * @class Combate
 * @brief Mediador do loop de batalha por turnos entre Aventureiro e Inimigo.
 *
 * Segue o padrão Mediator: desacopla as entidades combatentes coordenando
 * a ordem de ações, a renderização de estado e as condições de vitória/derrota.
 *
 * Todas as entradas do usuário são validadas defensivamente antes de serem
 * processadas. Exceções de domínio são capturadas e tratadas localmente
 * sempre que possível, expondo ao chamador apenas falhas não recuperáveis.
 */
class Combate {
public:
    /**
     * @brief Construtor defensivo da classe Combate.
     *
     * Valida o estado inicial de ambos os combatentes antes de aceitar
     * as referências. Um combate não pode ser criado com entidades mortas.
     *
     * @param jogador Referência para o Aventureiro controlado pelo usuário.
     * @param inimigo Referência para a subclasse de Inimigo adversária.
     *
     * @throws CombateInvalidoException se jogador ou inimigo não estiverem vivos.
     */
    Combate(Aventureiro& jogador, Inimigo& inimigo);

    /**
     * @brief Inicia e gerencia o loop principal da batalha.
     *
     * Executa turnos alternados até que um dos combatentes seja derrotado.
     * Processa recompensas (XP) em caso de vitória ou exibe game over em derrota.
     * Captura exceções internas para garantir que o loop nunca trave por
     * entradas inesperadas do usuário.
     *
     * @return true  se o aventureiro venceu.
     * @return false se o aventureiro foi derrotado.
     */
    bool iniciar();

private:
    Aventureiro& _jogador;  ///< Referência ao herói controlado pelo jogador.
    Inimigo&     _inimigo;  ///< Referência ao inimigo adversário.
    int          _turnoAtual; ///< Contador de turnos da batalha.

    /**
     * @brief Processa o turno do jogador com tratamento completo de exceções.
     *
     * Chama executarTurno do Aventureiro dentro de um bloco try-catch para
     * garantir que erros de entrada (std::invalid_argument, std::out_of_range)
     * nunca interrompam o fluxo de combate.
     */
    void turnoDoJogador();

    /**
     * @brief Processa o turno do inimigo com tratamento de exceções.
     *
     * Invoca o método polimórfico executarTurno do inimigo protegido por
     * try-catch. Erros inesperados de IA são logados mas não interrompem o combate.
     */
    void turnoDoInimigo();

    /**
     * @brief Verifica se o combate deve continuar.
     * @return true se ambos os combatentes ainda tiverem HP > 0.
     */
    bool verificarContinuacao() const;

    /** @brief Renderiza o cabeçalho do turno atual com os status dos combatentes. */
    void renderizarInterfaceTurno() const;

    /** @brief Exibe a introdução visual e narrativa do confronto. */
    void renderizarInicioCombate() const;

    /**
     * @brief Renderiza a tela de desfecho e processa recompensas ou game over.
     * @param jogadorVenceu true se o aventureiro venceu.
     */
    void renderizarFimCombate(bool jogadorVenceu);
};

#endif // COMBATE_HPP