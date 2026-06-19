/**
 * @file test_InimigoComum.cpp
 * @brief Testes de unidade para InimigoComum — cobertura máxima do InimigoComum.cpp
 */

#include "doctest.h"
#include "InimigoComum.hpp"
#include "Aventureiro.hpp"
#include <stdexcept>

// =========================================================
// SUITE 1: Construção e Estado Inicial
// Cobre: construtores, validarNivel(), atributos base, nomes padrão
// =========================================================

TEST_SUITE("InimigoComum - Estado Inicial") {

    TEST_CASE("DesafianteDoBar — atributos corretos no nível 1") {
        DesafianteDoBar d("Bêbado", 1);
        CHECK(d.estaVivo() == true);
        CHECK(d.getHP() == 35);
        CHECK(d.getXPRecompensa() == 15);
    }

    TEST_CASE("TrabalhadorNoturno — atributos corretos no nível 1") {
        TrabalhadorNoturno t("Trabalhador", 1);
        CHECK(t.estaVivo() == true);
        CHECK(t.getHP() == 20);
        CHECK(t.getXPRecompensa() == 18);
    }

    TEST_CASE("SegurancaDeBalada — atributos corretos no nível 1") {
        SegurancaDeBalada s("Segurança", 1);
        CHECK(s.estaVivo() == true);
        CHECK(s.getHP() == 60);
        CHECK(s.getXPRecompensa() == 25);
    }

    TEST_CASE("DesafianteDoBar — nome padrão se vazio") {
        DesafianteDoBar d("", 1);
        CHECK(d.getNome() == "Desafiante do Bar");
    }

    TEST_CASE("TrabalhadorNoturno — nome padrão se vazio") {
        TrabalhadorNoturno t("", 1);
        CHECK(t.getNome() == "Trabalhador Noturno Estressado");
    }

    TEST_CASE("SegurancaDeBalada — nome padrão se vazio") {
        SegurancaDeBalada s("", 1);
        CHECK(s.getNome() == "Segurança de Balada");
    }

    TEST_CASE("DesafianteDoBar — lança exceção para nível zero") {
        CHECK_THROWS_AS(DesafianteDoBar("X", 0), std::invalid_argument);
    }

    TEST_CASE("DesafianteDoBar — lança exceção para nível negativo") {
        CHECK_THROWS_AS(DesafianteDoBar("X", -1), std::invalid_argument);
    }

    TEST_CASE("TrabalhadorNoturno — lança exceção para nível inválido") {
        CHECK_THROWS_AS(TrabalhadorNoturno("X", 0), std::invalid_argument);
    }

    TEST_CASE("SegurancaDeBalada — lança exceção para nível inválido") {
        CHECK_THROWS_AS(SegurancaDeBalada("X", -2), std::invalid_argument);
    }
}

// =========================================================
// SUITE 2: Comportamento — DesafianteDoBar
// Cobre: executarTurno(), Soco Bêbado, guarda alvo morto/self morto
// =========================================================

TEST_SUITE("DesafianteDoBar - Comportamento") {

    TEST_CASE("Causa dano ao aventureiro") {
        Aventureiro a("Herói", 200, 1, 10);
        DesafianteDoBar d("Bêbado", 1);
        int hpAntes = a.getHP();
        d.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Não age se alvo estiver morto") {
        Aventureiro a("Herói", 100, 1, 10);
        DesafianteDoBar d("Bêbado", 1);
        a.receberDano(9999, TipoHabilidade::FISICO);
        CHECK_NOTHROW(d.executarTurno(a));
    }

    TEST_CASE("Não age se ele mesmo estiver morto") {
        Aventureiro a("Herói", 200, 1, 10);
        DesafianteDoBar d("Bêbado", 1);
        d.receberDano(9999, TipoHabilidade::FISICO);
        int hpAntes = a.getHP();
        d.executarTurno(a);
        CHECK(a.getHP() == hpAntes); // não causou dano
    }

    TEST_CASE("Morre com dano letal") {
        DesafianteDoBar d("Bêbado", 1);
        d.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(d.estaVivo() == false);
        CHECK(d.getHP() == 0);
    }

    TEST_CASE("Múltiplos turnos sem lançar exceção") {
        Aventureiro a("Herói", 1000, 1, 10);
        DesafianteDoBar d("Bêbado", 1);
        for (int i = 0; i < 5; i++) d.executarTurno(a);
        CHECK(d.estaVivo());
    }
}

// =========================================================
// SUITE 3: Comportamento — TrabalhadorNoturno
// Cobre: Surto de Raiva, Golpe Crítico (branch aleatório), guards
// =========================================================

