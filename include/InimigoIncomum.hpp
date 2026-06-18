#ifndef INIMIGO_INCOMUM_HPP
#define INIMIGO_INCOMUM_HPP

#include "Inimigo.hpp"
#include <string>
#include <stdexcept>

/**
 * @class RecrutaDoutrinado
 * @brief Membro intermediário do Projeto Mayhem que já passou pela iniciação.
 *
 * Comportamento misto de agressor físico e lutador psicológico:
 * - Ataque físico padrão (FISICO).
 * - Sangramento a cada 2 turnos (DOT).
 * - Buff de força em si mesmo a cada 4 turnos (SUPORTE) — a doutrinação intensifica a raiva.
 * - Debuff de defesa no jogador a cada 5 turnos (DEBUFF) — desequilibra para o próximo golpe.
 *
 * @invariant _nivel >= 1
 * @invariant _contadorTurnos >= 0
 */
class RecrutaDoutrinado : public Inimigo {
public:
    /**
     * @brief Constrói um Recruta Doutrinado para um determinado nível.
     * @param nome Nome do inimigo (usa "Recruta Doutrinado" se vazio).
     * @param nivel Nível de escalonamento dos atributos (deve ser >= 1).
     * @throws std::invalid_argument Se nivel < 1.
     */
    explicit RecrutaDoutrinado(std::string nome, int nivel);

    /**
     * @brief Executa o turno com rotação de 4 habilidades distintas baseada no contador.
     *
     * Rotação:
     * - Turno padrão: Soco Treinado (FISICO).
     * - A cada 2 turnos: Sangramento (DOT, 3 turnos).
     * - A cada 4 turnos: Grito de Doutrina (SUPORTE — buff de força em si, 2 turnos).
     * - A cada 5 turnos: Intimidação (DEBUFF — reduz defesa do alvo, 2 turnos).
     *
     * @param alvo Personagem que receberá os efeitos do turno.
     * @pre alvo referencia um Personagem válido.
     */
    void executarTurno(Personagem& alvo) override;
};

/**
 * @class GuardaPatrimonial
 * @brief Vigilante que protege prédios corporativos durante a noite.
 *
 * Combina resistência com kit de campo:
 * - Ataque físico com cassetete (FISICO).
 * - Atordoamento a cada 3 turnos (DEBUFF severo de força por 1 turno).
 * - Gás Lacrimogêneo a cada 4 turnos (DEBUFF de defesa no alvo por 2 turnos).
 * - Bandagem de Campo a cada 5 turnos (HOT — cura contínua em si mesmo por 2 turnos).
 *
 * @invariant _nivel >= 1
 * @invariant _contadorTurnos >= 0
 */
class GuardaPatrimonial : public Inimigo {
public:
    /**
     * @brief Constrói um Guarda Patrimonial para um determinado nível.
     * @param nome Nome do inimigo (usa "Guarda Patrimonial" se vazio).
     * @param nivel Nível de escalonamento dos atributos (deve ser >= 1).
     * @throws std::invalid_argument Se nivel < 1.
     */
    explicit GuardaPatrimonial(std::string nome, int nivel);

    /**
     * @brief Executa o turno com rotação de 4 habilidades distintas baseada no contador.
     *
     * Rotação:
     * - Turno padrão: Cassetete (FISICO).
     * - A cada 3 turnos: Atordoamento (DEBUFF de força severo, 1 turno).
     * - A cada 4 turnos: Gás Lacrimogêneo (DEBUFF de defesa, 2 turnos).
     * - A cada 5 turnos: Bandagem de Campo (HOT em si mesmo, 2 turnos).
     *
     * @param alvo Personagem que receberá os efeitos do turno.
     * @pre alvo referencia um Personagem válido.
     */
    void executarTurno(Personagem& alvo) override;
};

/**
 * @class SabotadorRival
 * @brief Agente que usa táticas de guerrilha urbana com explosivos e químicos.
 *
 * O mais perigoso dos incomuns — baixo HP mas arsenal devastador:
 * - Coquetel Molotov todo turno (ESPECIAL).
 * - Queimadura Química todo turno (DOT, 3 turnos).
 * - Gás Paralisante a cada 3 turnos (DEBUFF de força no alvo, 2 turnos).
 * - Explosão Total a cada 5 turnos (ULTRA — perfura o escudo do jogador completamente).
 *
 * @invariant _nivel >= 1
 * @invariant _contadorTurnos >= 0
 */
class SabotadorRival : public Inimigo {
public:
    /**
     * @brief Constrói um Sabotador Rival para um determinado nível.
     * @param nome Nome do inimigo (usa "Sabotador Rival" se vazio).
     * @param nivel Nível de escalonamento dos atributos (deve ser >= 1).
     * @throws std::invalid_argument Se nivel < 1.
     */
    explicit SabotadorRival(std::string nome, int nivel);

    /**
     * @brief Executa o turno com arsenal químico-explosivo completo.
     *
     * Rotação:
     * - Todo turno: Coquetel Molotov (ESPECIAL) + Queimadura Química (DOT, 3 turnos).
     * - A cada 3 turnos: Gás Paralisante adicional (DEBUFF de força, 2 turnos).
     * - A cada 5 turnos: Explosão Total (ULTRA) substitui o Molotov — perfura o escudo.
     *
     * @param alvo Personagem que receberá os efeitos do turno.
     * @pre alvo referencia um Personagem válido.
     */
    void executarTurno(Personagem& alvo) override;
};

#endif // INIMIGO_INCOMUM_HPP
