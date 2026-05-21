/**
 * @file test_Aventureiro.cpp
 * @brief Testes de unidade estruturais para a classe Aventureiro (Fase TDD Red).
 */

#include "doctest.h"
#include "Aventureiro.hpp"

// =========================================================
// SUITE: Estado Inicial
// =========================================================

TEST_SUITE("Aventureiro - Estado Inicial") {

    TEST_CASE("MP inicial - Valor Positivo") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getMP() > 0);
    }

    TEST_CASE("MP inicial - Igual ao Maximo") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getMP() == a.getMPMax());
    }

    TEST_CASE("Energia inicial - Valor Positivo") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getEnergia() > 0);
    }

    TEST_CASE("Energia inicial - Igual ao Maximo") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getEnergia() == a.getEnergiaMax());
    }

    TEST_CASE("Força total inicial - Sem bonus de arma") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getForcaTotal() == 15);
    }

    TEST_CASE("Defesa inicial - Sem bonus de armadura") {
        Aventureiro a("Herói", 100, 8, 15);
        CHECK(a.getDefesa() == 8);
    }

    TEST_CASE("Checkpoint inicial - Deve ser zero") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getIDCheckpoint() == 0);
    }
}

// =========================================================
// SUITE: Recursos de Combate
// =========================================================

TEST_SUITE("Aventureiro - Recursos de Combate") {

    TEST_CASE("consumirMP - Redução Correta") {
        Aventureiro a("Herói", 100, 5, 15);
        int mpInicial = a.getMP();
        a.consumirMP(3);
        CHECK(a.getMP() == mpInicial - 3);
    }

    TEST_CASE("consumirMP - Limite Inferior Zero") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirMP(9999);
        CHECK(a.getMP() == 0);
    }

    TEST_CASE("consumirEnergia - Redução Correta") {
        Aventureiro a("Herói", 100, 5, 15);
        int energiaInicial = a.getEnergia();
        a.consumirEnergia(4);
        CHECK(a.getEnergia() == energiaInicial - 4);
    }

    TEST_CASE("consumirEnergia - Limite Inferior Zero") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirEnergia(9999);
        CHECK(a.getEnergia() == 0);
    }

    TEST_CASE("recuperarRecursos - Incremento de MP") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirMP(3);
        int mpAntes = a.getMP();
        a.recuperarRecursos();
        CHECK(a.getMP() == mpAntes + 1);
    }

    TEST_CASE("recuperarRecursos - Incremento de Energia") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirEnergia(8);
        int energiaAntes = a.getEnergia();
        a.recuperarRecursos();
        CHECK(a.getEnergia() == energiaAntes + 5);
    }

    TEST_CASE("recuperarRecursos - Teto de MPMax") {
        Aventureiro a("Herói", 100, 5, 15);
        a.recuperarRecursos();
        CHECK(a.getMP() <= a.getMPMax());
    }

    TEST_CASE("recuperarRecursos - Teto de EnergiaMax") {
        Aventureiro a("Herói", 100, 5, 15);
        a.recuperarRecursos();
        CHECK(a.getEnergia() <= a.getEnergiaMax());
    }
}

// =========================================================
// SUITE: Escudo e Ações de Turno
// =========================================================

TEST_SUITE("Aventureiro - Escudo e Ações") {

    TEST_CASE("usarEscudo - Consumo de Recursos") {
        Aventureiro a("Herói", 100, 5, 15);
        int energiaAntes = a.getEnergia();
        a.usarEscudo();
        CHECK(a.getEnergia() == energiaAntes - 6);
    }

    TEST_CASE("receberDano e Escudo - Mitigação de HP") {
        Aventureiro a("Herói", 100, 5, 15);
        a.usarEscudo();
        int hpAntes = a.getHP();
        a.receberDano(30);
        CHECK(a.getHP() > hpAntes - 30);
    }

    TEST_CASE("Escudo - Comportamento de Desativação e Eficácia") {
        Aventureiro a1("Herói", 200, 5, 15);
        a1.usarEscudo();
        int hpAntes1 = a1.getHP();
        a1.receberDano(30);
        int danoComEscudo = hpAntes1 - a1.getHP();

        Aventureiro a2("Herói", 200, 5, 15);
        int hpAntes2 = a2.getHP();
        a2.receberDano(30);
        int danoSemEscudo = hpAntes2 - a2.getHP();

        // Valida as variáveis para sumir com os warnings e testar a mitigação
        CHECK(danoComEscudo < danoSemEscudo);
        CHECK(a1.getHP() > a2.getHP());
    }
}