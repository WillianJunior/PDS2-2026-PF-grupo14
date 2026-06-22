#ifndef MOTORJOGO_HPP
#define MOTORJOGO_HPP

#include "Aventureiro.hpp"
#include "Cena.hpp"
#include "InterfaceJogo.hpp"
#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <functional>
#include <tuple>

// Forward declarations para evitar acoplamento desnecessário e inclusões circulares
class Inimigo;
class Combate;

/**
 * @file MotorJogo.hpp
 * @brief Definição da classe controladora do fluxo global do RPG (Engine).
 */

/**
 * @class MotorJogo
 * @brief Classe controladora (Engine) que gerencia o ciclo de vida do jogo, conectando a narrativa ao combate.
 *
 * O MotorJogo atua como a máquina de estados principal do RPG textual. Ele armazena o roteiro completo,
 * controla a transição de cenas com base nas escolhas do usuário, intercepta eventos para acionar
 * combates dinâmicos e manipula o estado do jogador em caso de progresso ou derrota.
 */
class MotorJogo {
public:
    /**
     * @brief Construtor da classe MotorJogo.
     *
     * Associa o herói principal ao motor e engatilha a inicialização da história.
     *
     * @param player Referência para o Aventureiro que passará pelos eventos do jogo.
     */
    MotorJogo(Aventureiro& player);

    /**
     * @brief Executa o loop principal de transição de cenas e eventos narrativos.
     *
     * Este método mantém o jogo ativo, renderizando os textos da cena atual (@c _idCenaAtual),
     * solicitando as escolhas do jogador através da interface e direcionando o fluxo para as
     * próximas ramificações da história até atingir um estado final.
     *
     * @throws std::runtime_error Se o ID da cena atual não existir no roteiro.
     */
    void rodar();

    /**
     * @brief Orquestra a criação do inimigo, o gerenciamento do combate e a limpeza de memória.
     *
     * Atua como uma fábrica (Factory). Com base na string fornecida, o método instancia
     * dinamicamente o inimigo correspondente via RAII (unique_ptr), cria o objeto @c Combate,
     * inicia o loop de batalha e garante a desalocação correta sem new/delete manual.
     *
     * @param tipoInimigo Identificador textual que define qual criatura será instanciada.
     * @return true Se o jogador venceu o combate e pode prosseguir na narrativa.
     * @return false Se o jogador foi derrotado no confronto.
     *
     * @throws std::invalid_argument Se tipoInimigo for vazio ou não reconhecido.
     * @see Combate::iniciar()
     */
    bool realizarCombate(std::string tipoInimigo);

private:
    Aventureiro& _player;          ///< Referência ao herói controlado pelo jogador.
    InterfaceJogo _interface;      ///< Interface responsável por toda I/O do jogo.
    std::map<int, Cena> _roteiro;  ///< Dicionário que mapeia IDs únicos para suas respectivas Cenas.
    int _idCenaAtual;              ///< ID indicador de qual cena está ativa no momento.

    /**
     * @brief Instancia e popula o mapa do roteiro com todas as cenas e escolhas possíveis.
     *
     * Método chamado internamente no construtor para carregar os textos, ramificações de
     * diálogos e IDs de destinos estruturais da campanha.
     */
    void inicializarHistoria();

    /**
     * @brief Trata o fluxo de falha caso o jogador perca uma batalha.
     *
     * Restaura os recursos do jogador via dormir() e reconfigura o @c _idCenaAtual
     * para o último ponto de salvamento seguro (checkpoint).
     */
    void processarDerrota();

    /**
     * @brief Verifica e aplica bônus obtidos por escolhas narrativas.
     *
     * Analisa os metadados da escolha feita pelo jogador para identificar se ela
     * concede itens ou bônus permanentes ao aventureiro.
     *
     * @param escolha Referência constante para a estrutura de escolha selecionada.
     */
    void checarEventosEspeciais(const Escolha& escolha);

    /**
     * @brief Fábrica interna de inimigos — instancia o tipo correto via RAII.
     *
     * Recebe o identificador textual e retorna um unique_ptr para o inimigo correspondente.
     * Garante desalocação automática sem necessidade de delete manual.
     *
     * @param tipoInimigo String identificadora do inimigo a instanciar.
     * @return std::unique_ptr<Inimigo> Ponteiro proprietário para o inimigo criado.
     *
     * @throws std::invalid_argument Se tipoInimigo não corresponder a nenhum tipo conhecido.
     */
    std::unique_ptr<Inimigo> criarInimigo(const std::string& tipoInimigo);
};

#endif // MOTORJOGO_HPP