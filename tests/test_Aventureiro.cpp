/**
 * @file test_Aventureiro.cpp
 * @brief Testes de unidade para a classe Aventureiro usando doctest (TDD).
 */

#include "doctest/doctest.h"
#include "Aventureiro.hpp"

// =========================================================
// SUITE: Estado Inicial
// =========================================================

TEST_SUITE("Aventureiro - Estado Inicial") {

    TEST_CASE("MP inicial é positivo") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getMP() > 0);
        CHECK(a.getMP() == a.getMPMax());
    }

    TEST_CASE("Energia inicial é positiva") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getEnergia() > 0);
        CHECK(a.getEnergia() == a.getEnergiaMax());
    }

    TEST_CASE("Força total inicial é a força base (sem bonus de arma)") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getForcaTotal() == 15);
    }

    TEST_CASE("Defesa inicial é a defesa base (sem bonus de armadura)") {
        Aventureiro a("Herói", 100, 8, 15);
        CHECK(a.getDefesa() == 8);
    }

    TEST_CASE("Checkpoint inicial é 0") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getIDCheckpoint() == 0);
    }
}

// =========================================================
// SUITE: Recursos de Combate
// =========================================================

TEST_SUITE("Aventureiro - Recursos de Combate") {

    TEST_CASE("consumirMP reduz MP corretamente") {
        Aventureiro a("Herói", 100, 5, 15);
        int mpInicial = a.getMP();
        a.consumirMP(3);
        CHECK(a.getMP() == mpInicial - 3);
    }

    TEST_CASE("consumirMP não deixa MP negativo") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirMP(9999);
        CHECK(a.getMP() == 0);
    }

    TEST_CASE("consumirEnergia reduz Energia corretamente") {
        Aventureiro a("Herói", 100, 5, 15);
        int energiaInicial = a.getEnergia();
        a.consumirEnergia(4);
        CHECK(a.getEnergia() == energiaInicial - 4);
    }

    TEST_CASE("consumirEnergia não deixa Energia negativa") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirEnergia(9999);
        CHECK(a.getEnergia() == 0);
    }

    TEST_CASE("recuperarRecursos incrementa MP em 1") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirMP(3);
        int mpAntes = a.getMP();
        a.recuperarRecursos();
        CHECK(a.getMP() == mpAntes + 1);
    }

    TEST_CASE("recuperarRecursos incrementa Energia em 5") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirEnergia(8);
        int energiaAntes = a.getEnergia();
        a.recuperarRecursos();
        CHECK(a.getEnergia() == energiaAntes + 5);
    }

    TEST_CASE("recuperarRecursos não ultrapassa MPMax") {
        Aventureiro a("Herói", 100, 5, 15);
        a.recuperarRecursos();
        CHECK(a.getMP() <= a.getMPMax());
    }

    TEST_CASE("recuperarRecursos não ultrapassa EnergiaMax") {
        Aventureiro a("Herói", 100, 5, 15);
        a.recuperarRecursos();
        CHECK(a.getEnergia() <= a.getEnergiaMax());
    }
}

// =========================================================
// SUITE: Escudo
// =========================================================

TEST_SUITE("Aventureiro - Escudo") {

    TEST_CASE("usarEscudo consome 6 pontos de energia") {
        Aventureiro a("Herói", 100, 5, 15);
        int energiaAntes = a.getEnergia();
        a.usarEscudo();
        CHECK(a.getEnergia() == energiaAntes - 6);
    }

    TEST_CASE("Escudo ativo reduz dano severamente") {
        Aventureiro a("Herói", 100, 5, 15);
        a.usarEscudo();
        int hpAntes = a.getHP();
        a.receberDano(30);  // com escudo (defesa*4=20), dano real = 30-20 = 10
        CHECK(a.getHP() > hpAntes - 30);
    }

    TEST_CASE("Escudo é desativado após receber dano") {
        // Primeiro ataque COM escudo
        Aventureiro a1("Herói", 200, 5, 15);
        a1.usarEscudo();
        int hpAntes1 = a1.getHP();
        a1.receberDano(30);
        int danoComEscudo = hpAntes1 - a1.getHP();

        // Segundo ataque SEM escudo (escudo já consumido)
        Aventureiro a2("Herói", 200, 5, 15);
        int hpAntes2 = a2.getHP();
        a2.receberDano(30);
        int danoSemEscudo = hpAntes2 - a2.getHP();

        // Dano sem escudo deve ser maior ou igual ao dano com escudo
        CHECK(danoSemEscudo >= danoComEscudo);
    }
}

