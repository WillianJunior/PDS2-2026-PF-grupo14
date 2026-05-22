#ifndef INIMIGOS_BOSS_HPP
#define INIMIGOS_BOSS_HPP

#include "Inimigo.hpp"

//Esses inimigos são apenas exemplos por enquanto.


class Dragao : public Inimigo {
public:
    Dragao(std::string nome = "Dragão de Cinzas", int nivel = 10);

    void executarTurno(Personagem& alvo) override;

private:
    bool _emFuria; ///< Atributo exclusivo deste tipo de inimigo
    int _furiaContador;
};

#endif