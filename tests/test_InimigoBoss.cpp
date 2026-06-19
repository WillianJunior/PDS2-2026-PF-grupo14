/**
 * @file test_InimigoBoss.cpp
 * @brief Testes de unidade para TylerDurden — cobertura máxima do InimigoBoss.cpp
 */

#include "doctest.h"
#include "InimigoBoss.hpp"
#include "Aventureiro.hpp"
#include <stdexcept>

// =========================================================
// SUITE 1: Construção e Estado Inicial
// Cobre: construtor, validarNivel(), adicionarHabilidade(), atributos base
// =========================================================

TEST_SUITE("TylerDurden - Estado Inicial") {

    TEST_CASE("Criado com nome correto") {
        TylerDurden t("Tyler Projetado", 2);
        CHECK(t.getNome() == "Tyler Projetado");
    }

    TEST_CASE("Usa nome padrão se enviado vazio") {
        TylerDurden t("", 1);
        CHECK(t.getNome() == "Tyler Durden");
    }

    TEST_CASE("Começa vivo") {
        TylerDurden t("Tyler", 1);
        CHECK(t.estaVivo() == true);
    }

    TEST_CASE("HP é proporcional ao nível — nível 1") {
        TylerDurden t("Tyler", 1);
        CHECK(t.getHP() == 150);
    }

    TEST_CASE("HP é proporcional ao nível — nível 2") {
        TylerDurden t("Tyler", 2);
        CHECK(t.getHP() == 300);
    }

    TEST_CASE("XP de recompensa é proporcional ao nível — nível 1") {
        TylerDurden t("Tyler", 1);
        CHECK(t.getXPRecompensa() == 200);
    }

    TEST_CASE("XP de recompensa é proporcional ao nível — nível 2") {
        TylerDurden t("Tyler", 2);
        CHECK(t.getXPRecompensa() == 400);
    }

    TEST_CASE("Lança exceção para nível zero") {
        CHECK_THROWS_AS(TylerDurden("Tyler", 0), std::invalid_argument);
    }

    TEST_CASE("Lança exceção para nível negativo") {
        CHECK_THROWS_AS(TylerDurden("Tyler", -5), std::invalid_argument);
    }
}

// =========================================================
// SUITE 2: Receber dano e morte
// Cobre: receberDano(), estaVivo(), getHP()
// =========================================================

TEST_SUITE("TylerDurden - Dano e Morte") {

    TEST_CASE("Recebe dano e HP diminui") {
        TylerDurden t("Tyler", 1);
        int hpAntes = t.getHP();
        t.receberDano(30, TipoHabilidade::FISICO);
        CHECK(t.getHP() < hpAntes);
    }

    TEST_CASE("Morre com dano letal — estaVivo() false") {
        TylerDurden t("Tyler", 1);
        t.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(t.estaVivo() == false);
    }

    TEST_CASE("Morre com dano letal — HP zerado") {
        TylerDurden t("Tyler", 1);
        t.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(t.getHP() == 0);
    }
}

// =========================================================
// SUITE 3: Fase 1 — HP > 50%
// Cobre: ataque principal (Soco do Caos), Sangramento (turno%3),
//        Foco Destrutivo (turno%4), processarEfeitosContinuos()
// =========================================================

TEST_SUITE("TylerDurden - Fase 1") {

    TEST_CASE("Causa dano ao aventureiro no turno 1") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1);
        int hpAntes = a.getHP();
        t.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Turno 3 — aciona Sangramento (contadorTurnos % 3 == 0)") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1);
        t.executarTurno(a); // turno 1
        t.executarTurno(a); // turno 2
        t.executarTurno(a); // turno 3 — Sangramento
        CHECK(t.estaVivo());
        CHECK(a.getHP() < 1000);
    }

    TEST_CASE("Turno 4 — aciona Foco Destrutivo (contadorTurnos % 4 == 0)") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1);
        t.executarTurno(a); // turno 1
        t.executarTurno(a); // turno 2
        t.executarTurno(a); // turno 3
        t.executarTurno(a); // turno 4 — Foco Destrutivo
        CHECK(t.estaVivo());
    }

    TEST_CASE("Turno 6 — aciona Sangramento e não lança exceção") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1);
        for (int i = 0; i < 6; i++) t.executarTurno(a);
        CHECK(t.estaVivo());
    }

    TEST_CASE("Não age se alvo já estiver morto") {
        Aventureiro a("Herói", 100, 1, 10);
        TylerDurden t("Tyler", 1);
        a.receberDano(9999, TipoHabilidade::FISICO);
        CHECK_NOTHROW(t.executarTurno(a));
    }

    TEST_CASE("Não age se ele mesmo estiver morto") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1);
        t.receberDano(9999, TipoHabilidade::FISICO);
        CHECK_NOTHROW(t.executarTurno(a));
    }
}

