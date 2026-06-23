#ifndef INIMIGO_COMUM_HPP
#define INIMIGO_COMUM_HPP

#include "Inimigo.hpp"
#include <string>
#include <stdexcept>

/**
 * @class DesafianteDoBar
 * @brief Cidadão comum que frequenta os fundos do bar em busca de uma válvula de escape.
 *
 * Inimigo introdutório do Clube de Luta. Usa apenas dano físico bruto (socos e chutes),
 * tem baixa defesa física e nenhuma resistência mágica. Serve para o jogador aprender
 * a gerenciar HP e testar ataques básicos.
 *
 * @invariant _nivel >= 1
 * @invariant _hp >= 0 && _hp <= _hpMax
 */
class DesafianteDoBar : public Inimigo {
public:
    /**
     * @brief Constrói um Desafiante do Bar para um determinado nível.
     * @param nome Nome do inimigo (usa "Desafiante do Bar" se vazio).
     * @param nivel Nível de escalonamento dos atributos (deve ser >= 1).
     * @throws std::invalid_argument Se nivel < 1.
     */
    explicit DesafianteDoBar(std::string nome, int nivel);

    /**
     * @brief Executa o turno do Desafiante: sempre ataque físico direto.
     * @param alvo Personagem que receberá o dano.
     * @pre alvo referencia um Personagem válido.
     */
    void executarTurno(Personagem& alvo) override;
    std::string getDeclaracaoStatus() const override;
};

/**
 * @class TrabalhadorNoturno
 * @brief Oponente rápido e frágil, movido a pura exaustão.
 *
 * Possui pouquíssimo HP, mas tem alta chance de acerto crítico (dano dobrado).
 * Exige que o jogador o elimine rapidamente antes de levar rajadas de críticos.
 *
 * @invariant _nivel >= 1
 */
class TrabalhadorNoturno : public Inimigo {
public:
    /**
     * @brief Constrói um Trabalhador Noturno para um determinado nível.
     * @param nome Nome do inimigo (usa "Trabalhador Noturno Estressado" se vazio).
     * @param nivel Nível de escalonamento dos atributos (deve ser >= 1).
     * @throws std::invalid_argument Se nivel < 1.
     */
    explicit TrabalhadorNoturno(std::string nome, int nivel);

    /**
     * @brief Executa o turno: 40% de chance de crítico (dano x2), senão ataque normal.
     * @param alvo Personagem que receberá o dano.
     * @pre alvo referencia um Personagem válido.
     */
    void executarTurno(Personagem& alvo) override;
    std::string getDeclaracaoStatus() const override;
};

/**
 * @class SegurancaDeBalada
 * @brief Brutamontes que tenta bloquear o acesso a áreas restritas.
 *
 * Possui mais HP que os outros comuns e foca em resistência física.
 * A cada 3 turnos assume postura defensiva (escudo), absorvendo metade do dano.
 * Serve como teste de resistência física do jogador.
 *
 * @invariant _nivel >= 1
 * @invariant _contadorTurnos >= 0
 */
class SegurancaDeBalada : public Inimigo {
public:
    /**
     * @brief Constrói um Segurança de Balada para um determinado nível.
     * @param nome Nome do inimigo (usa "Segurança de Balada" se vazio).
     * @param nivel Nível de escalonamento dos atributos (deve ser >= 1).
     * @throws std::invalid_argument Se nivel < 1.
     */
    explicit SegurancaDeBalada(std::string nome, int nivel);

    /**
     * @brief Executa o turno: ataque físico padrão; a cada 3 turnos ativa escudo defensivo.
     * @param alvo Personagem que receberá o dano.
     * @pre alvo referencia um Personagem válido.
     */
    void executarTurno(Personagem& alvo) override;
    std::string getDeclaracaoStatus() const override;
};

#endif // INIMIGO_COMUM_HPP