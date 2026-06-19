/**
 * @file test_InimigoIncomum.cpp
 * @brief Testes de unidade para InimigoIncomum — cobertura máxima do InimigoIncomum.cpp
 */

#include "doctest.h"
#include "InimigoIncomum.hpp"
#include "Aventureiro.hpp"
#include <stdexcept>

// =========================================================
// SUITE 1: RecrutaDoutrinado — Construção
// Cobre: construtor, validarNivel(), nome padrão, atributos
// =========================================================

TEST_SUITE("RecrutaDoutrinado - Estado Inicial") {

    TEST_CASE("Atributos corretos no nível 1") {
        RecrutaDoutrinado r("Recruta", 1);
        CHECK(r.estaVivo() == true);
        CHECK(r.getHP() == 55);
        CHECK(r.getXPRecompensa() == 40);
    }

    TEST_CASE("Atributos corretos no nível 3") {
        RecrutaDoutrinado r("Recruta", 3);
        CHECK(r.getHP() == 165);
        CHECK(r.getXPRecompensa() == 120);
    }

    TEST_CASE("Nome padrão se vazio") {
        RecrutaDoutrinado r("", 1);
        CHECK(r.getNome() == "Recruta Doutrinado");
    }

    TEST_CASE("Lança exceção para nível zero") {
        CHECK_THROWS_AS(RecrutaDoutrinado("X", 0), std::invalid_argument);
    }

    TEST_CASE("Lança exceção para nível negativo") {
        CHECK_THROWS_AS(RecrutaDoutrinado("X", -1), std::invalid_argument);
    }

    TEST_CASE("Nível 3 tem mais HP e XP que nível 1") {
        RecrutaDoutrinado r1("R1", 1);
        RecrutaDoutrinado r3("R3", 3);
        CHECK(r3.getHP() > r1.getHP());
        CHECK(r3.getXPRecompensa() > r1.getXPRecompensa());
    }
}

// =========================================================
// SUITE 2: RecrutaDoutrinado — Comportamento
// Cobre: todos os branches de executarTurno()
//   turno 1 (padrão), turno 2 (Sangramento), turno 4 (Grito+ataque), turno 5 (Intimidação)
// =========================================================

TEST_SUITE("RecrutaDoutrinado - Comportamento") {

    TEST_CASE("Turno 1 — Soco Treinado causa dano") {
        Aventureiro a("Herói", 500, 1, 10);
        RecrutaDoutrinado r("Recruta", 1);
        int hpAntes = a.getHP();
        r.executarTurno(a); // turno 1 — padrão
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Turno 2 — Sangramento (contadorTurnos % 2 == 0)") {
        Aventureiro a("Herói", 500, 1, 10);
        RecrutaDoutrinado r("Recruta", 1);
        r.executarTurno(a); // turno 1
        r.executarTurno(a); // turno 2 — Sangramento
        CHECK(a.getHP() < 500);
    }

    TEST_CASE("Turno 4 — Grito de Doutrina + ataque (contadorTurnos % 4 == 0)") {
        Aventureiro a("Herói", 500, 1, 10);
        RecrutaDoutrinado r("Recruta", 1);
        r.executarTurno(a); // turno 1
        r.executarTurno(a); // turno 2
        r.executarTurno(a); // turno 3
        r.executarTurno(a); // turno 4 — Grito de Doutrina
        CHECK(r.estaVivo());
        CHECK(a.getHP() < 500);
    }

    TEST_CASE("Turno 5 — Intimidação (contadorTurnos % 5 == 0)") {
        Aventureiro a("Herói", 500, 1, 10);
        RecrutaDoutrinado r("Recruta", 1);
        for (int i = 0; i < 5; i++) r.executarTurno(a); // turno 5 — Intimidação
        CHECK(r.estaVivo());
    }

    TEST_CASE("Turno 10 — segundo ciclo completo sem exceção") {
        Aventureiro a("Herói", 9999, 1, 10);
        RecrutaDoutrinado r("Recruta", 1);
        for (int i = 0; i < 10; i++) r.executarTurno(a);
        CHECK(r.estaVivo());
    }

    TEST_CASE("Não age se alvo estiver morto") {
        Aventureiro a("Herói", 100, 1, 10);
        RecrutaDoutrinado r("Recruta", 1);
        a.receberDano(9999, TipoHabilidade::FISICO);
        CHECK_NOTHROW(r.executarTurno(a));
    }

    TEST_CASE("Não age se ele mesmo estiver morto") {
        Aventureiro a("Herói", 500, 1, 10);
        RecrutaDoutrinado r("Recruta", 1);
        r.receberDano(9999, TipoHabilidade::FISICO);
        int hpAntes = a.getHP();
        r.executarTurno(a);
        CHECK(a.getHP() == hpAntes);
    }

    TEST_CASE("Morre com dano letal") {
        RecrutaDoutrinado r("Recruta", 1);
        r.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(r.estaVivo() == false);
        CHECK(r.getHP() == 0);
    }
}

