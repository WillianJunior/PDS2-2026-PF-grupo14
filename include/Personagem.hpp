#ifndef PERSONAGEM_HPP
#define PERSONAGEM_HPP

#include <string>
#include <vector>
#include "Habilidade.hpp"

/**
 * @file Personagem.hpp
 * @brief Definição da classe base abstrata Personagem.
 */

/**
 * @brief Estrutura que define as propriedades de um efeito de dano contínuo (DoT).
 * * Mantém os dados necessários para que efeitos como veneno, queimadura ou 
 * sangramento persistam na memória do alvo por múltiplos turnos.
 */
struct EfeitoDoT {
    TipoHabilidade tipo;       /**< Tipo do efeito (obrigatoriamente TipoHabilidade::DOT para ignorar escudos). */
    int danoPorTurno;          /**< Quantidade de dano bruto que será infligida a cada rodada. */
    int turnosRestantes;       /**< Contador de rodadas que o efeito ainda permanecerá ativo. */
    std::string nomeEfeito;    /**< Nome identificador do efeito (ex: "Veneno Serpentino", "Sangramento"). */
};

/**
 * @class Personagem
 * @brief Classe abstrata que define os atributos, recursos de saúde e comportamentos básicos de qualquer entidade no jogo.
 * * A classe Personagem serve como a base conceitual para jogadores e monstros. Ela centraliza o
 * gerenciamento de ciclo de vida (pontos de vida, dano, cura e nível) e dita o contrato de turnos 
 * que o motor de combate precisa para rodar o jogo, além de guardar de forma privada o inventário 
 * de habilidades da entidade.
 */
class Personagem {
public:
    /**
     * @brief Construtor da classe Personagem.
     * * Configura os dados iniciais da entidade de combate. O parâmetro @p hp define tanto a 
     * vida atual quanto o limite máximo (@c _hpMax) no início do jogo.
     * * @param nome Nome de exibição da entidade.  
     * @param hp Pontos de vida máximos e atuais iniciais.
     * @param defesa Pontos de defesa base para mitigação de danos.
     * @param nivel Nível inicial de desafio ou progressão.
     * * @pre Os valores de @p hp, @p defesa e @p nivel devem ser maiores que zero.
     */
    Personagem(std::string nome, int hp, int defesa, int forca, int nivel);

    /** * @brief Destrutor virtual puro padronizado.
     * * Garante que, ao deletar um ponteiro polimórfico de Personagem, o destrutor da subclasse 
     * específica (@c Aventureiro ou @c Inimigo) seja invocado corretamente, evitando vazamentos de memória.
     */
    virtual ~Personagem() = default;

    /**
     * @brief Método virtual puro que define a Inteligência Artificial ou comportamento de turno da entidade.
     * * Este método deve ser obrigatoriamente implementado por todas as subclasses para ditar o que a 
     * entidade faz quando chega o seu momento de agir no ciclo de batalha.
     * * @param alvo Referência para a entidade oponente que sofrerá as ações do turno.
     */
    virtual void executarTurno(Personagem& alvo) = 0;

    /** * @brief Insere com segurança uma nova habilidade no repositório privado da entidade.
     * * @param habilidade Referência constante para o objeto Habilidade a ser armazenado.
     */
    void adicionarHabilidade(const Habilidade& habilidade);

    /** * @brief Fornece acesso controlado ao acervo de habilidades conhecidas pela entidade.
     * * @return const std::vector<Habilidade>& Referência de leitura do vetor de habilidades, impedindo cópias de memória.
     */
    const std::vector<Habilidade>& getHabilidades() const;

    /**
     * @brief Método virtual puro para exportar os dados de saúde e recursos formatados em texto.
     * * Deve ser sobrescrito pelas subclasses para que cada uma monte sua própria assinatura visual 
     * de status para o console (ex: incluindo ou não barras de mana/energia).
     * * @return std::string String multi-linha formatada pronta para impressão.
     */
    virtual std::string getDeclaracaoStatus() const = 0;

