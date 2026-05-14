#ifndef MOTORJOGO_HPP
#define MOTORJOGO_HPP

#include "Aventureiro.hpp"
#include "Cena.hpp"
#include <map>

class Inimigo; //A inclusão do Header é feita no arquivo.cpp.
class Combate; //A inclusão do Header é feita no arquivo.cpp.

/**
 * @class MotorJogo
 * @brief Classe controladora que conecta a narrativa ao sistema de combate.
 */
class MotorJogo {
public:
    MotorJogo(Aventureiro& player);

    /** @brief Roda o loop principal de transicao de cenas e eventos. */
    void rodar();

    /** * @brief Orquestra a criação do inimigo, o loop de combate e a limpeza de memória.
     * @return true se o jogador venceu, false se foi derrotado.
        */
    bool realizarCombate(std::string tipoInimigo);

private:
    Aventureiro& _player;
    std::map<int, Cena> _roteiro;
    int _idCenaAtual;

    /** @brief Instancia todas as cenas e escolhas do jogo. */
    void inicializarHistoria();
    
    /** @brief Trata o fluxo caso o jogador perca uma batalha (volta ao checkpoint). */
    void processarDerrota();
    
    /** @brief Verifica se a escolha feita concede bonus permanentes ao heroi. */
    void checarEventosEspeciais(const Escolha& escolha);
};

#endif