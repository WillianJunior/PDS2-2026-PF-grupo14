/**
 * @file test_Habilidade.cpp
 * @brief Testes de unidade estruturais e comportamentais para a classe Habilidade.
 */

#include "doctest.h"
#include "Habilidade.hpp"
#include <stdexcept>

// =========================================================
// SUITE: Construção, Getters e Estado Inicial
// =========================================================

TEST_SUITE("Habilidade - Construção e Atributos") {

    TEST_CASE("Instanciação Padrão - Armazenamento de Dados Básicos") {
        // Corrigido para a ordem: ..., valorBase, atributoAfetado, duracao
        Habilidade h("Bola de Fogo", TipoHabilidade::ESPECIAL, 0, 5, 20, "", 0);
        
        CHECK(h.getNome() == "Bola de Fogo");
        CHECK(h.getTipo() == TipoHabilidade::ESPECIAL);
        CHECK(h.getCustoEnergia() == 0);
        CHECK(h.getCustoMP() == 5);
        CHECK(h.getValorBase() == 20);
        CHECK(h.getDuracaoEfeito() == 0);
        CHECK(h.getAtributoAlvo().empty());
    }

    TEST_CASE("Instanciação com Efeito Temporal - Armazenamento de Duração") {
        // Ordem corrigida: ..., valorBase, atributoAfetado, duracao
        Habilidade h("Veneno", TipoHabilidade::DOT, 2, 0, 5, "", 3);
        
        CHECK(h.getDuracaoEfeito() == 3);
        CHECK(h.getTipo() == TipoHabilidade::DOT);
    }

    TEST_CASE("Instanciação de Modificadores - Armazenamento de Atributo Alvo") {
        // Ordem corrigida: ..., valorBase, atributoAfetado, duracao
        Habilidade h("Grito de Guerra", TipoHabilidade::SUPORTE, 4, 0, 10, "forca", 3);
        
        CHECK(h.getTipo() == TipoHabilidade::SUPORTE);
        CHECK(h.getValorBase() == 10);
        CHECK(h.getAtributoAlvo() == "forca");
    }
}

// =========================================================
// SUITE: Travas de Segurança (Robustez e Exceções)
// =========================================================

TEST_SUITE("Habilidade - Validações e Exceções") {

    TEST_CASE("Validação - Rejeitar Nome Vazio") {
        CHECK_THROWS_AS(
            Habilidade("", TipoHabilidade::FISICO, 3, 0, 10, "", 0),
            std::invalid_argument
        );
    }

    TEST_CASE("Validação - Rejeitar Custos Negativos") {
        // Energia negativa
        CHECK_THROWS_AS(
            Habilidade("Soco", TipoHabilidade::FISICO, -1, 0, 10, "", 0),
            std::invalid_argument
        );

        // Mana (MP) negativa
        CHECK_THROWS_AS(
            Habilidade("Centelha", TipoHabilidade::ESPECIAL, 0, -5, 10, "", 0),
            std::invalid_argument
        );
    }

    TEST_CASE("Validação - Rejeitar Valor Base Negativo") {
        CHECK_THROWS_AS(
            Habilidade("Dano Invalido", TipoHabilidade::FISICO, 2, 0, -5, "", 0),
            std::invalid_argument
        );
    }

    TEST_CASE("Validação - Exigir Duração Maior que Zero para Efeitos Contínuos") {
        // DoT com duração zero deve falhar
        CHECK_THROWS_AS(
            Habilidade("Sangramento Falho", TipoHabilidade::DOT, 2, 0, 5, "", 0),
            std::invalid_argument
        );

        // Debuff com duração zero deve falhar
        CHECK_THROWS_AS(
            Habilidade("Praga", TipoHabilidade::DEBUFF, 0, 4, 3, "defesa", 0),
            std::invalid_argument
        );
    }
}

// =========================================================
// SUITE: Lógica de Negócio (Efeitos Temporais)
// =========================================================

TEST_SUITE("Habilidade - Identificação de Efeito Contínuo") {

    TEST_CASE("Habilidades Instantâneas NÃO são efeitos contínuos") {
        Habilidade h1("Golpe Pesado", TipoHabilidade::FISICO, 4, 0, 15, "", 0);
        Habilidade h2("Cura Imediata", TipoHabilidade::CURA, 0, 6, 25, "", 0);
        Habilidade h3("Explosão Ultra", TipoHabilidade::ULTRA, 10, 10, 100, "", 0);

        CHECK_FALSE(h1.ehEfeitoContinuo());
        CHECK_FALSE(h2.ehEfeitoContinuo());
        CHECK_FALSE(h3.ehEfeitoContinuo());
    }

    TEST_CASE("Habilidades Temporais (DoT, HoT, Buff, Debuff) SÃO efeitos contínuos") {
        Habilidade dot("Incêndio", TipoHabilidade::DOT, 0, 8, 12, "", 3);
        Habilidade hot("Regenerar", TipoHabilidade::HOT, 0, 4, 8, "", 4);
        Habilidade buff("Bênção", TipoHabilidade::SUPORTE, 2, 0, 5, "defesa", 3);
        Habilidade debuff("Quebrar Guarda", TipoHabilidade::DEBUFF, 3, 0, 5, "defesa", 2);

        CHECK(dot.ehEfeitoContinuo());
        CHECK(hot.ehEfeitoContinuo());
        CHECK(buff.ehEfeitoContinuo());
        CHECK(debuff.ehEfeitoContinuo());
    }
}