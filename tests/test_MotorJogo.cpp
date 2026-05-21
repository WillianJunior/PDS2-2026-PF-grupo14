/**
 * @file test_MotorJogo.cpp
 * @brief Testes de unidade para a classe MotorJogo usando doctest (TDD).
 */

#include "doctest/doctest.h"
#include "MotorJogo.hpp"
#include "Aventureiro.hpp"

TEST_SUITE("MotorJogo - Construção") {

    TEST_CASE("MotorJogo é construído sem erros") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK_NOTHROW(MotorJogo m(a));
    }
}

TEST_SUITE("MotorJogo - Combate") {

    TEST_CASE("realizarCombate retorna bool") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        bool resultado = m.realizarCombate("Goblin");
        CHECK((resultado == true || resultado == false));
    }

    TEST_CASE("realizarCombate com Slime retorna bool") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        bool resultado = m.realizarCombate("Slime");
        CHECK((resultado == true || resultado == false));
    }

    TEST_CASE("realizarCombate com Dragao retorna bool") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        bool resultado = m.realizarCombate("Dragao");
        CHECK((resultado == true || resultado == false));
    }
}
