/**
 * @file test_InimigoBoss.cpp
 * @brief Testes de unidade para a classe TylerDurden usando doctest (TDD).
 */

#include "doctest.h"
#include "InimigoBoss.hpp"
#include "Aventureiro.hpp"
#include <stdexcept>

TEST_SUITE("TylerDurden - Estado Inicial") {

    TEST_CASE("TylerDurden é criado com nome correto") {
        TylerDurden t("Tyler Projetado", 2);
        CHECK(t.getNome() == "Tyler Projetado");
    }

    TEST_CASE("TylerDurden usa nome padrão se enviado vazio") {
        TylerDurden t("", 1);
        CHECK(t.getNome() == "Tyler Durden");
    }

    TEST_CASE("TylerDurden começa vivo") {
        TylerDurden t("Tyler", 1);
        CHECK(t.estaVivo() == true);
    }

    TEST_CASE("TylerDurden tem HP baseado no nível") {
        TylerDurden t("Tyler", 1); // 150 * 1
        CHECK(t.getHP() == 150);
        
        TylerDurden t2("Tyler Lvl 2", 2); // 150 * 2
        CHECK(t2.getHP() == 300);
    }

    TEST_CASE("TylerDurden tem XP de recompensa proporcional") {
        TylerDurden t("Tyler", 1); // 200 * 1
        CHECK(t.getXPRecompensa() == 200);
    }

    TEST_CASE("TylerDurden lança exceção para nível inválido") {
        CHECK_THROWS_AS(TylerDurden("Tyler", 0), std::invalid_argument);
        CHECK_THROWS_AS(TylerDurden("Tyler", -5), std::invalid_argument);
    }
}

TEST_SUITE("TylerDurden - Comportamento") {

    TEST_CASE("TylerDurden causa dano ao aventureiro") {
        Aventureiro a("Herói", 1000, 0, 10);
        TylerDurden t("Tyler", 1);
        int hpAntes = a.getHP();
        
        t.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("TylerDurden recebe dano e HP diminui") {
        TylerDurden t("Tyler", 1);
        int hpAntes = t.getHP();
        
        t.receberDano(30, TipoHabilidade::FISICO);
        CHECK(t.getHP() < hpAntes);
    }

    TEST_CASE("TylerDurden morre com dano letal - Status") {
        TylerDurden t("Tyler", 1);
        t.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(t.estaVivo() == false);
    }

    TEST_CASE("TylerDurden morre com dano letal - Zerar HP") {
        TylerDurden t("Tyler", 1);
        t.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(t.getHP() == 0);
    }
}