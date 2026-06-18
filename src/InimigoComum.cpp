#include "InimigoComum.hpp"
#include "InterfaceJogo.hpp"
#include <cstdlib>   // rand()
#include <stdexcept>
#include <string>

// ============================================================================
// Utilitário interno: valida nivel e lança std::invalid_argument se inválido
// ============================================================================
namespace {
    /**
     * @brief Valida que o nível é pelo menos 1.
     * @param nivel Valor a validar.
     * @param contexto Nome da classe para compor a mensagem de erro.
     * @throws std::invalid_argument Se nivel < 1.
     */
    void validarNivel(int nivel, const std::string& contexto) {
        if (nivel < 1) {
            throw std::invalid_argument(
                contexto + ": nivel deve ser >= 1, recebido: "
                + std::to_string(nivel));
        }
    }
} // namespace anônimo

// ============================================================================
// DESAFIANTE DO BAR
// ============================================================================

DesafianteDoBar::DesafianteDoBar(std::string nome, int nivel)
    : Inimigo(
        nome.empty() ? "Desafiante do Bar" : nome,
        /*hp*/     35 * nivel,
        /*defesa*/  3 * nivel,
        /*forca*/   7 * nivel,
        nivel,
        /*xp*/     15 * nivel)
{
    validarNivel(nivel, "DesafianteDoBar");

    // Arsenal: apenas soco bruto — sem habilidades especiais
    adicionarHabilidade(Habilidade(
        "Soco Bêbado",
        TipoHabilidade::FISICO,
        /*custoE*/ 0, /*custoMP*/ 0,
        /*valor*/  _forcaBase,
        /*atributo*/ ""));
}

void DesafianteDoBar::executarTurno(Personagem& alvo) {
    if (!alvo.estaVivo()) return;

    processarEfeitosContinuos();
    if (!estaVivo()) return;

    _contadorTurnos++;

    InterfaceJogo::exibirTexto(
        _nome + " parte para cima com um soco mal executado!");

    alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
}

// ============================================================================
// TRABALHADOR NOTURNO ESTRESSADO
// ============================================================================

TrabalhadorNoturno::TrabalhadorNoturno(std::string nome, int nivel)
    : Inimigo(
        nome.empty() ? "Trabalhador Noturno Estressado" : nome,
        /*hp*/     20 * nivel,   // frágil
        /*defesa*/  2 * nivel,
        /*forca*/  10 * nivel,   // alto, mas compensa com pouco HP
        nivel,
        /*xp*/     18 * nivel)
{
    validarNivel(nivel, "TrabalhadorNoturno");

    adicionarHabilidade(Habilidade(
        "Surto de Raiva",
        TipoHabilidade::FISICO,
        0, 0,
        _forcaBase,
        ""));

    adicionarHabilidade(Habilidade(
        "Golpe Crítico (Adrenalina)",
        TipoHabilidade::FISICO,
        0, 0,
        _forcaBase * 2,   // dano dobrado no crítico
        ""));
}

void TrabalhadorNoturno::executarTurno(Personagem& alvo) {
    if (!alvo.estaVivo()) return;

    processarEfeitosContinuos();
    if (!estaVivo()) return;

    _contadorTurnos++;

    // 40% de chance de crítico
    const bool critico = (rand() % 100) < 40;

    if (critico) {
        InterfaceJogo::exibirTexto(
            _nome + " entra em colapso de adrenalina — GOLPE CRÍTICO!");
        alvo.receberDano(_forcaBase * 2, TipoHabilidade::FISICO);
    } else {
        InterfaceJogo::exibirTexto(
            _nome + " desfere um soco desesperado!");
        alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
    }
}

// ============================================================================
// SEGURANÇA DE BALADA
// ============================================================================

SegurancaDeBalada::SegurancaDeBalada(std::string nome, int nivel)
    : Inimigo(
        nome.empty() ? "Segurança de Balada" : nome,
        /*hp*/     60 * nivel,   // mais HP que os outros comuns
        /*defesa*/  8 * nivel,   // resistente fisicamente
        /*forca*/   8 * nivel,
        nivel,
        /*xp*/     25 * nivel)
{
    validarNivel(nivel, "SegurancaDeBalada");

    adicionarHabilidade(Habilidade(
        "Empurrão de Segurança",
        TipoHabilidade::FISICO,
        0, 0,
        _forcaBase,
        ""));
}

void SegurancaDeBalada::executarTurno(Personagem& alvo) {
    if (!alvo.estaVivo()) return;

    processarEfeitosContinuos();
    if (!estaVivo()) return;

    _contadorTurnos++;

    // A cada 3 turnos assume postura defensiva (ativa o escudo interno)
    if (_contadorTurnos % 3 == 0) {
        _escudoAtivo = true;
        InterfaceJogo::exibirTexto(
            _nome + " cruza os braços e assume postura de bloqueio — "
            "resistência redobrada até o próximo turno!");
    }

    InterfaceJogo::exibirTexto(
        _nome + " empurra com força bruta!");
    alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
}