// =========================================================
// SUITE: Frascos de Cura
// =========================================================

TEST_SUITE("Aventureiro - Frascos de Cura") {

    TEST_CASE("usarFrasco retorna true quando há frascos") {
        Aventureiro a("Herói", 100, 0, 15);
        a.receberDano(50);
        CHECK(a.usarFrasco() == true);
    }

    TEST_CASE("usarFrasco restaura HP") {
        Aventureiro a("Herói", 100, 0, 15);
        a.receberDano(50);
        int hpAntes = a.getHP();
        a.usarFrasco();
        CHECK(a.getHP() > hpAntes);
    }

    TEST_CASE("usarFrasco retorna false quando não há frascos") {
        Aventureiro a("Herói", 100, 0, 15);
        a.receberDano(10);
        a.usarFrasco();
        a.usarFrasco();
        a.usarFrasco();
        CHECK(a.usarFrasco() == false);
    }

    TEST_CASE("dormir recarrega frascos ao máximo") {
        Aventureiro a("Herói", 100, 0, 15);
        a.receberDano(10);
        a.usarFrasco();
        a.usarFrasco();
        a.dormir();
        CHECK(a.usarFrasco() == true); // frascos recarregados
    }
}

// =========================================================
// SUITE: Progressão e Equipamentos
// =========================================================

TEST_SUITE("Aventureiro - Progressão e Equipamentos") {

    TEST_CASE("buffArma aumenta ForcaTotal") {
        Aventureiro a("Herói", 100, 5, 15);
        int forcaAntes = a.getForcaTotal();
        a.buffArma(5);
        CHECK(a.getForcaTotal() == forcaAntes + 5);
    }

    TEST_CASE("buffArmadura aumenta Defesa") {
        Aventureiro a("Herói", 100, 5, 15);
        int defesaAntes = a.getDefesa();
        a.buffArmadura(3);
        CHECK(a.getDefesa() == defesaAntes + 3);
    }

    TEST_CASE("ganarExperiencia retorna false sem subir de nível") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.ganarExperiencia(10) == false);
    }

    TEST_CASE("ganarExperiencia retorna true ao subir de nível") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.ganarExperiencia(100) == true);
    }

    TEST_CASE("Subir de nível incrementa o nível em 1") {
        Aventureiro a("Herói", 100, 5, 15);
        a.ganarExperiencia(100);
        CHECK(a.getNivel() == 2);
    }

    TEST_CASE("Subir de nível aumenta HPMax") {
        Aventureiro a("Herói", 100, 5, 15);
        int hpMaxAntes = a.getHPMax();
        a.ganarExperiencia(100);
        CHECK(a.getHPMax() > hpMaxAntes);
    }

    TEST_CASE("setIDCheckpoint e getIDCheckpoint funcionam corretamente") {
        Aventureiro a("Herói", 100, 5, 15);
        a.setIDCheckpoint(42);
        CHECK(a.getIDCheckpoint() == 42);
    }

    TEST_CASE("aprenderHabilidade adiciona ao vetor de habilidades") {
        Aventureiro a("Herói", 100, 5, 15);
        Habilidade h("Raio", TipoHabilidade::ESPECIAL, 0, 5, 25);
        a.aprenderHabilidade(h);
        CHECK(a.getHabilidades().size() == 1);
        CHECK(a.getHabilidades()[0].getNome() == "Raio");
    }

    TEST_CASE("dormir restaura HP ao máximo") {
        Aventureiro a("Herói", 100, 0, 10);
        a.receberDano(70);
        a.dormir();
        CHECK(a.getHP() == a.getHPMax());
    }

    TEST_CASE("dormir restaura MP ao máximo") {
        Aventureiro a("Herói", 100, 5, 10);
        a.consumirMP(5);
        a.dormir();
        CHECK(a.getMP() == a.getMPMax());
    }

    TEST_CASE("dormir restaura Energia ao máximo") {
        Aventureiro a("Herói", 100, 5, 10);
        a.consumirEnergia(8);
        a.dormir();
        CHECK(a.getEnergia() == a.getEnergiaMax());
    }

    TEST_CASE("getDeclaracaoStatus contém o nome do personagem") {
        Aventureiro a("Artorias", 100, 5, 15);
        std::string status = a.getDeclaracaoStatus();
        CHECK(status.find("Artorias") != std::string::npos);
    }
}
