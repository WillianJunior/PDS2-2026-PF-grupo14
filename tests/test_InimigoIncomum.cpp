/**
 * @file test_InimigoIncomum.cpp
 * @brief Testes de unidade estruturais e de cobertura total para inimigos incomuns.
 */

#include "doctest.h"
#include "InimigoIncomum.hpp"
#include "Aventureiro.hpp"
#include <stdexcept>
#include <string>

// =========================================================
// SUITE 1: RECRUTA DOUTRINADO
// =========================================================
TEST_SUITE("Inimigos Incomuns - Recruta Doutrinado") {

    TEST_CASE("RecrutaDoutrinado rejeita nivel invalido") {
        CHECK_THROWS_AS(RecrutaDoutrinado("Teste", 0), std::invalid_argument);
        CHECK_THROWS_AS(RecrutaDoutrinado("Teste", -1), std::invalid_argument);
    }

    TEST_CASE("RecrutaDoutrinado usa nome padrao") {
        RecrutaDoutrinado inimigo("", 1);
        CHECK(inimigo.getNome() == "Recruta Doutrinado");
    }

    TEST_CASE("RecrutaDoutrinado - Cobertura de Turnos e Métodos de Status") {
        RecrutaDoutrinado inimigo("Recruta Teste", 1);
        Aventureiro alvo("Heroi", 500, 1, 10);

        // Turno 1: Padrão
        inimigo.executarTurno(alvo);

        // Turno 2: Módulo 2 (Sangramento)
        inimigo.executarTurno(alvo);

        // Turno 3: Padrão
        inimigo.executarTurno(alvo);

        // Turno 4: Módulo 4 (Grito de Doutrina)
        inimigo.executarTurno(alvo);

        // Turno 5: Módulo 5 (Intimidação)
        int defesaAntes = alvo.getDefesa();
        inimigo.executarTurno(alvo);
        CHECK(alvo.getDefesa() < defesaAntes);

        // Cobertura do método virtual de status
        std::string status = inimigo.getDeclaracaoStatus();
        CHECK(status.find("Recruta") != std::string::npos);
    }
}

// =========================================================
// SUITE 2: GUARDA PATRIMONIAL
// =========================================================
TEST_SUITE("Inimigos Incomuns - Guarda Patrimonial") {

    TEST_CASE("GuardaPatrimonial rejeita nivel invalido") {
        CHECK_THROWS_AS(GuardaPatrimonial("Teste", 0), std::invalid_argument);
    }

    TEST_CASE("GuardaPatrimonial usa nome padrao") {
        GuardaPatrimonial inimigo("", 1);
        CHECK(inimigo.getNome() == "Guarda Patrimonial");
    }

    TEST_CASE("GuardaPatrimonial - Cobertura de Toda a Árvore de Decisão de Turnos") {
        GuardaPatrimonial inimigo("Guarda Teste", 1);
        Aventureiro alvo("Heroi", 500, 1, 10);

        // Turno 1: Padrão
        inimigo.executarTurno(alvo);

        // Turno 2: Padrão
        inimigo.executarTurno(alvo);

        // Turno 3: Módulo 3 (Atordoamento)
        inimigo.executarTurno(alvo);

        // Turno 4: Módulo 4 (Gás Lacrimogêneo)
        inimigo.executarTurno(alvo);

        // Turno 5: Módulo 5 (Bandagem de Campo)
        inimigo.executarTurno(alvo);

        // Cobertura do método virtual de status
        std::string status = inimigo.getDeclaracaoStatus();
        CHECK(status.find("Guarda") != std::string::npos);
    }
}

// =========================================================
// SUITE 3: SABOTADOR RIVAL
// =========================================================
TEST_SUITE("Inimigos Incomuns - Sabotador Rival") {

    TEST_CASE("SabotadorRival rejeita nivel invalido") {
        CHECK_THROWS_AS(SabotadorRival("Teste", 0), std::invalid_argument);
    }

    TEST_CASE("SabotadorRival usa nome padrao") {
        SabotadorRival inimigo("", 1);
        CHECK(inimigo.getNome() == "Sabotador Rival");
    }

    TEST_CASE("SabotadorRival - Cobertura de Turnos e Explosão Total") {
        SabotadorRival inimigo("Sabotador Teste", 1);
        Aventureiro alvo("Heroi", 500, 1, 10);

        // Turno 1: Padrão (Molotov)
        inimigo.executarTurno(alvo);

        // Turno 2: Padrão (Molotov)
        inimigo.executarTurno(alvo);

        // Turno 3: Módulo 3 (Gás Paralisante + Molotov)
        inimigo.executarTurno(alvo);

        // Turno 4: Padrão (Molotov)
        inimigo.executarTurno(alvo);

        // Turno 5: Módulo 5 (Explosão Total / ULTRA)
        int hpAntes = alvo.getHP();
        inimigo.executarTurno(alvo);
        CHECK(alvo.getHP() < hpAntes);

        // Cobertura do método virtual de status
        std::string status = inimigo.getDeclaracaoStatus();
        CHECK(status.find("Sabotador") != std::string::npos);
    }
}

// =========================================================
// SUITE 4: REGRAS GERAIS E INTERRUPÇÕES
// =========================================================
TEST_SUITE("Inimigos Incomuns - Fluxos Globais") {

    TEST_CASE("Inimigos incomuns nao atacam alvo morto") {
        RecrutaDoutrinado inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        alvo.receberDano(1000, TipoHabilidade::ULTRA);
        int hpFinal = alvo.getHP();

        inimigo.executarTurno(alvo);
        CHECK(alvo.getHP() == hpFinal);
    }

    TEST_CASE("Todos os inimigos incomuns iniciam vivos e validos") {
        RecrutaDoutrinado r("R", 1);
        GuardaPatrimonial g("G", 1);
        SabotadorRival s("S", 1);

        CHECK(r.estaVivo());
        CHECK(g.estaVivo());
        CHECK(s.estaVivo());
    }
}