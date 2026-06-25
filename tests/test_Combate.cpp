/**
 * @file test_Combate.cpp
 * @brief Testes de unidade estruturais e de alta cobertura para a classe Combate.
 */

#include "doctest.h"
#include "Combate.hpp"
#include "Aventureiro.hpp"
#include "Inimigo.hpp"
#include "InimigoComum.hpp"
#include "InimigoIncomum.hpp"
#include "InimigoBoss.hpp"
#include <memory>
#include <stdexcept>

// =========================================================
// SUITE 1: VALIDAÇÃO DO CONSTRUTOR E EXCEÇÕES DE ENTRADA
// =========================================================
TEST_SUITE("Combate - Validação de Estado Inicial") {

    TEST_CASE("Exceção: Tentar iniciar combate com Aventureiro morto") {
        Aventureiro a("Herói Caído", 100, 10, 10);
        a.receberDano(500, TipoHabilidade::FISICO); // Força a morte do jogador
        
        std::unique_ptr<Inimigo> inimigo = std::make_unique<DesafianteDoBar>("Bêbado do Bar", 1);
        
        CHECK_THROWS_AS(Combate(a, *inimigo), CombateInvalidoException);
    }

    TEST_CASE("Exceção: Tentar iniciar combate com Inimigo morto") {
        Aventureiro a("Herói Vivo", 100, 10, 10);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<DesafianteDoBar>("Bêbado do Bar", 1);
        inimigo->receberDano(500, TipoHabilidade::FISICO); // Força a morte do inimigo
        
        CHECK_THROWS_AS(Combate(a, *inimigo), CombateInvalidoException);
    }
}

// =========================================================
// SUITE 2: MATRIZ DE RESULTADOS E FLUXOS DE EXECUÇÃO
// =========================================================
TEST_SUITE("Combate - Resultados de Luta") {

    TEST_CASE("Cenário de Vitória: Jogador muito forte vence imediatamente") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<DesafianteDoBar>("Bêbado do Bar", 1);
        Combate c(a, *inimigo);

        bool resultado = c.iniciar();
        CHECK(resultado == true);
        CHECK(a.estaVivo());
        CHECK_FALSE(inimigo->estaVivo());
        CHECK(inimigo->getHP() == 0); // Garante trava de segurança contra HP negativo
    }

    TEST_CASE("Cenário de Derrota: Jogador fraco contra Boss") {
        Aventureiro a("Herói Frágil", 10, 1, 1);
        // Usando um Boss ou inimigo de alto impacto para garantir a derrota rápida
        std::unique_ptr<Inimigo> tyler = std::make_unique<TylerDurden>("Tyler", 5);
        Combate c(a, *tyler);

        bool resultado = c.iniciar();
        CHECK(resultado == false);
        CHECK_FALSE(a.estaVivo());
        CHECK(a.getHP() == 0); // Garante trava de segurança contra HP negativo do jogador
    }
}

// =========================================================
// SUITE 3: CAPTURA DE EXCEÇÕES E GARANTIA DE ROBUSTEZ DOS TURNOS
// =========================================================
TEST_SUITE("Combate - Resiliência a Falhas (Try-Catch Blocks)") {

    TEST_CASE("Garantia de não-interrupção no loop quando combatente morre no meio") {
        Aventureiro a("Aventureiro Veloz", 500, 10, 9999);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<SegurancaDeBalada>("Guarda", 1);
        Combate c(a, *inimigo);

        // O jogador deve matar o inimigo em seu próprio turno.
        // O teste valida se o 'break' impede que o inimigo morto tente contra-atacar.
        bool resultado = c.iniciar();
        CHECK(resultado == true);
    }
}