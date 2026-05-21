/**
 * @file test_InterfaceJogo.cpp
 * @brief Testes de unidade para a classe InterfaceJogo usando doctest (TDD).
 *
 * Como InterfaceJogo lida com I/O, os testes verificam que os métodos
 * existem e podem ser chamados sem causar erros (não lançam exceções).
 */

#include "doctest/doctest.h"
#include "InterfaceJogo.hpp"
#include "Aventureiro.hpp"

TEST_SUITE("InterfaceJogo - Metodos estaticos") {

    TEST_CASE("exibirTexto não lança exceção") {
        CHECK_NOTHROW(InterfaceJogo::exibirTexto("Texto de teste"));
    }

    TEST_CASE("exibirTexto com string vazia não lança exceção") {
        CHECK_NOTHROW(InterfaceJogo::exibirTexto(""));
    }

    TEST_CASE("exibirStatus não lança exceção") {
        Aventureiro a("Herói", 100, 5, 15);
        CHECK_NOTHROW(InterfaceJogo::exibirStatus(a));
    }

    TEST_CASE("limparTela não lança exceção") {
        CHECK_NOTHROW(InterfaceJogo::limparTela());
    }

    TEST_CASE("renderizarDivisor não lança exceção") {
        CHECK_NOTHROW(InterfaceJogo::renderizarDivisor());
    }

    TEST_CASE("solicitarEscolha retorna valor dentro do intervalo válido") {
        // Como solicitarEscolha lê stdin, testamos apenas que retorna 1
        // quando não há entrada (comportamento stub)
        std::vector<std::string> opcoes = {"Atacar", "Defender", "Fugir"};
        int resultado = InterfaceJogo::solicitarEscolha(opcoes);
        CHECK(resultado >= 1);
        CHECK(resultado <= (int)opcoes.size());
    }
}
