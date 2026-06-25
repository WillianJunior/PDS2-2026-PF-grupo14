/**
 * @file test_Aventureiro.cpp
 * @brief Testes de unidade estruturais de alta cobertura para Aventureiro e Personagem.
 */

#include "doctest.h"
#include "Aventureiro.hpp"
#include "Habilidade.hpp"
#include <stdexcept>
#include <string>
#include <vector>

// =========================================================
// SUITE 1: ESTADO INICIAL, CONSTRUTORES E EXCEÇÕES
// =========================================================
TEST_SUITE("Aventureiro & Personagem - Inicialização") {

    TEST_CASE("Valores Iniciais e Herança de Personagem") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getNome() == "Herói");
        CHECK(a.getHP() == 100);
        CHECK(a.getHPMax() == 100);
        CHECK(a.getDefesa() == 5);
        CHECK(a.getForcaTotal() == 15);
        CHECK(a.getNivel() == 1);
        CHECK(a.estaVivo());
    }

    TEST_CASE("Exceções de Validação de Parâmetros (Guardas da Base)") {
        CHECK_THROWS_AS(Aventureiro("", 100, 5, 15), std::invalid_argument);
        CHECK_THROWS_AS(Aventureiro("Herói", 0, 5, 15), std::invalid_argument);
        CHECK_THROWS_AS(Aventureiro("Herói", 100, 0, 15), std::invalid_argument);
        CHECK_THROWS_AS(Aventureiro("Herói", 100, 5, 0), std::invalid_argument);
    }
}

// =========================================================
// SUITE 2: MATRIZ DE DANOS, DEFESA E ESCUDOS (MECÂNICAS DA BASE)
// =========================================================
TEST_SUITE("Aventureiro & Personagem - Matriz de Combate") {

    TEST_CASE("Caminho de Retorno Imediato para Dano Inválido") {
        Aventureiro a("Alvo", 100, 10, 15);
        a.receberDano(0, TipoHabilidade::FISICO);
        a.receberDano(-20, TipoHabilidade::FISICO);
        CHECK(a.getHP() == 100);
    }

    TEST_CASE("Fórmula de Mitigação por Defesa e Garantia de Dano Mínimo") {
        // fatorMitigacao = 100 / (100 + 100) = 0.5
        Aventureiro a1("Defensivo", 100, 100, 10);
        a1.receberDano(50, TipoHabilidade::FISICO); // 50 * 0.5 = 25
        CHECK(a1.getHP() == 75);

        // Garantia de dano mínimo de 1 mesmo com defesa absurda
        Aventureiro a2("Tanque", 100, 9999, 10);
        a2.receberDano(10, TipoHabilidade::FISICO);
        CHECK(a2.getHP() == 99);
    }

    TEST_CASE("Interação com Escudo Ativo e Tipos de Habilidade") {
        // Escudo reduz dano FISICO/ESPECIAL pela metade (depois da mitigação da defesa)
        // Defesa base = 0 não é permitida, usamos 100 para simplificar a matemática (fator 0.5)
        Aventureiro a1("Escudeiro", 100, 100, 10);
        a1.usarEscudo(); // Ativa _escudoAtivo
        a1.receberDano(80, TipoHabilidade::FISICO); // 80 * 0.5 (defesa) * 0.5 (escudo) = 20 de dano real
        CHECK(a1.getHP() == 80);

        // Escudo ignorado por habilidades ULTRA
        Aventureiro a2("ContraUltra", 100, 100, 10);
        a2.usarEscudo();
        a2.receberDano(80, TipoHabilidade::ULTRA); // 80 * 0.5 (defesa) = 40 de dano real
        CHECK(a2.getHP() == 60);

        // Escudo ignorado por efeitos contínuos DOT
        Aventureiro a3("ContraDot", 100, 100, 10);
        a3.usarEscudo();
        a3.receberDano(80, TipoHabilidade::DOT); // 80 * 0.5 (defesa) = 40 de dano real
        CHECK(a3.getHP() == 60);
    }

}