// =========================================================
// SUITE 3: GuardaPatrimonial — Construção
// =========================================================

TEST_SUITE("GuardaPatrimonial - Estado Inicial") {

    TEST_CASE("Atributos corretos no nível 1") {
        GuardaPatrimonial g("Guarda", 1);
        CHECK(g.estaVivo() == true);
        CHECK(g.getHP() == 65);
        CHECK(g.getXPRecompensa() == 45);
    }

    TEST_CASE("Atributos corretos no nível 4") {
        GuardaPatrimonial g("Guarda", 4);
        CHECK(g.getHP() == 260);
        CHECK(g.getXPRecompensa() == 180);
    }

    TEST_CASE("Nome padrão se vazio") {
        GuardaPatrimonial g("", 1);
        CHECK(g.getNome() == "Guarda Patrimonial");
    }

    TEST_CASE("Lança exceção para nível zero") {
        CHECK_THROWS_AS(GuardaPatrimonial("X", 0), std::invalid_argument);
    }

    TEST_CASE("Lança exceção para nível negativo") {
        CHECK_THROWS_AS(GuardaPatrimonial("X", -1), std::invalid_argument);
    }

    TEST_CASE("Nível 3 tem mais HP e XP que nível 1") {
        GuardaPatrimonial g1("G1", 1);
        GuardaPatrimonial g3("G3", 3);
        CHECK(g3.getHP() > g1.getHP());
        CHECK(g3.getXPRecompensa() > g1.getXPRecompensa());
    }
}

// =========================================================
// SUITE 4: GuardaPatrimonial — Comportamento
// Cobre: turno padrão (Cassetete), turno%3 (Atordoamento),
//        turno%4 (Gás Lacrimogêneo), turno%5 (Bandagem + ataque)
// =========================================================

TEST_SUITE("GuardaPatrimonial - Comportamento") {

    TEST_CASE("Turno 1 — Cassetete causa dano") {
        Aventureiro a("Herói", 500, 1, 10);
        GuardaPatrimonial g("Guarda", 1);
        int hpAntes = a.getHP();
        g.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Turno 3 — Atordoamento (contadorTurnos % 3 == 0)") {
        Aventureiro a("Herói", 500, 1, 10);
        GuardaPatrimonial g("Guarda", 1);
        g.executarTurno(a); // turno 1
        g.executarTurno(a); // turno 2
        g.executarTurno(a); // turno 3 — Atordoamento
        CHECK(g.estaVivo());
    }

    TEST_CASE("Turno 4 — Gás Lacrimogêneo (contadorTurnos % 4 == 0)") {
        Aventureiro a("Herói", 500, 1, 10);
        GuardaPatrimonial g("Guarda", 1);
        for (int i = 0; i < 4; i++) g.executarTurno(a); // turno 4 — Gás
        CHECK(g.estaVivo());
    }

    TEST_CASE("Turno 5 — Bandagem de Campo + ataque (contadorTurnos % 5 == 0)") {
        Aventureiro a("Herói", 500, 1, 10);
        GuardaPatrimonial g("Guarda", 1);
        for (int i = 0; i < 5; i++) g.executarTurno(a); // turno 5 — Bandagem
        CHECK(g.estaVivo());
        CHECK(a.getHP() < 500);
    }

    TEST_CASE("Turno 10 — segundo ciclo completo sem exceção") {
        Aventureiro a("Herói", 9999, 1, 10);
        GuardaPatrimonial g("Guarda", 1);
        for (int i = 0; i < 10; i++) g.executarTurno(a);
        CHECK(g.estaVivo());
    }

    TEST_CASE("Não age se alvo estiver morto") {
        Aventureiro a("Herói", 100, 1, 10);
        GuardaPatrimonial g("Guarda", 1);
        a.receberDano(9999, TipoHabilidade::FISICO);
        CHECK_NOTHROW(g.executarTurno(a));
    }

    TEST_CASE("Não age se ele mesmo estiver morto") {
        Aventureiro a("Herói", 500, 1, 10);
        GuardaPatrimonial g("Guarda", 1);
        g.receberDano(9999, TipoHabilidade::FISICO);
        int hpAntes = a.getHP();
        g.executarTurno(a);
        CHECK(a.getHP() == hpAntes);
    }

    TEST_CASE("Morre com dano letal") {
        GuardaPatrimonial g("Guarda", 1);
        g.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(g.estaVivo() == false);
        CHECK(g.getHP() == 0);
    }
}

