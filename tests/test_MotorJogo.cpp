/**
 * @file test_MotorJogo.cpp
 * @brief Testes de unidade para MotorJogo — cobertura máxima e estrutural.
 */

#include "doctest.h"
#include "MotorJogo.hpp"
#include "Aventureiro.hpp"
#include <stdexcept>
#include <string>

// =========================================================
// SUITE 1: Construção e Efeitos de Inicialização
// =========================================================
TEST_SUITE("MotorJogo - Construção") {

    TEST_CASE("Construído sem erros com aventureiro válido") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK_NOTHROW(MotorJogo m(a));
    }

    TEST_CASE("Construído sem erros com stats altos") {
        Aventureiro a("Deus", 9999, 999, 999);
        CHECK_NOTHROW(MotorJogo m(a));
    }

    TEST_CASE("Checkpoint do aventureiro não é alterado pela construção") {
        Aventureiro a("Herói", 100, 5, 15);
        a.setIDCheckpoint(2);
        MotorJogo m(a);
        CHECK(a.getIDCheckpoint() == 2);
    }

    TEST_CASE("Efeito colateral da inicialização da história no player (Cena 3)") {
        Aventureiro a("Herói", 100, 5, 15);
        int forcaAntes = a.getForcaTotal();
        
        // O construtor chama inicializarHistoria() que aplica um buff na arma do player
        MotorJogo m(a);
        CHECK(a.getForcaTotal() > forcaAntes);
    }
}

// =========================================================
// SUITE 2: realizarCombate e Validação de Inimigos
// Cobre as validações e ramificações de erro de criarInimigo
// =========================================================
TEST_SUITE("MotorJogo - Validação de Inimigos via realizarCombate") {

    TEST_CASE("String vazia lança std::invalid_argument") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_THROWS_AS(m.realizarCombate(""), std::invalid_argument);
    }

    TEST_CASE("Tipos inexistentes ou obsoletos lançam std::invalid_argument") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);

        CHECK_THROWS_AS(m.realizarCombate("Goblin"), std::invalid_argument);
        CHECK_THROWS_AS(m.realizarCombate("Slime"), std::invalid_argument);
        CHECK_THROWS_AS(m.realizarCombate("Dragao"), std::invalid_argument);
        CHECK_THROWS_AS(m.realizarCombate("InimigoInexistente"), std::invalid_argument);
        CHECK_THROWS_AS(m.realizarCombate("desafiantedoBar"), std::invalid_argument);
    }
}

// =========================================================
// SUITE 3: Checkpoint e Fluxo de Derrota
// =========================================================
TEST_SUITE("MotorJogo - Checkpoint e Estado de Recursos") {

    TEST_CASE("Checkpoint pode ser atualizado múltiplas vezes") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        a.setIDCheckpoint(1);
        a.setIDCheckpoint(3);
        CHECK(a.getIDCheckpoint() == 3);
    }

    TEST_CASE("dormir() restaura completamente os atributosVitais") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirMP(10);
        a.consumirEnergia(10);
        
        a.dormir();
        CHECK(a.getMP() == a.getMPMax());
        CHECK(a.getEnergia() == a.getEnergiaMax());
    }
}

// =========================================================
// SUITE 4: Casos de Borda e Erros do Sistema
// =========================================================
TEST_SUITE("MotorJogo - Casos de Borda") {

    TEST_CASE("Múltiplas instâncias independentes de MotorJogo") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK_NOTHROW(MotorJogo m1(a));
        CHECK_NOTHROW(MotorJogo m2(a));
    }

    TEST_CASE("Aventureiro com estatísticas mínimas") {
        Aventureiro a("Mínimo", 1, 1, 1);
        CHECK_NOTHROW(MotorJogo m(a));
    }
}