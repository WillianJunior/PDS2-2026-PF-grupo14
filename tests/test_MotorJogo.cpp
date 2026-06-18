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

    TEST_CASE("Método rodar execulta após a inicialização") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_NOTHROW(m.rodar());
    }
}
TEST_SUITE("MotorJogo - Inicialização da História") {

    TEST_CASE("O ciclo de jogo (rodar) funciona perfeitamente após a história ser inicializada") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_NOTHROW(m.rodar());
    }
}
TEST_SUITE("MotorJogo - Cobertura Máxima de Fluxo") {

    TEST_CASE("Caminho de Cobertura 1: Transição Direta e Sala Segura (Checkpoint)") {
        Aventureiro a("Explorador", 100, 10, 10);
        MotorJogo m(a);
        a.setIDCheckpoint(3);

        CHECK_NOTHROW(m.rodar());
        CHECK(a.getIDCheckpoint() == 3);
    }
    TEST_CASE("Caminho de Cobertura 2: Simulação de Fim de Jogo Direto") {
        Aventureiro a("Destemido", 100, 10, 10);
        MotorJogo m(a);
        a.setIDCheckpoint(4);

        CHECK_NOTHROW(m.rodar());
    }
    TEST_CASE("Caminho de Cobertura 3: Resiliência a IDs Corrompidos fora da História") {
        Aventureiro a("Infiltrado", 100, 10, 10);
        MotorJogo m(a);
        a.setIDCheckpoint(999);

        CHECK_THROWS(m.rodar());
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
    
    TEST_CASE("Validação de combate com inimigo inexistente/vazio") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        
        // Testa como o método reage recebendo uma string vazia (deve retornar false por padrão no stub)
        bool resultado = m.realizarCombate("");
        CHECK(resultado == false);
    }
}
// =========================================================
// SUITE: MotorJogo - Cenários de Estado e Casos de Borda
// =========================================================

TEST_SUITE("MotorJogo - Estados do Jogo e Casos Críticos") {

    TEST_CASE("MotorJogo se comporta corretamente se o Aventureiro inicia debilitado") {
        Aventureiro a("Herói Caído", 0, 0, 0);
        MotorJogo m(a);
        
        CHECK_NOTHROW(m.rodar());
    }

    //TEST_CASE("Fluxo completo de sub-métodos de suporte ao ciclo de jogo") {
    //    Aventureiro a("Guerreiro", 120, 10, 12);
    //    MotorJogo m(a);
    //    CHECK_NOTHROW(m.eventosEspeciais());
    //    CHECK_NOTHROW(m.processarDerrota());
    //}
}