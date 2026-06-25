/**
 * @file test_InimigoBoss.cpp
 * @brief Testes de unidade para o Boss TylerDurden — Cobertura e Estados.
 */

#include "doctest.h"
#include "InimigoBoss.hpp"
#include "Aventureiro.hpp"
#include <string>
#include <iostream>

// =========================================================
// SUITE: TylerDurden - Ciclo de Vida e Fases
// =========================================================
TEST_SUITE("TylerDurden - Ciclo de Vida e Fases") {

    TEST_CASE("Fase 1: Fluxo Bruto e Buff de Fúria") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 400, 10, 10);

        // No turno 4 (múltiplo de 4), ele ativa o "Foco Destrutivo" que bufa a força
        // Forçamos 4 turnos na Fase 1 (HP cheio)
        int forcaInicial = boss.getForcaTotal();
        
        boss.executarTurno(alvo); // Turno 1
        boss.executarTurno(alvo); // Turno 2
        boss.executarTurno(alvo); // Turno 3
        boss.executarTurno(alvo); // Turno 4 -> Aplica Buff

        CHECK(boss.getForcaTotal() > forcaInicial);
    }

    TEST_CASE("Fase 2: Transição por HP e Disparo de Habilidades Mentais") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 400, 10, 10);

        // Guardamos a defesa original do herói antes dos debuffs
        int defesaOriginal = alvo.getDefesa();

        // Derruba o HP do Boss para entrar na Fase 2 (HP base é 400, tiramos 220 -> sobra 180, que é 45%)
        while(boss.getHP() >= 180) {
            boss.receberDano(50, TipoHabilidade::FISICO);
        }
        
        // Vamos rodar turnos suficientes até que o _contadorTurnos seja múltiplo de 3
        // para garantir o acionamento do debuff "Humilhação"
        for(int i = 0; i < 4; i++) {
            boss.executarTurno(alvo);
        }

        // Verifica se a defesa do herói foi reduzida pelo debuff
        CHECK(alvo.getDefesa() < defesaOriginal);
    }

TEST_CASE("Gatilhos Críticos: Cura Estóica e Ultra") {
    // --- Teste Isolado 1: O Ultra do Boss ---
    {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 400, 10, 10);

        // Força o HP do alvo para baixo de 30% (120 HP)
        while(boss.getHP() >= 120) {
            boss.receberDano(50, TipoHabilidade::FISICO);
        }

        int hpAntesDoUltra = alvo.getHP();
        boss.executarTurno(alvo); // Ativa Desconstrução Total
        CHECK(alvo.getHP() < hpAntesDoUltra);
    }

    // --- Teste Isolado 2: A Cura Estóica do Boss ---
    {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 400, 10, 10); // Herói com HP cheio para não ativar o Ultra

        // Força o HP do Boss para baixo de 20% (80 HP)
        // Como o HP máximo dele é 400, tirar 330 deixa ele com 70 HP (abaixo de 20%)
         while(boss.getHP() >= 80) {
            boss.receberDano(20, TipoHabilidade::FISICO);
        }
        int hpAntesDaCura = boss.getHP();
        boss.executarTurno(alvo); // Deve entrar no if do hpTylerRatio e curar

        CHECK(boss.getHP() > hpAntesDaCura);
    }
}

    TEST_CASE("Declaração de Status - Cobertura das Duas Ramificações") {
        TylerDurden boss("Tyler", 1);
        Aventureiro alvo("Heroi", 400, 10, 10);

        // --- Ramificação 1: Fase 1 ---
        // CORREÇÃO: Usando getDeclaracaoStatus() que existe no seu .cpp
        std::string statusFase1 = boss.getDeclaracaoStatus();
        CHECK(statusFase1.find("FASE 1: BRUTA") != std::string::npos);

        // --- Ramificação 2: Fase 2 ---
        boss.receberDano(400, TipoHabilidade::FISICO); // Deixa em Fase 2
        boss.executarTurno(alvo); 

        std::string statusFase2 = boss.getDeclaracaoStatus();
        CHECK(statusFase2.find("FASE 2: MENTAL") != std::string::npos);
    }
}