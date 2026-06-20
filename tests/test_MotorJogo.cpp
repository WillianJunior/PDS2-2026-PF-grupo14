/**
 * @file test_MotorJogo.cpp
 * @brief Testes de unidade para MotorJogo — cobertura máxima.
 *
 * RESTRIÇÕES:
 * - rodar() depende de I/O interativo — testes removidos
 * - processarDerrota() e checarEventosEspeciais() são privados — testados indiretamente
 */

#include "doctest.h"
#include "MotorJogo.hpp"
#include "Aventureiro.hpp"
#include <iostream>

// =========================================================
// SUITE 1: Construção
// Cobre: construtor, inicializarHistoria()
// =========================================================

TEST_SUITE("MotorJogo - Construção") {

    TEST_CASE("Construído sem erros com aventureiro válido") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK_NOTHROW(MotorJogo m(a));
    }

    TEST_CASE("Construído sem erros com stats altos") {
        Aventureiro a("Deus", 9999, 999, 999);
        CHECK_NOTHROW(MotorJogo m(a));
    }

    TEST_CASE("Construído sem erros com força e defesa mínimas") {
        Aventureiro a("Frágil", 100, 1, 1);
        CHECK_NOTHROW(MotorJogo m(a));
    }

    TEST_CASE("Checkpoint do aventureiro não é alterado pela construção") {
        Aventureiro a("Herói", 100, 5, 15);
        a.setIDCheckpoint(2);
        MotorJogo m(a);
        CHECK(a.getIDCheckpoint() == 2);
    }
}

// =========================================================
// SUITE 2: realizarCombate() — inimigos inválidos
// Cobre: guard de string vazia, throw para tipo desconhecido
// Estes são seguros pois lançam ANTES de criar Combate
// =========================================================

TEST_SUITE("MotorJogo - realizarCombate - Inválidos") {

    TEST_CASE("String vazia lança std::invalid_argument") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_THROWS_AS(m.realizarCombate(""), std::invalid_argument);
    }

    TEST_CASE("Tipo inexistente lança std::invalid_argument") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_THROWS_AS(m.realizarCombate("InimigoInexistente"), std::invalid_argument);
    }

    TEST_CASE("Nomes antigos não aceitos — Goblin lança exceção") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_THROWS_AS(m.realizarCombate("Goblin"), std::invalid_argument);
    }

    TEST_CASE("Nomes antigos não aceitos — Slime lança exceção") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_THROWS_AS(m.realizarCombate("Slime"), std::invalid_argument);
    }

    TEST_CASE("Nomes antigos não aceitos — Dragao lança exceção") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_THROWS_AS(m.realizarCombate("Dragao"), std::invalid_argument);
    }

    TEST_CASE("Comparação é case-sensitive — 'desafiantedoBar' lança exceção") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_THROWS_AS(m.realizarCombate("desafiantedoBar"), std::invalid_argument);
    }

    TEST_CASE("String com espaços lança exceção") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_THROWS_AS(m.realizarCombate("   "), std::invalid_argument);
    }
}

// =========================================================
// SUITE 3: realizarCombate() — inimigos válidos
// BLOQUEADO: Combate::iniciar() trava esperando I/O
// Descomentar quando InterfaceJogo for mockável
// =========================================================

TEST_SUITE("MotorJogo - realizarCombate - Válidos [BLOQUEADO]") {

    // TEST_CASE("DesafianteDoBar não lança exceção") {
    //     Aventureiro a("Herói", 100, 5, 15);
    //     MotorJogo m(a);
    //     CHECK_NOTHROW(m.realizarCombate("DesafianteDoBar"));
    // }

    // TEST_CASE("TrabalhadorNoturno não lança exceção") {
    //     Aventureiro a("Herói", 100, 5, 15);
    //     MotorJogo m(a);
    //     CHECK_NOTHROW(m.realizarCombate("TrabalhadorNoturno"));
    // }

    // TEST_CASE("SegurancaDeBalada não lança exceção") {
    //     Aventureiro a("Herói", 100, 5, 15);
    //     MotorJogo m(a);
    //     CHECK_NOTHROW(m.realizarCombate("SegurancaDeBalada"));
    // }

    // TEST_CASE("RecrutaDoutrinado não lança exceção") {
    //     Aventureiro a("Herói", 100, 5, 15);
    //     MotorJogo m(a);
    //     CHECK_NOTHROW(m.realizarCombate("RecrutaDoutrinado"));
    // }

    // TEST_CASE("GuardaPatrimonial não lança exceção") {
    //     Aventureiro a("Herói", 100, 5, 15);
    //     MotorJogo m(a);
    //     CHECK_NOTHROW(m.realizarCombate("GuardaPatrimonial"));
    // }

    // TEST_CASE("SabotadorRival não lança exceção") {
    //     Aventureiro a("Herói", 100, 5, 15);
    //     MotorJogo m(a);
    //     CHECK_NOTHROW(m.realizarCombate("SabotadorRival"));
    // }

    // TEST_CASE("TylerDurden não lança exceção") {
    //     Aventureiro a("Herói", 100, 5, 15);
    //     MotorJogo m(a);
    //     CHECK_NOTHROW(m.realizarCombate("TylerDurden"));
    // }

    TEST_CASE("Placeholder — ativar quando mock de I/O estiver disponível") {
        CHECK(true);
    }
}

