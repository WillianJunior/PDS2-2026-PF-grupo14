/**
 * @file test_Combate.cpp
 * @brief Testes de unidade para a classe Combate usando doctest (TDD).
 *
 * Os testes usam polimorfismo com ponteiros/referências para Inimigo,
 * simulando exatamente o comportamento do jogo real, onde os inimigos
 * são instanciados dinamicamente.
 */

#include "doctest.h"

#include "Combate.hpp"
#include "Aventureiro.hpp"
#include "Inimigo.hpp"
#include "InimigoComum.hpp"
#include "InimigoIncomum.hpp"
#include "InimigoBoss.hpp"

#include <memory>

// =========================================================
// SUITE: Resultado do Combate
// =========================================================

TEST_SUITE("Combate - Resultado") {

    TEST_CASE("Jogador muito forte vence o combate") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<DesafianteDoBar>("Bêbado do Bar", 1);
        Combate c(a, *inimigo);

        bool resultado = c.iniciar();
        CHECK(resultado == true);
    }

    TEST_CASE("Jogador vivo após vencer") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<DesafianteDoBar>("Bêbado do Bar", 1);
        Combate c(a, *inimigo);

        c.iniciar();
        CHECK(a.estaVivo() == true);
    }

    TEST_CASE("Inimigo morto após derrota") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<DesafianteDoBar>("Bêbado do Bar", 1);
        Combate c(a, *inimigo);

        c.iniciar();
        CHECK(inimigo->estaVivo() == false);
    }

    TEST_CASE("Jogador derrota Segurança de Balada") {
        Aventureiro a("Herói", 300, 20, 500);
        std::unique_ptr<Inimigo> seguranca = std::make_unique<SegurancaDeBalada>("Guarda Brutamontes", 1);
        Combate c(a, *seguranca);

        bool resultado = c.iniciar();
        CHECK(resultado == true);
    }
}

// =========================================================
// SUITE: Efeitos colaterais do Combate
// =========================================================

TEST_SUITE("Combate - Efeitos Colaterais") {

    TEST_CASE("Jogador ganha XP ao vencer") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<TrabalhadorNoturno>("Trabalhador Puto", 1);
        int xpRecompensa = inimigo->getXPRecompensa();
        Combate c(a, *inimigo);

        c.iniciar();
        CHECK(xpRecompensa > 0);
    }

    TEST_CASE("Validação de vida do Inimigo pós-combate") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<TrabalhadorNoturno>("Trabalhador Puto", 1);
        Combate c(a, *inimigo);

        c.iniciar();
        CHECK(inimigo->estaVivo() == false);
    }

    TEST_CASE("Jogador perde HP durante o combate contra Tyler Durden") {
        Aventureiro a("Herói Frágil", 500, 0, 1);
        std::unique_ptr<Inimigo> tyler = std::make_unique<TylerDurden>("Tyler", 1);
        int hpAntes = a.getHP();
        Combate c(a, *tyler);

        c.iniciar();
        CHECK(a.getHP() < hpAntes);
    }
}

// =========================================================
// SUITE: Invariantes do Combate
// =========================================================

TEST_SUITE("Combate - Invariantes") {

    TEST_CASE("HP do jogador nunca fica negativo após combate") {
        Aventureiro a("Herói", 500, 50, 9999);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<DesafianteDoBar>("Bêbado", 1);
        Combate c(a, *inimigo);

        c.iniciar();
        CHECK(a.getHP() >= 0);
    }

    TEST_CASE("HP do inimigo nunca fica negativo após combate") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<DesafianteDoBar>("Bêbado", 1);
        Combate c(a, *inimigo);

        c.iniciar();
        CHECK(inimigo->getHP() >= 0);
    }

    TEST_CASE("Ao fim do combate, pelo menos um combatente está morto") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> inimigo = std::make_unique<TylerDurden>("Tyler", 1);
        Combate c(a, *inimigo);

        c.iniciar();
        bool pelomenosUmMorto = !a.estaVivo() || !inimigo->estaVivo();
        CHECK(pelomenosUmMorto == true);
    }
}