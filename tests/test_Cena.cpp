/**
 * @file test_Cena.cpp
 * @brief Testes de unidade para a classe Cena usando doctest (TDD).
 */

#include "doctest.h"
#include "Cena.hpp"

// =========================================================
// SUITE: Construção e Getters
// =========================================================

TEST_SUITE("Cena - Construção") {

    TEST_CASE("ID é armazenado corretamente") {
        Cena c(1, "Você está na floresta.");
        CHECK(c.getID() == 1);
    }

    TEST_CASE("Texto narrativo é armazenado corretamente") {
        Cena c(2, "Uma taverna aconchegante aparece à sua frente.");
        CHECK(c.getTexto() == "Uma taverna aconchegante aparece à sua frente.");
    }

    TEST_CASE("Cena não é checkpoint por padrão") {
        Cena c(3, "Corredor escuro.");
        CHECK(c.isCheckpoint() == false);
    }

    TEST_CASE("Cena pode ser definida como checkpoint") {
        Cena c(4, "Início do capítulo.", true);
        CHECK(c.isCheckpoint() == true);
    }

    TEST_CASE("Cena começa sem escolhas") {
        Cena c(5, "Fim da jornada.");
        CHECK(c.getEscolhas().empty());
    }
}

// =========================================================
// SUITE: Gerenciamento de Escolhas
// =========================================================

TEST_SUITE("Cena - Escolhas") {

    TEST_CASE("adicionarEscolha inclui uma opção na cena") {
        Cena c(1, "Uma bifurcação.");
        Escolha e{"Ir para a direita", 2, false, "", 0};
        c.adicionarEscolha(e);
        CHECK(c.getEscolhas().size() == 1);
    }

    TEST_CASE("Texto da escolha é armazenado corretamente") {
        Cena c(1, "Uma bifurcação.");
        Escolha e{"Ir para a esquerda", 3, false, "", 0};
        c.adicionarEscolha(e);
        
        if (!c.getEscolhas().empty()) {
            CHECK(c.getEscolhas()[0].texto == "Ir para a esquerda");
        } else {
            CHECK(false); // Falha esperada no TDD (esqueleto vazio) sem dar Crash
        }
    }

    TEST_CASE("Destino da escolha é armazenado corretamente") {
        Cena c(1, "Floresta.");
        Escolha e{"Avançar", 5, false, "", 0};
        c.adicionarEscolha(e);
        
        if (!c.getEscolhas().empty()) {
            CHECK(c.getEscolhas()[0].destinoID == 5);
        } else {
            CHECK(false);
        }
    }

    TEST_CASE("Escolha com combate armazena flag geraCombate=true") {
        Cena c(1, "Um inimigo aparece!");
        Escolha e{"Lutar", 2, true, "Goblin", 0};
        c.adicionarEscolha(e);
        
        if (!c.getEscolhas().empty()) {
            CHECK(c.getEscolhas()[0].geraCombate == true);
        } else {
            CHECK(false);
        }
    }

    TEST_CASE("Escolha armazena tipo de inimigo corretamente") {
        Cena c(1, "Um monstro bloqueia o caminho.");
        Escolha e{"Enfrentar o Slime", 2, true, "Slime", 0};
        c.adicionarEscolha(e);
        
        if (!c.getEscolhas().empty()) {
            CHECK(c.getEscolhas()[0].tipoInimigo == "Slime");
        } else {
            CHECK(false);
        }
    }

    TEST_CASE("Escolha armazena itemGanhoID corretamente") {
        Cena c(1, "Você encontra um baú.");
        Escolha e{"Abrir o baú", 2, false, "", 7};
        c.adicionarEscolha(e);
        
        if (!c.getEscolhas().empty()) {
            CHECK(c.getEscolhas()[0].itemGanhoID == 7);
        } else {
            CHECK(false);
        }
    }

    TEST_CASE("Múltiplas escolhas são armazenadas corretamente") {
        Cena c(1, "Uma encruzilhada.");
        c.adicionarEscolha({"Ir para o norte", 2, false, "", 0});
        c.adicionarEscolha({"Ir para o sul",   3, false, "", 0});
        c.adicionarEscolha({"Descansar aqui",  1, false, "", 0});
        CHECK(c.getEscolhas().size() == 3);
    }

    TEST_CASE("Ordem das escolhas é preservada - Elemento 1") {
        Cena c(1, "Uma praça.");
        c.adicionarEscolha({"Primeira opção", 2, false, "", 0});
        c.adicionarEscolha({"Segunda opção",  3, false, "", 0});
        
        if (c.getEscolhas().size() >= 2) {
            CHECK(c.getEscolhas()[0].texto == "Primeira opção");
        } else {
            CHECK(false);
        }
    }

    TEST_CASE("Ordem das escolhas é preservada - Elemento 2") {
        Cena c(1, "Uma praça.");
        c.adicionarEscolha({"Primeira opção", 2, false, "", 0});
        c.adicionarEscolha({"Segunda opção",  3, false, "", 0});
        
        if (c.getEscolhas().size() >= 2) {
            CHECK(c.getEscolhas()[1].texto == "Segunda opção");
        } else {
            CHECK(false);
        }
    }

    TEST_CASE("Escolha sem combate tem geraCombate=false") {
        Cena c(1, "Um caminho tranquilo.");
        Escolha e{"Continuar", 2, false, "", 0};
        c.adicionarEscolha(e);
        
        if (!c.getEscolhas().empty()) {
            CHECK(c.getEscolhas()[0].geraCombate == false);
        } else {
            CHECK(false);
        }
    }

    TEST_CASE("Escolha sem item tem itemGanhoID=0") {
        Cena c(1, "Sala vazia.");
        Escolha e{"Sair da sala", 2, false, "", 0};
        c.adicionarEscolha(e);
        
        if (!c.getEscolhas().empty()) {
            CHECK(c.getEscolhas()[0].itemGanhoID == 0);
        } else {
            CHECK(false);
        }
    }
}

// =========================================================
// SUITE: Diferentes IDs e cenas
// =========================================================

TEST_SUITE("Cena - Identificação") {

    TEST_CASE("Duas cenas com IDs diferentes são distintas") {
        Cena c1(10, "Cena dez.");
        Cena c2(20, "Cena vinte.");
        CHECK(c1.getID() != c2.getID());
    }

    TEST_CASE("Cena com ID zero é válida") {
        Cena c(0, "Cena inicial.");
        CHECK(c.getID() == 0);
    }
}