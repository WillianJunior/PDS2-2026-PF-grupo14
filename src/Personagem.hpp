#ifndef PERSONAGEM_HPP
#define PERSONAGEM_HPP

#include <string>

/**
 * @file Personagem.hpp
 * @brief Definição da classe base Personagem.
 */

/**
 * @class Personagem
 * @brief Classe abstrata que define os atributos e métodos básicos de qualquer entidade no jogo.
 */
class Personagem {
public:
    /**
     * @brief Construtor da classe Personagem.
     * @param nome Nome da entidade.
     * @param hp Pontos de vida iniciais.
     * @param defesa Pontos de defesa base.
     * @param nivel Nível inicial.
     */
    Personagem(std::string nome, int hp, int defesa, int nivel);

    /** @brief Destrutor virtual para garantir a limpeza correta das subclasses. */
    virtual ~Personagem() = default;

    /**
     * @brief Método virtual puro para execução do turno.
     * @param alvo Ponteiro ou referência para o oponente.
     */
    virtual void executarTurno(Personagem& alvo) = 0;

    /**
     * @brief Reduz o HP do personagem com base no dano recebido e defesa.
     * @param dano Valor bruto do dano.
     */
    virtual void receberDano(int dano);

    /**
     * @brief Incrementa o HP do personagem respeitando o limite máximo.
     * @param valor Quantidade de cura.
     */
    virtual void receberCura(int valor);

    /** @brief Retorna o valor de defesa atual do personagem. */
    virtual int getDefesa() const;

    /** @brief Retorna o HP atual. */
    int getHP() const;
    /** @brief Retorna o HP máximo. */
    int getHPMax() const;
    /** @brief Verifica se o HP é maior que zero. */
    bool estaVivo() const;
    /** @brief Retorna o nome do personagem. */
    std::string getNome() const;
    /** @brief Retorna o nível atual. */
    int getNivel() const;

protected:
    std::string _nome; ///< Nome do personagem.
    int _hp;           ///< Pontos de vida atuais.
    int _hpMax;        ///< Limite máximo de vida.
    int _defesaBase;   ///< Defesa natural sem bônus.
    int _nivel;        ///< Nível da entidade.
};

#endif