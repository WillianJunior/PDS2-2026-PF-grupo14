/**
 * @file test_Personagem.cpp
 * @brief Testes de unidade para a classe base Personagem usando doctest (TDD).
 *
 * Como Personagem é abstrata, os testes utilizam a classe Aventureiro como
 * implementação concreta, verificando os comportamentos definidos na base.
 */

#include "doctest/doctest.h"
#include "Personagem.hpp"
#include "Aventureiro.hpp"
#include "Habilidade.hpp"

// =========================================================
// SUITE: Construção e estado inicial
// =========================================================

TEST_SUITE("Personagem - Estado Inicial") {

    TEST_CASE("Nome é armazenado corretamente") {
        Aventureiro a("Artorias", 100, 10, 15);
        CHECK(a.getNome() == "Artorias");
    }

    TEST_CASE("HP inicial é igual ao HPMax") {
        Aventureiro a("Herói", 80, 5, 10);
        CHECK(a.getHP() == 80);
        CHECK(a.getHPMax() == 80);
    }

    TEST_CASE("Nível inicial é 1") {
        Aventureiro a("Herói", 100, 10, 15);
        CHECK(a.getNivel() == 1);
    }

    TEST_CASE("Personagem está vivo ao ser criado") {
        Aventureiro a("Herói", 100, 10, 15);
        CHECK(a.estaVivo() == true);
    }
}

// =========================================================
// SUITE: Receber Dano
// =========================================================

TEST_SUITE("Personagem - Receber Dano") {

    TEST_CASE("Dano maior que defesa reduz HP") {
        Aventureiro a("Herói", 100, 5, 15);
        a.receberDano(20);  // dano real = 20 - 5 = 15
        CHECK(a.getHP() == 85);
    }

    TEST_CASE("Dano igual à defesa não altera HP") {
        Aventureiro a("Herói", 100, 10, 15);
        a.receberDano(10);  // dano real = 0
        CHECK(a.getHP() == 100);
    }

    TEST_CASE("Dano menor que defesa não altera HP") {
        Aventureiro a("Herói", 100, 10, 15);
        a.receberDano(5);
        CHECK(a.getHP() == 100);
    }

    TEST_CASE("HP não fica negativo após dano letal") {
        Aventureiro a("Herói", 10, 0, 5);
        a.receberDano(999);
        CHECK(a.getHP() == 0);
    }

    TEST_CASE("Personagem morre quando HP chega a zero") {
        Aventureiro a("Herói", 10, 0, 5);
        a.receberDano(999);
        CHECK(a.estaVivo() == false);
    }

    TEST_CASE("Múltiplos danos se acumulam corretamente") {
        Aventureiro a("Herói", 100, 0, 10);
        a.receberDano(20);
        a.receberDano(15);
        a.receberDano(10);
        CHECK(a.getHP() == 55);
    }
}

// =========================================================
// SUITE: Receber Cura
// =========================================================

TEST_SUITE("Personagem - Receber Cura") {

    TEST_CASE("Cura restaura HP corretamente") {
        Aventureiro a("Herói", 100, 0, 10);
        a.receberDano(40);
        a.receberCura(20);
        CHECK(a.getHP() == 80);
    }

    TEST_CASE("Cura não ultrapassa HPMax") {
        Aventureiro a("Herói", 100, 0, 10);
        a.receberDano(10);
        a.receberCura(999);
        CHECK(a.getHP() == 100);
    }

    TEST_CASE("Cura não ultrapassa HPMax quando HP está cheio") {
        Aventureiro a("Herói", 100, 0, 10);
        a.receberCura(50);
        CHECK(a.getHP() == 100);
    }
}

// =========================================================
// SUITE: Habilidades (interface de Personagem)
// =========================================================

TEST_SUITE("Personagem - Gerenciamento de Habilidades") {

    TEST_CASE("Personagem começa sem habilidades na lista da base") {
        Aventureiro a("Herói", 100, 5, 10);
        // getHabilidades() da base retorna o vetor privado _habilidades
        const Personagem& p = a;
        CHECK(p.getHabilidades().empty());
    }

    TEST_CASE("adicionarHabilidade (base) inclui habilidade no vetor da base") {
        Aventureiro a("Herói", 100, 5, 10);
        Habilidade h("Soco", TipoHabilidade::FISICO, 3, 0, 10);
        // Acessa via referência à base para usar o vetor correto
        Personagem& p = a;
        p.adicionarHabilidade(h);
        CHECK(p.getHabilidades().size() == 1);
        CHECK(p.getHabilidades()[0].getNome() == "Soco");
    }

    TEST_CASE("Múltiplas habilidades são armazenadas corretamente na base") {
        Aventureiro a("Herói", 100, 5, 10);
        Personagem& p = a;
        p.adicionarHabilidade(Habilidade("Soco", TipoHabilidade::FISICO, 3, 0, 10));
        p.adicionarHabilidade(Habilidade("Bola de Fogo", TipoHabilidade::ESPECIAL, 0, 5, 20));
        CHECK(p.getHabilidades().size() == 2);
    }
}