TEST_SUITE("TrabalhadorNoturno - Comportamento") {

    TEST_CASE("Causa dano ao aventureiro") {
        Aventureiro a("Herói", 200, 1, 10);
        TrabalhadorNoturno t("Trabalhador", 1);
        int hpAntes = a.getHP();
        t.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Não age se alvo estiver morto") {
        Aventureiro a("Herói", 100, 1, 10);
        TrabalhadorNoturno t("Trabalhador", 1);
        a.receberDano(9999, TipoHabilidade::FISICO);
        CHECK_NOTHROW(t.executarTurno(a));
    }

    TEST_CASE("Não age se ele mesmo estiver morto") {
        Aventureiro a("Herói", 200, 1, 10);
        TrabalhadorNoturno t("Trabalhador", 1);
        t.receberDano(9999, TipoHabilidade::FISICO);
        int hpAntes = a.getHP();
        t.executarTurno(a);
        CHECK(a.getHP() == hpAntes);
    }

    TEST_CASE("Morre com dano letal") {
        TrabalhadorNoturno t("Trabalhador", 1);
        t.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(t.estaVivo() == false);
    }

    TEST_CASE("Múltiplos turnos cobrem branch crítico/normal") {
        // Com 10 turnos e 40% de chance de crítico, ambos os branches são cobertos
        Aventureiro a("Herói", 9999, 1, 10);
        TrabalhadorNoturno t("Trabalhador", 1);
        for (int i = 0; i < 10; i++) t.executarTurno(a);
        CHECK(a.getHP() < 9999);
    }
}

// =========================================================
// SUITE 4: Comportamento — SegurancaDeBalada
// Cobre: Empurrão de Segurança, postura defensiva (turno%3), guards
// =========================================================

TEST_SUITE("SegurancaDeBalada - Comportamento") {

    TEST_CASE("Causa dano ao aventureiro") {
        Aventureiro a("Herói", 200, 1, 10);
        SegurancaDeBalada s("Segurança", 1);
        int hpAntes = a.getHP();
        s.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Turno 3 — ativa escudo sem lançar exceção") {
        Aventureiro a("Herói", 1000, 1, 10);
        SegurancaDeBalada s("Segurança", 1);
        s.executarTurno(a); // turno 1
        s.executarTurno(a); // turno 2
        s.executarTurno(a); // turno 3 — escudo ativo
        CHECK(s.estaVivo());
    }

    TEST_CASE("Turno 6 — segundo ciclo de escudo sem lançar exceção") {
        Aventureiro a("Herói", 1000, 1, 10);
        SegurancaDeBalada s("Segurança", 1);
        for (int i = 0; i < 6; i++) s.executarTurno(a);
        CHECK(s.estaVivo());
    }

    TEST_CASE("Não age se alvo estiver morto") {
        Aventureiro a("Herói", 100, 1, 10);
        SegurancaDeBalada s("Segurança", 1);
        a.receberDano(9999, TipoHabilidade::FISICO);
        CHECK_NOTHROW(s.executarTurno(a));
    }

    TEST_CASE("Não age se ele mesmo estiver morto") {
        Aventureiro a("Herói", 200, 1, 10);
        SegurancaDeBalada s("Segurança", 1);
        s.receberDano(9999, TipoHabilidade::FISICO);
        int hpAntes = a.getHP();
        s.executarTurno(a);
        CHECK(a.getHP() == hpAntes);
    }

    TEST_CASE("Morre com dano letal") {
        SegurancaDeBalada s("Segurança", 1);
        s.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(s.estaVivo() == false);
        CHECK(s.getHP() == 0);
    }
}

// =========================================================
// SUITE 5: Escalonamento por nível
// =========================================================

TEST_SUITE("InimigoComum - Escalonamento") {

    TEST_CASE("DesafianteDoBar nível 3 tem mais HP e XP que nível 1") {
        DesafianteDoBar d1("D1", 1);
        DesafianteDoBar d3("D3", 3);
        CHECK(d3.getHP() > d1.getHP());
        CHECK(d3.getXPRecompensa() > d1.getXPRecompensa());
    }

    TEST_CASE("TrabalhadorNoturno nível 2 tem mais HP e XP que nível 1") {
        TrabalhadorNoturno t1("T1", 1);
        TrabalhadorNoturno t2("T2", 2);
        CHECK(t2.getHP() > t1.getHP());
        CHECK(t2.getXPRecompensa() > t1.getXPRecompensa());
    }

    TEST_CASE("SegurancaDeBalada nível 3 tem mais HP e XP que nível 1") {
        SegurancaDeBalada s1("S1", 1);
        SegurancaDeBalada s3("S3", 3);
        CHECK(s3.getHP() > s1.getHP());
        CHECK(s3.getXPRecompensa() > s1.getXPRecompensa());
    }
}