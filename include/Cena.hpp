#ifndef CENA_HPP
#define CENA_HPP

#include <string>
#include <vector>

/** @struct Escolha @brief Define uma ramificação na história. */
struct Escolha {
    std::string texto;       ///< Texto exibido na opção.
    int destinoID;           ///< ID da cena para onde o jogador irá.
    bool geraCombate;        ///< Define se a escolha dispara uma luta.
    std::string tipoInimigo; ///< Caso haja combate, qual o script do inimigo.
    int itemGanhoID;         /// Opcional: 0 se nenhum, >0 se ganhar algo
};

/**
 * @class Cena
 * @brief Representa um nó narrativo do jogo.
 */
class Cena {
public:
    Cena(int id, std::string texto, bool checkpoint = false);

    /** @brief Adiciona uma opção de escolha à cena. */
    void adicionarEscolha(Escolha e);

    int getID() const;
    std::string getTexto() const;
    const std::vector<Escolha>& getEscolhas() const;
    bool isCheckpoint() const;

private:
    int _id;
    std::string _textoNarrativo;
    std::vector<Escolha> _escolhas;
    bool _checkpoint;
};

#endif