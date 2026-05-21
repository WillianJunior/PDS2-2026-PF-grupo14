/**
 * @file test_Habilidade.cpp
 * @brief Testes de unidade para a classe Habilidade usando doctest (TDD).
 *
 * Conforme a abordagem TDD, estes testes foram escritos antes ou junto
 * às implementações, descrevendo o comportamento esperado de cada método.
 */

#include "doctest/doctest.h"
#include "Habilidade.hpp"

// =========================================================
// SUITE: Construção e Getters
// =========================================================

TEST_SUITE("Habilidade - Construção") {

    TEST_CASE("Habilidade armazena nome corretamente") {
        Habilidade h("Bola de Fogo", TipoHabilidade::ESPECIAL, 0, 5, 20);
        CHECK(h.getNome() == "Bola de Fogo");
    }

    TEST_CASE("Habilidade armazena tipo corretamente") {
        Habilidade h("Golpe Pesado", TipoHabilidade::FISICO, 4, 0, 15);
        CHECK(h.getTipo() == TipoHabilidade::FISICO);
    }

    TEST_CASE("Habilidade armazena custo de Energia corretamente") {
        Habilidade h("Golpe Veloz", TipoHabilidade::FISICO, 3, 0, 10);
        CHECK(h.getCustoEnergia() == 3);
    }

    TEST_CASE("Habilidade armazena custo de MP corretamente") {
        Habilidade h("Cura Menor", TipoHabilidade::CURA, 0, 3, 15);
        CHECK(h.getCustoMP() == 3);
    }

    TEST_CASE("Habilidade armazena valor base corretamente") {
        Habilidade h("Raio", TipoHabilidade::ESPECIAL, 0, 8, 30);
        CHECK(h.getValorBase() == 30);
    }

    TEST_CASE("Duração padrão é zero quando não informada") {
        Habilidade h("Ataque Simples", TipoHabilidade::FISICO, 3, 0, 10);
        CHECK(h.getDuracao() == 0);
    }

    TEST_CASE("Duração é armazenada quando informada") {
        Habilidade h("Veneno", TipoHabilidade::DOT, 2, 0, 5, 3);
        CHECK(h.getDuracao() == 3);
    }
}

// =========================================================
// SUITE: Efeito Contínuo
// =========================================================

TEST_SUITE("Habilidade - Efeito Contínuo") {

    TEST_CASE("Habilidade instantânea NÃO é efeito contínuo") {
        Habilidade h("Soco", TipoHabilidade::FISICO, 3, 0, 10, 0);
        CHECK_FALSE(h.ehEfeitoContinuo());
    }

    TEST_CASE("Habilidade com duração > 0 É efeito contínuo") {
        Habilidade h("Sangramento", TipoHabilidade::DOT, 2, 0, 5, 3);
        CHECK(h.ehEfeitoContinuo());
    }

    TEST_CASE("HOT (cura por turno) é efeito contínuo") {
        Habilidade h("Regenerar", TipoHabilidade::HOT, 0, 4, 8, 4);
        CHECK(h.ehEfeitoContinuo());
    }

    TEST_CASE("DEBUFF com duração é efeito contínuo") {
        Habilidade h("Maldição", TipoHabilidade::DEBUFF, 0, 5, 0, 2);
        CHECK(h.ehEfeitoContinuo());
    }

    TEST_CASE("Habilidade tipo ULTRA sem duração NÃO é efeito contínuo") {
        Habilidade h("Golpe Final", TipoHabilidade::ULTRA, 10, 10, 100, 0);
        CHECK_FALSE(h.ehEfeitoContinuo());
    }
}

// =========================================================
// SUITE: Tipos de Habilidade
// =========================================================

TEST_SUITE("Habilidade - Tipos") {

    TEST_CASE("Tipo CURA é armazenado corretamente") {
        Habilidade h("Cura", TipoHabilidade::CURA, 0, 5, 20);
        CHECK(h.getTipo() == TipoHabilidade::CURA);
    }

    TEST_CASE("Tipo ULTRA é armazenado corretamente") {
        Habilidade h("Apocalipse", TipoHabilidade::ULTRA, 10, 10, 999);
        CHECK(h.getTipo() == TipoHabilidade::ULTRA);
    }

    TEST_CASE("Tipo SUPORTE é armazenado corretamente") {
        Habilidade h("Aura Protetora", TipoHabilidade::SUPORTE, 4, 0, 0, 3);
        CHECK(h.getTipo() == TipoHabilidade::SUPORTE);
    }
}
