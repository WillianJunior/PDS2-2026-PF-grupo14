#ifndef HABILIDADE_HPP
#define HABILIDADE_HPP

#include <string>

/**
 * @file Habilidade.hpp
 * @brief Definição da classe Habilidade e seu enumerador de tipos.
 */

/** * @enum TipoHabilidade 
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

/**
 * @class Habilidade
 * @brief Objeto que contém os metadados e parâmetros de uma ação especial no jogo.
 * * A classe Habilidade encapsula todas as informações necessárias para que o sistema de combate
 * calcule custos de recursos (Energia/Mana) e gerencie a persistência de efeitos contínuos 
 * (como envenenamentos ou buffs por turno).
 * * @param nome Nome identificador da habilidade (ex: "Bola de Fogo").
     * @param tipo Categoria mecânica da ação (@see TipoHabilidade).
     * @param custoE Quantidade de pontos de Energia consumidos pelo uso.
     * @param custoM Quantidade de pontos de Mana (MP) consumidos pelo uso.
     * @param valorBase Intensidade numérica base (pode representar dano, cura ou potência de efeito).
     * @param duracao Quantidade de turnos que o efeito persiste no alvo. Por padrão é 0 (efeito instantâneo).
     * @param atributoAfetado Em caso de Buffs ou Debuffs.
 */
class Habilidade {
public:
    /**
     * @brief Construtor completo e estrito da classe Habilidade.
     * * Inicializa uma habilidade com todos os seus atributos reguladores de combate.
     * * @param nome Nome identificador da habilidade (ex: "Bola de Fogo").
     * @param tipo Categoria mecânica da ação (@see TipoHabilidade).
     * @param custoE Quantidade de pontos de Energia consumidos pelo uso.
     * @param custoM Quantidade de pontos de Mana (MP) consumidos pelo uso.
     * @param valorBase Intensidade numérica base (pode representar dano, cura ou potência de efeito).
     * @param duracao Quantidade de turnos que o efeito persiste no alvo. Por padrão é 0 (efeito instantâneo).
     * @param atributoAfetado Em caso de Buffs ou Debuffs.

     * * @pre Os custos (@p custoE, @p custoM), @p valorBase e @p duracao não devem ser negativos.
     */
    Habilidade(std::string nome, TipoHabilidade tipo, int custoE, int custoM, int valorBase, std::string atributoAfetado, int duracao = 0);

    /** * @brief Retorna o nome da habilidade. 
     * @return std::string String contendo o nome para exibição em menus.
     */
    std::string getNome() const;

    /** * @brief Retorna o tipo/categoria da habilidade. 
     * @return TipoHabilidade O enumerador correspondente à mecânica da ação.
     */
    TipoHabilidade getTipo() const;

    /** * @brief Retorna o custo de Energia da habilidade. 
     * @return int Pontos de energia necessários para a conjuração (comum em classes físicas).
     */
    int getCustoEnergia() const;

    /** * @brief Retorna o custo de Pontos de Magia (MP). 
     * @return int Pontos de mana necessários para a conjuração (comum em classes mágicas).
     */
    int getCustoMP() const;

    /** * @brief Retorna o atributo afetado pelo buff ou debuff.
     * @return string com o nome do atributo afetado.
     */
    std::string getAtributoAlvo() const;

    /** * @brief Retorna o valor numérico base da habilidade. 
     * @return int Valor bruto antes de aplicar os modificadores de atributos do personagem ou defesas.
     */
    int getValorBase() const;

    /** * @brief Retorna a duração do efeito em turnos. 
     * @return int Quantidade de turnos que o efeito persiste (0 indica ação instantânea).
     */
    int getDuracao() const;

    /** * @brief Verifica se a habilidade aplica um efeito que persiste por múltiplos turnos.
     * * Analisa o tipo da habilidade para determinar se ela deve entrar na fila de status 
     * contínuos do alvo (como @c TipoHabilidade::DOT, @c HOT, @c SUPORTE ou @c DEBUFF).
     * * @return true Se a habilidade possuir um efeito de múltiplos turnos ativo (@c duracao > 0).
     * @return false Se for uma ação de impacto imediato.
     */
    bool ehEfeitoContinuo() const;

    int getDuracaoEfeito() const;
    
    std::string _nome;              ///< Nome identificador da habilidade.
    TipoHabilidade _tipo;           ///< Natureza mecânica da ação.
    int _custoEnergia;              ///< Custo de uso focado na barra de Energia.
    int _custoMP;                   ///< Custo de uso focado na barra de Mana.
    int _valorBase;                 ///< Potência bruta (Dano, Cura, ou valor de alteração de Status).
    int _duracao;                   ///< Tempo de vida do efeito em turnos (0 = imediato).
    std::string _atributoAfetado;   ///< Nome do Atributo afetado.

};

#endif // HABILIDADE_HPP