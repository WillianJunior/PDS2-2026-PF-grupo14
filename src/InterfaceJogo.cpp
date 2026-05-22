#include "InterfaceJogo.hpp"

// ============================================================================
// MÉTODOS DE ENTRADA E SAÍDA DE TEXTO (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void InterfaceJogo::exibirTexto(std::string texto) { (void)texto; }

int InterfaceJogo::solicitarEscolha(std::vector<std::string> opcoes) { (void)opcoes; return 1; }

// ============================================================================
// MÉTODOS DE RENDERIZAÇÃO VISUAL E STATUS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void InterfaceJogo::exibirStatus(const Personagem& p) { (void)p; }

void InterfaceJogo::limparTela() {}

void InterfaceJogo::renderizarDivisor() {}

// ============================================================================
// CONTROLE DE FLUXO (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void InterfaceJogo::pausar() {}