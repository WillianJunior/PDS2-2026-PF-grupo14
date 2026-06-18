#ifndef INIMIGO_BOSS_HPP
#define INIMIGO_BOSS_HPP

#include "Inimigo.hpp"
#include <string>
#include <stdexcept>

/**
 * @class TylerDurden
 * @brief O Reflexo Sombrio. Boss final do arco narrativo do Fight Club.
 *
 * Tyler combina todo o arsenal de mecânicas do jogo em quatro fases:
 *
 * - **Fase 1** (HP > 50%): Soco do Caos (FISICO) + Sangramento (DOT) a cada 3 turnos.
 *   A cada 4 turnos aplica Buff de força em si mesmo (SUPORTE — "Foco Destrutivo").
 *
 * - **Fase 2** (HP entre 20% e 50%): Impacto Psicológico (ESPECIAL) + Sangramento contínuo.
 *   A cada 3 turnos aplica Debuff de defesa no jogador (DEBUFF — "Humilhação").
 *   A cada 4 turnos ativa Resiliência Estóica (HOT em si mesmo por 2 turnos).
 *
 * - **Golpe Final** (jogador com HP < 30%): Desconstrução Total (ULTRA) uma única vez —
 *   perfura o escudo e causa dano massivo.
 *
 * - **Cura Estóica** (próprio HP < 20%, apenas 1 vez): Recupera HP fixo e continua atacando
 *   no mesmo turno.
 *
 * @invariant _nivel >= 1
 * @invariant _contadorTurnos >= 0
 * @invariant Flags booleanas (_curaEstoicaUsada, _ultraUsado) só transitam de false para true.
 */
class TylerDurden : public Inimigo {
public:
    /**
     * @brief Constrói Tyler Durden para um determinado nível.
     * @param nome Nome do boss (usa "Tyler Durden" se vazio).
     * @param nivel Nível de escalonamento dos atributos (deve ser >= 1).
     * @throws std::invalid_argument Se nivel < 1.
     */
    explicit TylerDurden(std::string nome = "Tyler Durden", int nivel = 10);

    /**
     * @brief Executa o turno de Tyler com lógica de múltiplas fases.
     *
     * Fase determinada pelo HP atual de Tyler em relação ao HP máximo.
     * Gatilhos adicionais verificam o HP do alvo e contadores de turno
     * para acionar habilidades especiais independentemente da fase.
     *
     * @param alvo O Aventureiro que enfrentará Tyler.
     * @pre alvo referencia um Personagem válido.
     * @pre alvo.getHPMax() > 0  (necessário para calcular proporção de HP).
     */
    void executarTurno(Personagem& alvo) override;

private:
    bool _curaEstoicaUsada;   ///< Garante que a Cura Estóica ocorra no máximo uma vez.
    bool _fase2Anunciada;     ///< Controla o anúncio narrativo da transição para a Fase 2.
    bool _ultraUsado;         ///< Garante que a Desconstrução Total ocorra no máximo uma vez.
};

#endif // INIMIGO_BOSS_HPP
