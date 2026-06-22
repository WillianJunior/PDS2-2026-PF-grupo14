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
        /*hp*/     150,
        /*defesa*/  15,
        /*forca*/   15,
        nivel,
        /*xp*/      60)
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

// ============================================================================
// EXECUÇÃO DE TURNO — DESAFIANTE DO BAR
// ============================================================================

void DesafianteDoBar::executarTurno(Personagem& alvo) {
    if (!alvo.estaVivo()) return;

    processarEfeitosContinuos();
    if (!estaVivo()) return;

    _contadorTurnos++;

    InterfaceJogo::exibirTexto("\n👊 [ATAQUE FÍSICO] " + _nome + " avança cambaleando com um Soco Bêbado!");
    alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
}

// ============================================================================
// TRABALHADOR NOTURNO ESTRESSADO
// ============================================================================

TrabalhadorNoturno::TrabalhadorNoturno(std::string nome, int nivel)
    : Inimigo(
        nome.empty() ? "Trabalhador Noturno Estressado" : nome,
        /*hp*/     150,   // frágil
        /*defesa*/  15,
        /*forca*/  20,   // alto, mas compensa com pouco HP
        nivel,
        /*xp*/     90)
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
        InterfaceJogo::exibirTexto("\n🚨 💥 [GOLPE CRÍTICO] " + _nome + " entra em colapso de raiva e ativa Surto de Raiva!");
        alvo.receberDano(_forcaBase * 2, TipoHabilidade::FISICO);
    } else {
        InterfaceJogo::exibirTexto("\n👊 [ATAQUE FÍSICO] " + _nome + " desfere um soco desesperado de exaustão!");
        alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
    }
}

// ============================================================================
// SEGURANÇA DE BALADA
// ============================================================================

SegurancaDeBalada::SegurancaDeBalada(std::string nome, int nivel)
    : Inimigo(
        nome.empty() ? "Segurança de Balada" : nome,
        /*hp*/     140,   // mais HP que os outros comuns
        /*defesa*/  25,   // resistente fisicamente
        /*forca*/   15,
        nivel,
        /*xp*/     200)
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
        InterfaceJogo::exibirTexto("\n🛡️  [POSTURA] " + _nome + " cruza os braços e finca a base!");
        InterfaceJogo::exibirTexto("    > Escudo Ativo: Impacto de ataques comuns mitigado pela metade até a próxima rodada.");
    }

    InterfaceJogo::exibirTexto("\n💥 [ATAQUE] " + _nome + " projeta o corpo à frente com um Empurrão de Segurança!");
    alvo.receberDano(_forcaBase, TipoHabilidade::FISICO);
}
