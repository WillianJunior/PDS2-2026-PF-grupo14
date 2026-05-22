/**
 * @file test_InimigoBoss.cpp
 * @brief Testes de unidade para a classe Dragao usando doctest (TDD).
 */

#include "doctest.h"
#include "InimigoBoss.hpp"
#include "Aventureiro.hpp"

TEST_SUITE("Dragao - Estado Inicial") {

    TEST_CASE("Dragao é criado com nome correto") {
        Dragao d("Dragão de Cinzas", 10);
        CHECK(d.getNome() == "Dragão de Cinzas");
    }

    TEST_CASE("Dragao começa vivo") {
        Dragao d;
        CHECK(d.estaVivo() == true);
    }

    TEST_CASE("Dragao tem HP positivo") {
        Dragao d;
        CHECK(d.getHP() > 0);
    }

    TEST_CASE("Dragao tem XP de recompensa positiva") {
        Dragao d;
        CHECK(d.getXPRecompensa() > 0);
    }

    TEST_CASE("Dragao tem nivel 10 por padrão") {
        Dragao d;
        CHECK(d.getNivel() == 10);
    }
}

TEST_SUITE("Dragao - Comportamento") {

    TEST_CASE("Dragao causa dano ao aventureiro") {
        Aventureiro a("Herói", 1000, 0, 10);
        Dragao d;
        int hpAntes = a.getHP();
        
        d.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Dragao recebe dano e HP diminui") {
        Dragao d;
        int hpAntes = d.getHP();
        
        d.receberDano(50);
        CHECK(d.getHP() < hpAntes);
    }

    TEST_CASE("Dragao morre com dano letal - Status") {
        Dragao d;
        d.receberDano(99999);
        CHECK(d.estaVivo() == false);
    }

    TEST_CASE("Dragao morre com dano letal - Zerar HP") {
        Dragao d;
        d.receberDano(99999);
        CHECK(d.getHP() == 0);
    }
}