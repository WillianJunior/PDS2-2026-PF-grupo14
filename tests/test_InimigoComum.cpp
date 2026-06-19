/**
 * @file test_InimigoComum.cpp
 * @brief Testes de unidade para as classes de Inimigos Comuns usando doctest (TDD).
 */

#include "doctest.h"
#include "InimigoComum.hpp"
#include "Aventureiro.hpp"
#include <stdexcept>

TEST_SUITE("Inimigos Comuns - Estado Inicial") {

    TEST_CASE("Desafiante do Bar começa vivo e com status correto") {
        DesafianteDoBar d("Bêbado", 1);
        CHECK(d.estaVivo() == true);
        CHECK(d.getHP() == 35); // 35 * 1
        CHECK(d.getXPRecompensa() == 15);
    }

    TEST_CASE("Trabalhador Noturno começa vivo e com status correto") {
        TrabalhadorNoturno t("Trabalhador Estressado", 1);
        CHECK(t.estaVivo() == true);
        CHECK(t.getHP() == 20); // 20 * 1 (frágil)
        CHECK(t.getXPRecompensa() == 18);
    }

    TEST_CASE("Segurança de Balada começa vivo e com status correto") {
        SegurancaDeBalada s("Segurança Brutamontes", 1);
        CHECK(s.estaVivo() == true);
        CHECK(s.getHP() == 60); // 60 * 1 (resistente)
        CHECK(s.getXPRecompensa() == 25);
    }

    TEST_CASE("Inimigos comuns usam nome padrão se string for vazia") {
        DesafianteDoBar d("", 1);
        CHECK(d.getNome() == "Desafiante do Bar");

        TrabalhadorNoturno t("", 1);
        CHECK(t.getNome() == "Trabalhador Noturno Estressado");
    }

    TEST_CASE("Validação de nível impede criação com nível inválido") {
        CHECK_THROWS_AS(DesafianteDoBar("Invalido", 0), std::invalid_argument);
        CHECK_THROWS_AS(TrabalhadorNoturno("Invalido", -1), std::invalid_argument);
    }
}

TEST_SUITE("Inimigos Comuns - Comportamento e Dano") {

    TEST_CASE("Desafiante do Bar causa dano ao aventureiro no seu turno") {
        Aventureiro a("Herói", 200, 0, 10);
        DesafianteDoBar d("Bêbado", 1);
        int hpAntes = a.getHP();

        d.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Trabalhador Noturno causa dano ao aventureiro no seu turno") {
        Aventureiro a("Herói", 200, 0, 10);
        TrabalhadorNoturno t("Trabalhador", 1);
        int hpAntes = a.getHP();

        t.executarTurno(a);
        CHECK(a.getHP() < hpAntes);
    }

    TEST_CASE("Inimigo comum recebe dano e HP diminui") {
        SegurancaDeBalada s("Segurança", 1);
        int hpAntes = s.getHP();

        s.receberDano(15, TipoHabilidade::FISICO);
        CHECK(s.getHP() < hpAntes);
    }

    TEST_CASE("Inimigo comum morre com dano letal") {
        DesafianteDoBar d("Bêbado", 1);
        d.receberDano(9999, TipoHabilidade::FISICO);
        CHECK(d.estaVivo() == false);
        CHECK(d.getHP() == 0);
    }
}

TEST_SUITE("Inimigos Comuns - Escalonamento") {

    TEST_CASE("Segurança de Balada nível 3 é mais forte que nível 1") {
        SegurancaDeBalada s1("Segurança Novato", 1);
        SegurancaDeBalada s3("Segurança Veterano", 3);
        
        CHECK(s3.getHP() > s1.getHP());
        CHECK(s3.getXPRecompensa() > s1.getXPRecompensa());
    }
}