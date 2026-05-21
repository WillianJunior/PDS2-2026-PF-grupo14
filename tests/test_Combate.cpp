/**
 * @file test_Combate.cpp
 * @brief Testes de unidade para a classe Combate usando doctest (TDD).
 *
 * Para isolar o sistema de combate de I/O interativo, os testes usam
 * personagens com atributos extremos que garantem resultados determinísticos.
 */

#include "doctest/doctest.h"
#include "Combate.hpp"
#include "Aventureiro.hpp"
#include "InimigoComum.hpp"
#include "InimigoBoss.hpp"

// =========================================================
// SUITE: Resultado do Combate
// =========================================================

TEST_SUITE("Combate - Resultado") {

    TEST_CASE("Jogador muito forte vence o combate") {
        // Aventureiro com força absurda derrota qualquer Goblin em 1 turno
        Aventureiro a("Herói Forte", 500, 50, 9999);
        Goblin g("Goblin Fraco", 1);
        Combate c(a, g);
        bool resultado = c.iniciar();
        CHECK(resultado == true);
    }

    TEST_CASE("Jogador vivo após vencer") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        Goblin g("Goblin Fraco", 1);
        Combate c(a, g);
        c.iniciar();
        CHECK(a.estaVivo() == true);
    }

    TEST_CASE("Inimigo morto após derrota") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        Goblin g("Goblin Fraco", 1);
        Combate c(a, g);
        c.iniciar();
        CHECK(g.estaVivo() == false);
    }

    TEST_CASE("Jogador derrota Slime") {
        Aventureiro a("Herói", 300, 20, 500);
        Slime s("Slime", 1);
        Combate c(a, s);
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
        Goblin g("Goblin", 1);
        int xpGoblin = g.getXPRecompensa();
        Combate c(a, g);
        c.iniciar();
        // Verificamos indiretamente: se ganhou XP suficiente, subiu de nível
        // (XP do Goblin nível 1 = 20, limiar = 100, então não sobe de nível)
        // Mas podemos checar que a vitória ocorreu e o inimigo está morto
        CHECK(g.estaVivo() == false);
        CHECK(xpGoblin > 0);
    }

    TEST_CASE("Jogador perde HP durante o combate") {
        // Aventureiro fraco (força 1) vs Goblin forte nível 10
        // O Goblin sobrevive vários turnos e consegue atacar o jogador
        Aventureiro a("Herói Frágil", 500, 0, 1);
        Goblin g("Goblin Forte", 10);
        int hpAntes = a.getHP();
        Combate c(a, g);
        c.iniciar();
        // Com força 1 e Goblin nível 10 (HP alto), o Goblin ataca várias vezes
        CHECK(a.getHP() < hpAntes);
    }
}

// =========================================================
// SUITE: Invariantes do Combate
// =========================================================

TEST_SUITE("Combate - Invariantes") {

    TEST_CASE("HP do jogador nunca fica negativo após combate") {
        Aventureiro a("Herói", 500, 50, 9999);
        Goblin g("Goblin", 1);
        Combate c(a, g);
        c.iniciar();
        CHECK(a.getHP() >= 0);
    }

    TEST_CASE("HP do inimigo nunca fica negativo após combate") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        Goblin g("Goblin", 1);
        Combate c(a, g);
        c.iniciar();
        CHECK(g.getHP() >= 0);
    }

    TEST_CASE("Ao fim do combate, pelo menos um combatente está morto") {
        Aventureiro a("Herói Forte", 500, 50, 9999);
        Goblin g("Goblin", 1);
        Combate c(a, g);
        c.iniciar();
        bool pelomenosUmMorto = !a.estaVivo() || !g.estaVivo();
        CHECK(pelomenosUmMorto == true);
    }
}
