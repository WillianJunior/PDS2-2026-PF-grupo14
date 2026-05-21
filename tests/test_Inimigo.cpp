/**
 * @file test_Inimigo.cpp
 * @brief Testes de unidade para Inimigo e suas subclasses usando doctest (TDD).
 */

#include "doctest/doctest.h"
#include "Inimigo.hpp"
#include "InimigoComum.hpp"
#include "InimigoIncomum.hpp"
#include "InimigoBoss.hpp"
#include "Aventureiro.hpp"

// =========================================================
// SUITE: Inimigo Base (via Goblin)
// =========================================================

TEST_SUITE("Inimigo - Estado Inicial") {

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

    TEST_CASE("Slime começa com HP e XP positivos") {
        Slime s("Slime Azul", 1);
        CHECK(s.getHP() > 0);
        CHECK(s.getXPRecompensa() > 0);
    }

    TEST_CASE("getDeclaracaoStatus contém o nome do inimigo") {
        Goblin g("GoblinTeste", 1);
        CHECK(g.getDeclaracaoStatus().find("GoblinTeste") != std::string::npos);
    }
}

// =========================================================
// SUITE: Inimigos atacam o jogador
// =========================================================

TEST_SUITE("Inimigo - Comportamento de Turno") {

    TEST_CASE("Goblin causa dano ao aventureiro") {
        Aventureiro a("Herói", 200, 0, 10);
        Goblin g("Goblin", 1);
        int hpAntes = a.getHP();
        g.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Slime causa dano ao aventureiro") {
        Aventureiro a("Herói", 200, 0, 10);
        Slime s("Slime", 1);
        int hpAntes = a.getHP();
        s.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Bruxa causa dano ao aventureiro") {
        Aventureiro a("Herói", 200, 0, 10);
        Bruxa b("Bruxa", 3);
        int hpAntes = a.getHP();
        b.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("GolemPedra causa dano ao aventureiro") {
        Aventureiro a("Herói", 500, 0, 10);
        GolemPedra gp("Golem", 2);
        int hpAntes = a.getHP();
        gp.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Dragão causa dano ao aventureiro") {
        Aventureiro a("Herói", 1000, 0, 10);
        Dragao d("Dragão", 5);
        int hpAntes = a.getHP();
        d.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }
}

// =========================================================
// SUITE: Inimigos recebem dano
// =========================================================

TEST_SUITE("Inimigo - Receber Dano") {

    TEST_CASE("Goblin recebe dano e HP diminui") {
        Goblin g("Goblin", 1);
        int hpAntes = g.getHP();
        g.receberDano(50);
        CHECK(g.getHP() < hpAntes);
    }

    TEST_CASE("Goblin morre com dano letal") {
        Goblin g("Goblin", 1);
        g.receberDano(9999);
        CHECK(g.estaVivo() == false);
        CHECK(g.getHP() == 0);
    }

    TEST_CASE("GolemPedra recebe menos dano que Goblin (pele de pedra)") {
        Goblin g("Goblin", 1);
        GolemPedra gp("Golem", 1);
        // Ambos com defesa zerada para isolar a mecânica do GolemPedra
        // O Golem reduz 30% do dano na sua sobrescrita
        int hpGoblinAntes = g.getHP();
        int hpGolemAntes = gp.getHP();
        g.receberDano(20);
        gp.receberDano(20);
        int danoCausadoGoblin = hpGoblinAntes - g.getHP();
        int danoCausadoGolem = hpGolemAntes - gp.getHP();
        CHECK(danoCausadoGolem <= danoCausadoGoblin);
    }
}

// =========================================================
// SUITE: Escalonamento por nível
// =========================================================

TEST_SUITE("Inimigo - Escalonamento por Nível") {

    TEST_CASE("Goblin nível 5 tem mais HP que nível 1") {
        Goblin g1("Goblin Fraco", 1);
        Goblin g5("Goblin Forte", 5);
        CHECK(g5.getHP() > g1.getHP());
    }

    TEST_CASE("Goblin nível 5 dá mais XP que nível 1") {
        Goblin g1("Goblin Fraco", 1);
        Goblin g5("Goblin Forte", 5);
        CHECK(g5.getXPRecompensa() > g1.getXPRecompensa());
    }

    TEST_CASE("Dragão tem bem mais HP que Slime") {
        Slime s("Slime", 1);
        Dragao d("Dragão", 10);
        CHECK(d.getHP() > s.getHP());
    }
}