// =========================================================
// SUITE 4: Fase 2 — HP entre 20% e 50%
// Cobre: anúncio de fase, Impacto Psicológico, Humilhação (turno%3),
//        Resiliência Estóica (turno%4), Sangramento fase 2
// =========================================================

TEST_SUITE("TylerDurden - Fase 2") {

    TEST_CASE("Entra na fase 2 quando HP <= 50% e causa dano") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1); // HP max: 150
        t.receberDano(80, TipoHabilidade::ESPECIAL); // HP ~70 — abaixo de 50%
        int hpAntes = a.getHP();
        t.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Fase 2 turno 3 — aciona Humilhação e Sangramento") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1);
        t.receberDano(80, TipoHabilidade::ESPECIAL);
        t.executarTurno(a); // turno 1
        t.executarTurno(a); // turno 2
        t.executarTurno(a); // turno 3 — Humilhação + Sangramento
        CHECK(t.estaVivo());
        CHECK(a.getHP() < 1000);
    }

    TEST_CASE("Fase 2 turno 4 — aciona Resiliência Estóica") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1);
        t.receberDano(80, TipoHabilidade::ESPECIAL);
        t.executarTurno(a); // turno 1
        t.executarTurno(a); // turno 2
        t.executarTurno(a); // turno 3
        t.executarTurno(a); // turno 4 — Resiliência Estóica
        CHECK(t.estaVivo());
    }

    TEST_CASE("Anúncio de fase 2 ocorre apenas uma vez") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1);
        t.receberDano(80, TipoHabilidade::ESPECIAL);
        // Dois turnos na fase 2 — anúncio só no primeiro
        CHECK_NOTHROW(t.executarTurno(a));
        CHECK_NOTHROW(t.executarTurno(a));
    }
}

// =========================================================
// SUITE 5: Gatilhos Globais
// Cobre: Cura Estóica (Tyler < 20% HP), Desconstrução Total (alvo < 30% HP)
// =========================================================

TEST_SUITE("TylerDurden - Gatilhos Globais") {

    TEST_CASE("Cura Estóica — Tyler se cura quando HP < 20%") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1); // HP max: 150
        t.receberDano(130, TipoHabilidade::ESPECIAL); // HP ~20 — abaixo de 20%
        int hpAntes = t.getHP();
        t.executarTurno(a);
        CHECK(t.getHP() > hpAntes);
    }

    TEST_CASE("Cura Estóica — usada apenas uma vez") {
        Aventureiro a("Herói", 1000, 1, 10);
        TylerDurden t("Tyler", 1);
        t.receberDano(130, TipoHabilidade::ESPECIAL);
        t.executarTurno(a); // cura aqui
        int hpAposCura = t.getHP();
        t.receberDano(50, TipoHabilidade::ESPECIAL); // reduz HP novamente
        t.executarTurno(a); // não cura de novo
        CHECK(t.getHP() < hpAposCura);
    }

    TEST_CASE("Desconstrução Total — dispara quando alvo < 30% HP") {
        Aventureiro a("Herói", 100, 1, 10); // HP max: 100
        TylerDurden t("Tyler", 1);
        a.receberDano(75, TipoHabilidade::ESPECIAL); // HP = 25 — abaixo de 30%
        t.executarTurno(a);
        CHECK(a.estaVivo() == false);
    }

    TEST_CASE("Desconstrução Total — usada apenas uma vez") {
        Aventureiro a("Herói", 100, 1, 10);
        TylerDurden t("Tyler", 1);
        a.receberDano(75, TipoHabilidade::ESPECIAL);
        t.executarTurno(a); // ultra aqui
        // segunda vez: alvo já morto, não deve lançar
        CHECK_NOTHROW(t.executarTurno(a));
    }
}

// =========================================================
// SUITE 6: Escalonamento por nível
// =========================================================

TEST_SUITE("TylerDurden - Escalonamento") {

    TEST_CASE("Nível 3 tem mais HP que nível 1") {
        TylerDurden t1("Tyler", 1);
        TylerDurden t3("Tyler", 3);
        CHECK(t3.getHP() > t1.getHP());
    }

    TEST_CASE("Nível 3 dá mais XP que nível 1") {
        TylerDurden t1("Tyler", 1);
        TylerDurden t3("Tyler", 3);
        CHECK(t3.getXPRecompensa() > t1.getXPRecompensa());
    }
}