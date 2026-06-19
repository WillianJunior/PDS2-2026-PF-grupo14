/**
 * @file test_InimigoIncomum.cpp
 * @brief Testes de unidade para RecrutaDoutrinado, GuardaPatrimonial e SabotadorRival usando doctest (TDD).
 */

#include "doctest.h"
#include "InimigoIncomum.hpp"
#include "Aventureiro.hpp"
#include <stdexcept>

// =========================================================
// SUITE: RecrutaDoutrinado
// =========================================================

TEST_SUITE("RecrutaDoutrinado - Estado Inicial e Comportamento") {

    TEST_CASE("RecrutaDoutrinado inicia com status corretos") {
        RecrutaDoutrinado r("Recruta 1", 3);
        CHECK(r.estaVivo() == true);
        CHECK(r.getHP() == 165); // 55 * 3
        CHECK(r.getXPRecompensa() == 120); // 40 * 3
    }

    TEST_CASE("RecrutaDoutrinado usa nome padrão se enviado vazio") {
        RecrutaDoutrinado r("", 1);
        CHECK(r.getNome() == "Recruta Doutrinado");
    }

    TEST_CASE("RecrutaDoutrinado causa dano ao aventureiro") {
        Aventureiro a("Herói", 500, 0, 10);
        RecrutaDoutrinado r("Recruta 2", 3);
        int hpAntes = a.getHP();
        
        r.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("RecrutaDoutrinado morre com dano letal") {
        RecrutaDoutrinado r("Recruta", 2);
        r.receberDano(99999, TipoHabilidade::FISICO);
        CHECK(r.estaVivo() == false);
        CHECK(r.getHP() == 0);
    }
}

// =========================================================
// SUITE: GuardaPatrimonial
// =========================================================

TEST_SUITE("GuardaPatrimonial - Estado Inicial e Comportamento") {

    TEST_CASE("GuardaPatrimonial inicia com status corretos") {
        GuardaPatrimonial g("Guarda 1", 4);
        CHECK(g.estaVivo() == true);
        CHECK(g.getHP() == 260); // 65 * 4
        CHECK(g.getXPRecompensa() == 180); // 45 * 4
    }

    TEST_CASE("GuardaPatrimonial recebe dano e HP diminui") {
        GuardaPatrimonial g("Guarda 2", 2);
        int hpAntes = g.getHP();
        
        g.receberDano(30, TipoHabilidade::FISICO);
        CHECK(g.getHP() < hpAntes);
    }

    TEST_CASE("GuardaPatrimonial morre com dano letal") {
        GuardaPatrimonial g("Guarda", 1);
        g.receberDano(99999, TipoHabilidade::FISICO);
        CHECK(g.estaVivo() == false);
        CHECK(g.getHP() == 0);
    }
}

// =========================================================
// SUITE: SabotadorRival
// =========================================================

TEST_SUITE("SabotadorRival - Estado Inicial e Comportamento") {

    TEST_CASE("SabotadorRival inicia com status corretos") {
        SabotadorRival s("Sabotador 1", 2);
        CHECK(s.estaVivo() == true);
        CHECK(s.getHP() == 50); // 25 * 2 (frágil)
        CHECK(s.getXPRecompensa() == 120); // 60 * 2 (alta recompensa)
    }

    TEST_CASE("SabotadorRival causa alto dano ao aventureiro") {
        Aventureiro a("Herói", 500, 0, 10);
        SabotadorRival s("Sabotador 2", 2);
        int hpAntes = a.getHP();
        
        s.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }
}

// =========================================================
// SUITE: Proteções e Exceções Comuns
// =========================================================

TEST_SUITE("Inimigos Incomuns - Validações") {

    TEST_CASE("Lança exceção ao instanciar com nível inválido") {
        CHECK_THROWS_AS(RecrutaDoutrinado("Erro", 0), std::invalid_argument);
        CHECK_THROWS_AS(GuardaPatrimonial("Erro", -1), std::invalid_argument);
        CHECK_THROWS_AS(SabotadorRival("Erro", -10), std::invalid_argument);
    }
}