/**
 * @file test_MotorJogo.cpp
 * @brief Testes de unidade para a classe MotorJogo usando doctest (TDD).
 */

#include "doctest.h"
#include "MotorJogo.hpp"
#include "Aventureiro.hpp"

// =========================================================
// SUITE: MotorJogo - Inicialização e Ciclo Inicial
// =========================================================

TEST_SUITE("MotorJogo - Construção e Ciclo") {

    TEST_CASE("MotorJogo é construído sem erros") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK_NOTHROW(MotorJogo m(a));
    }

    TEST_CASE("Método rodar executa sem lançar exceções no esqueleto") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_NOTHROW(m.rodar());
    }
}

// =========================================================
// SUITE: MotorJogo - Fábrica de Combate (Stubs)
// =========================================================

TEST_SUITE("MotorJogo - Combate") {

    TEST_CASE("realizarCombate com Goblin retorna valor esperado no Red") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        bool resultado = m.realizarCombate("Goblin");
        CHECK(resultado == false); // Validação estrita do stub de retorno do esqueleto
    }

    TEST_CASE("realizarCombate com Slime retorna valor esperado no Red") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        bool resultado = m.realizarCombate("Slime");
        CHECK(resultado == false);
    }

    TEST_CASE("realizarCombate com Dragao retorna valor esperado no Red") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        bool resultado = m.realizarCombate("Dragao");
        CHECK(resultado == false);
    }
}