    /**
     * @brief Aplica a redução de pontos de vida (HP) com base na fórmula de armadura.
     * * A implementação padrão calcula a mitigação usando o valor retornado por @c getDefesa(). 
     * Pode ser sobrescrita caso uma subclasse possua mecânicas únicas de imunidade ou escudos temporários.
     * * @param dano Valor bruto do dano direcionado à entidade.
     * * @param tipoDaHabilidade Tipo de dano que será recebido. 
     * * @note O HP resultante nunca será reduzido abaixo de zero.
     */
    virtual void receberDano(int dano, TipoHabilidade tipoDaHabilidade);

    /**
     * @brief Incrementa os pontos de vida (HP) atuais da entidade.
     * * Adiciona o valor à barra de saúde atual, garantindo que o teto estabelecido por @c _hpMax 
     * nunca seja ultrapassado.
     * * @param valor Quantidade de pontos de vida a serem restaurados.
     */
    virtual void receberCura(int valor);


    /**
     * @brief Injeta e registra um novo efeito de dano por turno (DoT) na lista do personagem.
     * * Cria uma instância de EfeitoDoT com os parâmetros fornecidos e a anexa ao vetor 
     * de monitoramento de efeitos ativos. Exibe um aviso textual informando o status aplicado.
     * * @param nome Nome do efeito/habilidade que gerou o dano contínuo.
     * @param dano Quantidade de dano que será processada a cada ativação de rodada.
     * @param duracao Quantidade total de turnos que o efeito persistirá ativo.
     */
    void aplicarDoT(std::string nome, int dano, int duracao);

/**
     * @brief Varre e processa todos os efeitos de dano por turno ativos no personagem.
     * * Deve ser invocado obrigatoriamente no início da rodada do personagem (dentro de executarTurno).
     * O método percorre o vetor de efeitos ativos, dispara a rotina receberDano para cada item, 
     * decrementa o tempo de vida de cada DoT e remove da memória os efeitos cuja duração expirou.
     */
    void processarEfeitosContinuos();

    /** * @brief Retorna o valor de defesa ativo do personagem para cálculos de mitigação.
     * * Pode ser sobrescrito em subclasses para adicionar bônus temporários de equipamentos ou buffs.
     * * @return int Valor de defesa utilizado no cálculo do dano sofrido.
     */
    virtual int getDefesa() const;

    int getHP() const;             ///< Retorna a quantidade de pontos de vida (HP) atuais.
    int getHPMax() const;          ///< Retorna o limite máximo de pontos de vida da entidade.
    bool estaVivo() const;         ///< Verifica a integridade da entidade. Retorna @c true se @c _hp > 0.
    std::string getNome() const;   ///< Retorna o nome identificador do personagem.
    int getNivel() const;          ///< Retorna o nível de poder atual da entidade.

protected:
    std::string _nome;    ///< Nome da entidade.
    int _hp;              ///< Pontos de vida (HP) atuais da entidade.
    int _hpMax;           ///< Capacidade máxima de armazenamento de saúde.
    int _defesaBase;      ///< Atributo nativo de mitigação de dano físico.
    int _nivel;           ///< Nível de poder ou escala de atributos da entidade.
    int _forcaBase;       ///< Atributo nativo de dano físico.
    bool _vivo;           ///< Indica se o Personagem esta vivo.
    bool _escudoAtivo;    ///< Estado de postura defensiva ativa que mitiga o próximo golpe sofrido.
/**
     * @brief Vetor que armazena todos os efeitos de dano contínuo ativos no personagem.
     */
    std::vector<EfeitoDoT> _dotsAtivos;

private: 
    std::vector<Habilidade> _habilidades; ///< Repositório privado e blindado contendo as técnicas da entidade.
};

#endif // PERSONAGEM_HPP