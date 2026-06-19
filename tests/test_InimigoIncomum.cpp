#include "doctest.h"

#include "InimigoIncomum.hpp"
#include "Aventureiro.hpp"

TEST_SUITE("Inimigos Incomuns") {

    // ==========================================================
    // RECRUTA DOUTRINADO
    // ==========================================================

    TEST_CASE("RecrutaDoutrinado rejeita nivel invalido") {
        CHECK_THROWS_AS(RecrutaDoutrinado("Teste", 0), std::invalid_argument);
        CHECK_THROWS_AS(RecrutaDoutrinado("Teste", -1), std::invalid_argument);
    }

    TEST_CASE("RecrutaDoutrinado usa nome padrao") {
        RecrutaDoutrinado inimigo("", 1);

        CHECK(inimigo.getNome() == "Recruta Doutrinado");
    }

    TEST_CASE("RecrutaDoutrinado causa dano no turno comum") {
        RecrutaDoutrinado inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        int hpAntes = alvo.getHP();

        inimigo.executarTurno(alvo);

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("RecrutaDoutrinado aplica sangramento no segundo turno") {
        RecrutaDoutrinado inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        inimigo.executarTurno(alvo);
        inimigo.executarTurno(alvo);

        int hpAntes = alvo.getHP();

        alvo.processarEfeitosContinuos();

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("RecrutaDoutrinado recebe buff no quarto turno") {
        RecrutaDoutrinado inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        int forcaInicial = inimigo.getForcaTotal();

        for(int i = 0; i < 4; i++) {
            inimigo.executarTurno(alvo);
        }

        CHECK(inimigo.getForcaTotal() > forcaInicial);
    }

    TEST_CASE("RecrutaDoutrinado escala com nivel") {
        RecrutaDoutrinado n1("Teste", 1);
        RecrutaDoutrinado n2("Teste", 2);

        CHECK(n2.getHPMax() > n1.getHPMax());
        CHECK(n2.getForcaTotal() > n1.getForcaTotal());
        CHECK(n2.getDefesa() > n1.getDefesa());
    }

    // ==========================================================
    // GUARDA PATRIMONIAL
    // ==========================================================

    TEST_CASE("GuardaPatrimonial rejeita nivel invalido") {
        CHECK_THROWS_AS(GuardaPatrimonial("Teste", 0), std::invalid_argument);
        CHECK_THROWS_AS(GuardaPatrimonial("Teste", -1), std::invalid_argument);
    }

    TEST_CASE("GuardaPatrimonial usa nome padrao") {
        GuardaPatrimonial inimigo("", 1);

        CHECK(inimigo.getNome() == "Guarda Patrimonial");
    }

    TEST_CASE("GuardaPatrimonial causa dano") {
        GuardaPatrimonial inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        int hpAntes = alvo.getHP();

        inimigo.executarTurno(alvo);

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("GuardaPatrimonial aplica HOT no quinto turno") {
        GuardaPatrimonial inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        inimigo.receberDano(20, TipoHabilidade::FISICO);

        int hpAntes = inimigo.getHP();

        for(int i = 0; i < 5; i++) {
            inimigo.executarTurno(alvo);
        }

        inimigo.processarEfeitosContinuos();

        CHECK(inimigo.getHP() > hpAntes);
    }

    TEST_CASE("GuardaPatrimonial escala com nivel") {
        GuardaPatrimonial n1("Teste", 1);
        GuardaPatrimonial n2("Teste", 2);

        CHECK(n2.getHPMax() > n1.getHPMax());
        CHECK(n2.getForcaTotal() > n1.getForcaTotal());
        CHECK(n2.getDefesa() > n1.getDefesa());
    }

    // ==========================================================
    // SABOTADOR RIVAL
    // ==========================================================

    TEST_CASE("SabotadorRival rejeita nivel invalido") {
        CHECK_THROWS_AS(SabotadorRival("Teste", 0), std::invalid_argument);
        CHECK_THROWS_AS(SabotadorRival("Teste", -1), std::invalid_argument);
    }

    TEST_CASE("SabotadorRival usa nome padrao") {
        SabotadorRival inimigo("", 1);

        CHECK(inimigo.getNome() == "Sabotador Rival");
    }

    TEST_CASE("SabotadorRival causa dano especial") {
        SabotadorRival inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        int hpAntes = alvo.getHP();

        inimigo.executarTurno(alvo);

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("SabotadorRival aplica queimadura") {
        SabotadorRival inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        inimigo.executarTurno(alvo);

        int hpAntes = alvo.getHP();

        alvo.processarEfeitosContinuos();

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("SabotadorRival executa ultra no quinto turno") {
        SabotadorRival inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        for(int i = 0; i < 4; i++) {
            inimigo.executarTurno(alvo);
        }

        int hpAntes = alvo.getHP();

        inimigo.executarTurno(alvo);

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("SabotadorRival escala com nivel") {
        SabotadorRival n1("Teste", 1);
        SabotadorRival n2("Teste", 2);

        CHECK(n2.getHPMax() > n1.getHPMax());
        CHECK(n2.getForcaTotal() > n1.getForcaTotal());
        CHECK(n2.getDefesa() > n1.getDefesa());
    }

    // ==========================================================
    // COMPORTAMENTO GERAL
    // ==========================================================

    TEST_CASE("Inimigos incomuns nao atacam alvo morto") {
        RecrutaDoutrinado inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        alvo.receberDano(1000, TipoHabilidade::ULTRA);

        int hpFinal = alvo.getHP();

        inimigo.executarTurno(alvo);

        CHECK(alvo.getHP() == hpFinal);
    }

    TEST_CASE("Todos os inimigos incomuns iniciam vivos") {
        RecrutaDoutrinado r("R", 1);
        GuardaPatrimonial g("G", 1);
        SabotadorRival s("S", 1);

        CHECK(r.estaVivo());
        CHECK(g.estaVivo());
        CHECK(s.estaVivo());
    }
}
