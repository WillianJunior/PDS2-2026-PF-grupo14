/**
 * @file main_test.cpp
 * @brief Ponto de entrada para o runner de testes doctest.
 *
 * Este arquivo define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN exatamente uma vez,
 * o que gera a função main() do executável de testes.
 * Todos os outros arquivos de teste incluem apenas "doctest/doctest.h".
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
