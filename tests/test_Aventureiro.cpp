/**
 * @file test_Aventureiro.cpp
 * @brief Testes de unidade estruturais para a classe Aventureiro (Fase TDD Red).
 */

#include "doctest.h"
#include "Aventureiro.hpp"

// =========================================================
// SUITE: Estado Inicial
// =========================================================

TEST_SUITE("Aventureiro - Estado Inicial") {

    TEST_CASE("MP inicial - Valor Positivo") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getMP() > 0);
    }

    TEST_CASE("MP inicial - Igual ao Maximo") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getMP() == a.getMPMax());
    }

    TEST_CASE("Energia inicial - Valor Positivo") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getEnergia() > 0);
    }

    TEST_CASE("Energia inicial - Igual ao Maximo") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getEnergia() == a.getEnergiaMax());
    }

    TEST_CASE("Força total inicial - Sem bonus de arma") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getForcaTotal() == 15);
    }

    TEST_CASE("Defesa inicial - Sem bonus de armadura") {
        Aventureiro a("Herói", 100, 8, 15);
        CHECK(a.getDefesa() == 8);
    }

    TEST_CASE("Checkpoint inicial - Deve ser zero") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK(a.getIDCheckpoint() == 0);
    }
}

// =========================================================
// SUITE: Recursos de Combate
// =========================================================

TEST_SUITE("Aventureiro - Recursos de Combate") {

    TEST_CASE("consumirMP - Redução Correta") {
        Aventureiro a("Herói", 100, 5, 15);
        int mpInicial = a.getMP();
        a.consumirMP(3);
        CHECK(a.getMP() == mpInicial - 3);
    }

    TEST_CASE("consumirMP - Limite Inferior Zero") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirMP(9999);
        CHECK(a.getMP() == 0);
    }

    TEST_CASE("consumirEnergia - Redução Correta") {
        Aventureiro a("Herói", 100, 5, 15);
        int energiaInicial = a.getEnergia();
        a.consumirEnergia(4);
        CHECK(a.getEnergia() == energiaInicial - 4);
    }

    TEST_CASE("consumirEnergia - Limite Inferior Zero") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirEnergia(9999);
        CHECK(a.getEnergia() == 0);
    }

    TEST_CASE("recuperarRecursos - Incremento de MP") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirMP(3);
        int mpAntes = a.getMP();
        a.recuperarRecursos();
        CHECK(a.getMP() == mpAntes + 1);
    }

    TEST_CASE("recuperarRecursos - Incremento de Energia") {
        Aventureiro a("Herói", 100, 5, 15);
        a.consumirEnergia(8);
        int energiaAntes = a.getEnergia();
        a.recuperarRecursos();
        CHECK(a.getEnergia() == energiaAntes + 5);
    }

    TEST_CASE("recuperarRecursos - Teto de MPMax") {
        Aventureiro a("Herói", 100, 5, 15);
        a.recuperarRecursos();
        CHECK(a.getMP() <= a.getMPMax());
    }

    TEST_CASE("recuperarRecursos - Teto de EnergiaMax") {
        Aventureiro a("Herói", 100, 5, 15);
        a.recuperarRecursos();
        CHECK(a.getEnergia() <= a.getEnergiaMax());
    }
}

// =========================================================
// SUITE: Escudo e Ações de Turno
// =========================================================

TEST_SUITE("Aventureiro - Escudo e Ações") {

    TEST_CASE("usarEscudo - Consumo de Recursos") {
        Aventureiro a("Herói", 100, 5, 15);
        int energiaAntes = a.getEnergia();
        a.usarEscudo();
        CHECK(a.getEnergia() == energiaAntes - 6);
    }

    TEST_CASE("receberDano e Escudo - Mitigação de HP") {
        Aventureiro a("Herói", 100, 5, 15);
        a.usarEscudo();
        int hpAntes = a.getHP();
        a.receberDano(30,TipoHabilidade::FISICO);
        CHECK(a.getHP() > hpAntes - 30);
    }

    TEST_CASE("Escudo - Comportamento de Desativação e Eficácia") {
        Aventureiro a1("Herói", 200, 5, 15);
        a1.usarEscudo();
        int hpAntes1 = a1.getHP();
        a1.receberDano(30,TipoHabilidade::FISICO);
        int danoComEscudo = hpAntes1 - a1.getHP();

        Aventureiro a2("Herói", 200, 5, 15);
        int hpAntes2 = a2.getHP();
        a2.receberDano(30,TipoHabilidade::FISICO);
        int danoSemEscudo = hpAntes2 - a2.getHP();

        // Valida as variáveis para sumir com os warnings e testar a mitigação
        CHECK(danoComEscudo < danoSemEscudo);
        CHECK(a1.getHP() > a2.getHP());
    }
}