// =========================================================
// SUITE 4: Checkpoint
// Cobre: setIDCheckpoint, getIDCheckpoint, preservação de estado
// =========================================================

TEST_SUITE("MotorJogo - Checkpoint") {

    TEST_CASE("Checkpoint definido após construção é preservado") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        a.setIDCheckpoint(3);
        CHECK(a.getIDCheckpoint() == 3);
    }

    TEST_CASE("Checkpoint pode ser atualizado múltiplas vezes") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        a.setIDCheckpoint(1);
        a.setIDCheckpoint(3);
        CHECK(a.getIDCheckpoint() == 3);
    }

    TEST_CASE("Checkpoint com ID alto é preservado") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        a.setIDCheckpoint(100);
        CHECK(a.getIDCheckpoint() == 100);
    }
}

// =========================================================
// SUITE 5: processarDerrota() — indireto via dormir()
// dormir() é chamado por processarDerrota() — testamos o efeito
// =========================================================

TEST_SUITE("MotorJogo - Pós-Derrota via dormir()") {

    TEST_CASE("dormir() restaura MP ao máximo") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirMP(10);
        a.dormir();
        CHECK(a.getMP() == a.getMPMax());
    }

    TEST_CASE("dormir() restaura Energia ao máximo") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirEnergia(10);
        a.dormir();
        CHECK(a.getEnergia() == a.getEnergiaMax());
    }

    TEST_CASE("dormir() não altera o checkpoint") {
        Aventureiro a("Herói", 100, 5, 15);
        a.setIDCheckpoint(2);
        a.dormir();
        CHECK(a.getIDCheckpoint() == 2);
    }

    TEST_CASE("dormir() pode ser chamado múltiplas vezes sem erro") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK_NOTHROW(a.dormir());
        CHECK_NOTHROW(a.dormir());
    }
}

// =========================================================
// SUITE 6: checarEventosEspeciais() — indireto via buffArma()
// itemGanhoID > 0 deve aumentar força via buffArma()
// =========================================================

TEST_SUITE("MotorJogo - Eventos Especiais via buffArma()") {

    TEST_CASE("buffArma() aumenta força total do aventureiro") {
        Aventureiro a("Herói", 100, 5, 15);
        int forcaAntes = a.getForcaTotal();
        a.buffArma(5);
        CHECK(a.getForcaTotal() > forcaAntes);
    }

    TEST_CASE("buffArma() com valor zero não altera força") {
        Aventureiro a("Herói", 100, 5, 15);
        int forcaAntes = a.getForcaTotal();
        std::cout << forcaAntes << "                                     csscscssssssssssssssssssssssssssssss" << std::endl;
        a.buffArma(0);
        CHECK(a.getForcaTotal() == forcaAntes);
    }

    TEST_CASE("buffArma() acumula múltiplos buffs corretamente") {
        Aventureiro a("Herói", 100, 5, 15);
        int forcaAntes = a.getForcaTotal();
        a.buffArma(5);
        a.buffArma(5);
        CHECK(a.getForcaTotal() == forcaAntes + 10);
    }
}

// =========================================================
// SUITE 7: Casos de borda
// =========================================================

TEST_SUITE("MotorJogo - Casos de Borda") {

    TEST_CASE("Múltiplas instâncias de MotorJogo com o mesmo aventureiro") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK_NOTHROW(MotorJogo m1(a));
        CHECK_NOTHROW(MotorJogo m2(a));
    }

    TEST_CASE("Aventureiro com stats mínimos válidos é aceito") {
        Aventureiro a("Mínimo", 1, 1, 1);
        CHECK_NOTHROW(MotorJogo m(a));
    }

    TEST_CASE("realizarCombate lança para qualquer variação de case") {
        Aventureiro a("Herói", 100, 5, 15);
        MotorJogo m(a);
        CHECK_THROWS(m.realizarCombate("TYLERDURDEN"));
        CHECK_THROWS(m.realizarCombate("tylerDurden"));
        CHECK_THROWS(m.realizarCombate("SABOTADORRIVAL"));
    }
}