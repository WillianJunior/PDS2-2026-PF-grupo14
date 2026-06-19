/**
 * @file test_Aventureiro.cpp
 * @brief Testes de unidade estruturais de alta cobertura para Aventureiro e Personagem.
 */

#include "doctest.h"
#include "Aventureiro.hpp"
#include "Habilidade.hpp"
#include <stdexcept>
#include <iostream>

// =========================================================
// SUITE 1: ESTADO INICIAL E CONSTRUTORES
// =========================================================
TEST_SUITE("Aventureiro - Estado Inicial e Exceções") {

    TEST_CASE("Valores Iniciais e Tetos Máximos") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getMP() > 0);
        CHECK(a.getMP() == a.getMPMax());
        CHECK(a.getEnergia() > 0);
        CHECK(a.getEnergia() == a.getEnergiaMax());
        CHECK(a.getForcaTotal() == 15);
        CHECK(a.getDefesa() == 5);
        CHECK(a.getIDCheckpoint() == 0);
        CHECK(a.estaVivo());
        CHECK(a.getNome() == "Herói");
    }

    TEST_CASE("Exceções das Guardas do Construtor Base") {
        CHECK_THROWS_AS(Aventureiro("", 100, 5, 15), std::invalid_argument);
        CHECK_THROWS_AS(Aventureiro("Herói", 0, 5, 15), std::invalid_argument);
        CHECK_THROWS_AS(Aventureiro("Herói", 100, 0, 15), std::invalid_argument);
        CHECK_THROWS_AS(Aventureiro("Herói", 100, 5, 0), std::invalid_argument);
    }
}

// =========================================================
// SUITE 2: GERENCIAMENTO DE RECURSOS (MP, ENERGIA, CURA)
// =========================================================
TEST_SUITE("Aventureiro - Controle de Recursos") {

    TEST_CASE("Consumo e Limites Inferiores (Zero)") {
        Aventureiro a("Herói", 100, 5, 15);
        int mpIni = a.getMP();
        int engIni = a.getEnergia();

        a.consumirMP(3);
        CHECK(a.getMP() == mpIni - 3);

        a.consumirEnergia(4);
        CHECK(a.getEnergia() == engIni - 4);

        // Força estouro para testar travas de limite inferior
        a.consumirMP(9999);
        a.consumirEnergia(9999);
        CHECK(a.getMP() == 0);
        CHECK(a.getEnergia() == 0);
    }

    TEST_CASE("Recuperação Progressiva e Limites Máximos") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirMP(10);
        a.consumirEnergia(20);

        int mpAntes = a.getMP();
        int engAntes = a.getEnergia();

        a.recuperarRecursos();
        CHECK(a.getMP() == mpAntes + 1);
        CHECK(a.getEnergia() == engAntes + 5);

        // Força estouro para testar travas de limite superior (Teto Máximo)
        for (int i = 0; i < 50; i++) {
            a.recuperarRecursos();
        }
        CHECK(a.getMP() <= a.getMPMax());
        CHECK(a.getEnergia() <= a.getEnergiaMax());
    }

    TEST_CASE("Sistema de Recebimento de Cura e Mecânica de Sono") {
        // Alterado defesa de 0 para 1 para evitar a exceção do construtor base
        Aventureiro a("Herói", 100, 1, 15);
        a.receberDano(40, TipoHabilidade::FISICO); // Reduz HP
        int hpAntes = a.getHP();

        a.receberCura(20);
        CHECK(a.getHP() == hpAntes + 20);

        a.receberCura(999); // Deve travar no HP Max inicial
        CHECK(a.getHP() == a.getHPMax());

        a.consumirMP(30);
        a.consumirEnergia(40);
        a.dormir();
        CHECK(a.getHP() == a.getHPMax());
        CHECK(a.getMP() == a.getMPMax());
        CHECK(a.getEnergia() == a.getEnergiaMax());
    }
}

// =========================================================
// SUITE 3: MATRIZ DE DANOS, MITIGAÇÕES E ESCUDOS
// =========================================================
TEST_SUITE("Aventureiro & Personagem - Combate Avançado") {

    TEST_CASE("Retorno Imediato e Casos de Borda de Dano") {
        Aventureiro a("Herói", 100, 10, 15);
        a.receberDano(0, TipoHabilidade::FISICO);
        a.receberDano(-5, TipoHabilidade::FISICO);
        CHECK(a.getHP() == 100);
    }

    TEST_CASE("Mitigação Base vs Modificadores de Escudo") {
        // Defesa alterada para 1 para passar na verificação defensiva
        Aventureiro a1("Alvo1", 100, 1, 15);
        a1.receberDano(20, TipoHabilidade::FISICO);
        CHECK(a1.getHP() < 100);

        // Escudo Ativo vs Ataque Comum
        Aventureiro a2("Alvo2", 100, 1, 15);
        a2.usarEscudo(); 
        a2.receberDano(20, TipoHabilidade::FISICO); 
        CHECK(a2.getHP() < 100);

        // Escudo Ativo vs ULTRA
        Aventureiro a3("Alvo3", 100, 1, 15);
        a3.usarEscudo();
        a3.receberDano(20, TipoHabilidade::ULTRA);
        CHECK(a3.getHP() < 100);

        // Escudo Ativo vs DOT
        Aventureiro a4("Alvo4", 100, 1, 15);
        a4.usarEscudo();
        a4.receberDano(20, TipoHabilidade::DOT);
        CHECK(a4.getHP() < 100);
    }

    TEST_CASE("Garantia de Dano Mínimo") {
        Aventureiro tanque("SuperTanque", 100, 9999, 15);
        tanque.receberDano(10, TipoHabilidade::FISICO);
        CHECK(tanque.getHP() == 99);
    }

    TEST_CASE("Gerenciamento do Estado Vital (Morte)") {
        Aventureiro a("Herói", 100, 1, 15);
        a.receberDano(250, TipoHabilidade::FISICO); 
        
        CHECK(a.getHP() == 0);
        CHECK_FALSE(a.estaVivo());

        a.aplicarDoT("Sangramento", 10, 3);
        a.aplicarHoT("Regen", 10, 3);
        a.processarEfeitosContinuos();
        CHECK(a.getHP() == 0);
    }
}

