#include "doctest.h"

#include "InimigoComum.hpp"
#include "Aventureiro.hpp"

TEST_SUITE("Inimigos Comuns") {

    // ==========================================================
    // DESAFIANTE DO BAR
    // ==========================================================

    TEST_CASE("DesafianteDoBar rejeita nivel invalido") {
        CHECK_THROWS_AS(DesafianteDoBar("Teste", 0), std::invalid_argument);
        CHECK_THROWS_AS(DesafianteDoBar("Teste", -1), std::invalid_argument);
    }

    TEST_CASE("DesafianteDoBar usa nome padrao") {
        DesafianteDoBar inimigo("", 1);

        CHECK(inimigo.getNome() == "Desafiante do Bar");
    }

    TEST_CASE("DesafianteDoBar causa dano ao atacar") {
        DesafianteDoBar inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        int hpAntes = alvo.getHP();

        inimigo.executarTurno(alvo);

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("DesafianteDoBar escala com nivel") {
        DesafianteDoBar n1("Teste", 1);
        DesafianteDoBar n2("Teste", 2);

        CHECK(n2.getHPMax() > n1.getHPMax());
        CHECK(n2.getForcaTotal() > n1.getForcaTotal());
        CHECK(n2.getDefesa() > n1.getDefesa());
    }

    // ==========================================================
    // TRABALHADOR NOTURNO
    // ==========================================================

    TEST_CASE("TrabalhadorNoturno rejeita nivel invalido") {
        CHECK_THROWS_AS(TrabalhadorNoturno("Teste", 0), std::invalid_argument);
        CHECK_THROWS_AS(TrabalhadorNoturno("Teste", -1), std::invalid_argument);
    }

    TEST_CASE("TrabalhadorNoturno usa nome padrao") {
        TrabalhadorNoturno inimigo("", 1);

        CHECK(inimigo.getNome() == "Trabalhador Noturno Estressado");
    }

    TEST_CASE("TrabalhadorNoturno causa dano") {
        TrabalhadorNoturno inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        int hpAntes = alvo.getHP();

        inimigo.executarTurno(alvo);

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("TrabalhadorNoturno escala com nivel") {
        TrabalhadorNoturno n1("Teste", 1);
        TrabalhadorNoturno n2("Teste", 2);

        CHECK(n2.getHPMax() > n1.getHPMax());
        CHECK(n2.getForcaTotal() > n1.getForcaTotal());
        CHECK(n2.getDefesa() > n1.getDefesa());
    }

    // ==========================================================
    // SEGURANCA DE BALADA
    // ==========================================================

    TEST_CASE("SegurancaDeBalada rejeita nivel invalido") {
        CHECK_THROWS_AS(SegurancaDeBalada("Teste", 0), std::invalid_argument);
        CHECK_THROWS_AS(SegurancaDeBalada("Teste", -1), std::invalid_argument);
    }

    TEST_CASE("SegurancaDeBalada usa nome padrao") {
        SegurancaDeBalada inimigo("", 1);

        CHECK(inimigo.getNome() == "Segurança de Balada");
    }

    TEST_CASE("SegurancaDeBalada causa dano") {
        SegurancaDeBalada inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        int hpAntes = alvo.getHP();

        inimigo.executarTurno(alvo);

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("SegurancaDeBalada escala com nivel") {
        SegurancaDeBalada n1("Teste", 1);
        SegurancaDeBalada n2("Teste", 2);

        CHECK(n2.getHPMax() > n1.getHPMax());
        CHECK(n2.getForcaTotal() > n1.getForcaTotal());
        CHECK(n2.getDefesa() > n1.getDefesa());
    }

    TEST_CASE("SegurancaDeBalada continua atacando apos varios turnos") {
        SegurancaDeBalada inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        int hpAntes = alvo.getHP();

        for(int i = 0; i < 3; i++) {
            inimigo.executarTurno(alvo);
        }

        CHECK(alvo.getHP() < hpAntes);
    }

    // ==========================================================
    // COMPORTAMENTOS GERAIS
    // ==========================================================

    TEST_CASE("Inimigos nao atacam alvos mortos") {
        DesafianteDoBar inimigo("Teste", 1);
        Aventureiro alvo("Heroi", 100, 1, 10);

        alvo.receberDano(1000, TipoHabilidade::ULTRA);

        int hpFinal = alvo.getHP();

        inimigo.executarTurno(alvo);

        CHECK(alvo.getHP() == hpFinal);
    }

    TEST_CASE("Todos os inimigos iniciam vivos") {
        DesafianteDoBar d("D", 1);
        TrabalhadorNoturno t("T", 1);
        SegurancaDeBalada s("S", 1);

        CHECK(d.estaVivo());
        CHECK(t.estaVivo());
        CHECK(s.estaVivo());
    }
}