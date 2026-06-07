#ifndef AVENTUREIRO_HPP
#define AVENTUREIRO_HPP

#include "Personagem.hpp"
#include "Habilidade.hpp"
#include <vector>
#include <string>

/**
 * @file Aventureiro.hpp
 * @brief Definição da classe Aventureiro, entidade controlada pelo usuário.
 */

/**
 * @class Aventureiro
 * @brief Representa o personagem jogável, contendo sistemas de progressão, inventário de habilidades e recursos duais.
 * * A classe Aventureiro estende @c Personagem introduzindo o gerenciamento de dois recursos de combate:
 * Pontos de Magia (MP) e Energia. Ela também encapsula a lógica de ganho de experiência (XP),
 * subida de nível (*Level Up*), bônus permanentes de equipamentos (Armas/Armaduras), e controle de 
 * estado temporário (como a mitigação de dano por escudo).
 */
class Aventureiro : public Personagem {
public:
    /**
     * @brief Construtor da classe Aventureiro.
     * * Inicializa o herói com seus atributos iniciais de combate. Por padrão, define os frascos
     * de cura iniciais em 3, o nível inicial como 1, e calcula as barras de MP e Energia máximas.
     * * @param nome Nome do avatar escolhido pelo jogador.
     * @param hp Pontos de vida máximos iniciais.
     * @param defesa Atributo de defesa física base.
     * @param forca Atributo de força base que dita o dano físico.
     */
    Aventureiro(std::string nome, int hp, int defesa, int forca);

    /** * @brief Sobrescreve a execução do turno para processar o menu interativo do jogador.
     * * Interrompe o fluxo automático para solicitar uma ação ao usuário através da @c InterfaceJogo 
     * (Atacar, Defender, Usar Habilidades ou Usar Itens).
     * * @param alvo Referência à entidade hostil alvo das ações deste turno.
     */
    void executarTurno(Personagem& alvo) override;
    
    /** * @brief Aplica a redução de HP considerando os pontos de defesa e o estado do Escudo.
     * * Sobrescreve o método base para verificar se a flag @c _escudoAtivo está ligada. Se estiver,
     * o dano recebido sofre uma mitigação severa adicional antes de atingir o @c _hp. Ao fim do cálculo, 
     * o escudo é desativado.
     * * @param dano Valor bruto do dano entrante.
     */
    void receberDano(int dano, TipoHabilidade tipoDaHabilidade) override;

    /** * @brief Calcula e retorna o valor de defesa total do herói.
     * * Realiza a soma aritmética da defesa base da classe com os bônus permanentes adquiridos.
     * * @return int Valor consolidado de defesa (@c _defesaBase + @c _bonusArmadura).
     */
    int getDefesa() const override;

    /** * @brief Regenera passivamente uma fração de Mana (MP) e Energia.
     * * Método disparado automaticamente pelo ciclo de combate no início de cada rodada do jogador,
     * impedindo que o fluxo de batalha fique travado por falta de recursos básicos.
     */
    void recuperarRecursos();
    
    /** * @brief Deduz uma quantidade específica da barra de Mana (MP).
     * * @param qtd Pontos de mana a serem consumidos pelo feitiço.
     * @pre @p qtd não deve ser maior que o MP atual disponível.
     */
    void consumirMP(int qtd);
    
    /** * @brief Deduz uma quantidade específica da barra de Energia.
     * * Utilizado no disparo de técnicas físicas ou ao levantar o escudo de proteção.
     * * @param qtd Pontos de energia a serem consumidos pela ação.
     * @pre @p qtd não deve ser maior que a Energia atual disponível.
     */
    void consumirEnergia(int qtd);

    
    /** * @brief Consome uma carga do consumível de cura para restaurar vida instantaneamente.
     * * Verifica a disponibilidade de frascos. Se houver cargas, consome 1 unidade e invoca
     * o método @c receberCura() aplicando um valor fixo ou percentual.
     * * @return true Se a cura foi aplicada com sucesso.
     * @return false Se não haviam mais frascos disponíveis no inventário.
     */
    bool usarFrasco();

    void usarEscudo();
    
    /** * @brief Restaura completamente a saúde, recursos do herói e recarrega os frascos de cura.
     * * Utilizado em nós narrativos de descanso/tavernas para reestabelecer o estado do herói.
     */
    void dormir();