// =========================================================
// SUITE 4: BUFFS, DEBUFFS E EFEITOS PERIÓDICOS
// =========================================================
TEST_SUITE("Aventureiro & Personagem - Status por Tempo") {

    TEST_CASE("Validações de Guardas de Atributos") {
        Aventureiro a("Herói", 100, 10, 15);
        a.aplicarBuff("Inv1", "forca", 0, 3);
        a.aplicarBuff("Inv2", "forca", 5, 0);
        a.aplicarDebuff("Inv3", "defesa", 0, 3);
        a.aplicarDebuff("Inv4", "defesa", 5, -1);

        CHECK(a.getForcaTotal() == 15);
        CHECK(a.getDefesa() == 10);
    }

    TEST_CASE("Acúmulo de Status e Limite Inferior de Defesa") {
        Aventureiro a("Herói", 100, 10, 15);
        a.aplicarBuff("Fúria", "forca", 5, 2);
        a.aplicarDebuff("Fraqueza", "forca", 2, 2);
        CHECK(a.getForcaTotal() == 18); 

        a.aplicarDebuff("Exposto", "defesa", 30, 2);
        CHECK(a.getDefesa() == 0); 
    }

 TEST_CASE("Buff de força dura a quantidade correta de turnos") {
    Aventureiro a("Herói", 100, 1, 15);

    CHECK(a.getForcaTotal() == 15);

    a.aplicarBuff("Surto", "forca", 10, 2);

    CHECK(a.getForcaTotal() == 25);

    // Após 1 turno ainda deve estar ativo
    a.processarEfeitosContinuos();
    CHECK(a.getForcaTotal() == 25);

    // Após o 2º turno deve expirar
    a.processarEfeitosContinuos();
    CHECK(a.getForcaTotal() == 15);


}
}

// =========================================================
// SUITE 5: PROGRESSÃO, SINO DE ATRIBUTOS E ESTRUTURAS INTERNAS
// =========================================================
TEST_SUITE("Aventureiro - Sistema de RPG e Menu") {

TEST_CASE("Mecânica de Experiência e Aprendizado de Habilidades") {
        Aventureiro a("Herói", 100, 10, 10);

        // Asserções para garantir que valores nulos ou negativos são ignorados
        a.ganharExperiencia(0);
        CHECK(a.getNivel() == 1);

        a.ganharExperiencia(-5);
        CHECK(a.getNivel() == 1);

        a.ganharExperiencia(-100);
        CHECK(a.getNivel() == 1);

        // Força a subida sequencial e o preenchimento correto das strings internas das Habilidades
        a.ganharExperiencia(70); 
        CHECK(a.getNivel() == 2);
        
        // Garante que tentativas de trapaça ou bugs com XP negativo após já ter upado sejam barradas
        a.ganharExperiencia(-50);
        CHECK(a.getNivel() == 2);

        a.ganharExperiencia(120);
        CHECK(a.getNivel() == 3);
    }

    TEST_CASE("Checkpoints de Fase") {
        Aventureiro a("Herói", 100, 5, 15);
        
        a.setIDCheckpoint(3);
        CHECK(a.getIDCheckpoint() == 3);

        a.setIDCheckpoint(-1);
        CHECK(a.getIDCheckpoint() == 3);
    }

    TEST_CASE("Bônus de Equipamento e Multiplicadores Permanentes") {
        Aventureiro a("Herói", 100, 10, 10);
        
        a.buffArma(50);
        a.buffArmadura(50);

        CHECK(a.getForcaTotal() > 10);
        CHECK(a.getDefesa() > 10);
    }

    TEST_CASE("Cobertura de Leitura de Listas e HUD") {
        Aventureiro a("Herói", 100, 5, 15);

        const auto& lista = a.getHabilidades();
        CHECK(lista.empty());

        std::string hud = a.getDeclaracaoStatus();
        CHECK_FALSE(hud.empty());
    }
}

// =========================================================
// SUITE 6: VETORES INTERNOS (adicionarHabilidade)
// =========================================================
TEST_SUITE("Aventureiro - Gerenciamento de Habilidades") {

    TEST_CASE("Inclusão de Técnicas no Vetor Conhecido (adicionarHabilidade)") {
        Aventureiro a("Herói", 100, 5, 15);
        
        CHECK(a.getHabilidades().empty());

        // Mudança crucial: Adicionando a string vazia "" para o parâmetro 'atributoAlvo' da struct
        Habilidade h1("Golpe Rápido", TipoHabilidade::FISICO, 10, 0, 25,"", 0);
        Habilidade h2("Cura Menor", TipoHabilidade::CURA, 0, 15, 20,"", 0);

        a.adicionarHabilidade(h1);
        REQUIRE(a.getHabilidades().size() == 1);
        CHECK(a.getHabilidades().at(0).getNome() == "Golpe Rápido");

        a.adicionarHabilidade(h2);
        REQUIRE(a.getHabilidades().size() == 2);
        CHECK(a.getHabilidades().at(1).getCustoMP() == 15);
    }
}