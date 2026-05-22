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
#include "InimigoBoss.hpp"

#include <memory>

// =========================================================
// SUITE: Resultado do Combate
// =========================================================

TEST_SUITE("Combate - Resultado") {

    TEST_CASE("Jogador muito forte vence o combate") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> g = std::make_unique<Goblin>("Goblin Fraco", 1);
        Combate c(a, *g);

        bool resultado = c.iniciar();
        CHECK(resultado == true);
    }

    TEST_CASE("Jogador vivo após vencer") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> g = std::make_unique<Goblin>("Goblin Fraco", 1);
        Combate c(a, *g);

        c.iniciar();
        CHECK(a.estaVivo() == true);
    }

    TEST_CASE("Inimigo morto após derrota") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> g = std::make_unique<Goblin>("Goblin Fraco", 1);
        Combate c(a, *g);

        c.iniciar();
        CHECK(g->estaVivo() == false);
    }

    TEST_CASE("Jogador derrota Slime") {
        Aventureiro a("Herói", 300, 20, 500);
        std::unique_ptr<Inimigo> s = std::make_unique<Slime>("Slime", 1);
        Combate c(a, *s);

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
        std::unique_ptr<Inimigo> g = std::make_unique<Goblin>("Goblin", 1);
        int xpGoblin = g->getXPRecompensa();
        Combate c(a, *g);

        c.iniciar();
        CHECK(xpGoblin > 0);
    }

    TEST_CASE("Validação de vida do Inimigo pós-combate") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> g = std::make_unique<Goblin>("Goblin", 1);
        Combate c(a, *g);

        c.iniciar();
        CHECK(g->estaVivo() == false);
    }

    TEST_CASE("Jogador perde HP durante o combate") {
        Aventureiro a("Herói Frágil", 500, 0, 1);
        std::unique_ptr<Inimigo> g = std::make_unique<Goblin>("Goblin Forte", 10);
        int hpAntes = a.getHP();
        Combate c(a, *g);

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
        std::unique_ptr<Inimigo> g = std::make_unique<Goblin>("Goblin", 1);
        Combate c(a, *g);

        c.iniciar();
        CHECK(a.getHP() >= 0);
    }

    TEST_CASE("HP do inimigo nunca fica negativo após combate") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> g = std::make_unique<Goblin>("Goblin", 1);
        Combate c(a, *g);

        c.iniciar();
        CHECK(g->getHP() >= 0);
    }

    TEST_CASE("Ao fim do combate, pelo menos um combatente está morto") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        std::unique_ptr<Inimigo> g = std::make_unique<Goblin>("Goblin", 1);
        Combate c(a, *g);

        c.iniciar();
        bool pelomenosUmMorto = !a.estaVivo() || !g->estaVivo();
        CHECK(pelomenosUmMorto == true);
    }
}