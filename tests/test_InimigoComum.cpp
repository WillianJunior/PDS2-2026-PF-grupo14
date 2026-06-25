/**
 * @file test_InimigoComum.cpp
 * @brief Testes de unidade estruturais e de cobertura total para inimigos comuns.
 */

#include "doctest.h"
#include "InimigoComum.hpp"
#include "Aventureiro.hpp"
#include <stdexcept>
#include <string>

// =========================================================
// SUITE 1: DESAFIANTE DO BAR
// =========================================================
TEST_SUITE("Inimigos Comuns - Desafiante Do Bar") {

    TEST_CASE("DesafianteDoBar - Validação de Contrato de Nível") {
        CHECK_THROWS_AS(DesafianteDoBar("Invalido", 0), std::invalid_argument);
        CHECK_THROWS_AS(DesafianteDoBar("Invalido", -5), std::invalid_argument);
    }

    TEST_CASE("DesafianteDoBar - String de Nome Padrão") {
        DesafianteDoBar inimigo("", 1);
        CHECK(inimigo.getNome() == "Desafiante do Bar");
    }

    TEST_CASE("DesafianteDoBar - Ciclo de Turno e Cobertura de Status") {
        DesafianteDoBar inimigo("Valentão", 2);
        Aventureiro alvo("Heroi", 200, 5, 10);

        int hpAntes = alvo.getHP();
        inimigo.executarTurno(alvo);
        CHECK(alvo.getHP() < hpAntes);

        // Cobertura da String de Interface
        std::string status = inimigo.getDeclaracaoStatus();
        CHECK(status.find("Desafiante") != std::string::npos);
        CHECK(status.find("Soco Bêbado") != std::string::npos);
    }
}

// =========================================================
// SUITE 2: TRABALHADOR NOTURNO ESTRESSADO
// =========================================================
TEST_SUITE("Inimigos Comuns - Trabalhador Noturno") {

    TEST_CASE("TrabalhadorNoturno - Validação de Contrato de Nível") {
        CHECK_THROWS_AS(TrabalhadorNoturno("Invalido", 0), std::invalid_argument);
    }

    TEST_CASE("TrabalhadorNoturno - String de Nome Padrão") {
        TrabalhadorNoturno inimigo("", 1);
        CHECK(inimigo.getNome() == "Trabalhador Noturno Estressado");
    }

    TEST_CASE("TrabalhadorNoturno - Cobertura Estatística das Ramificações (Ataque Comum vs Crítico)") {
        Aventureiro alvo("Heroi", 1000, 1, 10);
        
        // Loop repetido para forçar a cobertura determinística tanto do bloco 'if(critico)' quanto do 'else'
        bool cobriuNormal = false;
        bool cobriuCritico = false;

        for (int i = 0; i < 20; i++) {
            TrabalhadorNoturno inimigo("Operário Puto", 1);
            int hpAntes = alvo.getHP();
            inimigo.executarTurno(alvo);
            int danoTomado = hpAntes - alvo.getHP();

            if (danoTomado > 30) { // Considerando força base escalada dobrada
                cobriuCritico = true;
            } else if (danoTomado > 0) {
                cobriuNormal = true;
            }
            if (cobriuNormal && cobriuCritico) break;
        }

        CHECK(cobriuNormal);
        CHECK(cobriuCritico);

        // Cobertura da String de Interface
        TrabalhadorNoturno inimigoStatus("Operário", 1);
        std::string status = inimigoStatus.getDeclaracaoStatus();
        CHECK(status.find("Estressado") != std::string::npos);
    }
}

// =========================================================
// SUITE 3: SEGURANÇA DE BALADA
// =========================================================
TEST_SUITE("Inimigos Comuns - Segurança de Balada") {

    TEST_CASE("SegurancaDeBalada - Validação de Contrato de Nível") {
        CHECK_THROWS_AS(SegurancaDeBalada("Invalido", 0), std::invalid_argument);
    }

    TEST_CASE("SegurancaDeBalada - String de Nome Padrão") {
        SegurancaDeBalada inimigo("", 1);
        CHECK(inimigo.getNome() == "Segurança de Balada");
    }

    TEST_CASE("SegurancaDeBalada - Gatilho de Escudo e Estados de Status") {
        SegurancaDeBalada inimigo("Guarda", 1);
        Aventureiro alvo("Heroi", 200, 5, 10);

        // Testar ramificação com Escudo Inativo
        std::string statusInativo = inimigo.getDeclaracaoStatus();
        CHECK(statusInativo.find("Inativo") != std::string::npos);

        // Rodar turnos até alcançar o módulo % 3 == 0 (Ativação do Escudo)
        inimigo.executarTurno(alvo); // Turno 1
        inimigo.executarTurno(alvo); // Turno 2
        inimigo.executarTurno(alvo); // Turno 3 (Gatilha postura defensiva)

        // Testar ramificação com Escudo Ativado
        std::string statusAtivo = inimigo.getDeclaracaoStatus();
        CHECK(statusAtivo.find("ATIVADO") != std::string::npos);
    }
}

// =========================================================
// SUITE 4: REGRAS E FLUXOS DE INTERRUPÇÃO GERAIS
// =========================================================
TEST_SUITE("Inimigos Comuns - Contratos Globais de Turno") {

    TEST_CASE("Garantia de Early Return para Alvos já Derrotados") {
        DesafianteDoBar inimigo("Valentão", 1);
        Aventureiro alvo("Heroi", 100, 5, 10);

        alvo.receberDano(500, TipoHabilidade::ULTRA); // Mata o alvo antes do turno do inimigo
        int hpDefinitivo = alvo.getHP();

        inimigo.executarTurno(alvo);
        CHECK(alvo.getHP() == hpDefinitivo); // Nenhum dano adicional foi processado
    }
}