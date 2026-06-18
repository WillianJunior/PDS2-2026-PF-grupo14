#ifndef CENA_HPP
#define CENA_HPP

#include <string>
#include <vector>

/**
 * @file Cena.hpp
 * @brief Definição da estrutura Escolha e da classe Cena para o fluxo narrativo.
 */

/** * @struct Escolha 
 * @brief Define uma ramificação (opção) direcionada a partir de uma cena da história.
 * * Cada escolha atua como uma aresta em um grafo narrativo, conectando a cena atual
 * a um novo nó (destino), além de carregar metadados que podem modificar o estado do jogo.
 */
struct Escolha {
    std::string texto;       ///< Texto descritivo exibido no menu de opções para o jogador.
    int destinoID;           ///< ID identificador da próxima cena para onde o jogador será direcionado.
    bool geraCombate;        ///< Flag indicando se a seleção desta opção engatilha um combate imediato.
    std::string tipoInimigo; ///< String identificadora do monstro a ser gerado caso @c geraCombate seja true.
    int itemGanhoID;         ///< ID do item concedido ao jogador (0 se nenhum bônus for aplicado, >0 para itens válidos).
};

/**
 * @class Cena
 * @brief Representa um nó narrativo isolado dentro do grafo de história do jogo.
 * * A classe Cena funciona como um contêiner de exibição e transição. Ela armazena o texto
 * descritivo do ambiente ou situação e uma lista de ramificações possíveis (@see Escolha) 
 * que alteram o rumo da campanha linear.
 */
class Cena {
public:
    /**
     * @brief Construtor estrito da classe Cena.
     * * Inicializa o nó narrativo com seu identificador e corpo de texto principal.
     * * @param id Identificador numérico único do nó da cena (usado para mapeamento no roteiro).
     * @param texto Bloco de texto textual contendo a narrativa, diálogos ou descrição do cenário.
     * @param checkpoint Flag definindo se esta cena salva o progresso automático para retorno em caso de morte.
     * * @pre O @p id não deve ser negativo.
     */
    Cena(int id, std::string texto, bool checkpoint = false);

    /** * @brief Adiciona uma nova ramificação de opção associada a este nó narrativo.
     * * @param e Instância da estrutura @c Escolha contendo o texto e o ID do nó de destino.
     */
    void adicionarEscolha(Escolha e);

    /** * @brief Retorna o ID identificador único da cena.
     * @return int Código numérico de indexação da cena.
     */
    int getID() const;

    /** * @brief Retorna o texto narrativo principal armazenado para exibição.
     * @return std::string Bloco de texto contendo a descrição da cena.
     */
    std::string getTexto() const;

    /** * @brief Retorna uma referência constante para o vetor contendo as escolhas disponíveis.
     * * O retorno por referência constante evita cópias desnecessárias na memória em tempo de execução.
     * * @return const std::vector<Escolha>& Vetor contendo todas as ramificações ligadas a este nó.
     */
    const std::vector<Escolha>& getEscolhas() const;

    /** * @brief Verifica se a cena atual está classificada como um ponto de salvamento seguro.
     * @return true Se a cena for um checkpoint de retorno pós-derrota.
     * @return false Se for uma cena narrativa comum de transição.
     */
    bool isCheckpoint() const;

private:
    int _id;                            ///< ID numérico identificador exclusivo.
    std::string _textoNarrativo;        ///< Conteúdo de texto impresso para o usuário.
    std::vector<Escolha> _escolhas;     ///< Lista de opções de saída indexadas a este nó.
    bool _checkpoint;                   ///< Flag indicadora de estado de salvamento.
};

#endif // CENA_HPP