// =========================================================
// SUITE 5: SabotadorRival — Construção
// =========================================================

TEST_SUITE("SabotadorRival - Estado Inicial") {

    TEST_CASE("Atributos corretos no nível 1") {
        SabotadorRival s("Sabotador", 1);
        CHECK(s.estaVivo() == true);
        CHECK(s.getHP() == 25);
        CHECK(s.getXPRecompensa() == 60);
    }

    TEST_CASE("Atributos corretos no nível 2") {
        SabotadorRival s("Sabotador", 2);
        CHECK(s.getHP() == 50);
        CHECK(s.getXPRecompensa() == 120);
    }

    TEST_CASE("Nome padrão se vazio") {
        SabotadorRival s("", 1);
        CHECK(s.getNome() == "Sabotador Rival");
    }

    TEST_CASE("Lança exceção para nível zero") {
        CHECK_THROWS_AS(SabotadorRival("X", 0), std::invalid_argument);
    }

    TEST_CASE("Lança exceção para nível negativo") {
        CHECK_THROWS_AS(SabotadorRival("X", -10), std::invalid_argument);
    }

    TEST_CASE("Nível 3 tem mais HP e XP que nível 1") {
        SabotadorRival s1("S1", 1);
        SabotadorRival s3("S3", 3);
        CHECK(s3.getHP() > s1.getHP());
        CHECK(s3.getXPRecompensa() > s1.getXPRecompensa());
    }
}

// =========================================================
// SUITE 6: SabotadorRival — Comportamento
// Cobre: turno 1 (Molotov+Queimadura), turno 3 (Gás+Molotov),
//        turno 5 (Ultra+Queimadura), guards
// =========================================================

TEST_SUITE("SabotadorRival - Comportamento") {

    TEST_CASE("Turno 1 — Molotov + Queimadura causa dano") {
        Aventureiro a("Herói", 9999, 1, 10);
        SabotadorRival s("Sabotador", 1);
        int hpAntes = a.getHP();
        s.executarTurno(a); // turno 1 — Molotov
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Turno 3 — Gás Paralisante + Molotov (contadorTurnos % 3 == 0)") {
        Aventureiro a("Herói", 9999, 1, 10);
        SabotadorRival s("Sabotador", 1);
        s.executarTurno(a); // turno 1
        s.executarTurno(a); // turno 2
        s.executarTurno(a); // turno 3 — Gás + Molotov
        CHECK(s.estaVivo());
        CHECK(a.getHP() < 9999);
    }

    TEST_CASE("Turno 5 — Explosão Total ULTRA (contadorTurnos % 5 == 0)") {
        Aventureiro a("Herói", 9999, 1, 10);
        SabotadorRival s("Sabotador", 1);
        for (int i = 0; i < 5; i++) s.executarTurno(a); // turno 5 — Ultra
        CHECK(s.estaVivo());
        CHECK(a.getHP() < 9999);
    }

    TEST_CASE("Turno 10 — segundo ciclo completo sem exceção") {
        Aventureiro a("Herói", 9999, 1, 10);
        SabotadorRival s("Sabotador", 1);
        for (int i = 0; i < 10; i++) s.executarTurno(a);
        CHECK(s.estaVivo());
    }

    TEST_CASE("Não age se alvo estiver morto") {
        Aventureiro a("Herói", 100, 1, 10);
        SabotadorRival s("Sabotador", 1);
        a.receberDano(9999, TipoHabilidade::FISICO);
        CHECK_NOTHROW(s.executarTurno(a));
    }

    TEST_CASE("Não age se ele mesmo estiver morto") {
        Aventureiro a("Herói", 9999, 1, 10);
        SabotadorRival s("Sabotador", 1);
        s.receberDano(9999, TipoHabilidade::FISICO);
        int hpAntes = a.getHP();
        s.executarTurno(a);
        CHECK(a.getHP() == hpAntes);
    }

    TEST_CASE("Morre com dano letal") {
        SabotadorRival s("Sabotador", 1);
        s.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(s.estaVivo() == false);
        CHECK(s.getHP() == 0);
    }
}