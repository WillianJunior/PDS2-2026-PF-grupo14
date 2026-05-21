/**
 * @file test_InimigoComum.cpp
 * @brief Testes de unidade para Goblin e Slime usando doctest (TDD).
 */

#include "doctest/doctest.h"
#include "InimigoComum.hpp"
#include "Aventureiro.hpp"

TEST_SUITE("Goblin - Estado Inicial") {

    TEST_CASE("Goblin começa vivo") {
        Goblin g("Goblin", 1);
        CHECK(g.estaVivo() == true);
    }

    TEST_CASE("Goblin tem HP positivo") {
        Goblin g("Goblin", 1);
        CHECK(g.getHP() > 0);
    }

    TEST_CASE("Goblin tem XP de recompensa positiva") {
        Goblin g("Goblin", 1);
        CHECK(g.getXPRecompensa() > 0);
    }

    TEST_CASE("Goblin armazena nome corretamente") {
        Goblin g("GoblinTeste", 1);
        CHECK(g.getNome() == "GoblinTeste");
    }

    TEST_CASE("Goblin nível 5 tem mais HP que nível 1") {
        Goblin g1("Goblin", 1);
        Goblin g5("Goblin", 5);
        CHECK(g5.getHP() > g1.getHP());
    }
}

TEST_SUITE("Goblin - Comportamento") {

    TEST_CASE("Goblin causa dano ao aventureiro") {
        Aventureiro a("Herói", 500, 0, 10);
        Goblin g("Goblin", 1);
        int hpAntes = a.getHP();
        g.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Goblin recebe dano e HP diminui") {
        Goblin g("Goblin", 1);
        int hpAntes = g.getHP();
        g.receberDano(10);
        CHECK(g.getHP() < hpAntes);
    }

    TEST_CASE("Goblin morre com dano letal") {
        Goblin g("Goblin", 1);
        g.receberDano(99999);
        CHECK(g.estaVivo() == false);
        CHECK(g.getHP() == 0);
    }
}

TEST_SUITE("Slime - Estado Inicial") {

    TEST_CASE("Slime começa vivo") {
        Slime s("Slime Azul", 1);
        CHECK(s.estaVivo() == true);
    }

    TEST_CASE("Slime tem HP positivo") {
        Slime s("Slime Azul", 1);
        CHECK(s.getHP() > 0);
    }

    TEST_CASE("Slime tem XP de recompensa positiva") {
        Slime s("Slime Azul", 1);
        CHECK(s.getXPRecompensa() > 0);
    }
}

TEST_SUITE("Slime - Comportamento") {

    TEST_CASE("Slime causa dano ao aventureiro") {
        Aventureiro a("Herói", 500, 0, 10);
        Slime s("Slime Azul", 1);
        int hpAntes = a.getHP();
        s.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Slime morre com dano letal") {
        Slime s("Slime Azul", 1);
        s.receberDano(99999);
        CHECK(s.estaVivo() == false);
    }
}
