/**
 * @file test_Habilidade.cpp
 * @brief Testes de unidade estruturais de alta cobertura para Habilidade e Inimigo.
 */

#include "doctest.h"
#include "Habilidade.hpp"
#include "Inimigo.hpp"
#include "InimigoComum.hpp" // Dependência para instanciar subtipos de Inimigo se necessário
#include <stdexcept>
#include <string>

// =========================================================
// SUITE 1: EXTENSÃO - CLASSE INIMIGO (BASE POO)
// =========================================================
TEST_SUITE("Inimigo - Métodos da Classe Base") {

  TEST_CASE("Construção e Inicialização de Recompensa") {
    DesafianteDoBar inimigo("Desafiante do Bar", 1);            
    
    CHECK(inimigo.getNome() == "Desafiante do Bar"); // Ajustado
    CHECK(inimigo.getXPRecompensa() == 60);         // Ajustado
    CHECK(inimigo.getHP() == 150);                  // Ajustado
    CHECK(inimigo.estaVivo());
}
}

// =========================================================
// SUITE 2: CONSTRUÇÃO, GETTERS E ESTADO INICIAL
// =========================================================
TEST_SUITE("Habilidade - Construção e Atributos") {

    TEST_CASE("Instanciação Padrão - Armazenamento de Dados Básicos") {
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
        Habilidade h("Veneno", TipoHabilidade::DOT, 2, 0, 5, "", 3);
        
        CHECK(h.getDuracaoEfeito() == 3);
        CHECK(h.getTipo() == TipoHabilidade::DOT);
    }

    TEST_CASE("Instanciação de Modificadores - Armazenamento de Atributo Alvo") {
        Habilidade h("Grito de Guerra", TipoHabilidade::SUPORTE, 4, 0, 10, "forca", 3);
        
        CHECK(h.getTipo() == TipoHabilidade::SUPORTE);
        CHECK(h.getValorBase() == 10);
        CHECK(h.getAtributoAlvo() == "forca");
    }
}

// =========================================================
// SUITE 3: TRAVAS DE SEGURANÇA (ROBUSTEZ E EXCEÇÕES)
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
// SUITE 4: LÓGICA DE NEGÓCIO (EFEITOS TEMPORAIS)
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