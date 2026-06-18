/**
 * @file test_Personagem.cpp
 * @brief Testes de unidade focados em extrair 100% de cobertura da classe base Personagem.
 */

#include "doctest.h"
#include "Aventureiro.hpp"
#include "Habilidade.hpp"
#include <stdexcept>

// Usamos Aventureiro para instanciar a base Personagem nos testes
// já que as assinaturas herdam diretamente a estrutura.

// =========================================================
// SUITE 1: Construtor e Exceções (Linhas 13, 22, 25)
// =========================================================
TEST_SUITE("Personagem - Construtor") {

    TEST_CASE("Exceção - Nome Vazio") {
        CHECK_THROWS_AS(Aventureiro("", 100, 10, 10), std::invalid_argument);
    }

    TEST_CASE("Exceção - HP Inválido") {
        CHECK_THROWS_AS(Aventureiro("Heroi", 0, 10, 10), std::invalid_argument);
        CHECK_THROWS_AS(Aventureiro("Heroi", -5, 10, 10), std::invalid_argument);
    }

    TEST_CASE("Exceção - Defesa Inválida") {
        CHECK_THROWS_AS(Aventureiro("Heroi", 100, 0, 10), std::invalid_argument);
        CHECK_THROWS_AS(Aventureiro("Heroi", 100, -1, 10), std::invalid_argument);
    }

    TEST_CASE("Exceção - Força Inválida") {
        CHECK_THROWS_AS(Aventureiro("Heroi", 100, 10, 0), std::invalid_argument);
        CHECK_THROWS_AS(Aventureiro("Heroi", 100, 10, -3), std::invalid_argument);
    }
}

// =========================================================
// SUITE 2: Combate e Escudos (Linhas 39-44, 62-70)
// =========================================================
TEST_SUITE("Personagem - Mecânicas de Escudo e Dano Mínimo") {

    TEST_CASE("Dano Nulo ou Negativo - Retorno Imediato") {
        Aventureiro p("Alvo", 100, 10, 10);
        p.receberDano(0, TipoHabilidade::FISICO);
        p.receberDano(-10, TipoHabilidade::FISICO);
        CHECK(p.getHP() == 100);
    }

    TEST_CASE("Escudo Ativo vs Ataque ULTRA (Ignora)") {
        Aventureiro p("Alvo", 100, 0, 10); // Defesa 0 para simplificar matemática
        p.usarEscudo(); 
        
        p.receberDano(40, TipoHabilidade::ULTRA);
        CHECK(p.getHP() == 60); // 40 de dano integral sofrido
    }

    TEST_CASE("Escudo Ativo vs Ataque DoT (Ignora)") {
        Aventureiro p("Alvo", 100, 0, 10);
        p.usarEscudo();
        
        p.receberDano(20, TipoHabilidade::DOT);
        CHECK(p.getHP() == 80); // 20 de dano integral sofrido
    }

    TEST_CASE("Escudo Ativo vs Ataques Comuns (Mitiga 50%)") {
        Aventureiro p("Alvo", 100, 0, 10);
        p.usarEscudo();
        
        p.receberDano(30, TipoHabilidade::FISICO);
        CHECK(p.getHP() == 85); // 30 * 0.5 = 15 de dano sofrido
    }

    TEST_CASE("Garantia de Dano Mínimo") {
        // Defesa extremamente alta (9999) para forçar danoCalculado a ser zero
        Aventureiro p("SuperDefesa", 100, 9999, 10);
        
        p.receberDano(10, TipoHabilidade::FISICO);
        // O dano final seria zero, mas a trava obriga a receber pelo menos 1 de dano
        CHECK(p.getHP() == 99); 
    }
}

// =========================================================
// SUITE 3: Travas de Efeitos (Linhas 85, 93, 105-108)
// =========================================================
TEST_SUITE("Personagem - Travas de Buffs e Status") {

    TEST_CASE("Buffs e Debuffs com valores inválidos") {
        Aventureiro p("Heroi", 100, 10, 10);
        
        p.aplicarBuff("Invalido", "forca", 0, 3);
        p.aplicarBuff("Invalido2", "forca", 5, 0);
        p.aplicarDebuff("Invalido3", "defesa", 0, 3);
        p.aplicarDebuff("Invalido4", "defesa", 5, -1);
        
        // Atributos precisam continuar intactos
        CHECK(p.getForcaTotal() == 10);
        CHECK(p.getDefesa() == 10);
    }

    TEST_CASE("Aplicar DoT/HoT em Personagem Morto") {
        Aventureiro p("Cadaver", 100, 10, 10);
        p.receberDano(500, TipoHabilidade::FISICO); // Reduz HP a 0 e mata
        
        p.aplicarDoT("Veneno Tardio", 10, 3);
        p.aplicarHoT("Ressurreicao Tentativa", 10, 3);
        
        // Processar não deve fazer nada já que as travas impediram a inserção
        p.processarEfeitosContinuos();
        CHECK(p.getHP() == 0);
    }
}

// =========================================================
// SUITE 4: Ciclo e Expiração (Linhas 134-209)
// =========================================================
TEST_SUITE("Personagem - Loops de Processamento de Turno") {

    TEST_CASE("Efeitos Contínuos Agindo e Expirando por Iterador") {
        Aventureiro p("Alvo", 100, 0, 10);
        
        p.aplicarDoT("Sangramento", 10, 1); // Dura 1 turno
        p.aplicarHoT("Regen", 5, 2);        // Dura 2 turnos

        // Turno 1: Sangramento causa 10 de dano, Regen cura 5. Net = -5.
        // Sangramento chega a 0 turnos e cai no erase(it)
        p.processarEfeitosContinuos();
        CHECK(p.getHP() == 95);

        // Turno 2: Apenas Regen age (+5 de HP) e expira
        p.processarEfeitosContinuos();
        CHECK(p.getHP() == 100);

        // Turno 3: Vetor vazio, não faz nada
        p.processarEfeitosContinuos();
        CHECK(p.getHP() == 100);
    }

    TEST_CASE("Modificadores Acumulados Agindo simultaneamente") {
        Aventureiro p("Alvo", 100, 10, 10);
        
        p.aplicarBuff("Furia", "forca", 5, 1);
        p.aplicarDebuff("Fraqueza", "forca", 2, 2);
        p.aplicarBuff("Escudo Divino", "defesa", 4, 1);
        
        // Verifica cálculo com múltiplos modificadores ativos simultâneos
        CHECK(p.getForcaTotal() == 13); // 10 + 5 - 2
        CHECK(p.getDefesa() == 14);    // 10 + 4

        // Avança Turno 1: Furia e Escudo Divino expiram. Fraqueza continua.
        p.processarEfeitosContinuos();
        CHECK(p.getForcaTotal() == 8);  // 10 - 2
        CHECK(p.getDefesa() == 10);     // Voltou ao normal

        // Avança Turno 2: Fraqueza expira
        p.processarEfeitosContinuos();
        CHECK(p.getForcaTotal() == 10); // Força restaurada
    }
}