    /** * @brief Adiciona uma nova estrutura de habilidade ao grimório de conhecimentos do herói.
     * * @param hb Instância da @c Habilidade a ser aprendida e disponibilizada nos menus de combate.
     */
    void aprenderHabilidade(Habilidade hb);

    /** * @brief Concede pontos de experiência ao herói e gerencia o evento de subida de nível.
     * * Acumula o valor recebido e verifica se o montante ultrapassou o limiar de @c _xpProxNivel.
     * Caso positivo, dispara internamente o método privado @c subirNivel().
     * * @param qtd Quantidade de experiência líquida concedida (geralmente vinda do @c Inimigo::getXPRecompensa()).
     * @return true Se o herói subiu de nível nesta chamada.
     * @return false Se a experiência foi acumulada sem alteração de nível.
     */
    bool ganharExperiencia(int qtd);

    /** * @brief Incrementa permanentemente o bônus de dano de ataque por modificação de equipamento.
     * * @param valor Pontos de bônus ofensivo concedidos (ex: ao afiar uma espada em um evento).
     */
    void buffArma(int valor);

    /** * @brief Incrementa permanentemente o bônus de redução de dano por modificação de equipamento.
     * * @param valor Pontos de bônus defensivo concedidos (ex: ao forjar melhorias na armadura).
     */
    void buffArmadura(int valor);

    /** * @brief Empacota e retorna uma string ricamente formatada com os recursos específicos do jogador.
     * * Constrói o layout textual exibindo Nome, Nível, HP, MP e Energia para ser consumido pela interface.
     * * @return std::string Bloco de texto multi-linha formatado.
     */
    std::string getDeclaracaoStatus() const override;

    // ==========================================
    // GETTERS E SETTERS DE ESTADO
    // ==========================================

    /** @brief Retorna o poder de ataque total consolidado do herói (@c _forcaBase + @c _bonusArma). */
    int getForcaTotal() const;

    /** @brief Retorna o ID da cena que serve como ponto de ressurreição ativo. */
    int getIDCheckpoint() const;

    /** @brief Altera o ID da cena de salvamento ativo onde o jogador reaparecerá se morrer. */
    void setIDCheckpoint(int id);

    /** @brief Retorna uma referência constante para a lista de habilidades que o jogador possui. */
    const std::vector<Habilidade>& getHabilidades() const;

    int getMP() const;          ///< Retorna os pontos de Mana atuais.
    int getMPMax() const;       ///< Retorna o limite máximo de pontos de Mana.
    int getEnergia() const;     ///< Retorna os pontos de Energia atuais.
    int getEnergiaMax() const;  ///< Retorna o limite máximo de pontos de Energia.

private:
    int _energia;               ///< Pontos de fadiga física atuais.
    int _energiaMax;            ///< Capacidade máxima de armazenamento de fadiga.
    int _mp;                    ///< Pontos de energia mágica atuais.
    int _mpMax;                 ///< Capacidade máxima de armazenamento mágico.
    int _frascos;               ///< Quantidade restante de cargas de cura (limite base de 3).
    int _forcaBase;             ///< Atributo nativo de ataque do personagem.
    int _bonusArma;             ///< Modificador de dano somado por melhoria de equipamento.
    int _bonusArmadura;         ///< Modificador de defesa somado por melhoria de equipamento.
    int _xp;                    ///< Experiência acumulada no nível atual.
    int _xpProxNivel;           ///< Meta de experiência necessária para o próximo nível.
    int _idCenaCheckpoint;      ///< ID da cena mapeada como último ponto de descanso seguro visitado.
    bool _escudoAtivo;          ///< Variavel booleana que define a ativação do escudo.


    std::vector<Habilidade> _habilidadesConhecidas; ///< Vetor que armazena o acervo de técnicas utilizáveis do jogador.

    /** * @brief Executa as atualizações aritméticas de atributos ao subir de nível.
     * * Incrementa o nível, amplia as capacidades máximas de HP, MP e Energia, e restaura
     * completamente a saúde do herói como bônus de progressão.
     */
    void subirNivel();
};

#endif // AVENTUREIRO_HPP