TEST_CASE("Integração: Receber Dano Direto e Processamento de DoT") {

    SUBCASE("Ciclo de Turno Misto: Ataque Direto + Dano de DoT") {
        // Defesa = 1 para passar no construtor. Mitigação irrelevante para valores baixos.
        Aventureiro alvo("Alvo", 100, 1, 10);
        alvo.aplicarDoT("Veneno", 10, 2);

        // 1. Recebe um ataque físico direto no meio do turno
        // Fator de mitigação: 100 / 101 = ~0.99 -> 30 * 0.99 = 29.7 -> 29 de dano real
        alvo.receberDano(30, TipoHabilidade::FISICO);
        CHECK(alvo.getHP() == 71); // 100 - 29

        // 2. O turno passa e o efeito contínuo age
        // DoT de 10: 10 * 0.99 = 9.9 -> 9 de dano real
        alvo.processarEfeitosContinuos();
        CHECK(alvo.getHP() == 62); // 71 - 9
    }

    SUBCASE("Interação com Escudo: Ataque Físico vs Dano de DoT") {
        Aventureiro alvo("Alvo", 100, 1, 10);
        alvo.aplicarDoT("Sangramento", 20, 1);
        
        // Ativa a postura defensiva do escudo
        alvo.usarEscudo();

        // 1. Recebe um ataque FISICO direto com escudo ativo (Dano deve ser cortado pela metade)
        // Dano base mitigado pela defesa: 40 * 0.99 = 39.6 -> 39
        // Corte do escudo (50%): 39 * 0.5 = 19.5 -> 19 de dano final
        alvo.receberDano(40, TipoHabilidade::FISICO);
        CHECK(alvo.getHP() == 81); // 100 - 19

        // 2. Processa o DoT com o escudo ainda ativo (O escudo deve ignorar o DoT e causar dano cheio)
        // DoT base mitigado pela defesa: 20 * 0.99 = 19.8 -> 19 de dano final
        alvo.processarEfeitosContinuos();
        CHECK(alvo.getHP() == 62); // 81 - 19 (Provando que o escudo NÃO cortou o DoT pela metade)
    }

    SUBCASE("Overkill Combinado: Ataque Direto + DoT levando à morte") {
        Aventureiro alvo("Alvo", 30, 1, 10);
        alvo.aplicarDoT("Incêndio", 15, 2);

        // Ataque direto deixa o alvo em estado crítico
        // 15 * 0.99 = 14.85 -> 14 de dano
        alvo.receberDano(15, TipoHabilidade::FISICO);
        CHECK(alvo.getHP() == 16); // 30 - 14

        // O DoT age logo em seguida e zera a vida do alvo
        // 15 * 0.99 = 14 de dano
        alvo.processarEfeitosContinuos();
        CHECK(alvo.getHP() == 2); // 16 - 14

        // Próximo turno do DoT finaliza o personagem
        alvo.processarEfeitosContinuos();
        CHECK(alvo.getHP() == 0);
        CHECK_FALSE(alvo.estaVivo());
    }
    
}

// =========================================================
// SUITE: Ciclo de Vida de Efeitos Contínuos e Estatísticos
// =========================================================

