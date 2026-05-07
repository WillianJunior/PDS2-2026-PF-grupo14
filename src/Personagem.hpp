#ifndef PERSONAGEM_HPP
#define PERSONAGEM_HPP

#include <string>

/**
 * @file Personagem.hpp
 * @brief Classe base abstrata para todos as entidades do jogo.
 * @author PDS2 - Grupo 14
 * @date 2026-05-07
 *
 * Define os atributos e comportamentos comuns a Aventureiro e Inimigo,
 * centralizando a lógica de vida, defesa e recebimento de dano.
 */

/**
 * @class Personagem
 * @brief Classe base que representa um personagem genérico no sistema RPG.
 *
 * Contém os atributos fundamentais compartilhados por todos os personagens:
 * nome, HP, defesa e nível. As subclasses herdam essa estrutura e adicionam
 * comportamentos específicos.
 */
class Personagem {
public:
    /**
     * @brief Construtor da classe Personagem.
     * @param nome     Nome do personagem.
     * @param hp       Pontos de vida iniciais.
     * @param defesa   Valor base de defesa.
     * @param nivel    Nível inicial do personagem.
     */
    Personagem(const std::string& nome, int hp, int defesa, int nivel);

    /**
     * @brief Destrutor virtual para permitir herança segura.
     */
    virtual ~Personagem() = default;

    /**
     * @brief Aplica dano ao personagem, descontando a defesa.
     * @param dano  Valor bruto do dano recebido.
     * @note Dano final = max(0, dano - defesa). HP não fica negativo.
     */
    virtual void receberDano(int dano);

    /**
     * @brief Verifica se o personagem está vivo.
     * @return true se HP > 0, false caso contrário.
     */
    bool estaVivo() const;

    /**
     * @brief Retorna o nome do personagem.
     * @return Nome como string.
     */
    std::string getNome() const;

    /**
     * @brief Retorna o HP atual do personagem.
     * @return Valor inteiro do HP atual.
     */
    int getHP() const;

    /**
     * @brief Retorna o HP máximo do personagem.
     * @return Valor inteiro do HP máximo.
     */
    int getHPMax() const;

    /**
     * @brief Retorna o valor de defesa atual.
     * @return Valor inteiro da defesa.
     */
    int getDefesa() const;

    /**
     * @brief Retorna o nível atual do personagem.
     * @return Valor inteiro do nível.
     */
    int getNivel() const;

protected:
    std::string nome;   ///< Nome do personagem.
    int hp;             ///< HP atual.
    int hpMax;          ///< HP máximo.
    int defesa;         ///< Valor base de defesa.
    int nivel;          ///< Nível do personagem.
};

#endif // PERSONAGEM_HPP