/**
 * @file test_InimigoIncomum.cpp
 * @brief Testes de unidade para Bruxa e GolemPedra usando doctest (TDD).
 */

#include "doctest/doctest.h"
#include "InimigoIncomum.hpp"
#include "Aventureiro.hpp"

TEST_SUITE("Bruxa - Estado Inicial") {

    TEST_CASE("Bruxa começa viva") {
        Bruxa b("Bruxa", 3);
        CHECK(b.estaVivo() == true);
    }

    TEST_CASE("Bruxa tem HP positivo") {
        Bruxa b("Bruxa", 3);
        CHECK(b.getHP() > 0);
    }

    TEST_CASE("Bruxa tem XP de recompensa positiva") {
        Bruxa b("Bruxa", 3);
        CHECK(b.getXPRecompensa() > 0);
    }

    TEST_CASE("Bruxa armazena nome corretamente") {
        Bruxa b("Bruxa Sombria", 3);
        CHECK(b.getNome() == "Bruxa Sombria");
    }
}

TEST_SUITE("Bruxa - Comportamento") {

    TEST_CASE("Bruxa causa dano ao aventureiro") {
        Aventureiro a("Herói", 500, 0, 10);
        Bruxa b("Bruxa", 3);
        int hpAntes = a.getHP();
        b.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Bruxa recebe dano e HP diminui") {
        Bruxa b("Bruxa", 3);
        int hpAntes = b.getHP();
        b.receberDano(20);
        CHECK(b.getHP() < hpAntes);
    }

    TEST_CASE("Bruxa morre com dano letal") {
        Bruxa b("Bruxa", 3);
        b.receberDano(99999);
        CHECK(b.estaVivo() == false);
        CHECK(b.getHP() == 0);
    }
}

TEST_SUITE("GolemPedra - Estado Inicial") {

    TEST_CASE("GolemPedra começa vivo") {
        GolemPedra gp("Golem", 4);
        CHECK(gp.estaVivo() == true);
    }

    TEST_CASE("GolemPedra tem HP positivo") {
        GolemPedra gp("Golem", 4);
        CHECK(gp.getHP() > 0);
    }

    TEST_CASE("GolemPedra tem XP de recompensa positiva") {
        GolemPedra gp("Golem", 4);
        CHECK(gp.getXPRecompensa() > 0);
    }
}

TEST_SUITE("GolemPedra - Comportamento") {

    TEST_CASE("GolemPedra causa dano ao aventureiro") {
        Aventureiro a("Herói", 500, 0, 10);
        GolemPedra gp("Golem", 4);
        int hpAntes = a.getHP();
        gp.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("GolemPedra recebe dano e HP diminui") {
        GolemPedra gp("Golem", 4);
        int hpAntes = gp.getHP();
        gp.receberDano(30);
        CHECK(gp.getHP() < hpAntes);
    }

    TEST_CASE("GolemPedra morre com dano letal") {
        GolemPedra gp("Golem", 4);
        gp.receberDano(99999);
        CHECK(gp.estaVivo() == false);
    }
}