TEST_SUITE("Personagem - Efeitos Avançados (DoT, HoT, Buff, Debuff)") {

    TEST_CASE("DoT (Damage over Time) - Aplicação, Dano por Turno e Expiração") {
        Aventureiro heroi("Kael", 100, 0, 10); // Defesa 0 para dano puro
        
        // Aplica um DoT de Sangramento que causa 15 de dano por 2 turnos
        heroi.aplicarDoT("Sangramento", 15, 2);

        // Turno 1: Processa o efeito
        heroi.processarEfeitosContinuos();
        CHECK(heroi.getHP() == 85); // 100 - 15

        // Turno 2: Processa novamente e o efeito deve expirar após isso
        heroi.processarEfeitosContinuos();
        CHECK(heroi.getHP() == 70); // 85 - 15

        // Turno 3: O efeito já expirou, o HP deve permanecer igual
        heroi.processarEfeitosContinuos();
        CHECK(heroi.getHP() == 70); 
    }

    TEST_CASE("HoT (Heal over Time) - Regeneração e Teto de HP Máximo") {
        Aventureiro heroi("Kael", 100, 0, 10);
        heroi.receberDano(30, TipoHabilidade::FISICO); // Reduz HP para 70
        
        // Aplica um HoT de Regeneração de 10 por 2 turnos
        heroi.aplicarHoT("Regeneração", 10, 2);

        // Turno 1: Cura 10
        heroi.processarEfeitosContinuos();
        CHECK(heroi.getHP() == 80);

        // Turno 2: Cura mais 10 e expira
        heroi.processarEfeitosContinuos();
        CHECK(heroi.getHP() == 90);

        // Turno 3: Efeito expirado, sem cura adicional
        heroi.processarEfeitosContinuos();
        CHECK(heroi.getHP() == 90);
    }

    TEST_CASE("Buff e Debuff - Alteração de Atributos em Tempo Real") {
        Aventureiro heroi("Kael", 100, 10, 15); // Defesa Base = 10, Força Base = 15
        
        // 1. Testa aplicação imediata do Buff de Força (+5) por 1 turno
        heroi.aplicarBuff("Fúria", "forca", 5, 1);
        CHECK(heroi.getForcaTotal() == 20); // 15 + 5

        // 2. Testa aplicação imediata do Debuff de Defesa (-4) por 1 turno
        heroi.aplicarDebuff("Expor Armadura", "defesa", 4, 1);
        CHECK(heroi.getDefesa() == 6); // 10 - 4

        // 3. Passagem de Turno: Ambos os efeitos devem reduzir o contador para 0 e expirar
        heroi.processarEfeitosContinuos();
        
        // Atributos devem regressar instantaneamente aos valores originais de base
        CHECK(heroi.getForcaTotal() == 15);
        CHECK(heroi.getDefesa() == 10);
    }

    TEST_CASE("Acumulação - Múltiplos Buffs do mesmo Atributo") {
        Aventureiro heroi("Kael", 100, 10, 15);

        // Aplica dois buffs de força diferentes ao mesmo tempo
        heroi.aplicarBuff("Bênção", "forca", 5, 2);
        heroi.aplicarBuff("Poção de Força", "forca", 10, 1);

        // Força total deve acumular ambos os modificadores
        CHECK(heroi.getForcaTotal() == 30); // 15 + 5 + 10

        // Turno 1: O buff da poção (1 turno) deve expirar, mas a Bênção (2 turnos) continua
        heroi.processarEfeitosContinuos();
        CHECK(heroi.getForcaTotal() == 20); // 15 + 5

        // Turno 2: A Bênção também expira
        heroi.processarEfeitosContinuos();
        CHECK(heroi.getForcaTotal() == 15); // Volta à base
    }

    TEST_CASE("Caso de Borda - Debuff não pode deixar Atributo Negativo") {
        Aventureiro heroi("Kael", 100, 5, 15); // Defesa Base = 5

        // Aplica um debuff massivo de defesa (-20) por 1 turno
        heroi.aplicarDebuff("Quebra de Alma", "defesa", 20, 1);

        // O getter deve travar o valor em 0 usando std::max para evitar bugs de mitigação negativa
        CHECK(heroi.getDefesa() == 0);

        // Restaura após expirar
        heroi.processarEfeitosContinuos();
        CHECK(heroi.getDefesa() == 5);
    }

    TEST_CASE("Simulação de Turno Misto - Todos os efeitos agindo juntos") {
        Aventureiro heroi("Kael", 100, 10, 15);
        heroi.receberDano(40, TipoHabilidade::FISICO); // Reduz vida para testar HoT
        int hpInicial = heroi.getHP();

        // Cenário de Combate Caótico:
        heroi.aplicarDoT("Incêndio", 12, 1);
        heroi.aplicarHoT("Balsamo", 8, 1);
        heroi.aplicarBuff("Agilidade", "defesa", 10, 1);

        // Modificador de atributo deve agir imediatamente antes do processamento do turno
        CHECK(heroi.getDefesa() == 20); 

        // Processa o turno: DoT e HoT calculam seus efeitos no HP; Buff perde duração
        heroi.processarEfeitosContinuos();

        // HP final: hpInicial - 12 (DoT) + 8 (HoT)
        CHECK(heroi.getHP() == (hpInicial - 12 + 8));
        
        // O Buff deve ter expirado
        CHECK(heroi.getDefesa() == 10);
    }
}