
#include "doctest.h"

#include "InimigoBoss.hpp"
#include "Aventureiro.hpp"
#include <iostream>
TEST_SUITE("TylerDurden") {

    TEST_CASE("Construtor rejeita nivel invalido") {
        CHECK_THROWS_AS(TylerDurden("Tyler", 0), std::invalid_argument);
        CHECK_THROWS_AS(TylerDurden("Tyler", -1), std::invalid_argument);
    }

    TEST_CASE("Nome padrao quando string vazia") {
        TylerDurden boss("", 1);

        CHECK(boss.getNome() == "Tyler Durden");
    }

    TEST_CASE("Ataque basico da fase 1 causa dano") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 100, 5, 10);

        int hpAntes = alvo.getHP();

        boss.executarTurno(alvo);

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("Foco Destrutivo aumenta a forca do chefe") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 100, 5, 10);

        int forcaInicial = boss.getForcaTotal();

        for (int i = 0; i < 4; i++) {
            boss.executarTurno(alvo);
        }

        CHECK(boss.getForcaTotal() > forcaInicial);
    }

    TEST_CASE("Sangramento aplica dano continuo") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 100, 5, 10);

        for (int i = 0; i < 3; i++) {
            boss.executarTurno(alvo);
        }

        int hpAntes = alvo.getHP();

        alvo.processarEfeitosContinuos();

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("Cura Estoica ativa quando HP fica abaixo de 20 por cento") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 100, 5, 10);

        boss.receberDano(300, TipoHabilidade::FISICO);

        int hpAntes = boss.getHP();

        boss.executarTurno(alvo);

        CHECK(boss.getHP() > hpAntes);
    }

    TEST_CASE("Cura Estoica ocorre apenas uma vez") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 100, 5, 10);

        boss.receberDano(130, TipoHabilidade::FISICO);

        boss.executarTurno(alvo);

        int hpAposPrimeiraCura = boss.getHP();

        boss.receberDano(20, TipoHabilidade::FISICO);

        boss.executarTurno(alvo);

        CHECK(boss.getHP() <= hpAposPrimeiraCura);
    }

    TEST_CASE("Entrada na fase 2 quando HP fica abaixo de 50 por cento") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 100, 5, 10);

        boss.receberDano(80, TipoHabilidade::FISICO);

        int hpAntes = alvo.getHP();

        boss.executarTurno(alvo);

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("Desconstrucao Total ativa quando alvo fica abaixo de 30 por cento") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 100, 5, 10);

        alvo.receberDano(75, TipoHabilidade::ULTRA);

        int hpAntes = alvo.getHP();

        boss.executarTurno(alvo);

        CHECK(alvo.getHP() < hpAntes);
    }

    TEST_CASE("Desconstrucao Total ocorre apenas uma vez") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 100, 5, 10);

        alvo.receberDano(75, TipoHabilidade::ULTRA);

        boss.executarTurno(alvo);

        int hpDepoisDaUltra = alvo.getHP();

        boss.executarTurno(alvo);

        CHECK(alvo.getHP() >= hpDepoisDaUltra - 60);
    }

    TEST_CASE("Tyler permanece vivo apos construcao valida") {
        TylerDurden boss("Tyler", 1);

        CHECK(boss.estaVivo());
        CHECK(boss.getHP() > 0);
        CHECK(boss.getHPMax() > 0);
    }

    TEST_CASE("Atributos escalam com o nivel") {
        TylerDurden bossNivel1("Tyler", 1);
        TylerDurden bossNivel2("Tyler", 2);

        CHECK(bossNivel2.getHPMax() > bossNivel1.getHPMax());
        CHECK(bossNivel2.getForcaTotal() > bossNivel1.getForcaTotal());
        CHECK(bossNivel2.getDefesa() > bossNivel1.getDefesa());
    }
}
