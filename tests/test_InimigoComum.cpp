/**
 * @file test_InimigoComum.cpp
 * @brief Testes de unidade para Goblin e Slime usando doctest (TDD).
 */

#include "doctest.h"
#include "InimigoComum.hpp"
#include "Aventureiro.hpp"

TEST_SUITE("Inimigos Comun - Estado Inicial") {

    TEST_CASE("Goblin começa vivo e com status positivo") {
        Goblin g("Goblin", 1);
        CHECK(g.estaVivo() == true);
        CHECK(g.getHP() > 0);
        CHECK(g.getXPRecompensa() > 0);
    }

    TEST_CASE("Slime começa vivo e com status positivo") {
        Slime s("Slime Azul", 1);
        CHECK(s.estaVivo() == true);
        CHECK(s.getHP() > 0);
        CHECK(s.getXPRecompensa() > 0);
    }

    TEST_CASE("getDeclaracaoStatus do Goblin funciona no esqueleto") {
        Goblin g("GoblinTeste", 1);
        std::string status = g.getDeclaracaoStatus();
        if (!status.empty()) {
            CHECK(status.find("GoblinTeste") != std::string::npos);
        } else {
            CHECK(false); // Falha limpa planejada (TDD Red)
        }
    }
}

TEST_SUITE("Inimigos Comun - Comportamento e Dano") {

    TEST_CASE("Goblin causa dano ao aventureiro no seu turno") {
        Aventureiro a("Herói", 200, 0, 10);
        Goblin g("Goblin", 1);
        int hpAntes = a.getHP();

        g.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Slime causa dano ao aventureiro no seu turno") {
        Aventureiro a("Herói", 200, 0, 10);
        Slime s("Slime", 1);
        int hpAntes = a.getHP();

        s.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Goblin recebe dano e HP diminui") {
        Goblin g("Goblin", 1);
        int hpAntes = g.getHP();

        g.receberDano(20,TipoHabilidade::FISICO);
        CHECK(g.getHP() < hpAntes);
    }

    TEST_CASE("Goblin morre com dano letal") {
        Goblin g("Goblin", 1);
        g.receberDano(9999,TipoHabilidade::FISICO);
        CHECK(g.estaVivo() == false);
    }
}

TEST_SUITE("Inimigos Comun - Escalonamento") {

    TEST_CASE("Goblin nível 5 é mais forte que nível 1") {
        Goblin g1("Goblin Fraco", 1);
        Goblin g5("Goblin Forte", 5);
        CHECK(g5.getHP() > g1.getHP());
        CHECK(g5.getXPRecompensa() > g1.getXPRecompensa());
    }
}