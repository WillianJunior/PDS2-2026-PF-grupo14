#ifndef HABILIDADE_HPP
#define HABILIDADE_HPP

#include <string>
#include <stdexcept>

/**
 * @file Habilidade.hpp
 * @brief Definição da classe Habilidade, seu enumerador de tipos e exceções próprias.
 */

// ============================================================================
// EXCEÇÕES CUSTOMIZADAS DO DOMÍNIO DE HABILIDADE
// ============================================================================

/**
 * @class HabilidadeInvalidaException
 * @brief Exceção lançada quando os parâmetros de construção de uma Habilidade são inválidos.
 *
 * Herda de std::invalid_argument para ser compatível com o tratamento genérico
 * de exceções de argumento inválido da STL.
 */
class HabilidadeInvalidaException : public std::invalid_argument {
public:
    explicit HabilidadeInvalidaException(const std::string& mensagem)
        : std::invalid_argument("[Habilidade] " + mensagem) {}
};

// ============================================================================
// ENUMERADORES
// ============================================================================

/**
 * @enum TipoHabilidade
 * @brief Define a categoria mecânica e o comportamento do efeito da habilidade.
 */
enum class TipoHabilidade {
    FISICO,    ///< Ataques baseados em força física (gasta Energia).
    ESPECIAL,  ///< Magias ou técnicas focadas em dano bruto (gasta MP).
    ULTRA,     ///< Habilidades de alto impacto (pode gastar ambos os recursos).
    CURA,      ///< Restauração instantânea de pontos de vida (HP).
    SUPORTE,   ///< Efeitos benéficos aplicados a aliados (Buffs).
    DEBUFF,    ///< Efeitos prejudiciais aplicados a inimigos (Enfraquecimentos).
    DOT,       ///< Damage over Time: Dano distribuído ao longo de vários turnos.
    HOT        ///< Heal over Time: Cura distribuída ao longo de vários turnos.
};

// ============================================================================
// CLASSE HABILIDADE
// ============================================================================

/**
 * @class Habilidade
 * @brief Objeto imutável que encapsula os metadados e parâmetros de uma ação especial.
 *
 * A classe Habilidade é um value object: uma vez construída com parâmetros válidos,
 * seus dados não mudam. Todo acesso externo é feito exclusivamente por getters,
 * respeitando o princípio de encapsulamento.
 *
 * @note Lança HabilidadeInvalidaException se os parâmetros violarem as pré-condições.
 */
class Habilidade {
public:
    /**
     * @brief Construtor completo e validado da classe Habilidade.
     *
     * Valida todos os parâmetros antes de inicializar os membros. Garante que nenhum
     * objeto Habilidade exista em estado inválido (invariante de classe).
     *
     * @param nome         Nome identificador (não pode ser vazio).
     * @param tipo         Categoria mecânica da ação (@see TipoHabilidade).
     * @param custoE       Custo de Energia (>= 0).
     * @param custoM       Custo de Mana/MP (>= 0).
     * @param valorBase    Intensidade numérica base (>= 0).
     * @param atributoAfetado Atributo alvo para Buffs/Debuffs ("forca" ou "defesa").
     * @param duracao      Turnos de duração do efeito (>= 0; obrigatório > 0 para DoT/HoT/Buff/Debuff).
     *
     * @throws HabilidadeInvalidaException se qualquer pré-condição for violada.
     */
    Habilidade(std::string nome,
               TipoHabilidade tipo,
               int custoE,
               int custoM,
               int valorBase,
               std::string atributoAfetado,
               int duracao = 0);

    // ============================================================================
    // GETTERS (única forma de acesso externo — membros são privados)
    // ============================================================================

    /** @brief Retorna o nome da habilidade. */
    std::string getNome() const;

    /** @brief Retorna o tipo/categoria da habilidade. */
    TipoHabilidade getTipo() const;

    /** @brief Retorna o custo de Energia da habilidade. */
    int getCustoEnergia() const;

    /** @brief Retorna o custo de Pontos de Magia (MP). */
    int getCustoMP() const;

    /** @brief Retorna o valor numérico base da habilidade. */
    int getValorBase() const;

    /** @brief Retorna o atributo afetado pelo buff ou debuff. */
    std::string getAtributoAlvo() const;

    /** @brief Retorna a duração do efeito em turnos (0 = instantâneo). */
    int getDuracao() const;

    /** @brief Alias para getDuracao() mantido por compatibilidade. */
    int getDuracaoEfeito() const;

    /**
     * @brief Verifica se a habilidade aplica um efeito de múltiplos turnos.
     * @return true se duracao > 0.
     */
    bool ehEfeitoContinuo() const;

private:
    // Membros privados — acesso somente via getters
    std::string    _nome;            ///< Nome identificador da habilidade.
    TipoHabilidade _tipo;            ///< Natureza mecânica da ação.
    int            _custoEnergia;    ///< Custo de uso focado na barra de Energia.
    int            _custoMP;         ///< Custo de uso focado na barra de Mana.
    int            _valorBase;       ///< Potência bruta (Dano, Cura ou valor de Status).
    int            _duracao;         ///< Tempo de vida do efeito em turnos (0 = imediato).
    std::string    _atributoAfetado; ///< Nome do atributo afetado (para Buffs/Debuffs).
};

#endif // HABILIDADE_HPP