// =========================================================
// SUITE 3: MODIFICADORES TEMPORÁRIOS (BUFFS, DEBUFFS, DOT, HOT)
// =========================================================
TEST_SUITE("Aventureiro & Personagem - Status por Tempo") {

    TEST_CASE("Aplicação e Variações de Nomes nos Atributos (Case-Insensitive)") {
        Aventureiro a("Goku", 100, 10, 10);

        // Valida guardas de valores e turnos zerados/negativos
        a.aplicarBuff("Invalido", "forca", 0, 3);
        a.aplicarBuff("Invalido2", "forca", 10, 0);
        CHECK(a.getForcaTotal() == 10);

        // Buffs de força acumulados (testando strings aceitas: "forca", "Força")
        a.aplicarBuff("Furia", "forca", 5, 3);
        a.aplicarBuff("Poder", "Força", 5, 3);
        CHECK(a.getForcaTotal() == 20); // 10 base + 5 + 5

        // Debuffs de defesa acumulados (testando strings aceitas: "defesa", "DEFESA")
        a.aplicarDebuff("Exposto", "defesa", 4, 2);
        a.aplicarDebuff("Quebra", "DEFESA", 3, 2);
        CHECK(a.getDefesa() == 3); // 10 base - 4 - 3

        // Força limite inferior do atributo modificado para zero
        a.aplicarDebuff("Maldição", "defesa", 50, 2);
        CHECK(a.getDefesa() == 0); 
    }

    TEST_CASE("Processamento e Decaimento de Turnos (Efeitos por Tempo)") {
        Aventureiro a("Alvo", 100, 100, 10); // defesa 100 = mitigacao de 50%

        a.aplicarDoT("Veneno", 20, 2); // 20 bruto = 10 dano real por turno devido à defesa
        a.aplicarBuff("Surto", "forca", 10, 1);

        CHECK(a.getForcaTotal() == 20);

        // Turno 1
        a.processarEfeitosContinuos();
        CHECK(a.getHP() == 90); // 100 - 10 do DoT
        CHECK(a.getForcaTotal() == 10); // Buff expirou (passou de 1 para 0)

        // Turno 2 (DoT deve causar dano e expirar)
        a.processarEfeitosContinuos();
        CHECK(a.getHP() == 80);

        // Turno 3 (Nenhum efeito ativo deve restar no vetor)
        a.processarEfeitosContinuos();
        CHECK(a.getHP() == 80); 
    }

    TEST_CASE("Mecânica de Efeitos de Cura por Turno (HoT)") {
        Aventureiro a("Paciente", 100, 10, 10);
        a.receberDano(40, TipoHabilidade::FISICO);
        int hpFerido = a.getHP();

        a.aplicarHoT("Bênção", 15, 2);
        
        // Turno 1 do HoT
        a.processarEfeitosContinuos();
        CHECK(a.getHP() == hpFerido + 15);

        // Limpeza forçada de efeitos
        a.limparEfeitosTemporarios();
        a.processarEfeitosContinuos();
        // Não deve curar o segundo turno pois foi limpo
        CHECK(a.getHP() == hpFerido + 15);
    }
}

// =========================================================
// SUITE 4: PROGRESSÃO DE RPG, SKILLS E RECURSOS ESPECÍFICOS
// =========================================================
TEST_SUITE("Aventureiro - Sistema de Níveis e Recursos") {

    TEST_CASE("Subida de Nível e Recomposição de Atributos do Switch") {
        Aventureiro a("Herói RPG", 100, 10, 10);

        // Força passagem por cada 'case' do switch da subida de nível
        a.ganharExperiencia(65); // Nvl 2
        CHECK(a.getNivel() == 2);
        CHECK(a.getHabilidades().size() == 2);

        a.ganharExperiencia(125); // Nvl 3
        CHECK(a.getNivel() == 3);
        CHECK(a.getHabilidades().size() == 4);

        a.ganharExperiencia(185); // Nvl 4
        CHECK(a.getNivel() == 4);
        CHECK(a.getHabilidades().size() == 6);

        a.ganharExperiencia(245); // Nvl 5
        CHECK(a.getNivel() == 5);
        CHECK(a.getHabilidades().size() == 7);
    }

    TEST_CASE("Gerenciamento de Recursos Específicos e Dormir") {
        Aventureiro a("MagoGuerreiro", 100, 10, 10);
        
        a.consumirMP(20);
        a.consumirEnergia(30);
        CHECK(a.getMP() == 30);
        CHECK(a.getEnergia() == 70);

        a.recuperarRecursos(); // +2 MP, +8 Energia
        CHECK(a.getMP() == 32);
        CHECK(a.getEnergia() == 78);

        a.dormir();
        CHECK(a.getMP() == a.getMPMax());
        CHECK(a.getEnergia() == a.getEnergiaMax());
    }

    TEST_CASE("Checkpoints de Fase e Buffs de Equipamento") {
        Aventureiro a("Aventureiro", 100, 5, 10);
        
        a.setIDCheckpoint(3);
        CHECK(a.getIDCheckpoint() == 3);
        a.setIDCheckpoint(-5); // Deve ser ignorado pelo 'if (idCena > 0)'
        CHECK(a.getIDCheckpoint() == 3);

        a.buffArma(5);
        a.buffArmadura(5);
        CHECK(a.getForcaTotal() == 15);
        CHECK(a.getDefesa() == 10);
    }
}